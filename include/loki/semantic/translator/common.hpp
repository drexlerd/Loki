/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_SEMANTIC_TRANSLATOR_COMMON_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_COMMON_HPP_

#include "loki/pddl/pddl.hpp"

#include <cista/containers/optional.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

namespace loki::semantic
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
    IndexMap<pddl::Domain> domains;
    IndexMap<pddl::Task> tasks;
    std::unordered_map<std::string, ygg::IndexList<pddl::Type>> object_types_by_name;

    explicit TranslationStorage(size_t index = 1, const pddl::Repository* parent = nullptr) : repository(index, parent) {}
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

} // namespace detail
} // namespace loki::semantic

#endif
