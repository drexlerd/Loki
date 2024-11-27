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

#ifndef LOKI_INCLUDE_LOKI_PDDL_EQUAL_TO_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_EQUAL_TO_HPP_

#include "loki/details/pddl/declarations.hpp"

#include <functional>
#include <variant>

namespace loki
{

/// @brief `UniquePDDLEqualTo` is used to compare newly created PDDL objects for uniqueness.
/// Since the children are unique, it suffices to compare nested pointers.
template<typename T>
struct UniquePDDLEqualTo
{
    bool operator()(const T& l, const T& r) const { return std::equal_to<T>()(l, r); }
};

/// Spezialization for std::variant.
template<typename... Ts>
struct UniquePDDLEqualTo<std::variant<Ts...>>
{
    bool operator()(const std::variant<Ts...>& l, const std::variant<Ts...>& r) const
    {
        if (l.index() != r.index())
        {
            return false;  // Different types held
        }
        // Compare the held values, but only if they are of the same type
        return std::visit(
            [](const auto& lhs, const auto& rhs) -> bool
            {
                if constexpr (std::is_same_v<decltype(lhs), decltype(rhs)>)
                {
                    return UniquePDDLEqualTo<decltype(lhs)>()(lhs, rhs);
                }
                else
                {
                    return false;  // Different types can't be equal
                }
            },
            l,
            r);
    }
};

/**
 * Specializations for PDDL
 */

template<>
struct UniquePDDLEqualTo<Action>
{
    bool operator()(Action l, Action r) const;
};

template<>
struct UniquePDDLEqualTo<Atom>
{
    bool operator()(Atom l, Atom r) const;
};

template<>
struct UniquePDDLEqualTo<Axiom>
{
    bool operator()(Axiom l, Axiom r) const;
};

template<>
struct UniquePDDLEqualTo<ConditionLiteral>
{
    bool operator()(ConditionLiteral l, ConditionLiteral r) const;
};

template<>
struct UniquePDDLEqualTo<ConditionAnd>
{
    bool operator()(ConditionAnd l, ConditionAnd r) const;
};

template<>
struct UniquePDDLEqualTo<ConditionOr>
{
    bool operator()(ConditionOr l, ConditionOr r) const;
};

template<>
struct UniquePDDLEqualTo<ConditionNot>
{
    bool operator()(ConditionNot l, ConditionNot r) const;
};

template<>
struct UniquePDDLEqualTo<ConditionImply>
{
    bool operator()(ConditionImply l, ConditionImply r) const;
};

template<>
struct UniquePDDLEqualTo<ConditionExists>
{
    bool operator()(ConditionExists l, ConditionExists r) const;
};

template<>
struct UniquePDDLEqualTo<ConditionForall>
{
    bool operator()(ConditionForall l, ConditionForall r) const;
};

template<>
struct UniquePDDLEqualTo<Condition>
{
    bool operator()(Condition l, Condition r) const;
};

template<>
struct UniquePDDLEqualTo<Domain>
{
    bool operator()(Domain l, Domain r) const;
};

template<>
struct UniquePDDLEqualTo<EffectLiteral>
{
    bool operator()(EffectLiteral l, EffectLiteral r) const;
};

template<>
struct UniquePDDLEqualTo<EffectAnd>
{
    bool operator()(EffectAnd l, EffectAnd r) const;
};

template<>
struct UniquePDDLEqualTo<EffectNumeric>
{
    bool operator()(EffectNumeric l, EffectNumeric r) const;
};

template<>
struct UniquePDDLEqualTo<EffectCompositeForall>
{
    bool operator()(EffectCompositeForall l, EffectCompositeForall r) const;
};

template<>
struct UniquePDDLEqualTo<EffectCompositeWhen>
{
    bool operator()(EffectCompositeWhen l, EffectCompositeWhen r) const;
};

template<>
struct UniquePDDLEqualTo<EffectCompositeOneof>
{
    bool operator()(EffectCompositeOneof l, EffectCompositeOneof r) const;
};

template<>
struct UniquePDDLEqualTo<Effect>
{
    bool operator()(Effect l, Effect r) const;
};

template<>
struct UniquePDDLEqualTo<FunctionExpressionNumber>
{
    bool operator()(FunctionExpressionNumber l, FunctionExpressionNumber r) const;
};

template<>
struct UniquePDDLEqualTo<FunctionExpressionBinaryOperator>
{
    bool operator()(FunctionExpressionBinaryOperator l, FunctionExpressionBinaryOperator r) const;
};

template<>
struct UniquePDDLEqualTo<FunctionExpressionMultiOperator>
{
    bool operator()(FunctionExpressionMultiOperator l, FunctionExpressionMultiOperator r) const;
};

template<>
struct UniquePDDLEqualTo<FunctionExpressionMinus>
{
    bool operator()(FunctionExpressionMinus l, FunctionExpressionMinus r) const;
};

template<>
struct UniquePDDLEqualTo<FunctionExpressionFunction>
{
    bool operator()(FunctionExpressionFunction l, FunctionExpressionFunction r) const;
};

template<>
struct UniquePDDLEqualTo<FunctionExpression>
{
    bool operator()(FunctionExpression l, FunctionExpression r) const;
};

template<>
struct UniquePDDLEqualTo<FunctionSkeleton>
{
    bool operator()(FunctionSkeleton l, FunctionSkeleton r) const;
};

template<>
struct UniquePDDLEqualTo<Function>
{
    bool operator()(Function l, Function r) const;
};

template<>
struct UniquePDDLEqualTo<Literal>
{
    bool operator()(Literal l, Literal r) const;
};

template<>
struct UniquePDDLEqualTo<OptimizationMetric>
{
    bool operator()(OptimizationMetric l, OptimizationMetric r) const;
};

template<>
struct UniquePDDLEqualTo<NumericFluent>
{
    bool operator()(NumericFluent l, NumericFluent r) const;
};

template<>
struct UniquePDDLEqualTo<Object>
{
    bool operator()(Object l, Object r) const;
};

template<>
struct UniquePDDLEqualTo<Parameter>
{
    bool operator()(Parameter l, Parameter r) const;
};

template<>
struct UniquePDDLEqualTo<Predicate>
{
    bool operator()(Predicate l, Predicate r) const;
};

template<>
struct UniquePDDLEqualTo<Problem>
{
    bool operator()(Problem l, Problem r) const;
};

template<>
struct UniquePDDLEqualTo<Requirements>
{
    bool operator()(Requirements l, Requirements r) const;
};

template<>
struct UniquePDDLEqualTo<Term>
{
    bool operator()(Term l, Term r) const;
};

template<>
struct UniquePDDLEqualTo<Type>
{
    bool operator()(Type l, Type r) const;
};

template<>
struct UniquePDDLEqualTo<Variable>
{
    bool operator()(Variable l, Variable r) const;
};

}

#endif
