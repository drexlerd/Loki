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
        return std::visit(
            [](const auto& arg)
            {
                using DecayedType = std::decay_t<decltype(arg)>;
                return UniquePDDLHasher<DecayedType>()(arg);
            },
            variant);
    }
};

/**
 * Specializations for PDDL
 */

template<>
struct UniquePDDLHasher<Action>
{
    size_t operator()(Action e) const;
};

template<>
struct UniquePDDLHasher<Atom>
{
    size_t operator()(Atom e) const;
};

template<>
struct UniquePDDLHasher<Axiom>
{
    size_t operator()(Axiom e) const;
};

template<>
struct UniquePDDLHasher<ConditionLiteral>
{
    size_t operator()(ConditionLiteral e) const;
};

template<>
struct UniquePDDLHasher<ConditionAnd>
{
    size_t operator()(ConditionAnd e) const;
};

template<>
struct UniquePDDLHasher<ConditionOr>
{
    size_t operator()(ConditionOr e) const;
};

template<>
struct UniquePDDLHasher<ConditionNot>
{
    size_t operator()(ConditionNot e) const;
};

template<>
struct UniquePDDLHasher<ConditionImply>
{
    size_t operator()(ConditionImply e) const;
};

template<>
struct UniquePDDLHasher<ConditionExists>
{
    size_t operator()(ConditionExists e) const;
};

template<>
struct UniquePDDLHasher<ConditionForall>
{
    size_t operator()(ConditionForall e) const;
};

template<>
struct UniquePDDLHasher<Condition>
{
    size_t operator()(Condition e) const;
};

template<>
struct UniquePDDLHasher<Domain>
{
    size_t operator()(Domain e) const;
};

template<>
struct UniquePDDLHasher<EffectLiteral>
{
    size_t operator()(EffectLiteral e) const;
};

template<>
struct UniquePDDLHasher<EffectAnd>
{
    size_t operator()(EffectAnd e) const;
};

template<>
struct UniquePDDLHasher<EffectNumeric>
{
    size_t operator()(EffectNumeric e) const;
};

template<>
struct UniquePDDLHasher<EffectCompositeForall>
{
    size_t operator()(EffectCompositeForall e) const;
};

template<>
struct UniquePDDLHasher<EffectCompositeWhen>
{
    size_t operator()(EffectCompositeWhen e) const;
};

template<>
struct UniquePDDLHasher<EffectCompositeOneof>
{
    size_t operator()(EffectCompositeOneof e) const;
};

template<>
struct UniquePDDLHasher<Effect>
{
    size_t operator()(Effect e) const;
};

template<>
struct UniquePDDLHasher<FunctionExpressionNumber>
{
    size_t operator()(FunctionExpressionNumber e) const;
};

template<>
struct UniquePDDLHasher<FunctionExpressionBinaryOperator>
{
    size_t operator()(FunctionExpressionBinaryOperator e) const;
};

template<>
struct UniquePDDLHasher<FunctionExpressionMultiOperator>
{
    size_t operator()(FunctionExpressionMultiOperator e) const;
};

template<>
struct UniquePDDLHasher<FunctionExpressionMinus>
{
    size_t operator()(FunctionExpressionMinus e) const;
};

template<>
struct UniquePDDLHasher<FunctionExpressionFunction>
{
    size_t operator()(FunctionExpressionFunction e) const;
};

template<>
struct UniquePDDLHasher<FunctionExpression>
{
    size_t operator()(FunctionExpression e) const;
};

template<>
struct UniquePDDLHasher<FunctionSkeleton>
{
    size_t operator()(FunctionSkeleton e) const;
};

template<>
struct UniquePDDLHasher<Function>
{
    size_t operator()(Function e) const;
};

template<>
struct UniquePDDLHasher<Literal>
{
    size_t operator()(Literal e) const;
};

template<>
struct UniquePDDLHasher<OptimizationMetric>
{
    size_t operator()(OptimizationMetric e) const;
};

template<>
struct UniquePDDLHasher<NumericFluent>
{
    size_t operator()(NumericFluent e) const;
};

template<>
struct UniquePDDLHasher<Object>
{
    size_t operator()(Object e) const;
};

template<>
struct UniquePDDLHasher<Parameter>
{
    size_t operator()(Parameter e) const;
};

template<>
struct UniquePDDLHasher<Predicate>
{
    size_t operator()(Predicate e) const;
};

template<>
struct UniquePDDLHasher<Problem>
{
    size_t operator()(Problem e) const;
};

template<>
struct UniquePDDLHasher<Requirements>
{
    size_t operator()(Requirements e) const;
};

template<>
struct UniquePDDLHasher<Term>
{
    size_t operator()(Term e) const;
};

template<>
struct UniquePDDLHasher<Type>
{
    size_t operator()(Type e) const;
};

template<>
struct UniquePDDLHasher<Variable>
{
    size_t operator()(Variable e) const;
};

}

#endif
