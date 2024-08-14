/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *<
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LOKI_INCLUDE_LOKI_PDDL_HASH_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_HASH_HPP_

#include "loki/details/pddl/declarations.hpp"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <ranges>
#include <utility>
#include <variant>

namespace loki
{

/// @brief `UniquePDDLEqualTo` is used to compare newly created PDDL objects for uniqueness.
/// Since the children are unique, it suffices to create a combined hash from nested pointers.
template<typename T>
struct UniquePDDLHasher
{
    size_t operator()(const T& element) const { return std::hash<T>()(element); }
};

struct UniquePDDLHashCombiner
{
public:
    template<typename T>
    void operator()(size_t& seed, const T& value) const
    {
        seed ^= UniquePDDLHasher<T>()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    void operator()(size_t& seed, const std::size_t& value) const { seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2); }

    template<typename... Types>
    size_t operator()(const Types&... args) const
    {
        size_t seed = 0;
        ((*this)(seed, args), ...);
        return seed;
    }
};

/// Spezialization for std::ranges::forward_range.
template<typename ForwardRange>
requires std::ranges::forward_range<ForwardRange>
struct UniquePDDLHasher<ForwardRange>
{
    size_t operator()(const ForwardRange& range) const
    {
        std::size_t aggregated_hash = 0;
        for (const auto& item : range)
        {
            UniquePDDLHashCombiner()(aggregated_hash, item);
        }
        return aggregated_hash;
    }
};

/// Spezialization for std::variant.
template<typename... Ts>
struct UniquePDDLHasher<std::variant<Ts...>>
{
    size_t operator()(const std::variant<Ts...>& variant) const
    {
        return std::visit([](const auto& arg) { return UniquePDDLHasher<decltype(arg)>()(arg); }, variant);
    }
};

/**
 * Specializations for PDDL
 */

template<>
struct UniquePDDLHasher<const ActionImpl*>
{
    size_t operator()(const ActionImpl* e) const;
};

template<>
struct UniquePDDLHasher<const AtomImpl*>
{
    size_t operator()(const AtomImpl* e) const;
};

template<>
struct UniquePDDLHasher<const AxiomImpl*>
{
    size_t operator()(const AxiomImpl* e) const;
};

template<>
struct UniquePDDLHasher<const ConditionLiteralImpl&>
{
    size_t operator()(const ConditionLiteralImpl& e) const;
};

template<>
struct UniquePDDLHasher<const ConditionAndImpl&>
{
    size_t operator()(const ConditionAndImpl& e) const;
};

template<>
struct UniquePDDLHasher<const ConditionOrImpl&>
{
    size_t operator()(const ConditionOrImpl& e) const;
};

template<>
struct UniquePDDLHasher<const ConditionNotImpl&>
{
    size_t operator()(const ConditionNotImpl& e) const;
};

template<>
struct UniquePDDLHasher<const ConditionImplyImpl&>
{
    size_t operator()(const ConditionImplyImpl& e) const;
};

template<>
struct UniquePDDLHasher<const ConditionExistsImpl&>
{
    size_t operator()(const ConditionExistsImpl& e) const;
};

template<>
struct UniquePDDLHasher<const ConditionForallImpl&>
{
    size_t operator()(const ConditionForallImpl& e) const;
};

template<>
struct UniquePDDLHasher<const ConditionImpl*>
{
    size_t operator()(const ConditionImpl* e) const;
};

template<>
struct UniquePDDLHasher<const DomainImpl*>
{
    size_t operator()(const DomainImpl* e) const;
};

template<>
struct UniquePDDLHasher<const EffectLiteralImpl&>
{
    size_t operator()(const EffectLiteralImpl& e) const;
};

template<>
struct UniquePDDLHasher<const EffectAndImpl&>
{
    size_t operator()(const EffectAndImpl& e) const;
};

template<>
struct UniquePDDLHasher<const EffectNumericImpl&>
{
    size_t operator()(const EffectNumericImpl& e) const;
};

template<>
struct UniquePDDLHasher<const EffectConditionalForallImpl&>
{
    size_t operator()(const EffectConditionalForallImpl& e) const;
};

template<>
struct UniquePDDLHasher<const EffectConditionalWhenImpl&>
{
    size_t operator()(const EffectConditionalWhenImpl& e) const;
};

template<>
struct UniquePDDLHasher<const EffectImpl*>
{
    size_t operator()(const EffectImpl* e) const;
};

template<>
struct UniquePDDLHasher<const FunctionExpressionNumberImpl&>
{
    size_t operator()(const FunctionExpressionNumberImpl& e) const;
};

template<>
struct UniquePDDLHasher<const FunctionExpressionBinaryOperatorImpl&>
{
    size_t operator()(const FunctionExpressionBinaryOperatorImpl& e) const;
};

template<>
struct UniquePDDLHasher<const FunctionExpressionMultiOperatorImpl&>
{
    size_t operator()(const FunctionExpressionMultiOperatorImpl& e) const;
};

template<>
struct UniquePDDLHasher<const FunctionExpressionMinusImpl&>
{
    size_t operator()(const FunctionExpressionMinusImpl& e) const;
};

template<>
struct UniquePDDLHasher<const FunctionExpressionFunctionImpl&>
{
    size_t operator()(const FunctionExpressionFunctionImpl& e) const;
};

template<>
struct UniquePDDLHasher<const FunctionExpressionImpl*>
{
    size_t operator()(const FunctionExpressionImpl* e) const;
};

template<>
struct UniquePDDLHasher<const FunctionSkeletonImpl*>
{
    size_t operator()(const FunctionSkeletonImpl* e) const;
};

template<>
struct UniquePDDLHasher<const FunctionImpl*>
{
    size_t operator()(const FunctionImpl* e) const;
};

template<>
struct UniquePDDLHasher<const LiteralImpl*>
{
    size_t operator()(const LiteralImpl* e) const;
};

template<>
struct UniquePDDLHasher<const OptimizationMetricImpl*>
{
    size_t operator()(const OptimizationMetricImpl* e) const;
};

template<>
struct UniquePDDLHasher<const NumericFluentImpl*>
{
    size_t operator()(const NumericFluentImpl* e) const;
};

template<>
struct UniquePDDLHasher<const ObjectImpl*>
{
    size_t operator()(const ObjectImpl* e) const;
};

template<>
struct UniquePDDLHasher<const ParameterImpl*>
{
    size_t operator()(const ParameterImpl* e) const;
};

template<>
struct UniquePDDLHasher<const PredicateImpl*>
{
    size_t operator()(const PredicateImpl* e) const;
};

template<>
struct UniquePDDLHasher<const ProblemImpl*>
{
    size_t operator()(const ProblemImpl* e) const;
};

template<>
struct UniquePDDLHasher<const RequirementsImpl*>
{
    size_t operator()(const RequirementsImpl* e) const;
};

template<>
struct UniquePDDLHasher<const TermObjectImpl&>
{
    size_t operator()(const TermObjectImpl& e) const;
};

template<>
struct UniquePDDLHasher<const TermVariableImpl&>
{
    size_t operator()(const TermVariableImpl& e) const;
};

template<>
struct UniquePDDLHasher<const TermImpl*>
{
    size_t operator()(const TermImpl* e) const;
};

template<>
struct UniquePDDLHasher<const TypeImpl*>
{
    size_t operator()(const TypeImpl* e) const;
};

template<>
struct UniquePDDLHasher<const VariableImpl*>
{
    size_t operator()(const VariableImpl* e) const;
};

}

#endif
