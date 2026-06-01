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

#include "loki/formalism/formalism.hpp"

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
    formalism::Repository repository;
    ygg::Index<formalism::Domain> original_domain;
    ygg::Index<formalism::Domain> translated_domain;

    IndexMap<formalism::Requirement> requirements;
    IndexMap<formalism::Type> types;
    IndexMap<formalism::Object> objects;
    IndexMap<formalism::Variable> variables;
    IndexMap<formalism::Parameter> parameters;
    IndexMap<formalism::Predicate> predicates;
    IndexMap<formalism::FunctionSkeleton> functions;
    IndexMap<formalism::Term> terms;
    IndexMap<formalism::Atom> atoms;
    IndexMap<formalism::Literal> literals;
    IndexMap<formalism::FunctionExpressionNumber> numbers;
    IndexMap<formalism::FunctionTerm> function_terms;
    IndexMap<formalism::UnaryFunctionExpression> unary_expressions;
    IndexMap<formalism::BinaryFunctionExpression> binary_expressions;
    IndexMap<formalism::MultiFunctionExpression> multi_expressions;
    IndexMap<formalism::FunctionExpression> function_expressions;
    IndexMap<formalism::ConditionLiteral> condition_literals;
    IndexMap<formalism::ConditionAnd> condition_ands;
    IndexMap<formalism::ConditionOr> condition_ors;
    IndexMap<formalism::ConditionNot> condition_nots;
    IndexMap<formalism::ConditionImply> condition_implies;
    IndexMap<formalism::ConditionExists> condition_exists;
    IndexMap<formalism::ConditionForall> condition_foralls;
    IndexMap<formalism::ConditionNumericConstraint> condition_numeric_constraints;
    IndexMap<formalism::Condition> conditions;
    IndexMap<formalism::EffectLiteral> effect_literals;
    IndexMap<formalism::EffectAnd> effect_ands;
    IndexMap<formalism::EffectNumeric> effect_numerics;
    IndexMap<formalism::EffectForall> effect_foralls;
    IndexMap<formalism::EffectWhen> effect_whens;
    IndexMap<formalism::EffectOneOf> effect_one_ofs;
    IndexMap<formalism::EffectProbabilisticAlternative> effect_probabilistic_alternatives;
    IndexMap<formalism::EffectProbabilistic> effect_probabilistics;
    IndexMap<formalism::Effect> effects;
    IndexMap<formalism::Action> actions;
    IndexMap<formalism::Axiom> axioms;
    IndexMap<formalism::Metric> metrics;
    IndexMap<formalism::InitialFunctionValue> initial_function_values;
    IndexMap<formalism::Domain> domains;
    IndexMap<formalism::Task> tasks;
    std::unordered_map<std::string, ygg::IndexList<formalism::Type>> object_types_by_name;

    explicit TranslationStorage(size_t index = 1, const formalism::Repository* parent = nullptr) : repository(index, parent) {}
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
