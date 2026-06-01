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
#include "loki/formalism/formalism.hpp"
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

std::string read_file(const fs::path& path);

class Parser
{
public:
    explicit Parser(parser::ParserOptions options = {});

    const formalism::Repository& repository() const noexcept;
    formalism::Repository& repository() noexcept;

    bool has_domain() const noexcept;
    formalism::DomainView get_domain() const;

    formalism::DomainView parse_domain(const std::string& source);
    formalism::DomainView parse_domain(const fs::path& path);

    formalism::TaskView parse_task(const std::string& source);
    formalism::TaskView parse_task(const fs::path& path);

private:
    formalism::DomainView parse_domain_ast(const ast::Domain& domain)
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
        auto axioms = ygg::IndexList<formalism::Axiom> {};
        for (const auto& axiom : domain.axioms)
        {
            require_requirement(formalism::RequirementKind::DerivedPredicates, axiom);
            axioms.push_back(parse_axiom(axiom));
        }
        auto actions = ygg::IndexList<formalism::Action> {};
        for (const auto& action : domain.actions)
            actions.push_back(parse_action(action));

        types.clear();
        for (auto i = ygg::uint_t { 0 }; i < repo().size<formalism::Type>(); ++i)
            types.push_back(ygg::Index<formalism::Type>(i));
        auto data = ygg::Data<formalism::Domain>(to_cista(domain.name.text), std::move(requirements), std::move(types), std::move(constants), std::move(predicates), std::move(functions), std::move(actions), std::move(axioms));
        auto view = formalism::get_or_create<formalism::Domain>(repo(), std::move(data));
        const auto declared_requirements = m_domain_requirement_kinds;
        canonicalize_domain(view);
        m_domain_requirement_kinds = declared_requirements;
        m_active_requirements = declared_requirements;
        m_domain_name = domain.name.text;
        return get_domain();
    }

    formalism::TaskView parse_task_ast(const ast::Task& task)
    {
        if (!m_domain)
            throw_at(task, MissingDomainError("Cannot parse task before parsing a domain."));
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

        auto task_objects = std::unordered_map<std::string, ygg::Index<formalism::Object>> {};
        struct TaskObjectScope
        {
            Parser& parser;
            std::unordered_map<std::string, ygg::Index<formalism::Object>>* previous;
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
            std::unordered_set<formalism::RequirementKind> previous;
            ~RequirementScope() { parser.m_active_requirements = std::move(previous); }
        };
        RequirementScope requirement_scope { *this, m_active_requirements };
        m_active_requirements = m_domain_requirement_kinds;
        auto requirements = parse_requirements(task.requirements);
        auto objects = parse_objects(task.objects, task_objects);
        auto initial_literals = ygg::IndexList<formalism::Literal> {};
        auto initial_function_values = ygg::IndexList<formalism::InitialFunctionValue> {};
        for (const auto& element : task.initial)
        {
            boost::apply_visitor([&](const auto& node) { parse_initial_element(node, initial_literals, initial_function_values); }, element);
        }
        auto goal = cista::optional<ygg::Index<formalism::Condition>> {};
        if (task.goal)
            goal = parse_condition(*task.goal);
        auto metric = cista::optional<ygg::Index<formalism::Metric>> {};
        if (task.metric)
            metric = parse_metric(*task.metric);
        auto axioms = ygg::IndexList<formalism::Axiom> {};
        for (const auto& axiom : task.axioms)
        {
            require_requirement(formalism::RequirementKind::DerivedPredicates, axiom);
            axioms.push_back(parse_axiom(axiom));
        }

        auto data = ygg::Data<formalism::Task>(to_cista(task.name.text), *m_domain, std::move(requirements), std::move(objects), std::move(initial_literals), std::move(initial_function_values), goal, metric, std::move(axioms));
        auto view = formalism::get_or_create<formalism::Task>(repo(), std::move(data));
        return canonicalize_task(view, domain_storage);
    }

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
    std::unordered_set<std::string> m_declared_actions;
    std::unordered_set<formalism::RequirementKind> m_active_requirements;
    std::unordered_set<formalism::RequirementKind> m_domain_requirement_kinds;
    std::unordered_map<ygg::uint_t, ygg::IndexList<formalism::Type>> m_variable_types;
    std::vector<std::unordered_map<std::string, ygg::Index<formalism::Variable>>> m_variable_scopes;
    std::unordered_map<std::string, ygg::Index<formalism::Object>>* m_task_objects = nullptr;

    static cista::offset::string to_cista(const std::string& text) { return cista::offset::string(text); }

    formalism::Repository& repo() noexcept { return m_storage->repository; }
    const formalism::Repository& repo() const noexcept { return m_storage->repository; }

    static ParseError parse_error(const parser::ErrorHandlerType& error_handler, const std::string& fallback, parser::Iterator)
    {
        if (const auto& diagnostic = error_handler.last_error())
            return ParseError(diagnostic->message);
        return ParseError(fallback);
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
    [[noreturn]] void throw_at(const Node&, Error error) const
    {
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

        auto remember_type = [&](auto&& self, ygg::Index<formalism::Type> type) -> void
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

    void canonicalize_domain(formalism::DomainView domain)
    {
        auto canonical = std::make_shared<detail::TranslationStorage>(0);
        auto copier = detail::CanonicalCopyTranslator(canonical);
        auto copied = copier.copy_domain(domain);
        m_storage = std::move(canonical);
        m_task_storages.clear();
        m_domain = copied.get_index();
        rebuild_domain_symbols();
    }

    formalism::TaskView canonicalize_task(formalism::TaskView task, const std::shared_ptr<detail::TranslationStorage>& domain_storage)
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

    static std::string requirement_name(formalism::RequirementKind kind)
    {
        switch (kind)
        {
            case formalism::RequirementKind::Typing: return "typing";
            case formalism::RequirementKind::NegativePreconditions: return "negative-preconditions";
            case formalism::RequirementKind::DisjunctivePreconditions: return "disjunctive-preconditions";
            case formalism::RequirementKind::Equality: return "equality";
            case formalism::RequirementKind::ExistentialPreconditions: return "existential-preconditions";
            case formalism::RequirementKind::UniversalPreconditions: return "universal-preconditions";
            case formalism::RequirementKind::QuantifiedPreconditions: return "quantified-preconditions";
            case formalism::RequirementKind::ConditionalEffects: return "conditional-effects";
            case formalism::RequirementKind::Fluents: return "fluents";
            case formalism::RequirementKind::NumericFluents: return "numeric-fluents";
            case formalism::RequirementKind::DurativeActions: return "durative-actions";
            case formalism::RequirementKind::DerivedPredicates: return "derived-predicates";
            case formalism::RequirementKind::NonDeterministic: return "non-deterministic";
            case formalism::RequirementKind::ProbabilisticEffects: return "probabilistic-effects";
            case formalism::RequirementKind::Strips: return "strips";
        }
        return "unknown";
    }

    void remember_requirement(formalism::RequirementKind kind)
    {
        m_active_requirements.insert(kind);
        if (kind == formalism::RequirementKind::QuantifiedPreconditions)
        {
            m_active_requirements.insert(formalism::RequirementKind::ExistentialPreconditions);
            m_active_requirements.insert(formalism::RequirementKind::UniversalPreconditions);
        }
        if (kind == formalism::RequirementKind::Fluents)
            m_active_requirements.insert(formalism::RequirementKind::NumericFluents);
    }

    void remember_adl_requirements()
    {
        remember_requirement(formalism::RequirementKind::Typing);
        remember_requirement(formalism::RequirementKind::NegativePreconditions);
        remember_requirement(formalism::RequirementKind::DisjunctivePreconditions);
        remember_requirement(formalism::RequirementKind::Equality);
        remember_requirement(formalism::RequirementKind::QuantifiedPreconditions);
        remember_requirement(formalism::RequirementKind::ConditionalEffects);
    }

    template<typename Node>
    void require_requirement(formalism::RequirementKind kind, const Node& node) const
    {
        if (!m_options.strict)
            return;
        if (!m_active_requirements.contains(kind))
            throw_at(node, MissingRequirementError(requirement_name(kind)));
    }

    void require_typing_if_needed(const boost::optional<ast::TypeExpression>& type, const boost::spirit::x3::position_tagged& node) const
    {
        if (type)
            require_requirement(formalism::RequirementKind::Typing, node);
    }

    bool is_subtype(ygg::Index<formalism::Type> actual, ygg::Index<formalism::Type> expected) const
    {
        auto seen = std::unordered_set<ygg::uint_t> {};
        return is_subtype(actual, expected, seen);
    }

    bool is_subtype(ygg::Index<formalism::Type> actual, ygg::Index<formalism::Type> expected, std::unordered_set<ygg::uint_t>& seen) const
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

    bool types_compatible(const ygg::IndexList<formalism::Type>& actual_types, const ygg::IndexList<formalism::Type>& expected_types) const
    {
        if (expected_types.empty() || actual_types.empty())
            return true;
        for (auto actual : actual_types)
            for (auto expected : expected_types)
                if (is_subtype(actual, expected))
                    return true;
        return false;
    }

    ygg::IndexList<formalism::Type> term_types(const ast::Term& term) const
    {
        if (!term.variable)
            return repo()[object(term.name)].types;
        auto variable_index = variable(term.name);
        if (auto it = m_variable_types.find(ygg::uint_t(variable_index)); it != m_variable_types.end())
            return it->second;
        return ygg::IndexList<formalism::Type> { m_object_type };
    }

    template<typename Node>
    void check_argument_types(const std::string& name, const ygg::IndexList<formalism::Parameter>& parameters, const std::vector<ast::Term>& terms, const Node& node) const
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

    ygg::Index<formalism::Type> intern_type(const std::string& name, ygg::IndexList<formalism::Type> bases)
    {
        auto k = key(name);
        if (auto it = m_types.find(k); it != m_types.end() && bases.empty())
            return it->second;
        auto view = formalism::get_or_create<formalism::Type>(repo(), to_cista(k), std::move(bases));
        m_types[k] = view.get_index();
        return view.get_index();
    }

    ygg::IndexList<formalism::Requirement> parse_requirements(const std::vector<ast::Requirement>& nodes)
    {
        auto result = ygg::IndexList<formalism::Requirement> {};
        for (const auto& node : nodes)
        {
            const auto kind = requirement_kind(node);
            if (key(node.name.text) == "adl")
                remember_adl_requirements();
            else
                remember_requirement(kind);
            result.push_back(formalism::get_or_create<formalism::Requirement>(repo(), kind).get_index());
        }
        return result;
    }

    formalism::RequirementKind requirement_kind(const ast::Requirement& node) const
    {
        auto name = key(node.name.text);
        if (name == "strips") return formalism::RequirementKind::Strips;
        if (name == "typing") return formalism::RequirementKind::Typing;
        if (name == "negative-preconditions") return formalism::RequirementKind::NegativePreconditions;
        if (name == "disjunctive-preconditions") return formalism::RequirementKind::DisjunctivePreconditions;
        if (name == "equality") return formalism::RequirementKind::Equality;
        if (name == "existential-preconditions") return formalism::RequirementKind::ExistentialPreconditions;
        if (name == "universal-preconditions") return formalism::RequirementKind::UniversalPreconditions;
        if (name == "quantified-preconditions") return formalism::RequirementKind::QuantifiedPreconditions;
        if (name == "conditional-effects") return formalism::RequirementKind::ConditionalEffects;
        if (name == "fluents") return formalism::RequirementKind::Fluents;
        if (name == "numeric-fluents") return formalism::RequirementKind::NumericFluents;
        if (name == "action-costs") return formalism::RequirementKind::NumericFluents;
        if (name == "adl") return formalism::RequirementKind::QuantifiedPreconditions;
        if (name == "durative-actions") return formalism::RequirementKind::DurativeActions;
        if (name == "derived-predicates") return formalism::RequirementKind::DerivedPredicates;
        if (name == "non-deterministic") return formalism::RequirementKind::NonDeterministic;
        if (name == "probabilistic-effects") return formalism::RequirementKind::ProbabilisticEffects;
        throw_at(node.name, UnsupportedRequirementError(name));
    }

    ygg::IndexList<formalism::Type> parse_types(const std::vector<ast::TypedName>& nodes)
    {
        auto result = ygg::IndexList<formalism::Type> {};
        if (!nodes.empty())
            require_requirement(formalism::RequirementKind::Typing, nodes.front().name);
        for (const auto& node : nodes)
        {
            const auto name = key(node.name.text);
            ensure_new<DuplicateTypeError>(m_declared_types, name, node.name);
            auto bases = node.type ? parse_type_expression(*node.type) : ygg::IndexList<formalism::Type> { m_object_type };
            result.push_back(intern_type(name, std::move(bases)));
        }
        return result;
    }

    ygg::IndexList<formalism::Type> parse_type_expression(const ast::TypeExpression& type)
    {
        return boost::apply_visitor([&](const auto& node) { return parse_type_expression_node(node); }, type);
    }

    ygg::IndexList<formalism::Type> parse_type_expression_node(const ast::TypeReference& node)
    {
        auto result = ygg::IndexList<formalism::Type> {};
        auto k = key(node.name.text);
        if (auto it = m_types.find(k); it != m_types.end())
            result.push_back(it->second);
        else if (m_options.strict)
            throw_at(node.name, UndefinedTypeError(k));
        else
            result.push_back(intern_type(k, {}));
        return result;
    }

    ygg::IndexList<formalism::Type> parse_type_expression_node(const ast::EitherType& node)
    {
        auto result = ygg::IndexList<formalism::Type> {};
        for (const auto& alternative : node.alternatives)
        {
            auto part = parse_type_expression(alternative.get());
            result.insert(result.end(), part.begin(), part.end());
        }
        return result;
    }

    ygg::IndexList<formalism::Object> parse_objects(const std::vector<ast::TypedName>& nodes, std::unordered_map<std::string, ygg::Index<formalism::Object>>& table)
    {
        auto result = ygg::IndexList<formalism::Object> {};
        for (const auto& node : nodes)
        {
            const auto name = key(node.name.text);
            ensure_new<DuplicateObjectError>(m_declared_objects, name, node.name);
            require_typing_if_needed(node.type, node.name);
            auto types = node.type ? parse_type_expression(*node.type) : ygg::IndexList<formalism::Type> { m_object_type };
            auto view = formalism::get_or_create<formalism::Object>(repo(), to_cista(name), std::move(types));
            table[name] = view.get_index();
            result.push_back(view.get_index());
        }
        return result;
    }

    ygg::IndexList<formalism::Parameter> parse_parameters(const std::vector<ast::TypedVariable>& nodes)
    {
        auto result = ygg::IndexList<formalism::Parameter> {};
        for (const auto& node : nodes)
        {
            const auto name = key(node.variable.text);
            if (!m_variable_scopes.empty() && m_variable_scopes.back().contains(name))
                throw_at(node.variable, DuplicateVariableError(name));
            auto variable = formalism::get_or_create<formalism::Variable>(repo(), to_cista(name)).get_index();
            if (!m_variable_scopes.empty())
                m_variable_scopes.back()[name] = variable;
            require_typing_if_needed(node.type, node.variable);
            auto types = node.type ? parse_type_expression(*node.type) : ygg::IndexList<formalism::Type> { m_object_type };
            m_variable_types[ygg::uint_t(variable)] = types;
            result.push_back(formalism::get_or_create<formalism::Parameter>(repo(), variable, std::move(types)).get_index());
        }
        return result;
    }

    ygg::IndexList<formalism::Predicate> parse_predicates(const std::vector<ast::PredicateDeclaration>& nodes)
    {
        auto result = ygg::IndexList<formalism::Predicate> {};
        for (const auto& node : nodes)
        {
            m_variable_scopes.emplace_back();
            auto parameters = parse_parameters(node.parameters);
            m_variable_scopes.pop_back();
            const auto name = key(node.name.text);
            ensure_new<DuplicatePredicateError>(m_declared_predicates, name, node.name);
            auto view = formalism::get_or_create<formalism::Predicate>(repo(), to_cista(name), std::move(parameters));
            m_predicates[name] = view.get_index();
            result.push_back(view.get_index());
        }
        return result;
    }

    ygg::IndexList<formalism::FunctionSkeleton> parse_functions(const std::vector<ast::FunctionDeclaration>& nodes)
    {
        auto result = ygg::IndexList<formalism::FunctionSkeleton> {};
        for (const auto& node : nodes)
        {
            m_variable_scopes.emplace_back();
            auto parameters = parse_parameters(node.parameters);
            m_variable_scopes.pop_back();
            const auto name = key(node.name.text);
            require_requirement(formalism::RequirementKind::NumericFluents, node.name);
            ensure_new<DuplicateFunctionError>(m_declared_functions, name, node.name);
            require_typing_if_needed(node.type, node.name);
            auto type = node.type ? parse_type_expression(*node.type).front() : m_number_type;
            auto view = formalism::get_or_create<formalism::FunctionSkeleton>(repo(), to_cista(name), std::move(parameters), type);
            m_functions[name] = view.get_index();
            result.push_back(view.get_index());
        }
        return result;
    }

    ygg::Index<formalism::Predicate> predicate(const ast::Identifier& identifier, size_t arity)
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
        auto view = formalism::get_or_create<formalism::Predicate>(repo(), to_cista(name), ygg::IndexList<formalism::Parameter> {});
        m_predicates[name] = view.get_index();
        return view.get_index();
    }

    ygg::Index<formalism::Predicate> equality_predicate(const ast::Identifier& identifier, size_t arity)
    {
        require_requirement(formalism::RequirementKind::Equality, identifier);
        if (arity != 2)
            throw_at(identifier, InvalidEqualityError("expected 2 terms, got " + std::to_string(arity)));

        const auto name = key(identifier.text);
        if (auto it = m_predicates.find(name); it != m_predicates.end())
        {
            if (m_declared_predicates.contains(name))
                ensure_arity(name, repo()[it->second].parameters.size(), arity, identifier);
            return it->second;
        }

        auto types = ygg::IndexList<formalism::Type> { m_object_type };
        auto parameters = ygg::IndexList<formalism::Parameter> {};
        const auto left = formalism::get_or_create<formalism::Variable>(repo(), cista::offset::string("lhs")).get_index();
        const auto right = formalism::get_or_create<formalism::Variable>(repo(), cista::offset::string("rhs")).get_index();
        parameters.push_back(formalism::get_or_create<formalism::Parameter>(repo(), left, types).get_index());
        parameters.push_back(formalism::get_or_create<formalism::Parameter>(repo(), right, std::move(types)).get_index());
        auto view = formalism::get_or_create<formalism::Predicate>(repo(), cista::offset::string("="), std::move(parameters));
        m_predicates[name] = view.get_index();
        return view.get_index();
    }

    ygg::Index<formalism::FunctionSkeleton> function(const ast::Identifier& identifier, size_t arity)
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
        auto view = formalism::get_or_create<formalism::FunctionSkeleton>(repo(), to_cista(name), ygg::IndexList<formalism::Parameter> {}, m_number_type);
        m_functions[name] = view.get_index();
        return view.get_index();
    }

    ygg::Index<formalism::Variable> variable(const ast::Identifier& identifier) const
    {
        const auto name = key(identifier.text);
        for (auto it = m_variable_scopes.rbegin(); it != m_variable_scopes.rend(); ++it)
            if (auto found = it->find(name); found != it->end())
                return found->second;
        throw_at(identifier, UndefinedVariableError(identifier.text));
    }

    ygg::Index<formalism::Object> object(const ast::Identifier& identifier) const
    {
        const auto name = key(identifier.text);
        if (m_task_objects)
            if (auto it = m_task_objects->find(name); it != m_task_objects->end())
                return it->second;
        if (auto it = m_objects.find(name); it != m_objects.end())
            return it->second;
        throw_at(identifier, UndefinedObjectError(identifier.text));
    }

    ygg::Index<formalism::Term> parse_term(const ast::Term& node)
    {
        if (node.variable)
            return formalism::get_or_create<formalism::Term>(repo(), ygg::Data<formalism::Term>::Variant(variable(node.name))).get_index();
        return formalism::get_or_create<formalism::Term>(repo(), ygg::Data<formalism::Term>::Variant(object(node.name))).get_index();
    }

    ygg::IndexList<formalism::Term> parse_terms(const std::vector<ast::Term>& nodes)
    {
        auto result = ygg::IndexList<formalism::Term> {};
        for (const auto& node : nodes)
            result.push_back(parse_term(node));
        return result;
    }

    ygg::Index<formalism::Atom> parse_atom(const ast::Atom& node)
    {
        const auto name = key(node.predicate.text);
        auto pred = name == "=" ? equality_predicate(node.predicate, node.terms.size()) : predicate(node.predicate, node.terms.size());
        if (m_declared_predicates.contains(name))
            check_argument_types(name, repo()[pred].parameters, node.terms, node.predicate);
        auto terms = parse_terms(node.terms);
        return formalism::get_or_create<formalism::Atom>(repo(), pred, std::move(terms)).get_index();
    }

    ygg::Index<formalism::Literal> parse_literal(const ast::Literal& node)
    {
        return formalism::get_or_create<formalism::Literal>(repo(), node.positive, parse_atom(node.atom)).get_index();
    }

    ygg::Index<formalism::Condition> parse_condition(const ast::Condition& condition)
    {
        return boost::apply_visitor([&](const auto& node) { return parse_condition_node(node); }, condition);
    }

    ygg::Index<formalism::Condition> wrap_condition(ygg::Data<formalism::Condition>::Variant value)
    {
        return formalism::get_or_create<formalism::Condition>(repo(), std::move(value)).get_index();
    }

    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionLiteral& node) { return wrap_condition(formalism::get_or_create<formalism::ConditionLiteral>(repo(), parse_literal(node.literal)).get_index()); }
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionAnd& node)
    {
        auto list = ygg::IndexList<formalism::Condition> {};
        for (const auto& child : node.conditions) list.push_back(parse_condition(child.get()));
        return wrap_condition(formalism::get_or_create<formalism::ConditionAnd>(repo(), std::move(list)).get_index());
    }
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionOr& node)
    {
        require_requirement(formalism::RequirementKind::DisjunctivePreconditions, node);
        auto list = ygg::IndexList<formalism::Condition> {};
        for (const auto& child : node.conditions) list.push_back(parse_condition(child.get()));
        return wrap_condition(formalism::get_or_create<formalism::ConditionOr>(repo(), std::move(list)).get_index());
    }
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionNot& node)
    {
        require_requirement(formalism::RequirementKind::NegativePreconditions, node);
        return wrap_condition(formalism::get_or_create<formalism::ConditionNot>(repo(), parse_condition(node.condition.get())).get_index());
    }
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionImply& node) { return wrap_condition(formalism::get_or_create<formalism::ConditionImply>(repo(), parse_condition(node.left.get()), parse_condition(node.right.get())).get_index()); }
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionExists& node)
    {
        require_requirement(formalism::RequirementKind::ExistentialPreconditions, node);
        m_variable_scopes.emplace_back();
        auto parameters = parse_parameters(node.parameters);
        auto child = parse_condition(node.condition.get());
        m_variable_scopes.pop_back();
        return wrap_condition(formalism::get_or_create<formalism::ConditionExists>(repo(), std::move(parameters), child).get_index());
    }
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionForall& node)
    {
        require_requirement(formalism::RequirementKind::UniversalPreconditions, node);
        m_variable_scopes.emplace_back();
        auto parameters = parse_parameters(node.parameters);
        auto child = parse_condition(node.condition.get());
        m_variable_scopes.pop_back();
        return wrap_condition(formalism::get_or_create<formalism::ConditionForall>(repo(), std::move(parameters), child).get_index());
    }
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionNumericConstraint& node)
    {
        require_requirement(formalism::RequirementKind::NumericFluents, node);
        return wrap_condition(formalism::get_or_create<formalism::ConditionNumericConstraint>(repo(), comparator(node), parse_function_expression(node.left.get()), parse_function_expression(node.right.get())).get_index());
    }

    ygg::Index<formalism::FunctionTerm> parse_function_term(const ast::FunctionTerm& node)
    {
        auto skeleton = function(node.function, node.terms.size());
        if (m_declared_functions.contains(key(node.function.text)))
            check_argument_types(key(node.function.text), repo()[skeleton].parameters, node.terms, node.function);
        auto terms = parse_terms(node.terms);
        return formalism::get_or_create<formalism::FunctionTerm>(repo(), skeleton, std::move(terms)).get_index();
    }

    ygg::Index<formalism::FunctionExpression> parse_function_expression(const ast::FunctionExpression& expression)
    {
        return boost::apply_visitor([&](const auto& node) { return parse_function_expression_node(node); }, expression);
    }

    ygg::Index<formalism::FunctionExpression> wrap_function_expression(ygg::Data<formalism::FunctionExpression>::Variant value)
    {
        return formalism::get_or_create<formalism::FunctionExpression>(repo(), std::move(value)).get_index();
    }
    ygg::Index<formalism::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionNumber& node) { return wrap_function_expression(formalism::get_or_create<formalism::FunctionExpressionNumber>(repo(), node.value).get_index()); }
    ygg::Index<formalism::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionFunction& node) { return wrap_function_expression(parse_function_term(node.term)); }
    ygg::Index<formalism::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionUnary& node) { return wrap_function_expression(formalism::get_or_create<formalism::UnaryFunctionExpression>(repo(), formalism::UnaryArithmeticOperator::Minus, parse_function_expression(node.expression.get())).get_index()); }
    ygg::Index<formalism::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionBinary& node) { return wrap_function_expression(formalism::get_or_create<formalism::BinaryFunctionExpression>(repo(), binary_operator(node.op), parse_function_expression(node.left.get()), parse_function_expression(node.right.get())).get_index()); }
    ygg::Index<formalism::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionMulti& node)
    {
        auto expressions = ygg::IndexList<formalism::FunctionExpression> {};
        for (const auto& child : node.expressions) expressions.push_back(parse_function_expression(child.get()));
        return wrap_function_expression(formalism::get_or_create<formalism::MultiFunctionExpression>(repo(), multi_operator(node.op), std::move(expressions)).get_index());
    }

    ygg::Index<formalism::Effect> parse_effect(const ast::Effect& effect)
    {
        return boost::apply_visitor([&](const auto& node) { return parse_effect_node(node); }, effect);
    }
    ygg::Index<formalism::Effect> wrap_effect(ygg::Data<formalism::Effect>::Variant value) { return formalism::get_or_create<formalism::Effect>(repo(), std::move(value)).get_index(); }
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectLiteral& node) { return wrap_effect(formalism::get_or_create<formalism::EffectLiteral>(repo(), parse_literal(node.literal)).get_index()); }
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectAnd& node)
    {
        auto list = ygg::IndexList<formalism::Effect> {};
        for (const auto& child : node.effects) list.push_back(parse_effect(child.get()));
        return wrap_effect(formalism::get_or_create<formalism::EffectAnd>(repo(), std::move(list)).get_index());
    }
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectNumeric& node)
    {
        require_requirement(formalism::RequirementKind::NumericFluents, node);
        auto skeleton = function(node.function.function, node.function.terms.size());
        if (m_declared_functions.contains(key(node.function.function.text)))
            check_argument_types(key(node.function.function.text), repo()[skeleton].parameters, node.function.terms, node.function.function);
        return wrap_effect(formalism::get_or_create<formalism::EffectNumeric>(repo(), numeric_effect_operator(node), skeleton, parse_terms(node.function.terms), parse_function_expression(node.expression.get())).get_index());
    }
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectForall& node)
    {
        require_requirement(formalism::RequirementKind::UniversalPreconditions, node);
        m_variable_scopes.emplace_back();
        auto parameters = parse_parameters(node.parameters);
        auto child = parse_effect(node.effect.get());
        m_variable_scopes.pop_back();
        return wrap_effect(formalism::get_or_create<formalism::EffectForall>(repo(), std::move(parameters), child).get_index());
    }
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectWhen& node)
    {
        require_requirement(formalism::RequirementKind::ConditionalEffects, node);
        return wrap_effect(formalism::get_or_create<formalism::EffectWhen>(repo(), parse_condition(node.condition.get()), parse_effect(node.effect.get())).get_index());
    }
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectOneOf& node)
    {
        require_requirement(formalism::RequirementKind::NonDeterministic, node);
        auto list = ygg::IndexList<formalism::Effect> {};
        for (const auto& child : node.effects) list.push_back(parse_effect(child.get()));
        return wrap_effect(formalism::get_or_create<formalism::EffectOneOf>(repo(), std::move(list)).get_index());
    }
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectProbabilistic& node)
    {
        require_requirement(formalism::RequirementKind::ProbabilisticEffects, node);
        auto list = ygg::IndexList<formalism::EffectProbabilisticAlternative> {};
        auto total = 0.0;
        for (const auto& alternative : node.alternatives)
        {
            if (!std::isfinite(alternative.probability) || alternative.probability < 0.0 || alternative.probability > 1.0)
                throw_at(alternative, InvalidProbabilisticEffectError("probability must be in [0, 1]"));
            total += alternative.probability;
            list.push_back(formalism::get_or_create<formalism::EffectProbabilisticAlternative>(repo(), alternative.probability, parse_effect(alternative.effect.get())).get_index());
        }
        if (total > 1.0 + 1e-9)
            throw_at(node, InvalidProbabilisticEffectError("probabilities sum to more than 1"));
        return wrap_effect(formalism::get_or_create<formalism::EffectProbabilistic>(repo(), std::move(list)).get_index());
    }

    ygg::Index<formalism::Action> parse_action(const ast::Action& node)
    {
        const auto name = key(node.name.text);
        ensure_new<DuplicateActionError>(m_declared_actions, name, node.name);
        m_variable_scopes.emplace_back();
        auto parameters = parse_parameters(node.parameters);
        auto precondition = cista::optional<ygg::Index<formalism::Condition>> {};
        if (node.precondition) precondition = parse_condition(*node.precondition);
        auto effect = cista::optional<ygg::Index<formalism::Effect>> {};
        if (node.effect) effect = parse_effect(*node.effect);
        m_variable_scopes.pop_back();
        return formalism::get_or_create<formalism::Action>(repo(), to_cista(name), std::move(parameters), precondition, effect).get_index();
    }

    ygg::Index<formalism::Axiom> parse_axiom(const ast::Axiom& node)
    {
        m_variable_scopes.emplace_back();
        auto parameters = parse_parameters(node.head.parameters);
        auto terms = ygg::IndexList<formalism::Term> {};
        for (const auto& parameter : node.head.parameters)
        {
            auto term = ast::Term {};
            term.name = parameter.variable;
            term.variable = true;
            terms.push_back(parse_term(term));
        }
        auto pred = predicate(node.head.name, terms.size());
        auto atom = formalism::get_or_create<formalism::Atom>(repo(), pred, std::move(terms)).get_index();
        auto head = formalism::get_or_create<formalism::Literal>(repo(), true, atom).get_index();
        auto condition = parse_condition(node.condition);
        m_variable_scopes.pop_back();
        return formalism::get_or_create<formalism::Axiom>(repo(), std::move(parameters), head, condition).get_index();
    }

    void parse_initial_element(const ast::Literal& literal, ygg::IndexList<formalism::Literal>& literals, ygg::IndexList<formalism::InitialFunctionValue>&) { literals.push_back(parse_literal(literal)); }
    void parse_initial_element(const ast::InitialFunctionValue& value, ygg::IndexList<formalism::Literal>&, ygg::IndexList<formalism::InitialFunctionValue>& values) { values.push_back(formalism::get_or_create<formalism::InitialFunctionValue>(repo(), parse_function_term(value.function), parse_function_expression(value.value)).get_index()); }

    ygg::Index<formalism::Metric> parse_metric(const ast::Metric& node)
    {
        require_requirement(formalism::RequirementKind::NumericFluents, node);
        const auto optimization = key(node.optimization.text);
        if (optimization != "minimize" && optimization != "maximize")
            throw_at(node.optimization, InvalidMetricError(optimization));
        return formalism::get_or_create<formalism::Metric>(repo(), optimization == "minimize", parse_function_expression(node.expression)).get_index();
    }

    formalism::BinaryComparator comparator(const ast::ConditionNumericConstraint& node) const
    {
        auto op = key(node.comparator);
        if (op == "=") return formalism::BinaryComparator::Equal;
        if (op == "!=") return formalism::BinaryComparator::NotEqual;
        if (op == "<") return formalism::BinaryComparator::Less;
        if (op == "<=") return formalism::BinaryComparator::LessEqual;
        if (op == ">") return formalism::BinaryComparator::Greater;
        if (op == ">=") return formalism::BinaryComparator::GreaterEqual;
        throw_at(node, InvalidNumericConstraintError(op));
    }
    static formalism::BinaryArithmeticOperator binary_operator(std::string op)
    {
        op = key(std::move(op));
        if (op == "/") return formalism::BinaryArithmeticOperator::Divide;
        return formalism::BinaryArithmeticOperator::Subtract;
    }
    static formalism::MultiArithmeticOperator multi_operator(std::string op) { return key(std::move(op)) == "*" ? formalism::MultiArithmeticOperator::Multiply : formalism::MultiArithmeticOperator::Add; }
    formalism::NumericEffectOperator numeric_effect_operator(const ast::EffectNumeric& node) const
    {
        auto op = key(node.op);
        if (op == "assign") return formalism::NumericEffectOperator::Assign;
        if (op == "increase") return formalism::NumericEffectOperator::Increase;
        if (op == "decrease") return formalism::NumericEffectOperator::Decrease;
        if (op == "scale-up") return formalism::NumericEffectOperator::ScaleUp;
        if (op == "scale-down") return formalism::NumericEffectOperator::ScaleDown;
        throw_at(node, InvalidNumericEffectError(op));
    }
};

} // namespace loki::semantic

#endif
