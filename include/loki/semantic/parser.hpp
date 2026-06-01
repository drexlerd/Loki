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
#include "loki/parser.hpp"
#include "loki/semantic/errors.hpp"
#include "loki/pddl/pddl.hpp"
#include "loki/semantic/translator.hpp"

#include <boost/optional.hpp>
#include <boost/variant/apply_visitor.hpp>

#include <cista/containers/optional.h>
#include <cista/containers/string.h>

#include <filesystem>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <type_traits>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <utility>
#include <vector>

namespace loki::semantic
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
        if (!parser::parse_full(first, source.cend(), parser::domain(), domain_ast, error_handler, m_options))
            throw parse_error(error_handler, "Could not parse PDDL domain.", first);
        auto scope = ErrorHandlerScope { *this, error_handler };
        return parse_domain_ast(domain_ast);
    }

    pddl::DomainView parse_domain(const fs::path& path) { return parse_domain(read_file(path)); }

    pddl::TaskView parse_task(const std::string& source)
    {
        auto first = source.cbegin();
        parser::ErrorHandlerType error_handler(first, source.cend(), std::cerr);
        ast::Task task_ast;
        if (!parser::parse_full(first, source.cend(), parser::task(), task_ast, error_handler, m_options))
            throw parse_error(error_handler, "Could not parse PDDL task.", first);
        auto scope = ErrorHandlerScope { *this, error_handler };
        return parse_task_ast(task_ast);
    }

    pddl::TaskView parse_task(const fs::path& path) { return parse_task(read_file(path)); }

private:
    pddl::DomainView parse_domain_ast(const ast::Domain& domain)
    {
        clear_domain_symbols();
        m_object_type = intern_type("object", {});
        m_number_type = intern_type("number", {});

        auto requirements = parse_requirements(domain.requirements);
        m_domain_requirement_kinds = m_active_requirements;
        auto types = parse_types(domain.types);
        auto constants = parse_objects(domain.constants, m_objects);
        auto predicates = parse_predicates(domain.predicates);
        auto functions = parse_functions(domain.functions);
        auto axioms = ygg::IndexList<pddl::Axiom> {};
        for (const auto& axiom : domain.axioms)
        {
            require_requirement(pddl::RequirementKind::DerivedPredicates, axiom);
            axioms.push_back(parse_axiom(axiom));
        }
        auto actions = ygg::IndexList<pddl::Action> {};
        for (const auto& action : domain.actions)
            actions.push_back(parse_action(action));

        types.clear();
        for (auto i = ygg::uint_t { 0 }; i < repo().size<pddl::Type>(); ++i)
            types.push_back(ygg::Index<pddl::Type>(i));
        auto data = ygg::Data<pddl::Domain>(to_cista(domain.name.text), std::move(requirements), std::move(types), std::move(constants), std::move(predicates), std::move(functions), std::move(actions), std::move(axioms));
        auto view = pddl::get_or_create<pddl::Domain>(repo(), std::move(data));
        const auto declared_requirements = m_domain_requirement_kinds;
        canonicalize_domain(view);
        m_domain_requirement_kinds = declared_requirements;
        m_active_requirements = declared_requirements;
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

        struct RequirementScope
        {
            Parser& parser;
            std::unordered_set<pddl::RequirementKind> previous;
            ~RequirementScope() { parser.m_active_requirements = std::move(previous); }
        };
        RequirementScope requirement_scope { *this, m_active_requirements };
        m_active_requirements = m_domain_requirement_kinds;
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
        {
            require_requirement(pddl::RequirementKind::DerivedPredicates, axiom);
            axioms.push_back(parse_axiom(axiom));
        }

        auto data = ygg::Data<pddl::Task>(to_cista(task.name.text), *m_domain, std::move(requirements), std::move(objects), std::move(initial_literals), std::move(initial_function_values), goal, metric, std::move(axioms));
        auto view = pddl::get_or_create<pddl::Task>(repo(), std::move(data));
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
    std::unordered_set<std::string> m_declared_actions;
    std::unordered_set<pddl::RequirementKind> m_active_requirements;
    std::unordered_set<pddl::RequirementKind> m_domain_requirement_kinds;
    std::unordered_map<ygg::uint_t, ygg::IndexList<pddl::Type>> m_variable_types;
    std::vector<std::unordered_map<std::string, ygg::Index<pddl::Variable>>> m_variable_scopes;
    std::unordered_map<std::string, ygg::Index<pddl::Object>>* m_task_objects = nullptr;

    static cista::offset::string to_cista(const std::string& text) { return cista::offset::string(text); }

    pddl::Repository& repo() noexcept { return m_storage->repository; }
    const pddl::Repository& repo() const noexcept { return m_storage->repository; }

    static ParseError parse_error(const parser::ErrorHandlerType& error_handler, const std::string& fallback, parser::Iterator position)
    {
        if (const auto& diagnostic = error_handler.last_error())
        {
            const auto source_position = parser::source_position(error_handler, diagnostic->position);
            return ParseError(diagnostic->message, parser::SourceRange { source_position, source_position });
        }
        const auto source_position = parser::source_position(error_handler, position);
        return ParseError(fallback, parser::SourceRange { source_position, source_position });
    }

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
        m_declared_actions.clear();
        m_active_requirements.clear();
        m_domain_requirement_kinds.clear();
        m_variable_types.clear();
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
        for (auto requirement : domain.requirements)
            remember_requirement(repo()[requirement].kind);
        m_domain_requirement_kinds = m_active_requirements;
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
        for (auto action : domain.actions)
            m_declared_actions.insert(std::string(repo()[action].name));

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
        m_declared_actions.clear();
        m_active_requirements.clear();
        m_domain_requirement_kinds.clear();
        m_variable_types.clear();
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

    static std::string requirement_name(pddl::RequirementKind kind)
    {
        switch (kind)
        {
            case pddl::RequirementKind::Typing: return "typing";
            case pddl::RequirementKind::NegativePreconditions: return "negative-preconditions";
            case pddl::RequirementKind::DisjunctivePreconditions: return "disjunctive-preconditions";
            case pddl::RequirementKind::Equality: return "equality";
            case pddl::RequirementKind::ExistentialPreconditions: return "existential-preconditions";
            case pddl::RequirementKind::UniversalPreconditions: return "universal-preconditions";
            case pddl::RequirementKind::QuantifiedPreconditions: return "quantified-preconditions";
            case pddl::RequirementKind::ConditionalEffects: return "conditional-effects";
            case pddl::RequirementKind::Fluents: return "fluents";
            case pddl::RequirementKind::NumericFluents: return "numeric-fluents";
            case pddl::RequirementKind::DurativeActions: return "durative-actions";
            case pddl::RequirementKind::DerivedPredicates: return "derived-predicates";
            case pddl::RequirementKind::NonDeterministic: return "non-deterministic";
            case pddl::RequirementKind::ProbabilisticEffects: return "probabilistic-effects";
            case pddl::RequirementKind::Strips: return "strips";
        }
        return "unknown";
    }

    void remember_requirement(pddl::RequirementKind kind)
    {
        m_active_requirements.insert(kind);
        if (kind == pddl::RequirementKind::QuantifiedPreconditions)
        {
            m_active_requirements.insert(pddl::RequirementKind::ExistentialPreconditions);
            m_active_requirements.insert(pddl::RequirementKind::UniversalPreconditions);
        }
        if (kind == pddl::RequirementKind::Fluents)
            m_active_requirements.insert(pddl::RequirementKind::NumericFluents);
    }

    void remember_adl_requirements()
    {
        remember_requirement(pddl::RequirementKind::Typing);
        remember_requirement(pddl::RequirementKind::NegativePreconditions);
        remember_requirement(pddl::RequirementKind::DisjunctivePreconditions);
        remember_requirement(pddl::RequirementKind::Equality);
        remember_requirement(pddl::RequirementKind::QuantifiedPreconditions);
        remember_requirement(pddl::RequirementKind::ConditionalEffects);
    }

    template<typename Node>
    void require_requirement(pddl::RequirementKind kind, const Node& node) const
    {
        if (!m_options.strict)
            return;
        if (!m_active_requirements.contains(kind))
            throw_at(node, MissingRequirementError(requirement_name(kind)));
    }

    void require_typing_if_needed(const boost::optional<ast::TypeExpression>& type, const boost::spirit::x3::position_tagged& node) const
    {
        if (type)
            require_requirement(pddl::RequirementKind::Typing, node);
    }

    bool is_subtype(ygg::Index<pddl::Type> actual, ygg::Index<pddl::Type> expected) const
    {
        auto seen = std::unordered_set<ygg::uint_t> {};
        return is_subtype(actual, expected, seen);
    }

    bool is_subtype(ygg::Index<pddl::Type> actual, ygg::Index<pddl::Type> expected, std::unordered_set<ygg::uint_t>& seen) const
    {
        if (actual == expected)
            return true;
        if (!seen.insert(ygg::uint_t(actual)).second)
            return false;
        for (auto base : repo()[actual].bases)
            if (is_subtype(base, expected, seen))
                return true;
        return false;
    }

    bool types_compatible(const ygg::IndexList<pddl::Type>& actual_types, const ygg::IndexList<pddl::Type>& expected_types) const
    {
        if (expected_types.empty() || actual_types.empty())
            return true;
        for (auto actual : actual_types)
            for (auto expected : expected_types)
                if (is_subtype(actual, expected))
                    return true;
        return false;
    }

    ygg::IndexList<pddl::Type> term_types(const ast::Term& term) const
    {
        if (!term.variable)
            return repo()[object(term.name)].types;
        auto variable_index = variable(term.name);
        if (auto it = m_variable_types.find(ygg::uint_t(variable_index)); it != m_variable_types.end())
            return it->second;
        return ygg::IndexList<pddl::Type> { m_object_type };
    }

    template<typename Node>
    void check_argument_types(const std::string& name, const ygg::IndexList<pddl::Parameter>& parameters, const std::vector<ast::Term>& terms, const Node& node) const
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
        auto view = pddl::get_or_create<pddl::Type>(repo(), to_cista(k), std::move(bases));
        m_types[k] = view.get_index();
        return view.get_index();
    }

    ygg::IndexList<pddl::Requirement> parse_requirements(const std::vector<ast::Requirement>& nodes)
    {
        auto result = ygg::IndexList<pddl::Requirement> {};
        for (const auto& node : nodes)
        {
            const auto kind = requirement_kind(node);
            if (key(node.name.text) == "adl")
                remember_adl_requirements();
            else
                remember_requirement(kind);
            result.push_back(pddl::get_or_create<pddl::Requirement>(repo(), kind).get_index());
        }
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
        if (!nodes.empty())
            require_requirement(pddl::RequirementKind::Typing, nodes.front().name);
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
            require_typing_if_needed(node.type, node.name);
            auto types = node.type ? parse_type_expression(*node.type) : ygg::IndexList<pddl::Type> { m_object_type };
            auto view = pddl::get_or_create<pddl::Object>(repo(), to_cista(name), std::move(types));
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
            auto variable = pddl::get_or_create<pddl::Variable>(repo(), to_cista(name)).get_index();
            if (!m_variable_scopes.empty())
                m_variable_scopes.back()[name] = variable;
            require_typing_if_needed(node.type, node.variable);
            auto types = node.type ? parse_type_expression(*node.type) : ygg::IndexList<pddl::Type> { m_object_type };
            m_variable_types[ygg::uint_t(variable)] = types;
            result.push_back(pddl::get_or_create<pddl::Parameter>(repo(), variable, std::move(types)).get_index());
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
            auto view = pddl::get_or_create<pddl::Predicate>(repo(), to_cista(name), std::move(parameters));
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
            require_requirement(pddl::RequirementKind::NumericFluents, node.name);
            ensure_new(m_declared_functions, name, SemanticErrorCode::DuplicateFunction, "function", node.name);
            require_typing_if_needed(node.type, node.name);
            auto type = node.type ? parse_type_expression(*node.type).front() : m_number_type;
            auto view = pddl::get_or_create<pddl::FunctionSkeleton>(repo(), to_cista(name), std::move(parameters), type);
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
        auto view = pddl::get_or_create<pddl::Predicate>(repo(), to_cista(name), ygg::IndexList<pddl::Parameter> {});
        m_predicates[name] = view.get_index();
        return view.get_index();
    }

    ygg::Index<pddl::Predicate> equality_predicate(const ast::Identifier& identifier, size_t arity)
    {
        require_requirement(pddl::RequirementKind::Equality, identifier);
        if (arity != 2)
            throw_at(identifier, InvalidEqualityError("expected 2 terms, got " + std::to_string(arity)));

        const auto name = key(identifier.text);
        if (auto it = m_predicates.find(name); it != m_predicates.end())
        {
            if (m_declared_predicates.contains(name))
                ensure_arity(name, repo()[it->second].parameters.size(), arity, identifier);
            return it->second;
        }

        auto types = ygg::IndexList<pddl::Type> { m_object_type };
        auto parameters = ygg::IndexList<pddl::Parameter> {};
        const auto left = pddl::get_or_create<pddl::Variable>(repo(), cista::offset::string("lhs")).get_index();
        const auto right = pddl::get_or_create<pddl::Variable>(repo(), cista::offset::string("rhs")).get_index();
        parameters.push_back(pddl::get_or_create<pddl::Parameter>(repo(), left, types).get_index());
        parameters.push_back(pddl::get_or_create<pddl::Parameter>(repo(), right, std::move(types)).get_index());
        auto view = pddl::get_or_create<pddl::Predicate>(repo(), cista::offset::string("="), std::move(parameters));
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
        auto view = pddl::get_or_create<pddl::FunctionSkeleton>(repo(), to_cista(name), ygg::IndexList<pddl::Parameter> {}, m_number_type);
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
            return pddl::get_or_create<pddl::Term>(repo(), ygg::Data<pddl::Term>::Variant(variable(node.name))).get_index();
        return pddl::get_or_create<pddl::Term>(repo(), ygg::Data<pddl::Term>::Variant(object(node.name))).get_index();
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
        const auto name = key(node.predicate.text);
        auto pred = name == "=" ? equality_predicate(node.predicate, node.terms.size()) : predicate(node.predicate, node.terms.size());
        if (m_declared_predicates.contains(name))
            check_argument_types(name, repo()[pred].parameters, node.terms, node.predicate);
        auto terms = parse_terms(node.terms);
        return pddl::get_or_create<pddl::Atom>(repo(), pred, std::move(terms)).get_index();
    }

    ygg::Index<pddl::Literal> parse_literal(const ast::Literal& node)
    {
        return pddl::get_or_create<pddl::Literal>(repo(), node.positive, parse_atom(node.atom)).get_index();
    }

    ygg::Index<pddl::Condition> parse_condition(const ast::Condition& condition)
    {
        return boost::apply_visitor([&](const auto& node) { return parse_condition_node(node); }, condition);
    }

    ygg::Index<pddl::Condition> wrap_condition(ygg::Data<pddl::Condition>::Variant value)
    {
        return pddl::get_or_create<pddl::Condition>(repo(), std::move(value)).get_index();
    }

    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionLiteral& node) { return wrap_condition(pddl::get_or_create<pddl::ConditionLiteral>(repo(), parse_literal(node.literal)).get_index()); }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionAnd& node)
    {
        auto list = ygg::IndexList<pddl::Condition> {};
        for (const auto& child : node.conditions) list.push_back(parse_condition(child.get()));
        return wrap_condition(pddl::get_or_create<pddl::ConditionAnd>(repo(), std::move(list)).get_index());
    }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionOr& node)
    {
        require_requirement(pddl::RequirementKind::DisjunctivePreconditions, node);
        auto list = ygg::IndexList<pddl::Condition> {};
        for (const auto& child : node.conditions) list.push_back(parse_condition(child.get()));
        return wrap_condition(pddl::get_or_create<pddl::ConditionOr>(repo(), std::move(list)).get_index());
    }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionNot& node)
    {
        require_requirement(pddl::RequirementKind::NegativePreconditions, node);
        return wrap_condition(pddl::get_or_create<pddl::ConditionNot>(repo(), parse_condition(node.condition.get())).get_index());
    }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionImply& node) { return wrap_condition(pddl::get_or_create<pddl::ConditionImply>(repo(), parse_condition(node.left.get()), parse_condition(node.right.get())).get_index()); }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionExists& node)
    {
        require_requirement(pddl::RequirementKind::ExistentialPreconditions, node);
        m_variable_scopes.emplace_back();
        auto parameters = parse_parameters(node.parameters);
        auto child = parse_condition(node.condition.get());
        m_variable_scopes.pop_back();
        return wrap_condition(pddl::get_or_create<pddl::ConditionExists>(repo(), std::move(parameters), child).get_index());
    }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionForall& node)
    {
        require_requirement(pddl::RequirementKind::UniversalPreconditions, node);
        m_variable_scopes.emplace_back();
        auto parameters = parse_parameters(node.parameters);
        auto child = parse_condition(node.condition.get());
        m_variable_scopes.pop_back();
        return wrap_condition(pddl::get_or_create<pddl::ConditionForall>(repo(), std::move(parameters), child).get_index());
    }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionNumericConstraint& node)
    {
        require_requirement(pddl::RequirementKind::NumericFluents, node);
        return wrap_condition(pddl::get_or_create<pddl::ConditionNumericConstraint>(repo(), comparator(node), parse_function_expression(node.left.get()), parse_function_expression(node.right.get())).get_index());
    }

    ygg::Index<pddl::FunctionTerm> parse_function_term(const ast::FunctionTerm& node)
    {
        auto skeleton = function(node.function, node.terms.size());
        if (m_declared_functions.contains(key(node.function.text)))
            check_argument_types(key(node.function.text), repo()[skeleton].parameters, node.terms, node.function);
        auto terms = parse_terms(node.terms);
        return pddl::get_or_create<pddl::FunctionTerm>(repo(), skeleton, std::move(terms)).get_index();
    }

    ygg::Index<pddl::FunctionExpression> parse_function_expression(const ast::FunctionExpression& expression)
    {
        return boost::apply_visitor([&](const auto& node) { return parse_function_expression_node(node); }, expression);
    }

    ygg::Index<pddl::FunctionExpression> wrap_function_expression(ygg::Data<pddl::FunctionExpression>::Variant value)
    {
        return pddl::get_or_create<pddl::FunctionExpression>(repo(), std::move(value)).get_index();
    }
    ygg::Index<pddl::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionNumber& node) { return wrap_function_expression(pddl::get_or_create<pddl::FunctionExpressionNumber>(repo(), node.value).get_index()); }
    ygg::Index<pddl::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionFunction& node) { return wrap_function_expression(parse_function_term(node.term)); }
    ygg::Index<pddl::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionUnary& node) { return wrap_function_expression(pddl::get_or_create<pddl::UnaryFunctionExpression>(repo(), pddl::UnaryArithmeticOperator::Minus, parse_function_expression(node.expression.get())).get_index()); }
    ygg::Index<pddl::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionBinary& node) { return wrap_function_expression(pddl::get_or_create<pddl::BinaryFunctionExpression>(repo(), binary_operator(node.op), parse_function_expression(node.left.get()), parse_function_expression(node.right.get())).get_index()); }
    ygg::Index<pddl::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionMulti& node)
    {
        auto expressions = ygg::IndexList<pddl::FunctionExpression> {};
        for (const auto& child : node.expressions) expressions.push_back(parse_function_expression(child.get()));
        return wrap_function_expression(pddl::get_or_create<pddl::MultiFunctionExpression>(repo(), multi_operator(node.op), std::move(expressions)).get_index());
    }

    ygg::Index<pddl::Effect> parse_effect(const ast::Effect& effect)
    {
        return boost::apply_visitor([&](const auto& node) { return parse_effect_node(node); }, effect);
    }
    ygg::Index<pddl::Effect> wrap_effect(ygg::Data<pddl::Effect>::Variant value) { return pddl::get_or_create<pddl::Effect>(repo(), std::move(value)).get_index(); }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectLiteral& node) { return wrap_effect(pddl::get_or_create<pddl::EffectLiteral>(repo(), parse_literal(node.literal)).get_index()); }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectAnd& node)
    {
        auto list = ygg::IndexList<pddl::Effect> {};
        for (const auto& child : node.effects) list.push_back(parse_effect(child.get()));
        return wrap_effect(pddl::get_or_create<pddl::EffectAnd>(repo(), std::move(list)).get_index());
    }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectNumeric& node)
    {
        require_requirement(pddl::RequirementKind::NumericFluents, node);
        auto skeleton = function(node.function.function, node.function.terms.size());
        if (m_declared_functions.contains(key(node.function.function.text)))
            check_argument_types(key(node.function.function.text), repo()[skeleton].parameters, node.function.terms, node.function.function);
        return wrap_effect(pddl::get_or_create<pddl::EffectNumeric>(repo(), numeric_effect_operator(node), skeleton, parse_terms(node.function.terms), parse_function_expression(node.expression.get())).get_index());
    }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectForall& node)
    {
        require_requirement(pddl::RequirementKind::UniversalPreconditions, node);
        m_variable_scopes.emplace_back();
        auto parameters = parse_parameters(node.parameters);
        auto child = parse_effect(node.effect.get());
        m_variable_scopes.pop_back();
        return wrap_effect(pddl::get_or_create<pddl::EffectForall>(repo(), std::move(parameters), child).get_index());
    }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectWhen& node)
    {
        require_requirement(pddl::RequirementKind::ConditionalEffects, node);
        return wrap_effect(pddl::get_or_create<pddl::EffectWhen>(repo(), parse_condition(node.condition.get()), parse_effect(node.effect.get())).get_index());
    }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectOneOf& node)
    {
        require_requirement(pddl::RequirementKind::NonDeterministic, node);
        auto list = ygg::IndexList<pddl::Effect> {};
        for (const auto& child : node.effects) list.push_back(parse_effect(child.get()));
        return wrap_effect(pddl::get_or_create<pddl::EffectOneOf>(repo(), std::move(list)).get_index());
    }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectProbabilistic& node)
    {
        require_requirement(pddl::RequirementKind::ProbabilisticEffects, node);
        auto list = ygg::IndexList<pddl::EffectProbabilisticAlternative> {};
        auto total = 0.0;
        for (const auto& alternative : node.alternatives)
        {
            if (!std::isfinite(alternative.probability) || alternative.probability < 0.0 || alternative.probability > 1.0)
                throw_at(alternative, InvalidProbabilisticEffectError("probability must be in [0, 1]"));
            total += alternative.probability;
            list.push_back(pddl::get_or_create<pddl::EffectProbabilisticAlternative>(repo(), alternative.probability, parse_effect(alternative.effect.get())).get_index());
        }
        if (total > 1.0 + 1e-9)
            throw_at(node, InvalidProbabilisticEffectError("probabilities sum to more than 1"));
        return wrap_effect(pddl::get_or_create<pddl::EffectProbabilistic>(repo(), std::move(list)).get_index());
    }

    ygg::Index<pddl::Action> parse_action(const ast::Action& node)
    {
        const auto name = key(node.name.text);
        ensure_new(m_declared_actions, name, SemanticErrorCode::DuplicateAction, "action", node.name);
        m_variable_scopes.emplace_back();
        auto parameters = parse_parameters(node.parameters);
        auto precondition = cista::optional<ygg::Index<pddl::Condition>> {};
        if (node.precondition) precondition = parse_condition(*node.precondition);
        auto effect = cista::optional<ygg::Index<pddl::Effect>> {};
        if (node.effect) effect = parse_effect(*node.effect);
        m_variable_scopes.pop_back();
        return pddl::get_or_create<pddl::Action>(repo(), to_cista(name), std::move(parameters), precondition, effect).get_index();
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
        auto atom = pddl::get_or_create<pddl::Atom>(repo(), pred, std::move(terms)).get_index();
        auto head = pddl::get_or_create<pddl::Literal>(repo(), true, atom).get_index();
        auto condition = parse_condition(node.condition);
        m_variable_scopes.pop_back();
        return pddl::get_or_create<pddl::Axiom>(repo(), std::move(parameters), head, condition).get_index();
    }

    void parse_initial_element(const ast::Literal& literal, ygg::IndexList<pddl::Literal>& literals, ygg::IndexList<pddl::InitialFunctionValue>&) { literals.push_back(parse_literal(literal)); }
    void parse_initial_element(const ast::InitialFunctionValue& value, ygg::IndexList<pddl::Literal>&, ygg::IndexList<pddl::InitialFunctionValue>& values) { values.push_back(pddl::get_or_create<pddl::InitialFunctionValue>(repo(), parse_function_term(value.function), parse_function_expression(value.value)).get_index()); }

    ygg::Index<pddl::Metric> parse_metric(const ast::Metric& node)
    {
        require_requirement(pddl::RequirementKind::NumericFluents, node);
        const auto optimization = key(node.optimization.text);
        if (optimization != "minimize" && optimization != "maximize")
            throw_at(node.optimization, InvalidMetricError(optimization));
        return pddl::get_or_create<pddl::Metric>(repo(), optimization == "minimize", parse_function_expression(node.expression)).get_index();
    }

    pddl::BinaryComparator comparator(const ast::ConditionNumericConstraint& node) const
    {
        auto op = key(node.comparator);
        if (op == "=") return pddl::BinaryComparator::Equal;
        if (op == "!=") return pddl::BinaryComparator::NotEqual;
        if (op == "<") return pddl::BinaryComparator::Less;
        if (op == "<=") return pddl::BinaryComparator::LessEqual;
        if (op == ">") return pddl::BinaryComparator::Greater;
        if (op == ">=") return pddl::BinaryComparator::GreaterEqual;
        throw_at(node, InvalidNumericConstraintError(op));
    }
    static pddl::BinaryArithmeticOperator binary_operator(std::string op)
    {
        op = key(std::move(op));
        if (op == "/") return pddl::BinaryArithmeticOperator::Divide;
        return pddl::BinaryArithmeticOperator::Subtract;
    }
    static pddl::MultiArithmeticOperator multi_operator(std::string op) { return key(std::move(op)) == "*" ? pddl::MultiArithmeticOperator::Multiply : pddl::MultiArithmeticOperator::Add; }
    pddl::NumericEffectOperator numeric_effect_operator(const ast::EffectNumeric& node) const
    {
        auto op = key(node.op);
        if (op == "assign") return pddl::NumericEffectOperator::Assign;
        if (op == "increase") return pddl::NumericEffectOperator::Increase;
        if (op == "decrease") return pddl::NumericEffectOperator::Decrease;
        if (op == "scale-up") return pddl::NumericEffectOperator::ScaleUp;
        if (op == "scale-down") return pddl::NumericEffectOperator::ScaleDown;
        throw_at(node, InvalidNumericEffectError(op));
    }
};

} // namespace loki::semantic

#endif
