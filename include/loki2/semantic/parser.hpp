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
#include "loki2/pddl/pddl.hpp"

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
#include <utility>
#include <vector>

namespace loki2::semantic
{
namespace fs = std::filesystem;

class SemanticError : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

inline std::string read_file(const fs::path& path)
{
    std::ifstream in(path);
    if (!in)
        throw SemanticError("Could not open PDDL file: " + path.string());
    std::ostringstream out;
    out << in.rdbuf();
    return out.str();
}

class Parser
{
public:
    Parser() :
        m_repository(0)
    {
        m_object_type = intern_type("object", {});
        m_number_type = intern_type("number", {});
    }

    const pddl::Repository& repository() const noexcept { return m_repository; }
    pddl::Repository& repository() noexcept { return m_repository; }
    const pddl::Builder& builder() const noexcept { return m_builder; }
    pddl::Builder& builder() noexcept { return m_builder; }

    bool has_domain() const noexcept { return m_domain.has_value(); }
    pddl::DomainView get_domain() const
    {
        if (!m_domain)
            throw SemanticError("No domain has been parsed.");
        return ygg::make_view(*m_domain, m_repository);
    }

    pddl::DomainView parse_domain(const ast::Domain& domain)
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

        auto data = ygg::Data<pddl::Domain>(to_cista(domain.name.text), std::move(requirements), std::move(types), std::move(constants), std::move(predicates), std::move(functions), std::move(actions), std::move(axioms));
        auto view = m_builder.domain(m_repository, std::move(data));
        m_domain = view.get_index();
        m_domain_name = domain.name.text;
        return view;
    }

    pddl::DomainView parse_domain(const std::string& source)
    {
        auto first = source.cbegin();
        parser::ErrorHandlerType error_handler(first, source.cend(), std::cerr);
        ast::Domain domain_ast;
        if (!parser::parse_domain(source, domain_ast, error_handler))
            throw SemanticError("Could not parse PDDL domain.");
        return parse_domain(domain_ast);
    }

    pddl::DomainView parse_domain_file(const fs::path& path) { return parse_domain(read_file(path)); }

    pddl::TaskView parse_task(const ast::Task& task)
    {
        if (!m_domain)
            throw SemanticError("Cannot parse task before parsing a domain.");
        if (!m_domain_name.empty() && task.domain_name.text != m_domain_name)
            throw SemanticError("Task references domain '" + task.domain_name.text + "' but parser holds domain '" + m_domain_name + "'.");

        auto task_objects = std::unordered_map<std::string, ygg::Index<pddl::Object>> {};
        struct TaskObjectScope
        {
            Parser& parser;
            std::unordered_map<std::string, ygg::Index<pddl::Object>>* previous;
            ~TaskObjectScope() { parser.m_task_objects = previous; }
        };
        TaskObjectScope task_object_scope { *this, m_task_objects };
        m_task_objects = &task_objects;

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
        auto view = m_builder.task(m_repository, std::move(data));
        return view;
    }

    pddl::TaskView parse_task(const std::string& source)
    {
        auto first = source.cbegin();
        parser::ErrorHandlerType error_handler(first, source.cend(), std::cerr);
        ast::Task task_ast;
        if (!parser::parse_task(source, task_ast, error_handler))
            throw SemanticError("Could not parse PDDL task.");
        return parse_task(task_ast);
    }

    pddl::TaskView parse_task_file(const fs::path& path) { return parse_task(read_file(path)); }

private:
    pddl::Repository m_repository;
    pddl::Builder m_builder;
    cista::optional<ygg::Index<pddl::Domain>> m_domain;
    std::string m_domain_name;

    ygg::Index<pddl::Type> m_object_type;
    ygg::Index<pddl::Type> m_number_type;
    std::unordered_map<std::string, ygg::Index<pddl::Type>> m_types;
    std::unordered_map<std::string, ygg::Index<pddl::Object>> m_objects;
    std::unordered_map<std::string, ygg::Index<pddl::Predicate>> m_predicates;
    std::unordered_map<std::string, ygg::Index<pddl::FunctionSkeleton>> m_functions;
    std::vector<std::unordered_map<std::string, ygg::Index<pddl::Variable>>> m_variable_scopes;
    std::unordered_map<std::string, ygg::Index<pddl::Object>>* m_task_objects = nullptr;

    static cista::offset::string to_cista(const std::string& text) { return cista::offset::string(text); }

    void clear_domain_symbols()
    {
        m_domain = {};
        m_domain_name.clear();
        m_types.clear();
        m_objects.clear();
        m_predicates.clear();
        m_functions.clear();
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

    ygg::Index<pddl::Type> intern_type(const std::string& name, ygg::IndexList<pddl::Type> bases)
    {
        auto k = key(name);
        if (auto it = m_types.find(k); it != m_types.end() && bases.empty())
            return it->second;
        auto view = m_builder.type(m_repository, to_cista(k), std::move(bases));
        m_types[k] = view.get_index();
        return view.get_index();
    }

    ygg::IndexList<pddl::Requirement> parse_requirements(const std::vector<ast::Requirement>& nodes)
    {
        auto result = ygg::IndexList<pddl::Requirement> {};
        for (const auto& node : nodes)
            result.push_back(m_builder.requirement(m_repository, requirement_kind(node.name.text)).get_index());
        return result;
    }

    pddl::RequirementKind requirement_kind(std::string name) const
    {
        name = key(std::move(name));
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
        throw SemanticError("Unsupported requirement: :" + name);
    }

    ygg::IndexList<pddl::Type> parse_types(const std::vector<ast::TypedName>& nodes)
    {
        auto result = ygg::IndexList<pddl::Type> {};
        for (const auto& node : nodes)
        {
            auto bases = node.type ? parse_type_expression(*node.type) : ygg::IndexList<pddl::Type> { m_object_type };
            result.push_back(intern_type(node.name.text, std::move(bases)));
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
            auto types = node.type ? parse_type_expression(*node.type) : ygg::IndexList<pddl::Type> { m_object_type };
            auto view = m_builder.object(m_repository, to_cista(node.name.text), std::move(types));
            table[node.name.text] = view.get_index();
            result.push_back(view.get_index());
        }
        return result;
    }

    ygg::IndexList<pddl::Parameter> parse_parameters(const std::vector<ast::TypedVariable>& nodes)
    {
        auto result = ygg::IndexList<pddl::Parameter> {};
        for (const auto& node : nodes)
        {
            auto variable = m_builder.variable(m_repository, to_cista(node.variable.text)).get_index();
            if (!m_variable_scopes.empty())
                m_variable_scopes.back()[node.variable.text] = variable;
            auto types = node.type ? parse_type_expression(*node.type) : ygg::IndexList<pddl::Type> { m_object_type };
            result.push_back(m_builder.parameter(m_repository, variable, std::move(types)).get_index());
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
            auto view = m_builder.predicate(m_repository, to_cista(node.name.text), std::move(parameters));
            m_predicates[node.name.text] = view.get_index();
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
            auto type = node.type ? parse_type_expression(*node.type).front() : m_number_type;
            auto view = m_builder.function_skeleton(m_repository, to_cista(node.name.text), std::move(parameters), type);
            m_functions[node.name.text] = view.get_index();
            result.push_back(view.get_index());
        }
        return result;
    }

    ygg::Index<pddl::Predicate> predicate(const std::string& name)
    {
        if (auto it = m_predicates.find(name); it != m_predicates.end())
            return it->second;
        auto view = m_builder.predicate(m_repository, to_cista(name), {});
        m_predicates[name] = view.get_index();
        return view.get_index();
    }

    ygg::Index<pddl::FunctionSkeleton> function(const std::string& name)
    {
        if (auto it = m_functions.find(name); it != m_functions.end())
            return it->second;
        auto view = m_builder.function_skeleton(m_repository, to_cista(name), {}, m_number_type);
        m_functions[name] = view.get_index();
        return view.get_index();
    }

    ygg::Index<pddl::Variable> variable(const std::string& name) const
    {
        for (auto it = m_variable_scopes.rbegin(); it != m_variable_scopes.rend(); ++it)
            if (auto found = it->find(name); found != it->end())
                return found->second;
        throw SemanticError("Undefined variable: ?" + name);
    }

    ygg::Index<pddl::Object> object(const std::string& name) const
    {
        if (m_task_objects)
            if (auto it = m_task_objects->find(name); it != m_task_objects->end())
                return it->second;
        if (auto it = m_objects.find(name); it != m_objects.end())
            return it->second;
        throw SemanticError("Undefined object: " + name);
    }

    ygg::Index<pddl::Term> parse_term(const ast::Term& node)
    {
        if (node.variable)
            return m_builder.term(m_repository, ygg::Data<pddl::Term>::Variant(variable(node.name.text))).get_index();
        return m_builder.term(m_repository, ygg::Data<pddl::Term>::Variant(object(node.name.text))).get_index();
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
        return m_builder.atom(m_repository, predicate(node.predicate.text), parse_terms(node.terms)).get_index();
    }

    ygg::Index<pddl::Literal> parse_literal(const ast::Literal& node)
    {
        return m_builder.literal(m_repository, node.positive, parse_atom(node.atom)).get_index();
    }

    ygg::Index<pddl::Condition> parse_condition(const ast::Condition& condition)
    {
        return boost::apply_visitor([&](const auto& node) { return parse_condition_node(node); }, condition);
    }

    ygg::Index<pddl::Condition> wrap_condition(ygg::Data<pddl::Condition>::Variant value)
    {
        return m_builder.condition(m_repository, std::move(value)).get_index();
    }

    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionLiteral& node) { return wrap_condition(m_builder.condition_literal(m_repository, parse_literal(node.literal)).get_index()); }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionAnd& node)
    {
        auto list = ygg::IndexList<pddl::Condition> {};
        for (const auto& child : node.conditions) list.push_back(parse_condition(child.get()));
        return wrap_condition(m_builder.condition_and(m_repository, std::move(list)).get_index());
    }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionOr& node)
    {
        auto list = ygg::IndexList<pddl::Condition> {};
        for (const auto& child : node.conditions) list.push_back(parse_condition(child.get()));
        return wrap_condition(m_builder.condition_or(m_repository, std::move(list)).get_index());
    }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionNot& node) { return wrap_condition(m_builder.condition_not(m_repository, parse_condition(node.condition.get())).get_index()); }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionImply& node) { return wrap_condition(m_builder.condition_imply(m_repository, parse_condition(node.left.get()), parse_condition(node.right.get())).get_index()); }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionExists& node)
    {
        m_variable_scopes.emplace_back();
        auto parameters = parse_parameters(node.parameters);
        auto child = parse_condition(node.condition.get());
        m_variable_scopes.pop_back();
        return wrap_condition(m_builder.condition_exists(m_repository, std::move(parameters), child).get_index());
    }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionForall& node)
    {
        m_variable_scopes.emplace_back();
        auto parameters = parse_parameters(node.parameters);
        auto child = parse_condition(node.condition.get());
        m_variable_scopes.pop_back();
        return wrap_condition(m_builder.condition_forall(m_repository, std::move(parameters), child).get_index());
    }
    ygg::Index<pddl::Condition> parse_condition_node(const ast::ConditionNumericConstraint& node)
    {
        return wrap_condition(m_builder.condition_numeric_constraint(m_repository, comparator(node.comparator), parse_function_expression(node.left.get()), parse_function_expression(node.right.get())).get_index());
    }

    ygg::Index<pddl::FunctionTerm> parse_function_term(const ast::FunctionTerm& node)
    {
        return m_builder.function_term(m_repository, function(node.function.text), parse_terms(node.terms)).get_index();
    }

    ygg::Index<pddl::FunctionExpression> parse_function_expression(const ast::FunctionExpression& expression)
    {
        return boost::apply_visitor([&](const auto& node) { return parse_function_expression_node(node); }, expression);
    }

    ygg::Index<pddl::FunctionExpression> wrap_function_expression(ygg::Data<pddl::FunctionExpression>::Variant value)
    {
        return m_builder.function_expression(m_repository, std::move(value)).get_index();
    }
    ygg::Index<pddl::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionNumber& node) { return wrap_function_expression(m_builder.number(m_repository, node.value).get_index()); }
    ygg::Index<pddl::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionFunction& node) { return wrap_function_expression(parse_function_term(node.term)); }
    ygg::Index<pddl::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionUnary& node) { return wrap_function_expression(m_builder.unary_function_expression(m_repository, pddl::UnaryArithmeticOperator::Minus, parse_function_expression(node.expression.get())).get_index()); }
    ygg::Index<pddl::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionBinary& node) { return wrap_function_expression(m_builder.binary_function_expression(m_repository, binary_operator(node.op), parse_function_expression(node.left.get()), parse_function_expression(node.right.get())).get_index()); }
    ygg::Index<pddl::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionMulti& node)
    {
        auto expressions = ygg::IndexList<pddl::FunctionExpression> {};
        for (const auto& child : node.expressions) expressions.push_back(parse_function_expression(child.get()));
        return wrap_function_expression(m_builder.multi_function_expression(m_repository, multi_operator(node.op), std::move(expressions)).get_index());
    }

    ygg::Index<pddl::Effect> parse_effect(const ast::Effect& effect)
    {
        return boost::apply_visitor([&](const auto& node) { return parse_effect_node(node); }, effect);
    }
    ygg::Index<pddl::Effect> wrap_effect(ygg::Data<pddl::Effect>::Variant value) { return m_builder.effect(m_repository, std::move(value)).get_index(); }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectLiteral& node) { return wrap_effect(m_builder.effect_literal(m_repository, parse_literal(node.literal)).get_index()); }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectAnd& node)
    {
        auto list = ygg::IndexList<pddl::Effect> {};
        for (const auto& child : node.effects) list.push_back(parse_effect(child.get()));
        return wrap_effect(m_builder.effect_and(m_repository, std::move(list)).get_index());
    }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectNumeric& node) { return wrap_effect(m_builder.effect_numeric(m_repository, numeric_effect_operator(node.op), function(node.function.function.text), parse_terms(node.function.terms), parse_function_expression(node.expression.get())).get_index()); }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectForall& node)
    {
        m_variable_scopes.emplace_back();
        auto parameters = parse_parameters(node.parameters);
        auto child = parse_effect(node.effect.get());
        m_variable_scopes.pop_back();
        return wrap_effect(m_builder.effect_forall(m_repository, std::move(parameters), child).get_index());
    }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectWhen& node) { return wrap_effect(m_builder.effect_when(m_repository, parse_condition(node.condition.get()), parse_effect(node.effect.get())).get_index()); }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectOneOf& node)
    {
        auto list = ygg::IndexList<pddl::Effect> {};
        for (const auto& child : node.effects) list.push_back(parse_effect(child.get()));
        return wrap_effect(m_builder.effect_one_of(m_repository, std::move(list)).get_index());
    }
    ygg::Index<pddl::Effect> parse_effect_node(const ast::EffectProbabilistic& node)
    {
        auto list = ygg::IndexList<pddl::EffectProbabilisticAlternative> {};
        for (const auto& alternative : node.alternatives)
            list.push_back(m_builder.effect_probabilistic_alternative(m_repository, alternative.probability, parse_effect(alternative.effect.get())).get_index());
        return wrap_effect(m_builder.effect_probabilistic(m_repository, std::move(list)).get_index());
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
        return m_builder.action(m_repository, to_cista(node.name.text), std::move(parameters), precondition, effect).get_index();
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
        auto pred = predicate(node.head.name.text);
        auto atom = m_builder.atom(m_repository, pred, std::move(terms)).get_index();
        auto head = m_builder.literal(m_repository, true, atom).get_index();
        auto condition = parse_condition(node.condition);
        m_variable_scopes.pop_back();
        return m_builder.axiom(m_repository, std::move(parameters), head, condition).get_index();
    }

    void parse_initial_element(const ast::Literal& literal, ygg::IndexList<pddl::Literal>& literals, ygg::IndexList<pddl::InitialFunctionValue>&) { literals.push_back(parse_literal(literal)); }
    void parse_initial_element(const ast::InitialFunctionValue& value, ygg::IndexList<pddl::Literal>&, ygg::IndexList<pddl::InitialFunctionValue>& values) { values.push_back(m_builder.initial_function_value(m_repository, parse_function_term(value.function), parse_function_expression(value.value)).get_index()); }

    ygg::Index<pddl::Metric> parse_metric(const ast::Metric& node)
    {
        return m_builder.metric(m_repository, key(node.optimization.text) != "maximize", parse_function_expression(node.expression)).get_index();
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
