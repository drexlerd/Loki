/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_SEMANTIC_PARSER_HPP_
#define LOKI2_SEMANTIC_PARSER_HPP_

#include "loki2/ast.hpp"
#include "loki2/parser.hpp"
#include "loki2/semantic/errors.hpp"
#include "loki2/pddl/pddl.hpp"
#include "loki2/semantic/translator.hpp"

#include <boost/optional.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <cista/containers/optional.h>
#include <cista/containers/string.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <utility>
#include <vector>

namespace loki2::semantic
{
namespace fs = std::filesystem;

inline std::string read_file(const fs::path& path)
{
    std::ifstream in(path);
    if (!in)
        throw SemanticError(SemanticErrorCode::ParseFailure, "Could not open PDDL file: " + path.string());
    std::ostringstream out;
    out << in.rdbuf();
    return out.str();
}

class Parser
{
public:
    explicit Parser(parser::ParserOptions options = {}) :
        m_options(options),
        m_storage(std::make_shared<detail::TranslationStorage>(0))
    {
        m_object_type = intern_type("object", {});
        m_number_type = intern_type("number", {});
    }

    const pddl::Repository& repository() const noexcept { return repo(); }
    pddl::Repository& repository() noexcept { return repo(); }
    const pddl::Builder& builder() const noexcept { return build(); }
    pddl::Builder& builder() noexcept { return build(); }

    bool has_domain() const noexcept { return m_domain.has_value(); }
    pddl::DomainView get_domain() const
    {
        if (!m_domain)
            throw SemanticError(SemanticErrorCode::MissingDomain, "No domain has been parsed.");
        return ygg::make_view(*m_domain, repo());
    }

    pddl::DomainView parse_domain(const std::string& source)
    {
        auto first = source.cbegin();
        parser::ErrorHandlerType error_handler(first, source.cend(), std::cerr);
        ast::Domain domain_ast;
        if (!parser::parse_domain_with_options(source, domain_ast, error_handler, m_options))
            throw ParseError("Could not parse PDDL domain.");
        auto scope = ErrorHandlerScope { *this, error_handler };
        return parse_domain_ast(domain_ast);
    }

    pddl::DomainView parse_domain(const fs::path& path) { return parse_domain(read_file(path)); }
    pddl::DomainView parse_domain_file(const fs::path& path) { return parse_domain(path); }

    pddl::TaskView parse_task(const std::string& source)
    {
        auto first = source.cbegin();
        parser::ErrorHandlerType error_handler(first, source.cend(), std::cerr);
        ast::Task task_ast;
        if (!parser::parse_task_with_options(source, task_ast, error_handler, m_options))
            throw ParseError("Could not parse PDDL task.");
        auto scope = ErrorHandlerScope { *this, error_handler };
        return parse_task_ast(task_ast);
    }

    pddl::TaskView parse_task(const fs::path& path) { return parse_task(read_file(path)); }
    pddl::TaskView parse_task_file(const fs::path& path) { return parse_task(path); }

private:
    pddl::DomainView parse_domain_ast(const ast::Domain& domain)
    {
        clear_domain_symbols();
        m_object_type = intern_type("object", {});
        m_number_type = intern_type("number", {});

        auto requirements = parse_requirements(domain.requirements);
        auto types = parse_types(domain.types);
        auto constants = parse_objects(domain.constants, m_objects);
        auto predicates = parse_predicates(domain.predicates);
        auto functions = parse_functions(domain.functions);
        auto axioms = ygg::IndexList<pddl::Axiom> {};
        for (const auto& axiom : domain.axioms)
            axioms.push_back(parse_axiom(axiom));
        auto actions = ygg::IndexList<pddl::Action> {};
        for (const auto& action : domain.actions)
            actions.push_back(parse_action(action));

        types.clear();
        for (auto i = ygg::uint_t { 0 }; i < repo().size<pddl::Type>(); ++i)
            types.push_back(ygg::Index<pddl::Type>(i));
        auto data = ygg::Data<pddl::Domain>(to_cista(domain.name.text), std::move(requirements), std::move(types), std::move(constants), std::move(predicates), std::move(functions), std::move(actions), std::move(axioms));
        auto view = build().domain(repo(), std::move(data));
        canonicalize_domain(view);
        m_domain_name = domain.name.text;
        return get_domain();
    }

    pddl::TaskView parse_task_ast(const ast::Task& task)
    {
        if (!m_domain)
            throw_at(task, SemanticError(SemanticErrorCode::MissingDomain, "Cannot parse task before parsing a domain."));
        if (!m_domain_name.empty() && task.domain_name.text != m_domain_name)
            throw_at(task.domain_name, MismatchedDomainError(m_domain_name, task.domain_name.text));

        const auto domain_storage = m_storage;
        auto parse_storage = std::make_shared<detail::TranslationStorage>(m_task_storages.size() + 1, &domain_storage->repository);
        detail::inherit_domain_identity_mappings(*parse_storage, *domain_storage);
        struct StorageScope
        {
            Parser& parser;
            std::shared_ptr<detail::TranslationStorage> previous;
            ~StorageScope() { parser.m_storage = std::move(previous); }
        };
        StorageScope storage_scope { *this, m_storage };
        m_storage = std::move(parse_storage);

        auto task_objects = std::unordered_map<std::string, ygg::Index<pddl::Object>> {};
        struct TaskObjectScope
        {
            Parser& parser;
            std::unordered_map<std::string, ygg::Index<pddl::Object>>* previous;
            ~TaskObjectScope() { parser.m_task_objects = previous; }
        };
        TaskObjectScope task_object_scope { *this, m_task_objects };
        m_task_objects = &task_objects;

        struct ObjectDeclarationScope
        {
            Parser& parser;
            std::unordered_set<std::string> previous;
            ~ObjectDeclarationScope() { parser.m_declared_objects = std::move(previous); }
        };
        ObjectDeclarationScope object_declaration_scope { *this, m_declared_objects };

        auto requirements = parse_requirements(task.requirements);
        auto objects = parse_objects(task.objects, task_objects);
        auto initial_literals = ygg::IndexList<pddl::Literal> {};
        auto initial_function_values = ygg::IndexList<pddl::InitialFunctionValue> {};
        for (const auto& element : task.initial)
        {
            boost::apply_visitor([&](const auto& node) { parse_initial_element(node, initial_literals, initial_function_values); }, element);
        }
        auto goal = cista::optional<ygg::Index<pddl::Condition>> {};
        if (task.goal)
            goal = parse_condition(*task.goal);
        auto metric = cista::optional<ygg::Index<pddl::Metric>> {};
        if (task.metric)
            metric = parse_metric(*task.metric);
        auto axioms = ygg::IndexList<pddl::Axiom> {};
        for (const auto& axiom : task.axioms)
            axioms.push_back(parse_axiom(axiom));

        auto data = ygg::Data<pddl::Task>(to_cista(task.name.text), *m_domain, std::move(requirements), std::move(objects), std::move(initial_literals), std::move(initial_function_values), goal, metric, std::move(axioms));
        auto view = build().task(repo(), std::move(data));
        return canonicalize_task(view, domain_storage);
    }

    parser::ParserOptions m_options;
    const parser::ErrorHandlerType* m_error_handler = nullptr;
    std::shared_ptr<detail::TranslationStorage> m_storage;
    std::vector<std::shared_ptr<detail::TranslationStorage>> m_task_storages;
    cista::optional<ygg::Index<pddl::Domain>> m_domain;
    std::string m_domain_name;

    ygg::Index<pddl::Type> m_object_type;
    ygg::Index<pddl::Type> m_number_type;
    std::unordered_map<std::string, ygg::Index<pddl::Type>> m_types;
    std::unordered_map<std::string, ygg::Index<pddl::Object>> m_objects;
    std::unordered_map<std::string, ygg::Index<pddl::Predicate>> m_predicates;
    std::unordered_map<std::string, ygg::Index<pddl::FunctionSkeleton>> m_functions;
    std::unordered_set<std::string> m_declared_types;
    std::unordered_set<std::string> m_declared_objects;
    std::unordered_set<std::string> m_declared_predicates;
    std::unordered_set<std::string> m_declared_functions;
    std::vector<std::unordered_map<std::string, ygg::Index<pddl::Variable>>> m_variable_scopes;
    std::unordered_map<std::string, ygg::Index<pddl::Object>>* m_task_objects = nullptr;

    static cista::offset::string to_cista(const std::string& text) { return cista::offset::string(text); }

    pddl::Repository& repo() noexcept { return m_storage->repository; }
    const pddl::Repository& repo() const noexcept { return m_storage->repository; }
    pddl::Builder& build() noexcept { return m_storage->builder; }
    const pddl::Builder& build() const noexcept { return m_storage->builder; }

    struct ErrorHandlerScope
    {
        Parser& parser;
        const parser::ErrorHandlerType* previous;

        ErrorHandlerScope(Parser& parser, const parser::ErrorHandlerType& error_handler) :
            parser(parser),
            previous(parser.m_error_handler)
        {
            parser.m_error_handler = &error_handler;
        }

        ~ErrorHandlerScope() { parser.m_error_handler = previous; }
    };

    template<typename Node, typename Error>
    [[noreturn]] void throw_at(const Node& node, Error error) const
    {
        if (m_error_handler)
            if (auto source_range = parser::source_range(*m_error_handler, node))
                error.set_source_range(*source_range);
        throw error;
    }

    void rebuild_domain_symbols()
    {
        m_types.clear();
        m_objects.clear();
        m_predicates.clear();
        m_functions.clear();
        m_declared_types.clear();
        m_declared_objects.clear();
        m_declared_predicates.clear();
        m_declared_functions.clear();
        if (!m_domain)
            return;

        auto remember_type = [&](auto&& self, ygg::Index<pddl::Type> type) -> void
        {
            const auto& data = repo()[type];
            m_types[std::string(data.name)] = type;
            m_declared_types.insert(std::string(data.name));
            for (auto base : data.bases)
                self(self, base);
        };

        const auto& domain = repo()[*m_domain];
        for (auto type : domain.types)
            remember_type(remember_type, type);
        for (auto object : domain.constants)
        {
            const auto& data = repo()[object];
            m_objects[std::string(data.name)] = object;
            m_declared_objects.insert(std::string(data.name));
            for (auto type : data.types)
                remember_type(remember_type, type);
        }
        for (auto predicate : domain.predicates)
        {
            const auto name = std::string(repo()[predicate].name);
            m_predicates[name] = predicate;
            m_declared_predicates.insert(name);
        }
        for (auto function : domain.functions)
        {
            const auto& data = repo()[function];
            m_functions[std::string(data.name)] = function;
            m_declared_functions.insert(std::string(data.name));
            remember_type(remember_type, data.type);
        }

        if (auto it = m_types.find("object"); it != m_types.end())
            m_object_type = it->second;
        else
            m_object_type = intern_type("object", {});

        if (auto it = m_types.find("number"); it != m_types.end())
            m_number_type = it->second;
        else
            m_number_type = intern_type("number", {});
    }

    void canonicalize_domain(pddl::DomainView domain)
    {
        auto canonical = std::make_shared<detail::TranslationStorage>(0);
        auto copier = detail::CanonicalCopyTranslator(canonical);
        auto copied = copier.copy_domain(domain);
        m_storage = std::move(canonical);
        m_task_storages.clear();
        m_domain = copied.get_index();
        rebuild_domain_symbols();
    }

    pddl::TaskView canonicalize_task(pddl::TaskView task, const std::shared_ptr<detail::TranslationStorage>& domain_storage)
    {
        auto canonical = std::make_shared<detail::TranslationStorage>(m_task_storages.size() + 1, &domain_storage->repository);
        detail::inherit_domain_identity_mappings(*canonical, *domain_storage);
        auto copier = detail::CanonicalCopyTranslator(canonical);
        auto copied = copier.copy_task(task);
        m_task_storages.push_back(canonical);
        return ygg::make_view(copied.get_index(), m_task_storages.back()->repository);
    }

    void clear_domain_symbols()
    {
        m_storage = std::make_shared<detail::TranslationStorage>(0);
        m_task_storages.clear();
        m_domain = {};
        m_domain_name.clear();
        m_types.clear();
        m_objects.clear();
        m_predicates.clear();
        m_functions.clear();
        m_declared_types.clear();
        m_declared_objects.clear();
        m_declared_predicates.clear();
        m_declared_functions.clear();
        m_variable_scopes.clear();
        m_task_objects = nullptr;
    }

    static std::string key(std::string text)
    {
        if (!text.empty() && text.front() == ':')
            text.erase(text.begin());
        for (auto& c : text)
            if (c >= 'A' && c <= 'Z')
                c = static_cast<char>(c - 'A' + 'a');
        return text;
    }

    template<typename Node>
    void ensure_new(std::unordered_set<std::string>& names, std::string name, SemanticErrorCode code, const std::string& kind, const Node& node) const
    {
        if (!names.insert(name).second)
            throw_at(node, DuplicateDefinitionError(code, kind, name));
    }

    template<typename Node>
    void ensure_arity(const std::string& name, size_t expected, size_t actual, const Node& node) const
    {
        if (expected != actual)
            throw_at(node, ArityMismatchError(name, expected, actual));
    }

    ygg::Index<pddl::Type> intern_type(const std::string& name, ygg::IndexList<pddl::Type> bases)
    {
        auto k = key(name);
        if (auto it = m_types.find(k); it != m_types.end() && bases.empty())
            return it->second;
        auto view = build().type(repo(), to_cista(k), std::move(bases));
        m_types[k] = view.get_index();
        return view.get_index();
    }

    ygg::IndexList<pddl::Requirement> parse_requirements(const std::vector<ast::Requirement>& nodes)
    {
        auto result = ygg::IndexList<pddl::Requirement> {};
        for (const auto& node : nodes)
            result.push_back(build().requirement(repo(), requirement_kind(node)).get_index());
        return result;
    }

    pddl::RequirementKind requirement_kind(const ast::Requirement& node) const
    {
        auto name = key(node.name.text);
        if (name == "strips") return pddl::RequirementKind::Strips;
        if (name == "typing") return pddl::RequirementKind::Typing;
        if (name == "negative-preconditions") return pddl::RequirementKind::NegativePreconditions;
        if (name == "disjunctive-preconditions") return pddl::RequirementKind::DisjunctivePreconditions;
        if (name == "equality") return pddl::RequirementKind::Equality;
        if (name == "existential-preconditions") return pddl::RequirementKind::ExistentialPreconditions;
        if (name == "universal-preconditions") return pddl::RequirementKind::UniversalPreconditions;
        if (name == "quantified-preconditions") return pddl::RequirementKind::QuantifiedPreconditions;
        if (name == "conditional-effects") return pddl::RequirementKind::ConditionalEffects;
        if (name == "fluents") return pddl::RequirementKind::Fluents;
        if (name == "numeric-fluents") return pddl::RequirementKind::NumericFluents;
        if (name == "action-costs") return pddl::RequirementKind::NumericFluents;
        if (name == "adl") return pddl::RequirementKind::QuantifiedPreconditions;
        if (name == "durative-actions") return pddl::RequirementKind::DurativeActions;
        if (name == "derived-predicates") return pddl::RequirementKind::DerivedPredicates;
        if (name == "non-deterministic") return pddl::RequirementKind::NonDeterministic;
        if (name == "probabilistic-effects") return pddl::RequirementKind::ProbabilisticEffects;
        throw_at(node.name, UnsupportedRequirementError(name));
    }

    ygg::IndexList<pddl::Type> parse_types(const std::vector<ast::TypedName>& nodes)
    {
        auto result = ygg::IndexList<pddl::Type> {};
        for (const auto& node : nodes)
        {
            const auto name = key(node.name.text);
            ensure_new(m_declared_types, name, SemanticErrorCode::DuplicateType, "type", node.name);
            auto bases = node.type ? parse_type_expression(*node.type) : ygg::IndexList<pddl::Type> { m_object_type };
            result.push_back(intern_type(name, std::move(bases)));
        }
        return result;
    }

    ygg::IndexList<pddl::Type> parse_type_expression(const ast::TypeExpression& type)
    {
        return boost::apply_visitor([&](const auto& node) { return parse_type_expression_node(node); }, type);
    }

    ygg::IndexList<pddl::Type> parse_type_expression_node(const ast::TypeReference& node)
    {
        auto result = ygg::IndexList<pddl::Type> {};
        auto k = key(node.name.text);
        if (auto it = m_types.find(k); it != m_types.end())
            result.push_back(it->second);
        else if (m_options.strict)
            throw_at(node.name, UndefinedTypeError(k));
        else
            result.push_back(intern_type(k, {}));
        return result;
    }

    ygg::IndexList<pddl::Type> parse_type_expression_node(const ast::EitherType& node)
    {
        auto result = ygg::IndexList<pddl::Type> {};
        for (const auto& alternative : node.alternatives)
        {
            auto part = parse_type_expression(alternative.get());
            result.insert(result.end(), part.begin(), part.end());
        }
        return result;
    }

    ygg::IndexList<pddl::Object> parse_objects(const std::vector<ast::TypedName>& nodes, std::unordered_map<std::string, ygg::Index<pddl::Object>>& table)
    {
        auto result = ygg::IndexList<pddl::Object> {};
        for (const auto& node : nodes)
        {
            const auto name = key(node.name.text);
            ensure_new(m_declared_objects, name, SemanticErrorCode::DuplicateObject, "object", node.name);
            auto types = node.type ? parse_type_expression(*node.type) : ygg::IndexList<pddl::Type> { m_object_type };
            auto view = build().object(repo(), to_cista(name), std::move(types));
            table[name] = view.get_index();
            result.push_back(view.get_index());
        }
        return result;
    }

    ygg::IndexList<pddl::Parameter> parse_parameters(const std::vector<ast::TypedVariable>& nodes)
    {
        auto result = ygg::IndexList<pddl::Parameter> {};
        for (const auto& node : nodes)
        {
            const auto name = key(node.variable.text);
            if (!m_variable_scopes.empty() && m_variable_scopes.back().contains(name))
                throw_at(node.variable, DuplicateDefinitionError(SemanticErrorCode::DuplicateVariable, "variable", name));
            auto variable = build().variable(repo(), to_cista(name)).get_index();
            if (!m_variable_scopes.empty())
                m_variable_scopes.back()[name] = variable;
            auto types = node.type ? parse_type_expression(*node.type) : ygg::IndexList<pddl::Type> { m_object_type };
            result.push_back(build().parameter(repo(), variable, std::move(types)).get_index());
        }
        return result;
    }

    ygg::IndexList<pddl::Predicate> parse_predicates(const std::vector<ast::PredicateDeclaration>& nodes)
    {
        auto result = ygg::IndexList<pddl::Predicate> {};
        for (const auto& node : nodes)
        {
            m_variable_scopes.emplace_back();
            auto parameters = parse_parameters(node.parameters);
            m_variable_scopes.pop_back();
            const auto name = key(node.name.text);
            ensure_new(m_declared_predicates, name, SemanticErrorCode::DuplicatePredicate, "predicate", node.name);
            auto view = build().predicate(repo(), to_cista(name), std::move(parameters));
            m_predicates[name] = view.get_index();
            result.push_back(view.get_index());
        }
        return result;
    }

    ygg::IndexList<pddl::FunctionSkeleton> parse_functions(const std::vector<ast::FunctionDeclaration>& nodes)
    {
        auto result = ygg::IndexList<pddl::FunctionSkeleton> {};
        for (const auto& node : nodes)
        {
            m_variable_scopes.emplace_back();
            auto parameters = parse_parameters(node.parameters);
            m_variable_scopes.pop_back();
            const auto name = key(node.name.text);
            ensure_new(m_declared_functions, name, SemanticErrorCode::DuplicateFunction, "function", node.name);
            auto type = node.type ? parse_type_expression(*node.type).front() : m_number_type;
            auto view = build().function_skeleton(repo(), to_cista(name), std::move(parameters), type);
            m_functions[name] = view.get_index();
            result.push_back(view.get_index());
        }
        return result;
    }

    ygg::Index<pddl::Predicate> predicate(const ast::Identifier& identifier, size_t arity)
    {
        const auto name = key(identifier.text);
        if (auto it = m_predicates.find(name); it != m_predicates.end())
        {
            if (m_declared_predicates.contains(name))
                ensure_arity(name, repo()[it->second].parameters.size(), arity, identifier);
            return it->second;
        }
        if (m_options.strict)
            throw_at(identifier, UndefinedPredicateError(name));
        auto view = build().predicate(repo(), to_cista(name), {});
        m_predicates[name] = view.get_index();
        return view.get_index();
    }

    ygg::Index<pddl::FunctionSkeleton> function(const ast::Identifier& identifier, size_t arity)
    {
        const auto name = key(identifier.text);
        if (auto it = m_functions.find(name); it != m_functions.end())
        {
            if (m_declared_functions.contains(name))
                ensure_arity(name, repo()[it->second].parameters.size(), arity, identifier);
            return it->second;
        }
        if (m_options.strict)
            throw_at(identifier, UndefinedFunctionError(name));
        auto view = build().function_skeleton(repo(), to_cista(name), {}, m_number_type);
        m_functions[name] = view.get_index();
        return view.get_index();
    }

    ygg::Index<pddl::Variable> variable(const ast::Identifier& identifier) const
    {
        const auto name = key(identifier.text);
        for (auto it = m_variable_scopes.rbegin(); it != m_variable_scopes.rend(); ++it)
            if (auto found = it->find(name); found != it->end())
                return found->second;
        throw_at(identifier, UndefinedVariableError(identifier.text));
    }

    ygg::Index<pddl::Object> object(const ast::Identifier& identifier) const
    {
        const auto name = key(identifier.text);
        if (m_task_objects)
            if (auto it = m_task_objects->find(name); it != m_task_objects->end())
                return it->second;
        if (auto it = m_objects.find(name); it != m_objects.end())
            return it->second;
        throw_at(identifier, UndefinedObjectError(identifier.text));
    }

    ygg::Index<pddl::Term> parse_term(const ast::Term& node)
    {
        if (node.variable)
            return build().term(repo(), ygg::Data<pddl::Term>::Variant(variable(node.name))).get_index();
        return build().term(repo(), ygg::Data<pddl::Term>::Variant(object(node.name))).get_index();
    }

    ygg::IndexList<pddl::Term> parse_terms(const std::vector<ast::Term>& nodes)
    {
        auto result = ygg::IndexList<pddl::Term> {};
        for (const auto& node : nodes)
            result.push_back(parse_term(node));
        return result;
    }

    ygg::Index<pddl::Atom> parse_atom(const ast::Atom& node)
    {
        auto terms = parse_terms(node.terms);
        return build().atom(repo(), predicate(node.predicate, terms.size()), std::move(terms)).get_index();
    }

    ygg::Index<pddl::Literal> parse_literal(const ast::Literal& node)
    {
        return build().literal(repo(), node.positive, parse_atom(node.atom)).get_index();
    }

    ygg::Index<pddl::Condition> parse_condition(const ast::Condition& condition)
    {
        return boost::apply_visitor([&](const auto& node) { return parse_condition_node(node); }, condition);
    }

    ygg::Index<pddl::Condition> wrap_condition(ygg::Data<pddl::Condition>::Variant value)
    {
        return build().condition(repo(), std::move(value)).get_index();
    }

    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionLiteral& node) { return wrap_condition(build().condition_literal(repo(), parse_literal(node.literal)).get_index()); }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionAnd& node)
    {
        auto list = ygg::IndexList<pddl::Condition> {};
        for (const auto& child : node.conditions) list.push_back(parse_condition(child.get()));
        return wrap_condition(build().condition_and(repo(), std::move(list)).get_index());
    }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionOr& node)
    {
        auto list = ygg::IndexList<pddl::Condition> {};
        for (const auto& child : node.conditions) list.push_back(parse_condition(child.get()));
        return wrap_condition(build().condition_or(repo(), std::move(list)).get_index());
    }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionNot& node) { return wrap_condition(build().condition_not(repo(), parse_condition(node.condition.get())).get_index()); }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionImply& node) { return wrap_condition(build().condition_imply(repo(), parse_condition(node.left.get()), parse_condition(node.right.get())).get_index()); }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionExists& node)
    {
        m_variable_scopes.emplace_back();
        auto parameters = parse_parameters(node.parameters);
        auto child = parse_condition(node.condition.get());
        m_variable_scopes.pop_back();
        return wrap_condition(build().condition_exists(repo(), std::move(parameters), child).get_index());
    }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionForall& node)
    {
        m_variable_scopes.emplace_back();
        auto parameters = parse_parameters(node.parameters);
        auto child = parse_condition(node.condition.get());
        m_variable_scopes.pop_back();
        return wrap_condition(build().condition_forall(repo(), std::move(parameters), child).get_index());
    }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionNumericConstraint& node)
    {
        return wrap_condition(build().condition_numeric_constraint(repo(), comparator(node.comparator), parse_function_expression(node.left.get()), parse_function_expression(node.right.get())).get_index());
    }

    ygg::Index<pddl::FunctionTerm> parse_function_term(const ast::FunctionTerm& node)
    {
        auto terms = parse_terms(node.terms);
        return build().function_term(repo(), function(node.function, terms.size()), std::move(terms)).get_index();
    }

    ygg::Index<pddl::FunctionExpression> parse_function_expression(const ast::FunctionExpression& expression)
    {
        return boost::apply_visitor([&](const auto& node) { return parse_function_expression_node(node); }, expression);
    }

    ygg::Index<pddl::FunctionExpression> wrap_function_expression(ygg::Data<pddl::FunctionExpression>::Variant value)
    {
        return build().function_expression(repo(), std::move(value)).get_index();
    }
    ygg::Index<pddl::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionNumber& node) { return wrap_function_expression(build().number(repo(), node.value).get_index()); }
    ygg::Index<pddl::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionFunction& node) { return wrap_function_expression(parse_function_term(node.term)); }
    ygg::Index<pddl::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionUnary& node) { return wrap_function_expression(build().unary_function_expression(repo(), pddl::UnaryArithmeticOperator::Minus, parse_function_expression(node.expression.get())).get_index()); }
    ygg::Index<pddl::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionBinary& node) { return wrap_function_expression(build().binary_function_expression(repo(), binary_operator(node.op), parse_function_expression(node.left.get()), parse_function_expression(node.right.get())).get_index()); }
    ygg::Index<pddl::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionMulti& node)
    {
        auto expressions = ygg::IndexList<pddl::FunctionExpression> {};
        for (const auto& child : node.expressions) expressions.push_back(parse_function_expression(child.get()));
        return wrap_function_expression(build().multi_function_expression(repo(), multi_operator(node.op), std::move(expressions)).get_index());
    }

    ygg::Index<pddl::Effect> parse_effect(const ast::Effect& effect)
    {
        return boost::apply_visitor([&](const auto& node) { return parse_effect_node(node); }, effect);
    }
    ygg::Index<pddl::Effect> wrap_effect(ygg::Data<pddl::Effect>::Variant value) { return build().effect(repo(), std::move(value)).get_index(); }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectLiteral& node) { return wrap_effect(build().effect_literal(repo(), parse_literal(node.literal)).get_index()); }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectAnd& node)
    {
        auto list = ygg::IndexList<pddl::Effect> {};
        for (const auto& child : node.effects) list.push_back(parse_effect(child.get()));
        return wrap_effect(build().effect_and(repo(), std::move(list)).get_index());
    }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectNumeric& node) { return wrap_effect(build().effect_numeric(repo(), numeric_effect_operator(node.op), function(node.function.function, node.function.terms.size()), parse_terms(node.function.terms), parse_function_expression(node.expression.get())).get_index()); }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectForall& node)
    {
        m_variable_scopes.emplace_back();
        auto parameters = parse_parameters(node.parameters);
        auto child = parse_effect(node.effect.get());
        m_variable_scopes.pop_back();
        return wrap_effect(build().effect_forall(repo(), std::move(parameters), child).get_index());
    }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectWhen& node) { return wrap_effect(build().effect_when(repo(), parse_condition(node.condition.get()), parse_effect(node.effect.get())).get_index()); }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectOneOf& node)
    {
        auto list = ygg::IndexList<pddl::Effect> {};
        for (const auto& child : node.effects) list.push_back(parse_effect(child.get()));
        return wrap_effect(build().effect_one_of(repo(), std::move(list)).get_index());
    }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectProbabilistic& node)
    {
        auto list = ygg::IndexList<pddl::EffectProbabilisticAlternative> {};
        for (const auto& alternative : node.alternatives)
            list.push_back(build().effect_probabilistic_alternative(repo(), alternative.probability, parse_effect(alternative.effect.get())).get_index());
        return wrap_effect(build().effect_probabilistic(repo(), std::move(list)).get_index());
    }

    ygg::Index<pddl::Action> parse_action(const ast::Action& node)
    {
        m_variable_scopes.emplace_back();
        auto parameters = parse_parameters(node.parameters);
        auto precondition = cista::optional<ygg::Index<pddl::Condition>> {};
        if (node.precondition) precondition = parse_condition(*node.precondition);
        auto effect = cista::optional<ygg::Index<pddl::Effect>> {};
        if (node.effect) effect = parse_effect(*node.effect);
        m_variable_scopes.pop_back();
        return build().action(repo(), to_cista(node.name.text), std::move(parameters), precondition, effect).get_index();
    }

    ygg::Index<pddl::Axiom> parse_axiom(const ast::Axiom& node)
    {
        m_variable_scopes.emplace_back();
        auto parameters = parse_parameters(node.head.parameters);
        auto terms = ygg::IndexList<pddl::Term> {};
        for (const auto& parameter : node.head.parameters)
        {
            auto term = ast::Term {};
            term.name = parameter.variable;
            term.variable = true;
            terms.push_back(parse_term(term));
        }
        auto pred = predicate(node.head.name, terms.size());
        auto atom = build().atom(repo(), pred, std::move(terms)).get_index();
        auto head = build().literal(repo(), true, atom).get_index();
        auto condition = parse_condition(node.condition);
        m_variable_scopes.pop_back();
        return build().axiom(repo(), std::move(parameters), head, condition).get_index();
    }

    void parse_initial_element(const ast::Literal& literal, ygg::IndexList<pddl::Literal>& literals, ygg::IndexList<pddl::InitialFunctionValue>&) { literals.push_back(parse_literal(literal)); }
    void parse_initial_element(const ast::InitialFunctionValue& value, ygg::IndexList<pddl::Literal>&, ygg::IndexList<pddl::InitialFunctionValue>& values) { values.push_back(build().initial_function_value(repo(), parse_function_term(value.function), parse_function_expression(value.value)).get_index()); }

    ygg::Index<pddl::Metric> parse_metric(const ast::Metric& node)
    {
        return build().metric(repo(), key(node.optimization.text) != "maximize", parse_function_expression(node.expression)).get_index();
    }

    static pddl::BinaryComparator comparator(std::string op)
    {
        op = key(std::move(op));
        if (op == "!=") return pddl::BinaryComparator::NotEqual;
        if (op == "<") return pddl::BinaryComparator::Less;
        if (op == "<=") return pddl::BinaryComparator::LessEqual;
        if (op == ">") return pddl::BinaryComparator::Greater;
        if (op == ">=") return pddl::BinaryComparator::GreaterEqual;
        return pddl::BinaryComparator::Equal;
    }
    static pddl::BinaryArithmeticOperator binary_operator(std::string op)
    {
        op = key(std::move(op));
        if (op == "/") return pddl::BinaryArithmeticOperator::Divide;
        return pddl::BinaryArithmeticOperator::Subtract;
    }
    static pddl::MultiArithmeticOperator multi_operator(std::string op) { return key(std::move(op)) == "*" ? pddl::MultiArithmeticOperator::Multiply : pddl::MultiArithmeticOperator::Add; }
    static pddl::NumericEffectOperator numeric_effect_operator(std::string op)
    {
        op = key(std::move(op));
        if (op == "increase") return pddl::NumericEffectOperator::Increase;
        if (op == "decrease") return pddl::NumericEffectOperator::Decrease;
        if (op == "scale-up") return pddl::NumericEffectOperator::ScaleUp;
        if (op == "scale-down") return pddl::NumericEffectOperator::ScaleDown;
        return pddl::NumericEffectOperator::Assign;
    }
};

} // namespace loki2::semantic

#endif
