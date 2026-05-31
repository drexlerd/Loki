/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_SEMANTIC_TRANSLATOR_HPP_
#define LOKI2_SEMANTIC_TRANSLATOR_HPP_

#include "loki2/pddl/pddl.hpp"

#include <cista/containers/optional.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

namespace loki2::semantic
{

struct TranslatorOptions
{
    bool remove_typing = true;
};

inline const std::vector<std::string_view>& domain_translation_steps()
{
    static const auto steps = std::vector<std::string_view> {
        "to-negation-normal-form",
        "rename-quantified-variables",
        "remove-universal-quantifiers",
        "to-disjunctive-normal-form",
        "split-disjunctive-conditions",
        "move-existential-quantifiers",
        "add-type-predicates",
        "to-effect-normal-form",
        "initialize-equality",
    };
    return steps;
}

inline const std::vector<std::string_view>& task_translation_steps()
{
    static const auto steps = std::vector<std::string_view> {
        "to-negation-normal-form",
        "rename-quantified-variables",
        "remove-universal-quantifiers",
        "simplify-goal",
        "to-disjunctive-normal-form",
        "split-disjunctive-conditions",
        "move-existential-quantifiers",
        "to-effect-normal-form",
        "initialize-equality",
        "add-type-predicates",
    };
    return steps;
}

namespace detail
{

template<typename T>
using IndexMap = std::unordered_map<ygg::uint_t, ygg::Index<T>>;

struct TranslationStorage
{
    pddl::Repository repository;
    pddl::Builder builder;
    ygg::Index<pddl::Domain> original_domain;
    ygg::Index<pddl::Domain> translated_domain;

    IndexMap<pddl::Requirement> requirements;
    IndexMap<pddl::Type> types;
    IndexMap<pddl::Object> objects;
    IndexMap<pddl::Variable> variables;
    IndexMap<pddl::Parameter> parameters;
    IndexMap<pddl::Predicate> predicates;
    IndexMap<pddl::FunctionSkeleton> functions;
    IndexMap<pddl::Term> terms;
    IndexMap<pddl::Atom> atoms;
    IndexMap<pddl::Literal> literals;
    IndexMap<pddl::FunctionExpressionNumber> numbers;
    IndexMap<pddl::FunctionTerm> function_terms;
    IndexMap<pddl::UnaryFunctionExpression> unary_expressions;
    IndexMap<pddl::BinaryFunctionExpression> binary_expressions;
    IndexMap<pddl::MultiFunctionExpression> multi_expressions;
    IndexMap<pddl::FunctionExpression> function_expressions;
    IndexMap<pddl::ConditionLiteral> condition_literals;
    IndexMap<pddl::ConditionAnd> condition_ands;
    IndexMap<pddl::ConditionOr> condition_ors;
    IndexMap<pddl::ConditionNot> condition_nots;
    IndexMap<pddl::ConditionImply> condition_implies;
    IndexMap<pddl::ConditionExists> condition_exists;
    IndexMap<pddl::ConditionForall> condition_foralls;
    IndexMap<pddl::ConditionNumericConstraint> condition_numeric_constraints;
    IndexMap<pddl::Condition> conditions;
    IndexMap<pddl::EffectLiteral> effect_literals;
    IndexMap<pddl::EffectAnd> effect_ands;
    IndexMap<pddl::EffectNumeric> effect_numerics;
    IndexMap<pddl::EffectForall> effect_foralls;
    IndexMap<pddl::EffectWhen> effect_whens;
    IndexMap<pddl::EffectOneOf> effect_one_ofs;
    IndexMap<pddl::EffectProbabilisticAlternative> effect_probabilistic_alternatives;
    IndexMap<pddl::EffectProbabilistic> effect_probabilistics;
    IndexMap<pddl::Effect> effects;
    IndexMap<pddl::Action> actions;
    IndexMap<pddl::Axiom> axioms;
    IndexMap<pddl::Metric> metrics;
    IndexMap<pddl::InitialFunctionValue> initial_function_values;
    IndexMap<pddl::Task> tasks;
    std::unordered_map<std::string, ygg::IndexList<pddl::Type>> object_types_by_name;

    TranslationStorage() : repository(1) {}
};

template<typename T>
bool find_mapped(const IndexMap<T>& map, ygg::Index<T> source, ygg::Index<T>& out)
{
    if (auto it = map.find(source.get_value()); it != map.end())
    {
        out = it->second;
        return true;
    }
    return false;
}

template<typename T>
void remember(IndexMap<T>& map, ygg::Index<T> source, ygg::Index<T> target)
{
    map.emplace(source.get_value(), target);
}

class CopyTranslator
{
public:
    explicit CopyTranslator(std::shared_ptr<TranslationStorage> storage, bool remove_typing = true) : m_storage(std::move(storage)), m_remove_typing(remove_typing) {}

    cista::optional<ygg::Index<pddl::ConditionOr>> public_as_or(ygg::Index<pddl::Condition> condition) const
    {
        auto result = cista::optional<ygg::Index<pddl::ConditionOr>> {};
        std::visit([&](const auto& node)
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, ygg::Index<pddl::ConditionOr>>)
                result = node;
        }, m_storage->repository[condition].value);
        return result;
    }

    ygg::IndexList<pddl::Action> split_disjunctive_actions(const ygg::IndexList<pddl::Action>& actions)
    {
        auto result = ygg::IndexList<pddl::Action> {};
        for (auto action : actions)
        {
            const auto data = m_storage->repository[action];
            if (data.precondition)
            {
                if (const auto condition_or = public_as_or(*data.precondition))
                {
                    for (auto part : m_storage->repository[*condition_or].conditions)
                        result.push_back(m_storage->builder.action(m_storage->repository, data.name, data.parameters, part, data.effect).get_index());
                    continue;
                }
            }
            result.push_back(action);
        }
        return result;
    }

    ygg::IndexList<pddl::Axiom> split_disjunctive_axioms(const ygg::IndexList<pddl::Axiom>& axioms)
    {
        auto result = ygg::IndexList<pddl::Axiom> {};
        for (auto axiom : axioms)
        {
            const auto data = m_storage->repository[axiom];
            if (const auto condition_or = public_as_or(data.condition))
            {
                for (auto part : m_storage->repository[*condition_or].conditions)
                    result.push_back(m_storage->builder.axiom(m_storage->repository, data.parameters, data.head, part).get_index());
            }
            else
            {
                result.push_back(axiom);
            }
        }
        return result;
    }

    bool has_requirement(const ygg::IndexList<pddl::Requirement>& requirements, pddl::RequirementKind kind) const
    {
        for (auto requirement : requirements)
            if (m_storage->repository[requirement].kind == kind)
                return true;
        return false;
    }

    bool equality_required(const ygg::Data<pddl::Task>& task) const
    {
        if (has_requirement(task.requirements, pddl::RequirementKind::Equality))
            return true;
        const auto& domain = m_storage->repository[m_storage->translated_domain];
        return has_requirement(domain.requirements, pddl::RequirementKind::Equality);
    }

    ygg::Index<pddl::Predicate> ensure_equality_predicate()
    {
        if (m_equality_predicate)
            return *m_equality_predicate;
        auto domain = m_storage->repository[m_storage->translated_domain];
        for (auto predicate : domain.predicates)
        {
            if (std::string(m_storage->repository[predicate].name) == "=")
            {
                m_equality_predicate = predicate;
                return predicate;
            }
        }
        auto parameters = ygg::IndexList<pddl::Parameter> {};
        auto types = ygg::IndexList<pddl::Type> {};
        for (auto type : domain.types)
        {
            if (std::string(m_storage->repository[type].name) == "object")
            {
                types.push_back(type);
                break;
            }
        }
        const auto left = m_storage->builder.variable(m_storage->repository, cista::offset::string("lhs")).get_index();
        const auto right = m_storage->builder.variable(m_storage->repository, cista::offset::string("rhs")).get_index();
        parameters.push_back(m_storage->builder.parameter(m_storage->repository, left, types).get_index());
        parameters.push_back(m_storage->builder.parameter(m_storage->repository, right, types).get_index());
        const auto predicate = m_storage->builder.predicate(m_storage->repository, cista::offset::string("="), std::move(parameters)).get_index();
        domain.predicates.push_back(predicate);
        m_storage->translated_domain = m_storage->builder.domain(m_storage->repository, std::move(domain)).get_index();
        m_equality_predicate = predicate;
        return predicate;
    }

    ygg::Index<pddl::Literal> equality_literal(ygg::Index<pddl::Predicate> predicate, ygg::Index<pddl::Object> object)
    {
        auto terms = ygg::IndexList<pddl::Term> {};
        const auto term = m_storage->builder.term(m_storage->repository, ygg::Data<pddl::Term>::Variant(object)).get_index();
        terms.push_back(term);
        terms.push_back(term);
        const auto atom = m_storage->builder.atom(m_storage->repository, predicate, std::move(terms)).get_index();
        return m_storage->builder.literal(m_storage->repository, true, atom).get_index();
    }

    bool reflexive_equality_object_name(ygg::Index<pddl::Literal> literal, std::string& name) const
    {
        const auto& literal_data = m_storage->repository[literal];
        if (!literal_data.positive)
            return false;
        const auto& atom = m_storage->repository[literal_data.atom];
        if (atom.terms.size() != 2 || std::string(m_storage->repository[atom.predicate].name) != "=")
            return false;
        auto left_name = std::string {};
        auto right_name = std::string {};
        std::visit([&](const auto& value)
        {
            using Value = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<Value, ygg::Index<pddl::Object>>)
                left_name = std::string(m_storage->repository[value].name);
        }, m_storage->repository[atom.terms[0]].value);
        std::visit([&](const auto& value)
        {
            using Value = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<Value, ygg::Index<pddl::Object>>)
                right_name = std::string(m_storage->repository[value].name);
        }, m_storage->repository[atom.terms[1]].value);
        if (left_name.empty() || left_name != right_name)
            return false;
        name = std::move(left_name);
        return true;
    }

    void compact_reflexive_equalities(ygg::Data<pddl::Task>& task) const
    {
        auto seen = std::unordered_set<std::string> {};
        auto compacted = ygg::IndexList<pddl::Literal> {};
        for (auto literal : task.initial_literals)
        {
            auto name = std::string {};
            if (reflexive_equality_object_name(literal, name))
            {
                if (!seen.insert(name).second)
                    continue;
            }
            compacted.push_back(literal);
        }
        task.initial_literals = std::move(compacted);
    }

    void initialize_equality(ygg::Data<pddl::Task>& task)
    {
        if (!equality_required(task))
            return;
        const auto predicate = ensure_equality_predicate();
        const auto& domain = m_storage->repository[m_storage->translated_domain];
        auto seen = std::unordered_set<std::string> {};
        for (auto literal : task.initial_literals)
        {
            const auto& literal_data = m_storage->repository[literal];
            if (!literal_data.positive)
                continue;
            const auto& atom = m_storage->repository[literal_data.atom];
            if (atom.predicate != predicate || atom.terms.size() != 2)
                continue;
            auto left_name = std::string {};
            auto right_name = std::string {};
            std::visit([&](const auto& value)
            {
                using Value = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<Value, ygg::Index<pddl::Object>>)
                    left_name = std::string(m_storage->repository[value].name);
            }, m_storage->repository[atom.terms[0]].value);
            std::visit([&](const auto& value)
            {
                using Value = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<Value, ygg::Index<pddl::Object>>)
                    right_name = std::string(m_storage->repository[value].name);
            }, m_storage->repository[atom.terms[1]].value);
            if (!left_name.empty() && left_name == right_name)
                seen.insert(left_name);
        }
        auto add_object = [&](ygg::Index<pddl::Object> object)
        {
            if (seen.insert(std::string(m_storage->repository[object].name)).second)
                task.initial_literals.push_back(equality_literal(predicate, object));
        };
        for (auto object : domain.constants)
            add_object(object);
        for (auto object : task.objects)
            add_object(object);
        task.domain = m_storage->translated_domain;
        compact_reflexive_equalities(task);
    }

    pddl::DomainView copy_domain(pddl::DomainView domain)
    {
        m_storage->original_domain = domain.get_index();
        auto data = domain.get_data();
        data.index = {};
        data.requirements = copy_list<pddl::Requirement>(data.requirements, domain.get_context());
        data.types = copy_list<pddl::Type>(data.types, domain.get_context());
        data.constants = copy_list<pddl::Object>(data.constants, domain.get_context());
        data.predicates = copy_list<pddl::Predicate>(data.predicates, domain.get_context());
        data.functions = copy_list<pddl::FunctionSkeleton>(data.functions, domain.get_context());
        data.actions = split_disjunctive_actions(copy_list<pddl::Action>(data.actions, domain.get_context()));
        data.axioms = split_disjunctive_axioms(copy_list<pddl::Axiom>(data.axioms, domain.get_context()));
        add_type_predicates_to_domain(data);
        append_generated_domain_objects(data);
        auto view = m_storage->builder.domain(m_storage->repository, std::move(data));
        m_storage->translated_domain = view.get_index();
        return view;
    }

    pddl::TaskView copy_task(pddl::TaskView task)
    {
        auto data = task.get_data();
        m_append_generated_axioms_to_domain = false;
        data.index = {};
        data.domain = m_storage->translated_domain;
        data.requirements = copy_list<pddl::Requirement>(data.requirements, task.get_context());
        data.objects = copy_list<pddl::Object>(data.objects, task.get_context());
        data.initial_literals = copy_list<pddl::Literal>(data.initial_literals, task.get_context());
        data.initial_function_values = copy_list<pddl::InitialFunctionValue>(data.initial_function_values, task.get_context());
        data.goal = copy_optional<pddl::Condition>(data.goal, task.get_context());
        data.metric = copy_optional<pddl::Metric>(data.metric, task.get_context());
        data.axioms = split_disjunctive_axioms(copy_list<pddl::Axiom>(data.axioms, task.get_context()));
        if (data.goal)
            data.goal = simplify_goal_condition(*data.goal);
        for (auto axiom : m_generated_axioms)
            data.axioms.push_back(axiom);
        if (!m_generated_predicates.empty() || !m_generated_axioms.empty())
        {
            update_translated_domain();
            data.domain = m_storage->translated_domain;
        }
        initialize_type_literals(data);
        initialize_equality(data);
        auto view = m_storage->builder.task(m_storage->repository, std::move(data));
        remember(m_storage->tasks, task.get_index(), view.get_index());
        return view;
    }

private:
    std::shared_ptr<TranslationStorage> m_storage;
    std::unordered_map<ygg::uint_t, size_t> m_num_quantifications;
    std::vector<ygg::IndexList<pddl::Parameter>> m_active_parameters;
    ygg::IndexList<pddl::Predicate> m_generated_predicates;
    ygg::IndexList<pddl::Axiom> m_generated_axioms;
    size_t m_num_generated_axioms = 0;
    bool m_append_generated_axioms_to_domain = true;
    cista::optional<ygg::Index<pddl::Predicate>> m_equality_predicate;
    std::unordered_map<ygg::uint_t, ygg::Index<pddl::Predicate>> m_type_predicates;
    bool m_remove_typing = true;
    bool m_renaming_enabled = true;

    void increment_quantifications(const ygg::IndexList<pddl::Parameter>& parameters, const pddl::Repository& repository)
    {
        for (auto parameter : parameters)
        {
            const auto variable = repository[parameter].variable;
            const auto key = variable.get_value();
            if (auto it = m_num_quantifications.find(key); it != m_num_quantifications.end())
                ++it->second;
            else
                m_num_quantifications.emplace(key, 0);
        }
    }

    template<typename T>
    ygg::IndexList<T> copy_list(const ygg::IndexList<T>& source, const pddl::Repository& repository)
    {
        auto result = ygg::IndexList<T> {};
        for (auto index : source)
            result.push_back(copy(index, repository));
        return result;
    }

    ygg::IndexList<pddl::Parameter> copy_parameters(const ygg::IndexList<pddl::Parameter>& source, const pddl::Repository& repository)
    {
        return copy_list<pddl::Parameter>(source, repository);
    }

    ygg::IndexList<pddl::Type> collect_type_hierarchy(ygg::Index<pddl::Type> type) const
    {
        auto result = ygg::IndexList<pddl::Type> {};
        auto seen = std::unordered_set<ygg::uint_t> {};
        auto visit = [&](auto&& self, ygg::Index<pddl::Type> current) -> void
        {
            if (!seen.insert(current.get_value()).second)
                return;
            result.push_back(current);
            for (auto base : m_storage->repository[current].bases)
                self(self, base);
        };
        visit(visit, type);
        return result;
    }

    ygg::IndexList<pddl::Type> collect_type_hierarchy(const ygg::IndexList<pddl::Type>& types) const
    {
        auto result = ygg::IndexList<pddl::Type> {};
        auto seen = std::unordered_set<ygg::uint_t> {};
        for (auto type : types)
        {
            for (auto collected : collect_type_hierarchy(type))
            {
                if (seen.insert(collected.get_value()).second)
                    result.push_back(collected);
            }
        }
        return result;
    }

    ygg::IndexList<pddl::Type> maybe_strip_types(const ygg::IndexList<pddl::Type>& types) const
    {
        return m_remove_typing ? ygg::IndexList<pddl::Type> {} : types;
    }

    ygg::Index<pddl::Parameter> maybe_strip_parameter(ygg::Index<pddl::Parameter> parameter)
    {
        if (!m_remove_typing)
            return parameter;
        const auto& data = m_storage->repository[parameter];
        return m_storage->builder.parameter(m_storage->repository, data.variable, {}).get_index();
    }

    ygg::IndexList<pddl::Parameter> maybe_strip_parameters(const ygg::IndexList<pddl::Parameter>& parameters)
    {
        auto result = ygg::IndexList<pddl::Parameter> {};
        for (auto parameter : parameters)
            result.push_back(maybe_strip_parameter(parameter));
        return result;
    }

    ygg::Index<pddl::Object> maybe_strip_object(ygg::Index<pddl::Object> object)
    {
        if (!m_remove_typing)
            return object;
        const auto& data = m_storage->repository[object];
        return m_storage->builder.object(m_storage->repository, data.name, {}).get_index();
    }

    ygg::Index<pddl::Predicate> type_predicate(ygg::Index<pddl::Type> type)
    {
        if (auto it = m_type_predicates.find(type.get_value()); it != m_type_predicates.end())
            return it->second;
        const auto name = std::string(m_storage->repository[type].name);
        if (m_storage->translated_domain != ygg::Index<pddl::Domain>::max())
        {
            const auto& domain = m_storage->repository[m_storage->translated_domain];
            for (auto predicate : domain.predicates)
            {
                if (std::string(m_storage->repository[predicate].name) == name)
                {
                    m_type_predicates.emplace(type.get_value(), predicate);
                    return predicate;
                }
            }
        }
        auto parameters = ygg::IndexList<pddl::Parameter> {};
        const auto variable = m_storage->builder.variable(m_storage->repository, cista::offset::string("arg")).get_index();
        auto parameter_types = ygg::IndexList<pddl::Type> {};
        if (!m_remove_typing)
            parameter_types.push_back(type);
        parameters.push_back(m_storage->builder.parameter(m_storage->repository, variable, std::move(parameter_types)).get_index());
        const auto predicate = m_storage->builder.predicate(m_storage->repository, cista::offset::string(name), std::move(parameters)).get_index();
        m_type_predicates.emplace(type.get_value(), predicate);
        return predicate;
    }

    ygg::Index<pddl::Literal> type_literal(ygg::Index<pddl::Type> type, ygg::Index<pddl::Term> term)
    {
        auto terms = ygg::IndexList<pddl::Term> {};
        terms.push_back(term);
        const auto atom = m_storage->builder.atom(m_storage->repository, type_predicate(type), std::move(terms)).get_index();
        return m_storage->builder.literal(m_storage->repository, true, atom).get_index();
    }

    ygg::Index<pddl::Condition> type_condition(ygg::Index<pddl::Type> type, ygg::Index<pddl::Variable> variable)
    {
        const auto term = m_storage->builder.term(m_storage->repository, ygg::Data<pddl::Term>::Variant(variable)).get_index();
        return wrap_condition(m_storage->builder.condition_literal(m_storage->repository, type_literal(type, term)).get_index());
    }

    ygg::IndexList<pddl::Condition> type_conditions_for_parameters(const ygg::IndexList<pddl::Parameter>& parameters)
    {
        auto result = ygg::IndexList<pddl::Condition> {};
        for (auto parameter : parameters)
        {
            const auto& data = m_storage->repository[parameter];
            for (auto type : collect_type_hierarchy(data.types))
                result.push_back(type_condition(type, data.variable));
        }
        return result;
    }

    void prepend_type_conditions(cista::optional<ygg::Index<pddl::Condition>>& condition, const ygg::IndexList<pddl::Parameter>& parameters)
    {
        auto parts = type_conditions_for_parameters(parameters);
        if (parts.empty())
            return;
        if (condition)
            parts.push_back(*condition);
        condition = make_conjunction(std::move(parts));
    }

    void prepend_type_conditions(ygg::Index<pddl::Condition>& condition, const ygg::IndexList<pddl::Parameter>& parameters)
    {
        auto optional = cista::optional<ygg::Index<pddl::Condition>>(condition);
        prepend_type_conditions(optional, parameters);
        condition = *optional;
    }

    void enter_scope(const ygg::IndexList<pddl::Parameter>& parameters) { m_active_parameters.push_back(parameters); }

    void leave_scope() { m_active_parameters.pop_back(); }

    void append_generated_domain_objects(ygg::Data<pddl::Domain>& data)
    {
        if (m_generated_predicates.empty() && m_generated_axioms.empty())
            return;

        for (auto predicate : m_generated_predicates)
            data.predicates.push_back(predicate);
        if (m_append_generated_axioms_to_domain)
        {
            for (auto axiom : m_generated_axioms)
                data.axioms.push_back(axiom);
        }

        auto has_derived_requirement = false;
        for (auto requirement : data.requirements)
        {
            if (m_storage->repository[requirement].kind == pddl::RequirementKind::DerivedPredicates)
            {
                has_derived_requirement = true;
                break;
            }
        }
        if (!has_derived_requirement)
            data.requirements.push_back(m_storage->builder.requirement(m_storage->repository, pddl::RequirementKind::DerivedPredicates).get_index());
    }

    void update_translated_domain()
    {
        auto data = m_storage->repository[m_storage->translated_domain];
        append_generated_domain_objects(data);
        m_storage->translated_domain = m_storage->builder.domain(m_storage->repository, std::move(data)).get_index();
    }

    ygg::IndexList<pddl::Requirement> strip_typing_requirement(ygg::IndexList<pddl::Requirement> requirements)
    {
        if (!m_remove_typing)
            return requirements;
        auto result = ygg::IndexList<pddl::Requirement> {};
        for (auto requirement : requirements)
            if (m_storage->repository[requirement].kind != pddl::RequirementKind::Typing)
                result.push_back(requirement);
        return result;
    }

    void add_type_predicates_to_domain(ygg::Data<pddl::Domain>& data)
    {
        auto existing = std::unordered_set<std::string> {};
        for (auto predicate : data.predicates)
            existing.insert(std::string(m_storage->repository[predicate].name));
        for (auto type : collect_type_hierarchy(data.types))
        {
            const auto name = std::string(m_storage->repository[type].name);
            if (existing.insert(name).second)
                data.predicates.push_back(type_predicate(type));
        }
        data.requirements = strip_typing_requirement(data.requirements);
        if (m_remove_typing)
            data.types = {};
    }

    void add_type_literals_for_object(ygg::IndexList<pddl::Literal>& literals, ygg::Index<pddl::Object> object)
    {
        const auto& data = m_storage->repository[object];
        const auto term = m_storage->builder.term(m_storage->repository, ygg::Data<pddl::Term>::Variant(object)).get_index();
        auto types = data.types;
        if (types.empty())
        {
            if (auto it = m_storage->object_types_by_name.find(std::string(data.name)); it != m_storage->object_types_by_name.end())
                types = it->second;
        }
        for (auto type : collect_type_hierarchy(types))
            literals.push_back(type_literal(type, term));
    }

    void initialize_type_literals(ygg::Data<pddl::Task>& task)
    {
        const auto& domain = m_storage->repository[m_storage->translated_domain];
        for (auto object : domain.constants)
            add_type_literals_for_object(task.initial_literals, object);
        for (auto object : task.objects)
            add_type_literals_for_object(task.initial_literals, object);
        task.requirements = strip_typing_requirement(task.requirements);
    }

    template<typename T>
    cista::optional<ygg::Index<T>> copy_optional(const cista::optional<ygg::Index<T>>& source, const pddl::Repository& repository)
    {
        if (!source)
            return {};
        return copy(*source, repository);
    }

    ygg::Index<pddl::Requirement> copy(ygg::Index<pddl::Requirement> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Requirement> out;
        if (find_mapped(m_storage->requirements, source, out)) return out;
        auto kind = repository[source].kind;
        out = m_storage->builder.requirement(m_storage->repository, kind).get_index();
        remember(m_storage->requirements, source, out);
        return out;
    }

    ygg::Index<pddl::Type> copy(ygg::Index<pddl::Type> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Type> out;
        if (find_mapped(m_storage->types, source, out)) return out;
        const auto& data = repository[source];
        out = m_storage->builder.type(m_storage->repository, data.name, copy_list<pddl::Type>(data.bases, repository)).get_index();
        remember(m_storage->types, source, out);
        return out;
    }

    ygg::Index<pddl::Object> copy(ygg::Index<pddl::Object> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Object> out;
        if (find_mapped(m_storage->objects, source, out)) return out;
        const auto& data = repository[source];
        auto types = copy_list<pddl::Type>(data.types, repository);
        m_storage->object_types_by_name[std::string(data.name)] = types;
        out = m_storage->builder.object(m_storage->repository, data.name, maybe_strip_types(types)).get_index();
        remember(m_storage->objects, source, out);
        return out;
    }

    ygg::Index<pddl::Variable> copy(ygg::Index<pddl::Variable> source, const pddl::Repository& repository)
    {
        auto name = std::string(repository[source].name);
        if (m_renaming_enabled)
        {
            const auto key = source.get_value();
            if (auto it = m_num_quantifications.find(key); it != m_num_quantifications.end())
                name += "_" + std::to_string(it->second);
        }
        return m_storage->builder.variable(m_storage->repository, cista::offset::string(name)).get_index();
    }

    ygg::Index<pddl::Parameter> copy(ygg::Index<pddl::Parameter> source, const pddl::Repository& repository)
    {
        const auto& data = repository[source];
        return m_storage->builder.parameter(m_storage->repository, copy(data.variable, repository), copy_list<pddl::Type>(data.types, repository)).get_index();
    }

    ygg::Index<pddl::Predicate> copy(ygg::Index<pddl::Predicate> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Predicate> out;
        if (find_mapped(m_storage->predicates, source, out)) return out;
        const auto& data = repository[source];
        const auto previous = m_renaming_enabled;
        m_renaming_enabled = false;
        out = m_storage->builder.predicate(m_storage->repository, data.name, copy_list<pddl::Parameter>(data.parameters, repository)).get_index();
        m_renaming_enabled = previous;
        remember(m_storage->predicates, source, out);
        return out;
    }

    ygg::Index<pddl::FunctionSkeleton> copy(ygg::Index<pddl::FunctionSkeleton> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::FunctionSkeleton> out;
        if (find_mapped(m_storage->functions, source, out)) return out;
        const auto& data = repository[source];
        const auto previous = m_renaming_enabled;
        m_renaming_enabled = false;
        out = m_storage->builder.function_skeleton(m_storage->repository, data.name, copy_list<pddl::Parameter>(data.parameters, repository), copy(data.type, repository)).get_index();
        m_renaming_enabled = previous;
        remember(m_storage->functions, source, out);
        return out;
    }

    ygg::Index<pddl::Term> copy(ygg::Index<pddl::Term> source, const pddl::Repository& repository)
    {
        auto value = std::visit([&](const auto& arg) -> ygg::Data<pddl::Term>::Variant { return ygg::Data<pddl::Term>::Variant(copy(arg, repository)); }, repository[source].value);
        return m_storage->builder.term(m_storage->repository, std::move(value)).get_index();
    }

    ygg::Index<pddl::Atom> copy(ygg::Index<pddl::Atom> source, const pddl::Repository& repository)
    {
        const auto& data = repository[source];
        return m_storage->builder.atom(m_storage->repository, copy(data.predicate, repository), copy_list<pddl::Term>(data.terms, repository)).get_index();
    }

    ygg::Index<pddl::Literal> copy(ygg::Index<pddl::Literal> source, const pddl::Repository& repository)
    {
        const auto& data = repository[source];
        return m_storage->builder.literal(m_storage->repository, data.positive, copy(data.atom, repository)).get_index();
    }

    ygg::Index<pddl::FunctionExpressionNumber> copy(ygg::Index<pddl::FunctionExpressionNumber> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::FunctionExpressionNumber> out;
        if (find_mapped(m_storage->numbers, source, out)) return out;
        out = m_storage->builder.number(m_storage->repository, repository[source].value).get_index();
        remember(m_storage->numbers, source, out);
        return out;
    }

    ygg::Index<pddl::FunctionTerm> copy(ygg::Index<pddl::FunctionTerm> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::FunctionTerm> out;
        if (find_mapped(m_storage->function_terms, source, out)) return out;
        const auto& data = repository[source];
        out = m_storage->builder.function_term(m_storage->repository, copy(data.function, repository), copy_list<pddl::Term>(data.terms, repository)).get_index();
        remember(m_storage->function_terms, source, out);
        return out;
    }

    ygg::Index<pddl::UnaryFunctionExpression> copy(ygg::Index<pddl::UnaryFunctionExpression> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::UnaryFunctionExpression> out;
        if (find_mapped(m_storage->unary_expressions, source, out)) return out;
        const auto& data = repository[source];
        out = m_storage->builder.unary_function_expression(m_storage->repository, data.op, copy(data.expression, repository)).get_index();
        remember(m_storage->unary_expressions, source, out);
        return out;
    }

    ygg::Index<pddl::BinaryFunctionExpression> copy(ygg::Index<pddl::BinaryFunctionExpression> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::BinaryFunctionExpression> out;
        if (find_mapped(m_storage->binary_expressions, source, out)) return out;
        const auto& data = repository[source];
        out = m_storage->builder.binary_function_expression(m_storage->repository, data.op, copy(data.left, repository), copy(data.right, repository)).get_index();
        remember(m_storage->binary_expressions, source, out);
        return out;
    }

    ygg::Index<pddl::MultiFunctionExpression> copy(ygg::Index<pddl::MultiFunctionExpression> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::MultiFunctionExpression> out;
        if (find_mapped(m_storage->multi_expressions, source, out)) return out;
        const auto& data = repository[source];
        out = m_storage->builder.multi_function_expression(m_storage->repository, data.op, copy_list<pddl::FunctionExpression>(data.expressions, repository)).get_index();
        remember(m_storage->multi_expressions, source, out);
        return out;
    }

    ygg::Index<pddl::FunctionExpression> copy(ygg::Index<pddl::FunctionExpression> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::FunctionExpression> out;
        if (find_mapped(m_storage->function_expressions, source, out)) return out;
        auto value = std::visit([&](const auto& arg) -> ygg::Data<pddl::FunctionExpression>::Variant { return ygg::Data<pddl::FunctionExpression>::Variant(copy(arg, repository)); }, repository[source].value);
        out = m_storage->builder.function_expression(m_storage->repository, std::move(value)).get_index();
        remember(m_storage->function_expressions, source, out);
        return out;
    }

    ygg::Index<pddl::ConditionLiteral> copy(ygg::Index<pddl::ConditionLiteral> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::ConditionLiteral> out;
        if (find_mapped(m_storage->condition_literals, source, out)) return out;
        out = m_storage->builder.condition_literal(m_storage->repository, copy(repository[source].literal, repository)).get_index();
        remember(m_storage->condition_literals, source, out);
        return out;
    }

    ygg::Index<pddl::ConditionAnd> copy(ygg::Index<pddl::ConditionAnd> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::ConditionAnd> out;
        if (find_mapped(m_storage->condition_ands, source, out)) return out;
        out = m_storage->builder.condition_and(m_storage->repository, copy_list<pddl::Condition>(repository[source].conditions, repository)).get_index();
        remember(m_storage->condition_ands, source, out);
        return out;
    }

    ygg::Index<pddl::ConditionOr> copy(ygg::Index<pddl::ConditionOr> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::ConditionOr> out;
        if (find_mapped(m_storage->condition_ors, source, out)) return out;
        out = m_storage->builder.condition_or(m_storage->repository, copy_list<pddl::Condition>(repository[source].conditions, repository)).get_index();
        remember(m_storage->condition_ors, source, out);
        return out;
    }

    ygg::Index<pddl::ConditionNot> copy(ygg::Index<pddl::ConditionNot> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::ConditionNot> out;
        if (find_mapped(m_storage->condition_nots, source, out)) return out;
        out = m_storage->builder.condition_not(m_storage->repository, copy(repository[source].condition, repository)).get_index();
        remember(m_storage->condition_nots, source, out);
        return out;
    }

    ygg::Index<pddl::ConditionImply> copy(ygg::Index<pddl::ConditionImply> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::ConditionImply> out;
        if (find_mapped(m_storage->condition_implies, source, out)) return out;
        const auto& data = repository[source];
        out = m_storage->builder.condition_imply(m_storage->repository, copy(data.left, repository), copy(data.right, repository)).get_index();
        remember(m_storage->condition_implies, source, out);
        return out;
    }

    ygg::Index<pddl::ConditionExists> copy(ygg::Index<pddl::ConditionExists> source, const pddl::Repository& repository)
    {
        const auto& data = repository[source];
        increment_quantifications(data.parameters, repository);
        auto parameters = copy_parameters(data.parameters, repository);
        enter_scope(parameters);
        const auto condition = copy(data.condition, repository);
        leave_scope();
        auto typed_condition = condition;
        prepend_type_conditions(typed_condition, parameters);
        return m_storage->builder.condition_exists(m_storage->repository, maybe_strip_parameters(parameters), typed_condition).get_index();
    }

    ygg::Index<pddl::ConditionForall> copy(ygg::Index<pddl::ConditionForall> source, const pddl::Repository& repository)
    {
        const auto& data = repository[source];
        increment_quantifications(data.parameters, repository);
        auto parameters = copy_parameters(data.parameters, repository);
        enter_scope(parameters);
        const auto condition = copy(data.condition, repository);
        leave_scope();
        auto typed_condition = condition;
        prepend_type_conditions(typed_condition, parameters);
        return m_storage->builder.condition_forall(m_storage->repository, maybe_strip_parameters(parameters), typed_condition).get_index();
    }

    ygg::Index<pddl::ConditionNumericConstraint> copy(ygg::Index<pddl::ConditionNumericConstraint> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::ConditionNumericConstraint> out;
        if (find_mapped(m_storage->condition_numeric_constraints, source, out)) return out;
        const auto& data = repository[source];
        out = m_storage->builder.condition_numeric_constraint(m_storage->repository, data.comparator, copy(data.left, repository), copy(data.right, repository)).get_index();
        remember(m_storage->condition_numeric_constraints, source, out);
        return out;
    }


    ygg::Index<pddl::Term> term_from_variable(ygg::Index<pddl::Variable> variable)
    {
        return m_storage->builder.term(m_storage->repository, ygg::Data<pddl::Term>::Variant(variable)).get_index();
    }

    void collect_free_variables(ygg::Index<pddl::Term> term, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
    {
        std::visit([&](const auto& value)
        {
            using Value = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<Value, ygg::Index<pddl::Variable>>)
            {
                if (!bound.contains(value.get_value()))
                    free.insert(value.get_value());
            }
        }, m_storage->repository[term].value);
    }

    void collect_free_variables(ygg::Index<pddl::Atom> atom, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
    {
        for (auto term : m_storage->repository[atom].terms)
            collect_free_variables(term, bound, free);
    }

    void collect_free_variables(ygg::Index<pddl::Literal> literal, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
    {
        collect_free_variables(m_storage->repository[literal].atom, bound, free);
    }

    void collect_free_variables(ygg::Index<pddl::Condition> condition, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
    {
        std::visit([&](const auto& node) { collect_free_variables(node, bound, free); }, m_storage->repository[condition].value);
    }

    void collect_free_variables(ygg::Index<pddl::ConditionLiteral> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const { collect_free_variables(m_storage->repository[node].literal, bound, free); }
    void collect_free_variables(ygg::Index<pddl::ConditionNumericConstraint>, std::unordered_set<ygg::uint_t>&, std::unordered_set<ygg::uint_t>&) const {}
    void collect_free_variables(ygg::Index<pddl::ConditionNot> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const { collect_free_variables(m_storage->repository[node].condition, bound, free); }
    void collect_free_variables(ygg::Index<pddl::ConditionImply> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
    {
        collect_free_variables(m_storage->repository[node].left, bound, free);
        collect_free_variables(m_storage->repository[node].right, bound, free);
    }
    void collect_free_variables(ygg::Index<pddl::ConditionAnd> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
    {
        for (auto condition : m_storage->repository[node].conditions)
            collect_free_variables(condition, bound, free);
    }
    void collect_free_variables(ygg::Index<pddl::ConditionOr> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
    {
        for (auto condition : m_storage->repository[node].conditions)
            collect_free_variables(condition, bound, free);
    }
    void collect_free_variables(ygg::Index<pddl::ConditionExists> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
    {
        const auto& data = m_storage->repository[node];
        for (auto parameter : data.parameters)
            bound.insert(m_storage->repository[parameter].variable.get_value());
        collect_free_variables(data.condition, bound, free);
        for (auto parameter : data.parameters)
            bound.erase(m_storage->repository[parameter].variable.get_value());
    }
    void collect_free_variables(ygg::Index<pddl::ConditionForall> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
    {
        const auto& data = m_storage->repository[node];
        for (auto parameter : data.parameters)
            bound.insert(m_storage->repository[parameter].variable.get_value());
        collect_free_variables(data.condition, bound, free);
        for (auto parameter : data.parameters)
            bound.erase(m_storage->repository[parameter].variable.get_value());
    }

    ygg::IndexList<pddl::Parameter> free_parameters_in_scope(ygg::Index<pddl::Condition> condition) const
    {
        auto bound = std::unordered_set<ygg::uint_t> {};
        auto free = std::unordered_set<ygg::uint_t> {};
        collect_free_variables(condition, bound, free);

        auto result = ygg::IndexList<pddl::Parameter> {};
        for (const auto& scope : m_active_parameters)
        {
            for (auto parameter : scope)
            {
                if (free.contains(m_storage->repository[parameter].variable.get_value()))
                    result.push_back(parameter);
            }
        }
        return result;
    }

    ygg::Index<pddl::Condition> make_generated_axiom_condition(ygg::Index<pddl::Condition> condition)
    {
        const auto free_parameters = free_parameters_in_scope(condition);
        auto predicate_parameters = ygg::IndexList<pddl::Parameter> {};
        auto terms = ygg::IndexList<pddl::Term> {};
        for (auto parameter : free_parameters)
        {
            predicate_parameters.push_back(parameter);
            terms.push_back(term_from_variable(m_storage->repository[parameter].variable));
        }

        const auto name = cista::offset::string("_universal_" + std::to_string(m_num_generated_axioms++));
        const auto predicate = m_storage->builder.predicate(m_storage->repository, name, predicate_parameters).get_index();
        const auto atom = m_storage->builder.atom(m_storage->repository, predicate, terms).get_index();
        const auto positive_head = m_storage->builder.literal(m_storage->repository, true, atom).get_index();
        const auto negative_literal = m_storage->builder.literal(m_storage->repository, false, atom).get_index();
        const auto axiom = m_storage->builder.axiom(m_storage->repository, free_parameters, positive_head, condition).get_index();
        m_generated_predicates.push_back(predicate);
        m_generated_axioms.push_back(axiom);
        return wrap_condition(m_storage->builder.condition_literal(m_storage->repository, negative_literal).get_index());
    }

    ygg::Index<pddl::Condition> make_generated_goal_condition(ygg::Index<pddl::Condition> condition)
    {
        const auto name = cista::offset::string("_goal_" + std::to_string(m_num_generated_axioms++));
        const auto predicate = m_storage->builder.predicate(m_storage->repository, name, {}).get_index();
        const auto atom = m_storage->builder.atom(m_storage->repository, predicate, {}).get_index();
        const auto literal = m_storage->builder.literal(m_storage->repository, true, atom).get_index();
        const auto axiom = m_storage->builder.axiom(m_storage->repository, {}, literal, condition).get_index();
        m_generated_predicates.push_back(predicate);
        m_generated_axioms.push_back(axiom);
        return wrap_condition(m_storage->builder.condition_literal(m_storage->repository, literal).get_index());
    }

    ygg::Index<pddl::Condition> simplify_goal_condition(ygg::Index<pddl::Condition> condition)
    {
        return std::visit([&](const auto& node) { return simplify_goal_condition_node(condition, node); }, m_storage->repository[condition].value);
    }

    ygg::Index<pddl::Condition> simplify_goal_condition_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionLiteral>) { return condition; }
    ygg::Index<pddl::Condition> simplify_goal_condition_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionNumericConstraint>) { return condition; }
    ygg::Index<pddl::Condition> simplify_goal_condition_node(ygg::Index<pddl::Condition>, ygg::Index<pddl::ConditionAnd> node)
    {
        auto conditions = ygg::IndexList<pddl::Condition> {};
        for (auto child : m_storage->repository[node].conditions)
            conditions.push_back(simplify_goal_condition(child));
        return wrap_condition(m_storage->builder.condition_and(m_storage->repository, std::move(conditions)).get_index());
    }
    template<typename T>
    ygg::Index<pddl::Condition> simplify_goal_condition_node(ygg::Index<pddl::Condition> condition, ygg::Index<T>)
    {
        return make_generated_goal_condition(condition);
    }

    cista::optional<ygg::Index<pddl::ConditionExists>> as_exists(ygg::Index<pddl::Condition> condition) const
    {
        auto result = cista::optional<ygg::Index<pddl::ConditionExists>> {};
        std::visit([&](const auto& node)
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, ygg::Index<pddl::ConditionExists>>)
                result = node;
        }, m_storage->repository[condition].value);
        return result;
    }

    ygg::Index<pddl::Condition> move_existentials(ygg::Index<pddl::Condition> condition)
    {
        return std::visit([&](const auto& node) { return move_existentials_node(condition, node); }, m_storage->repository[condition].value);
    }

    ygg::Index<pddl::Condition> move_existentials_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionLiteral>) { return condition; }
    ygg::Index<pddl::Condition> move_existentials_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionNumericConstraint>) { return condition; }
    ygg::Index<pddl::Condition> move_existentials_node(ygg::Index<pddl::Condition>, ygg::Index<pddl::ConditionAnd> node)
    {
        auto parameters = ygg::IndexList<pddl::Parameter> {};
        auto parts = ygg::IndexList<pddl::Condition> {};
        for (auto child : m_storage->repository[node].conditions)
        {
            const auto moved = move_existentials(child);
            if (const auto exists = as_exists(moved))
            {
                const auto& exists_data = m_storage->repository[*exists];
                for (auto parameter : exists_data.parameters)
                    parameters.push_back(parameter);
                parts.push_back(exists_data.condition);
            }
            else
            {
                parts.push_back(moved);
            }
        }
        auto conjunction = wrap_condition(m_storage->builder.condition_and(m_storage->repository, std::move(parts)).get_index());
        if (parameters.empty())
            return conjunction;
        return wrap_condition(m_storage->builder.condition_exists(m_storage->repository, std::move(parameters), conjunction).get_index());
    }
    ygg::Index<pddl::Condition> move_existentials_node(ygg::Index<pddl::Condition>, ygg::Index<pddl::ConditionExists> node)
    {
        const auto& data = m_storage->repository[node];
        return wrap_condition(m_storage->builder.condition_exists(m_storage->repository, data.parameters, move_existentials(data.condition)).get_index());
    }
    ygg::Index<pddl::Condition> move_existentials_node(ygg::Index<pddl::Condition>, ygg::Index<pddl::ConditionOr> node)
    {
        auto parts = ygg::IndexList<pddl::Condition> {};
        for (auto child : m_storage->repository[node].conditions)
            parts.push_back(move_existentials(child));
        return wrap_condition(m_storage->builder.condition_or(m_storage->repository, std::move(parts)).get_index());
    }
    template<typename T>
    ygg::Index<pddl::Condition> move_existentials_node(ygg::Index<pddl::Condition> condition, ygg::Index<T>)
    {
        return condition;
    }

    void lift_top_level_exists(ygg::IndexList<pddl::Parameter>& parameters, cista::optional<ygg::Index<pddl::Condition>>& condition)
    {
        if (!condition)
            return;
        auto moved = move_existentials(*condition);
        if (const auto exists = as_exists(moved))
        {
            const auto& data = m_storage->repository[*exists];
            for (auto parameter : data.parameters)
                parameters.push_back(parameter);
            moved = data.condition;
        }
        condition = moved;
    }

    void lift_top_level_exists(ygg::IndexList<pddl::Parameter>& parameters, ygg::Index<pddl::Condition>& condition)
    {
        auto optional = cista::optional<ygg::Index<pddl::Condition>>(condition);
        lift_top_level_exists(parameters, optional);
        condition = *optional;
    }

    ygg::Index<pddl::Condition> wrap_condition(ygg::Data<pddl::Condition>::Variant value)
    {
        return m_storage->builder.condition(m_storage->repository, std::move(value)).get_index();
    }

    ygg::Index<pddl::Condition> wrap_condition(ygg::Index<pddl::ConditionLiteral> value) { return wrap_condition(ygg::Data<pddl::Condition>::Variant(value)); }
    ygg::Index<pddl::Condition> wrap_condition(ygg::Index<pddl::ConditionAnd> value) { return wrap_condition(ygg::Data<pddl::Condition>::Variant(value)); }
    ygg::Index<pddl::Condition> wrap_condition(ygg::Index<pddl::ConditionOr> value) { return wrap_condition(ygg::Data<pddl::Condition>::Variant(value)); }
    ygg::Index<pddl::Condition> wrap_condition(ygg::Index<pddl::ConditionNot> value) { return wrap_condition(ygg::Data<pddl::Condition>::Variant(value)); }
    ygg::Index<pddl::Condition> wrap_condition(ygg::Index<pddl::ConditionImply> value) { return wrap_condition(ygg::Data<pddl::Condition>::Variant(value)); }
    ygg::Index<pddl::Condition> wrap_condition(ygg::Index<pddl::ConditionExists> value) { return wrap_condition(ygg::Data<pddl::Condition>::Variant(value)); }
    ygg::Index<pddl::Condition> wrap_condition(ygg::Index<pddl::ConditionForall> value) { return wrap_condition(ygg::Data<pddl::Condition>::Variant(value)); }
    ygg::Index<pddl::Condition> wrap_condition(ygg::Index<pddl::ConditionNumericConstraint> value) { return wrap_condition(ygg::Data<pddl::Condition>::Variant(value)); }

    cista::optional<ygg::Index<pddl::ConditionOr>> as_or(ygg::Index<pddl::Condition> condition) const
    {
        auto result = cista::optional<ygg::Index<pddl::ConditionOr>> {};
        std::visit([&](const auto& node)
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, ygg::Index<pddl::ConditionOr>>)
                result = node;
        }, m_storage->repository[condition].value);
        return result;
    }

    ygg::Index<pddl::Condition> make_conjunction(ygg::IndexList<pddl::Condition> conditions)
    {
        return wrap_condition(m_storage->builder.condition_and(m_storage->repository, std::move(conditions)).get_index());
    }

    ygg::Index<pddl::Condition> make_disjunction(ygg::IndexList<pddl::Condition> conditions)
    {
        return wrap_condition(m_storage->builder.condition_or(m_storage->repository, std::move(conditions)).get_index());
    }

    ygg::Index<pddl::Condition> to_dnf(ygg::Index<pddl::Condition> condition)
    {
        return std::visit([&](const auto& node) { return to_dnf_node(condition, node); }, m_storage->repository[condition].value);
    }

    ygg::Index<pddl::Condition> to_dnf_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionLiteral>) { return condition; }
    ygg::Index<pddl::Condition> to_dnf_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionNumericConstraint>) { return condition; }
    ygg::Index<pddl::Condition> to_dnf_node(ygg::Index<pddl::Condition>, ygg::Index<pddl::ConditionOr> node)
    {
        auto parts = ygg::IndexList<pddl::Condition> {};
        for (auto child : m_storage->repository[node].conditions)
        {
            const auto dnf = to_dnf(child);
            if (const auto child_or = as_or(dnf))
            {
                for (auto nested : m_storage->repository[*child_or].conditions)
                    parts.push_back(nested);
            }
            else
            {
                parts.push_back(dnf);
            }
        }
        return make_disjunction(std::move(parts));
    }
    ygg::Index<pddl::Condition> to_dnf_node(ygg::Index<pddl::Condition>, ygg::Index<pddl::ConditionAnd> node)
    {
        auto combinations = std::vector<ygg::IndexList<pddl::Condition>> { ygg::IndexList<pddl::Condition> {} };
        for (auto child : m_storage->repository[node].conditions)
        {
            const auto dnf = to_dnf(child);
            auto alternatives = ygg::IndexList<pddl::Condition> {};
            if (const auto child_or = as_or(dnf))
            {
                for (auto nested : m_storage->repository[*child_or].conditions)
                    alternatives.push_back(nested);
            }
            else
            {
                alternatives.push_back(dnf);
            }
            auto next = std::vector<ygg::IndexList<pddl::Condition>> {};
            for (const auto& combination : combinations)
            {
                for (auto alternative : alternatives)
                {
                    auto extended = combination;
                    extended.push_back(alternative);
                    next.push_back(std::move(extended));
                }
            }
            combinations = std::move(next);
        }
        if (combinations.size() == 1)
            return make_conjunction(std::move(combinations.front()));
        auto disjuncts = ygg::IndexList<pddl::Condition> {};
        for (auto& combination : combinations)
            disjuncts.push_back(make_conjunction(std::move(combination)));
        return make_disjunction(std::move(disjuncts));
    }
    ygg::Index<pddl::Condition> to_dnf_node(ygg::Index<pddl::Condition>, ygg::Index<pddl::ConditionExists> node)
    {
        const auto& data = m_storage->repository[node];
        const auto child = to_dnf(data.condition);
        if (const auto child_or = as_or(child))
        {
            auto parts = ygg::IndexList<pddl::Condition> {};
            for (auto nested : m_storage->repository[*child_or].conditions)
                parts.push_back(wrap_condition(m_storage->builder.condition_exists(m_storage->repository, data.parameters, nested).get_index()));
            return to_dnf(make_disjunction(std::move(parts)));
        }
        return wrap_condition(m_storage->builder.condition_exists(m_storage->repository, data.parameters, child).get_index());
    }
    template<typename T>
    ygg::Index<pddl::Condition> to_dnf_node(ygg::Index<pddl::Condition> condition, ygg::Index<T>)
    {
        return condition;
    }

    static pddl::BinaryComparator negate_comparator(pddl::BinaryComparator comparator)
    {
        switch (comparator)
        {
            case pddl::BinaryComparator::Equal: return pddl::BinaryComparator::NotEqual;
            case pddl::BinaryComparator::NotEqual: return pddl::BinaryComparator::Equal;
            case pddl::BinaryComparator::Less: return pddl::BinaryComparator::GreaterEqual;
            case pddl::BinaryComparator::LessEqual: return pddl::BinaryComparator::Greater;
            case pddl::BinaryComparator::Greater: return pddl::BinaryComparator::LessEqual;
            case pddl::BinaryComparator::GreaterEqual: return pddl::BinaryComparator::Less;
        }
        return pddl::BinaryComparator::NotEqual;
    }

    ygg::Index<pddl::Condition> negate_condition(ygg::Index<pddl::Condition> source, const pddl::Repository& repository)
    {
        return std::visit([&](const auto& arg) { return negate_condition_node(arg, repository); }, repository[source].value);
    }

    ygg::Index<pddl::Condition> negate_condition_node(ygg::Index<pddl::ConditionLiteral> source, const pddl::Repository& repository)
    {
        const auto literal = repository[repository[source].literal];
        const auto negated_literal = m_storage->builder.literal(m_storage->repository, !literal.positive, copy(literal.atom, repository)).get_index();
        return wrap_condition(m_storage->builder.condition_literal(m_storage->repository, negated_literal).get_index());
    }

    ygg::Index<pddl::Condition> negate_condition_node(ygg::Index<pddl::ConditionAnd> source, const pddl::Repository& repository)
    {
        auto conditions = ygg::IndexList<pddl::Condition> {};
        for (auto condition : repository[source].conditions)
            conditions.push_back(negate_condition(condition, repository));
        return wrap_condition(m_storage->builder.condition_or(m_storage->repository, std::move(conditions)).get_index());
    }

    ygg::Index<pddl::Condition> negate_condition_node(ygg::Index<pddl::ConditionOr> source, const pddl::Repository& repository)
    {
        auto conditions = ygg::IndexList<pddl::Condition> {};
        for (auto condition : repository[source].conditions)
            conditions.push_back(negate_condition(condition, repository));
        return wrap_condition(m_storage->builder.condition_and(m_storage->repository, std::move(conditions)).get_index());
    }

    ygg::Index<pddl::Condition> negate_condition_node(ygg::Index<pddl::ConditionNot> source, const pddl::Repository& repository)
    {
        return copy(repository[source].condition, repository);
    }

    ygg::Index<pddl::Condition> negate_condition_node(ygg::Index<pddl::ConditionImply> source, const pddl::Repository& repository)
    {
        const auto& data = repository[source];
        auto conditions = ygg::IndexList<pddl::Condition> {};
        conditions.push_back(copy(data.left, repository));
        conditions.push_back(negate_condition(data.right, repository));
        return wrap_condition(m_storage->builder.condition_and(m_storage->repository, std::move(conditions)).get_index());
    }

    ygg::Index<pddl::Condition> negate_condition_node(ygg::Index<pddl::ConditionExists> source, const pddl::Repository& repository)
    {
        const auto& data = repository[source];
        increment_quantifications(data.parameters, repository);
        auto parameters = copy_parameters(data.parameters, repository);
        enter_scope(parameters);
        const auto negated = negate_condition(data.condition, repository);
        leave_scope();
        const auto exists_not = wrap_condition(m_storage->builder.condition_exists(m_storage->repository, std::move(parameters), negated).get_index());
        return make_generated_axiom_condition(exists_not);
    }

    ygg::Index<pddl::Condition> negate_condition_node(ygg::Index<pddl::ConditionForall> source, const pddl::Repository& repository)
    {
        const auto& data = repository[source];
        increment_quantifications(data.parameters, repository);
        auto parameters = copy_parameters(data.parameters, repository);
        enter_scope(parameters);
        const auto condition = negate_condition(data.condition, repository);
        leave_scope();
        return wrap_condition(m_storage->builder.condition_exists(m_storage->repository, std::move(parameters), condition).get_index());
    }

    ygg::Index<pddl::Condition> negate_condition_node(ygg::Index<pddl::ConditionNumericConstraint> source, const pddl::Repository& repository)
    {
        const auto& data = repository[source];
        return wrap_condition(m_storage->builder.condition_numeric_constraint(m_storage->repository, negate_comparator(data.comparator), copy(data.left, repository), copy(data.right, repository)).get_index());
    }

    ygg::Index<pddl::Condition> copy_condition_node(ygg::Index<pddl::ConditionLiteral> source, const pddl::Repository& repository) { return wrap_condition(copy(source, repository)); }
    ygg::Index<pddl::Condition> copy_condition_node(ygg::Index<pddl::ConditionAnd> source, const pddl::Repository& repository) { return wrap_condition(copy(source, repository)); }
    ygg::Index<pddl::Condition> copy_condition_node(ygg::Index<pddl::ConditionOr> source, const pddl::Repository& repository) { return wrap_condition(copy(source, repository)); }
    ygg::Index<pddl::Condition> copy_condition_node(ygg::Index<pddl::ConditionNot> source, const pddl::Repository& repository) { return negate_condition(repository[source].condition, repository); }
    ygg::Index<pddl::Condition> copy_condition_node(ygg::Index<pddl::ConditionImply> source, const pddl::Repository& repository)
    {
        const auto& data = repository[source];
        auto conditions = ygg::IndexList<pddl::Condition> {};
        conditions.push_back(negate_condition(data.left, repository));
        conditions.push_back(copy(data.right, repository));
        return wrap_condition(m_storage->builder.condition_or(m_storage->repository, std::move(conditions)).get_index());
    }
    ygg::Index<pddl::Condition> copy_condition_node(ygg::Index<pddl::ConditionExists> source, const pddl::Repository& repository) { return wrap_condition(copy(source, repository)); }
    ygg::Index<pddl::Condition> copy_condition_node(ygg::Index<pddl::ConditionForall> source, const pddl::Repository& repository)
    {
        const auto& data = repository[source];
        increment_quantifications(data.parameters, repository);
        auto parameters = copy_parameters(data.parameters, repository);
        enter_scope(parameters);
        const auto negated = negate_condition(data.condition, repository);
        leave_scope();
        const auto exists_not = wrap_condition(m_storage->builder.condition_exists(m_storage->repository, std::move(parameters), negated).get_index());
        return make_generated_axiom_condition(exists_not);
    }
    ygg::Index<pddl::Condition> copy_condition_node(ygg::Index<pddl::ConditionNumericConstraint> source, const pddl::Repository& repository) { return wrap_condition(copy(source, repository)); }

    ygg::Index<pddl::Condition> copy(ygg::Index<pddl::Condition> source, const pddl::Repository& repository)
    {
        return to_dnf(std::visit([&](const auto& arg) { return copy_condition_node(arg, repository); }, repository[source].value));
    }

    ygg::Index<pddl::Effect> wrap_effect(ygg::Data<pddl::Effect>::Variant value)
    {
        return m_storage->builder.effect(m_storage->repository, std::move(value)).get_index();
    }

    template<typename T>
    cista::optional<ygg::Index<T>> as_effect(ygg::Index<pddl::Effect> effect) const
    {
        auto result = cista::optional<ygg::Index<T>> {};
        std::visit([&](const auto& node)
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, ygg::Index<T>>)
                result = node;
        }, m_storage->repository[effect].value);
        return result;
    }

    ygg::Index<pddl::Effect> normalize_effect(ygg::Index<pddl::Effect> effect)
    {
        return std::visit([&](const auto& node) { return normalize_effect_node(effect, node); }, m_storage->repository[effect].value);
    }

    ygg::Index<pddl::Effect> normalize_effect_node(ygg::Index<pddl::Effect> effect, ygg::Index<pddl::EffectLiteral>) { return effect; }
    ygg::Index<pddl::Effect> normalize_effect_node(ygg::Index<pddl::Effect> effect, ygg::Index<pddl::EffectNumeric>) { return effect; }
    ygg::Index<pddl::Effect> normalize_effect_node(ygg::Index<pddl::Effect> effect, ygg::Index<pddl::EffectOneOf>) { return effect; }
    ygg::Index<pddl::Effect> normalize_effect_node(ygg::Index<pddl::Effect> effect, ygg::Index<pddl::EffectProbabilistic>) { return effect; }

    ygg::Index<pddl::Effect> normalize_effect_node(ygg::Index<pddl::Effect>, ygg::Index<pddl::EffectAnd> node)
    {
        auto effects = ygg::IndexList<pddl::Effect> {};
        for (auto child : m_storage->repository[node].effects)
        {
            const auto normalized = normalize_effect(child);
            if (const auto nested = as_effect<pddl::EffectAnd>(normalized))
            {
                for (auto part : m_storage->repository[*nested].effects)
                    effects.push_back(part);
            }
            else
            {
                effects.push_back(normalized);
            }
        }
        return wrap_effect(ygg::Data<pddl::Effect>::Variant(m_storage->builder.effect_and(m_storage->repository, std::move(effects)).get_index()));
    }

    ygg::Index<pddl::Effect> normalize_effect_node(ygg::Index<pddl::Effect>, ygg::Index<pddl::EffectForall> node)
    {
        const auto& data = m_storage->repository[node];
        const auto nested = normalize_effect(data.effect);
        if (const auto nested_and = as_effect<pddl::EffectAnd>(nested))
        {
            auto effects = ygg::IndexList<pddl::Effect> {};
            for (auto part : m_storage->repository[*nested_and].effects)
                effects.push_back(normalize_effect(wrap_effect(ygg::Data<pddl::Effect>::Variant(m_storage->builder.effect_forall(m_storage->repository, data.parameters, part).get_index()))));
            return normalize_effect(wrap_effect(ygg::Data<pddl::Effect>::Variant(m_storage->builder.effect_and(m_storage->repository, std::move(effects)).get_index())));
        }
        if (const auto nested_forall = as_effect<pddl::EffectForall>(nested))
        {
            auto parameters = data.parameters;
            for (auto parameter : m_storage->repository[*nested_forall].parameters)
                parameters.push_back(parameter);
            return normalize_effect(wrap_effect(ygg::Data<pddl::Effect>::Variant(m_storage->builder.effect_forall(m_storage->repository, std::move(parameters), m_storage->repository[*nested_forall].effect).get_index())));
        }
        return wrap_effect(ygg::Data<pddl::Effect>::Variant(m_storage->builder.effect_forall(m_storage->repository, data.parameters, nested).get_index()));
    }

    ygg::Index<pddl::Effect> normalize_effect_node(ygg::Index<pddl::Effect>, ygg::Index<pddl::EffectWhen> node)
    {
        const auto& data = m_storage->repository[node];
        const auto condition = data.condition;
        const auto nested = normalize_effect(data.effect);

        if (const auto condition_or = as_or(condition))
        {
            auto effects = ygg::IndexList<pddl::Effect> {};
            for (auto part : m_storage->repository[*condition_or].conditions)
                effects.push_back(normalize_effect(wrap_effect(ygg::Data<pddl::Effect>::Variant(m_storage->builder.effect_when(m_storage->repository, part, nested).get_index()))));
            return normalize_effect(wrap_effect(ygg::Data<pddl::Effect>::Variant(m_storage->builder.effect_and(m_storage->repository, std::move(effects)).get_index())));
        }

        if (const auto nested_when = as_effect<pddl::EffectWhen>(nested))
        {
            auto conditions = ygg::IndexList<pddl::Condition> {};
            conditions.push_back(condition);
            conditions.push_back(m_storage->repository[*nested_when].condition);
            const auto combined_condition = make_conjunction(std::move(conditions));
            return normalize_effect(wrap_effect(ygg::Data<pddl::Effect>::Variant(m_storage->builder.effect_when(m_storage->repository, combined_condition, m_storage->repository[*nested_when].effect).get_index())));
        }
        if (const auto nested_and = as_effect<pddl::EffectAnd>(nested))
        {
            auto effects = ygg::IndexList<pddl::Effect> {};
            for (auto part : m_storage->repository[*nested_and].effects)
                effects.push_back(normalize_effect(wrap_effect(ygg::Data<pddl::Effect>::Variant(m_storage->builder.effect_when(m_storage->repository, condition, part).get_index()))));
            return normalize_effect(wrap_effect(ygg::Data<pddl::Effect>::Variant(m_storage->builder.effect_and(m_storage->repository, std::move(effects)).get_index())));
        }
        if (const auto nested_forall = as_effect<pddl::EffectForall>(nested))
        {
            const auto pushed_when = wrap_effect(ygg::Data<pddl::Effect>::Variant(m_storage->builder.effect_when(m_storage->repository, condition, m_storage->repository[*nested_forall].effect).get_index()));
            return normalize_effect(wrap_effect(ygg::Data<pddl::Effect>::Variant(m_storage->builder.effect_forall(m_storage->repository, m_storage->repository[*nested_forall].parameters, pushed_when).get_index())));
        }
        if (const auto exists = as_exists(condition))
        {
            const auto pushed_when = wrap_effect(ygg::Data<pddl::Effect>::Variant(m_storage->builder.effect_when(m_storage->repository, m_storage->repository[*exists].condition, nested).get_index()));
            return normalize_effect(wrap_effect(ygg::Data<pddl::Effect>::Variant(m_storage->builder.effect_forall(m_storage->repository, m_storage->repository[*exists].parameters, pushed_when).get_index())));
        }

        return wrap_effect(ygg::Data<pddl::Effect>::Variant(m_storage->builder.effect_when(m_storage->repository, condition, nested).get_index()));
    }

    ygg::Index<pddl::EffectLiteral> copy(ygg::Index<pddl::EffectLiteral> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::EffectLiteral> out;
        if (find_mapped(m_storage->effect_literals, source, out)) return out;
        out = m_storage->builder.effect_literal(m_storage->repository, copy(repository[source].literal, repository)).get_index();
        remember(m_storage->effect_literals, source, out);
        return out;
    }

    ygg::Index<pddl::EffectAnd> copy(ygg::Index<pddl::EffectAnd> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::EffectAnd> out;
        if (find_mapped(m_storage->effect_ands, source, out)) return out;
        out = m_storage->builder.effect_and(m_storage->repository, copy_list<pddl::Effect>(repository[source].effects, repository)).get_index();
        remember(m_storage->effect_ands, source, out);
        return out;
    }

    ygg::Index<pddl::EffectNumeric> copy(ygg::Index<pddl::EffectNumeric> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::EffectNumeric> out;
        if (find_mapped(m_storage->effect_numerics, source, out)) return out;
        const auto& data = repository[source];
        out = m_storage->builder.effect_numeric(m_storage->repository, data.op, copy(data.function, repository), copy_list<pddl::Term>(data.terms, repository), copy(data.expression, repository)).get_index();
        remember(m_storage->effect_numerics, source, out);
        return out;
    }

    ygg::Index<pddl::EffectForall> copy(ygg::Index<pddl::EffectForall> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::EffectForall> out;
        if (find_mapped(m_storage->effect_foralls, source, out)) return out;
        const auto& data = repository[source];
        increment_quantifications(data.parameters, repository);
        auto parameters = copy_parameters(data.parameters, repository);
        enter_scope(parameters);
        auto effect = copy(data.effect, repository);
        auto guard = type_conditions_for_parameters(parameters);
        if (!guard.empty())
        {
            const auto condition = make_conjunction(std::move(guard));
            effect = wrap_effect(ygg::Data<pddl::Effect>::Variant(m_storage->builder.effect_when(m_storage->repository, condition, effect).get_index()));
        }
        out = m_storage->builder.effect_forall(m_storage->repository, maybe_strip_parameters(parameters), effect).get_index();
        leave_scope();
        remember(m_storage->effect_foralls, source, out);
        return out;
    }

    ygg::Index<pddl::EffectWhen> copy(ygg::Index<pddl::EffectWhen> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::EffectWhen> out;
        if (find_mapped(m_storage->effect_whens, source, out)) return out;
        const auto& data = repository[source];
        out = m_storage->builder.effect_when(m_storage->repository, copy(data.condition, repository), copy(data.effect, repository)).get_index();
        remember(m_storage->effect_whens, source, out);
        return out;
    }

    ygg::Index<pddl::EffectOneOf> copy(ygg::Index<pddl::EffectOneOf> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::EffectOneOf> out;
        if (find_mapped(m_storage->effect_one_ofs, source, out)) return out;
        out = m_storage->builder.effect_one_of(m_storage->repository, copy_list<pddl::Effect>(repository[source].effects, repository)).get_index();
        remember(m_storage->effect_one_ofs, source, out);
        return out;
    }

    ygg::Index<pddl::EffectProbabilisticAlternative> copy(ygg::Index<pddl::EffectProbabilisticAlternative> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::EffectProbabilisticAlternative> out;
        if (find_mapped(m_storage->effect_probabilistic_alternatives, source, out)) return out;
        const auto& data = repository[source];
        out = m_storage->builder.effect_probabilistic_alternative(m_storage->repository, data.probability, copy(data.effect, repository)).get_index();
        remember(m_storage->effect_probabilistic_alternatives, source, out);
        return out;
    }

    ygg::Index<pddl::EffectProbabilistic> copy(ygg::Index<pddl::EffectProbabilistic> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::EffectProbabilistic> out;
        if (find_mapped(m_storage->effect_probabilistics, source, out)) return out;
        out = m_storage->builder.effect_probabilistic(m_storage->repository, copy_list<pddl::EffectProbabilisticAlternative>(repository[source].alternatives, repository)).get_index();
        remember(m_storage->effect_probabilistics, source, out);
        return out;
    }

    ygg::Index<pddl::Effect> copy(ygg::Index<pddl::Effect> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Effect> out;
        if (find_mapped(m_storage->effects, source, out)) return out;
        auto value = std::visit([&](const auto& arg) -> ygg::Data<pddl::Effect>::Variant { return ygg::Data<pddl::Effect>::Variant(copy(arg, repository)); }, repository[source].value);
        out = normalize_effect(m_storage->builder.effect(m_storage->repository, std::move(value)).get_index());
        remember(m_storage->effects, source, out);
        return out;
    }

    ygg::Index<pddl::Action> copy(ygg::Index<pddl::Action> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Action> out;
        if (find_mapped(m_storage->actions, source, out)) return out;
        const auto& data = repository[source];
        m_num_quantifications.clear();
        increment_quantifications(data.parameters, repository);
        auto parameters = copy_parameters(data.parameters, repository);
        enter_scope(parameters);
        auto precondition = copy_optional<pddl::Condition>(data.precondition, repository);
        lift_top_level_exists(parameters, precondition);
        prepend_type_conditions(precondition, parameters);
        out = m_storage->builder.action(m_storage->repository, data.name, maybe_strip_parameters(parameters), precondition, copy_optional<pddl::Effect>(data.effect, repository)).get_index();
        leave_scope();
        remember(m_storage->actions, source, out);
        return out;
    }

    ygg::Index<pddl::Axiom> copy(ygg::Index<pddl::Axiom> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Axiom> out;
        if (find_mapped(m_storage->axioms, source, out)) return out;
        const auto& data = repository[source];
        m_num_quantifications.clear();
        increment_quantifications(data.parameters, repository);
        auto parameters = copy_parameters(data.parameters, repository);
        enter_scope(parameters);
        auto condition = copy(data.condition, repository);
        lift_top_level_exists(parameters, condition);
        prepend_type_conditions(condition, parameters);
        out = m_storage->builder.axiom(m_storage->repository, maybe_strip_parameters(parameters), copy(data.head, repository), condition).get_index();
        leave_scope();
        remember(m_storage->axioms, source, out);
        return out;
    }

    ygg::Index<pddl::Metric> copy(ygg::Index<pddl::Metric> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Metric> out;
        if (find_mapped(m_storage->metrics, source, out)) return out;
        const auto& data = repository[source];
        out = m_storage->builder.metric(m_storage->repository, data.minimize, copy(data.expression, repository)).get_index();
        remember(m_storage->metrics, source, out);
        return out;
    }

    ygg::Index<pddl::InitialFunctionValue> copy(ygg::Index<pddl::InitialFunctionValue> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::InitialFunctionValue> out;
        if (find_mapped(m_storage->initial_function_values, source, out)) return out;
        const auto& data = repository[source];
        out = m_storage->builder.initial_function_value(m_storage->repository, copy(data.function, repository), copy(data.value, repository)).get_index();
        remember(m_storage->initial_function_values, source, out);
        return out;
    }
};

} // namespace detail

class DomainTranslationResult
{
public:
    DomainTranslationResult(pddl::DomainView original_domain_, std::shared_ptr<detail::TranslationStorage> storage_) :
        m_original_domain(original_domain_),
        m_storage(std::move(storage_))
    {
    }

    pddl::DomainView get_original_domain() const noexcept { return m_original_domain; }
    pddl::DomainView get_translated_domain() const noexcept { return ygg::make_view(m_storage->translated_domain, m_storage->repository); }
    const pddl::Repository& get_repository() const noexcept { return m_storage->repository; }
    pddl::Repository& get_repository() noexcept { return m_storage->repository; }

private:
    friend pddl::TaskView translate(pddl::TaskView task, const DomainTranslationResult& result, const TranslatorOptions& options);

    pddl::DomainView m_original_domain;
    std::shared_ptr<detail::TranslationStorage> m_storage;
};

inline DomainTranslationResult translate(pddl::DomainView domain, const TranslatorOptions& options = {})
{
    auto storage = std::make_shared<detail::TranslationStorage>();
    auto copier = detail::CopyTranslator(storage, options.remove_typing);
    for ([[maybe_unused]] auto step : domain_translation_steps())
    {
    }
    copier.copy_domain(domain);
    return DomainTranslationResult(domain, std::move(storage));
}

inline pddl::TaskView translate(pddl::TaskView task, const DomainTranslationResult& result, const TranslatorOptions& options = {})
{
    if (task.get_data().domain != result.get_original_domain().get_index())
        throw std::runtime_error("translate(task, result): task domain must match original domain in DomainTranslationResult.");

    auto copier = detail::CopyTranslator(result.m_storage, options.remove_typing);
    for ([[maybe_unused]] auto step : task_translation_steps())
    {
    }
    return copier.copy_task(task);
}

} // namespace loki2::semantic

#endif
