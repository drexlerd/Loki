/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_SEMANTIC_PARSER_HPP_
#define LOKI_SEMANTIC_PARSER_HPP_

#include "loki/ast.hpp"
#include "loki/formalism/formalism.hpp"
#include "loki/parser.hpp"
#include "loki/parser/error_handler.hpp"
#include "loki/semantic/errors.hpp"
#include "loki/semantic/translator.hpp"

#include <boost/optional.hpp>
#include <cista/containers/optional.h>
#include <cista/containers/string.h>
#include <cstddef>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace loki::semantic
{
namespace fs = std::filesystem;

std::string read_file(const fs::path& path);

class Parser
{
public:
    explicit Parser(const std::string& domain_source, parser::ParserOptions options = {});
    explicit Parser(const fs::path& domain_path, parser::ParserOptions options = {});

    const formalism::Repository& repository() const noexcept;
    formalism::Repository& repository() noexcept;

    formalism::DomainView get_domain() const noexcept;

    formalism::TaskView parse_task(const std::string& source);
    formalism::TaskView parse_task(const fs::path& path);

private:
    formalism::DomainView parse_domain_ast(const ast::Domain& domain);

    formalism::TaskView parse_task_ast(const ast::Task& task);

    parser::ParserOptions m_options;
    const parser::ErrorHandlerType* m_error_handler = nullptr;
    std::shared_ptr<detail::TranslationStorage> m_storage;
    std::vector<std::shared_ptr<detail::TranslationStorage>> m_task_storages;
    cista::optional<ygg::Index<formalism::Domain>> m_domain;
    std::string m_domain_name;

    ygg::Index<formalism::Type> m_object_type;
    ygg::Index<formalism::Type> m_number_type;
    std::unordered_map<std::string, ygg::Index<formalism::Type>> m_types;
    std::unordered_map<std::string, ygg::Index<formalism::Object>> m_objects;
    std::unordered_map<std::string, ygg::Index<formalism::Predicate>> m_predicates;
    std::unordered_map<std::string, ygg::Index<formalism::FunctionSkeleton>> m_functions;
    std::unordered_set<std::string> m_declared_types;
    std::unordered_set<std::string> m_declared_objects;
    std::unordered_set<std::string> m_declared_predicates;
    std::unordered_set<std::string> m_declared_functions;
    std::unordered_set<formalism::RequirementKind> m_active_requirements;
    std::unordered_set<formalism::RequirementKind> m_domain_requirement_kinds;
    std::unordered_map<ygg::uint_t, ygg::IndexList<formalism::Type>> m_variable_types;
    std::vector<std::unordered_map<std::string, ygg::Index<formalism::Variable>>> m_variable_scopes;
    std::unordered_map<std::string, ygg::Index<formalism::Object>>* m_task_objects = nullptr;

    static cista::offset::string to_cista(const std::string& text);

    formalism::Repository& repo() noexcept;
    const formalism::Repository& repo() const noexcept;

    static ParseError parse_error(const parser::ErrorHandlerType& error_handler, const std::string& fallback, parser::Iterator);

    struct ErrorHandlerScope
    {
        Parser& parser;
        const parser::ErrorHandlerType* previous;

        ErrorHandlerScope(Parser& parser, const parser::ErrorHandlerType& error_handler);
        ~ErrorHandlerScope();
    };

    template<typename Node, typename Error>
    [[noreturn]] void throw_at(const Node& node, Error error) const
    {
        if (m_error_handler)
        {
            if (auto range = parser::source_range(*m_error_handler, node))
            {
                error.set_source_range(SourceRange { SourcePosition { range->begin.line, range->begin.column, range->begin.offset },
                                                     SourcePosition { range->end.line, range->end.column, range->end.offset } });
            }
        }
        throw error;
    }

    void rebuild_domain_symbols();

    void canonicalize_domain(formalism::DomainView domain);

    formalism::TaskView canonicalize_task(formalism::TaskView task, const std::shared_ptr<detail::TranslationStorage>& domain_storage);

    void clear_domain_symbols();

    static std::string key(std::string text);

    static std::string requirement_name(formalism::RequirementKind kind);

    void remember_requirement(formalism::RequirementKind kind);

    void remember_adl_requirements();

    template<typename Node>
    void require_requirement(formalism::RequirementKind kind, const Node& node) const
    {
        if (!m_options.strict)
            return;
        if (!m_active_requirements.contains(kind))
            throw_at(node, MissingRequirementError(requirement_name(kind)));
    }

    void require_typing_if_needed(const boost::optional<ast::TypeExpression>& type, const boost::spirit::x3::position_tagged& node) const;

    bool is_subtype(ygg::Index<formalism::Type> actual, ygg::Index<formalism::Type> expected) const;

    bool is_subtype(ygg::Index<formalism::Type> actual, ygg::Index<formalism::Type> expected, std::unordered_set<ygg::uint_t>& seen) const;

    bool types_compatible(const ygg::IndexList<formalism::Type>& actual_types, const ygg::IndexList<formalism::Type>& expected_types) const;

    ygg::IndexList<formalism::Type> term_types(const ast::Term& term) const;

    template<typename Node>
    void check_argument_types(const std::string& name,
                              const ygg::IndexList<formalism::Parameter>& parameters,
                              const std::vector<ast::Term>& terms,
                              const Node& node) const
    {
        if (!m_options.strict)
            return;
        for (auto i = std::size_t { 0 }; i < terms.size(); ++i)
        {
            const auto actual = term_types(terms[i]);
            const auto& expected = repo()[parameters[i]].types;
            if (!types_compatible(actual, expected))
                throw_at(node, TypeMismatchError(name));
        }
    }

    template<typename Error, typename Node>
    void ensure_new(std::unordered_set<std::string>& names, std::string name, const Node& node) const
    {
        if (!names.insert(name).second)
            throw_at(node, Error(name));
    }

    template<typename Node>
    void ensure_arity(const std::string& name, size_t expected, size_t actual, const Node& node) const
    {
        if (expected != actual)
            throw_at(node, ArityMismatchError(name, expected, actual));
    }

    ygg::Index<formalism::Type> intern_type(const std::string& name, ygg::IndexList<formalism::Type> bases);

    ygg::IndexList<formalism::Requirement> parse_requirements(const std::vector<ast::Requirement>& nodes);

    formalism::RequirementKind requirement_kind(const ast::Requirement& node) const;

    ygg::IndexList<formalism::Type> parse_types(const std::vector<ast::TypedName>& nodes);

    ygg::IndexList<formalism::Type> parse_type_expression(const ast::TypeExpression& type);

    ygg::IndexList<formalism::Type> parse_type_expression_node(const ast::TypeReference& node);

    ygg::IndexList<formalism::Type> parse_type_expression_node(const ast::EitherType& node);

    ygg::IndexList<formalism::Object> parse_objects(const std::vector<ast::TypedName>& nodes,
                                                    std::unordered_map<std::string, ygg::Index<formalism::Object>>& table);

    ygg::IndexList<formalism::Parameter> parse_parameters(const std::vector<ast::TypedVariable>& nodes);

    ygg::IndexList<formalism::Predicate> parse_predicates(const std::vector<ast::PredicateDeclaration>& nodes);

    ygg::IndexList<formalism::FunctionSkeleton> parse_functions(const std::vector<ast::FunctionDeclaration>& nodes);

    ygg::Index<formalism::Predicate> predicate(const ast::Identifier& identifier, size_t arity);

    ygg::Index<formalism::Predicate> equality_predicate(const ast::Identifier& identifier, size_t arity);

    ygg::Index<formalism::FunctionSkeleton> function(const ast::Identifier& identifier, size_t arity);

    ygg::Index<formalism::Variable> variable(const ast::Identifier& identifier) const;

    ygg::Index<formalism::Object> object(const ast::Identifier& identifier) const;

    ygg::Index<formalism::Term> parse_term(const ast::Term& node);

    ygg::IndexList<formalism::Term> parse_terms(const std::vector<ast::Term>& nodes);

    ygg::Index<formalism::Atom> parse_atom(const ast::Atom& node);

    ygg::Index<formalism::Literal> parse_literal(const ast::Literal& node);

    ygg::Index<formalism::Condition> parse_condition(const ast::Condition& condition);

    ygg::Index<formalism::Condition> wrap_condition(ygg::Data<formalism::Condition>::Variant value);

    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionLiteral& node);
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionAnd& node);
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionOr& node);
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionNot& node);
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionImply& node);
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionExists& node);
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionForall& node);
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionNumericConstraint& node);

    ygg::Index<formalism::FunctionTerm> parse_function_term(const ast::FunctionTerm& node);

    ygg::Index<formalism::FunctionExpression> parse_function_expression(const ast::FunctionExpression& expression);

    ygg::Index<formalism::FunctionExpression> wrap_function_expression(ygg::Data<formalism::FunctionExpression>::Variant value);
    ygg::Index<formalism::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionNumber& node);
    ygg::Index<formalism::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionFunction& node);
    ygg::Index<formalism::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionUnary& node);
    ygg::Index<formalism::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionBinary& node);
    ygg::Index<formalism::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionMulti& node);

    ygg::Index<formalism::Effect> parse_effect(const ast::Effect& effect);
    ygg::Index<formalism::Effect> wrap_effect(ygg::Data<formalism::Effect>::Variant value);
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectLiteral& node);
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectAnd& node);
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectNumeric& node);
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectForall& node);
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectWhen& node);
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectOneOf& node);
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectProbabilistic& node);

    ygg::Index<formalism::Action> parse_action(const ast::Action& node);

    ygg::Index<formalism::Axiom> parse_axiom(const ast::Axiom& node);

    void parse_initial_element(const ast::Literal& literal, ygg::IndexList<formalism::Literal>& literals, ygg::IndexList<formalism::InitialFunctionValue>&);
    void
    parse_initial_element(const ast::InitialFunctionValue& value, ygg::IndexList<formalism::Literal>&, ygg::IndexList<formalism::InitialFunctionValue>& values);

    ygg::Index<formalism::Metric> parse_metric(const ast::Metric& node);

    formalism::BinaryComparator comparator(const ast::ConditionNumericConstraint& node) const;
    static formalism::BinaryArithmeticOperator binary_operator(std::string op);
    static formalism::MultiArithmeticOperator multi_operator(std::string op);
    formalism::NumericEffectOperator numeric_effect_operator(const ast::EffectNumeric& node) const;
};

}  // namespace loki::semantic

#endif
