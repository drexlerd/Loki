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
struct UniquePDDLEqualTo<const ActionImpl*>
{
    bool operator()(const ActionImpl* l, const ActionImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const AtomImpl*>
{
    bool operator()(const AtomImpl* l, const AtomImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const AxiomImpl*>
{
    bool operator()(const AxiomImpl* l, const AxiomImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const ConditionLiteralImpl&>
{
    bool operator()(const ConditionLiteralImpl& l, const ConditionLiteralImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const ConditionAndImpl&>
{
    bool operator()(const ConditionAndImpl& l, const ConditionAndImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const ConditionOrImpl&>
{
    bool operator()(const ConditionOrImpl& l, const ConditionOrImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const ConditionNotImpl&>
{
    bool operator()(const ConditionNotImpl& l, const ConditionNotImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const ConditionImplyImpl&>
{
    bool operator()(const ConditionImplyImpl& l, const ConditionImplyImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const ConditionExistsImpl&>
{
    bool operator()(const ConditionExistsImpl& l, const ConditionExistsImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const ConditionForallImpl&>
{
    bool operator()(const ConditionForallImpl& l, const ConditionForallImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const ConditionImpl*>
{
    bool operator()(const ConditionImpl* l, const ConditionImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const DomainImpl*>
{
    bool operator()(const DomainImpl* l, const DomainImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const EffectLiteralImpl&>
{
    bool operator()(const EffectLiteralImpl& l, const EffectLiteralImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const EffectAndImpl&>
{
    bool operator()(const EffectAndImpl& l, const EffectAndImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const EffectNumericImpl&>
{
    bool operator()(const EffectNumericImpl& l, const EffectNumericImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const EffectCompositeForallImpl&>
{
    bool operator()(const EffectCompositeForallImpl& l, const EffectCompositeForallImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const EffectCompositeWhenImpl&>
{
    bool operator()(const EffectCompositeWhenImpl& l, const EffectCompositeWhenImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const EffectCompositeOneofImpl&>
{
    bool operator()(const EffectCompositeOneofImpl& l, const EffectCompositeOneofImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const EffectImpl*>
{
    bool operator()(const EffectImpl* l, const EffectImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const FunctionExpressionNumberImpl&>
{
    bool operator()(const FunctionExpressionNumberImpl& l, const FunctionExpressionNumberImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const FunctionExpressionBinaryOperatorImpl&>
{
    bool operator()(const FunctionExpressionBinaryOperatorImpl& l, const FunctionExpressionBinaryOperatorImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const FunctionExpressionMultiOperatorImpl&>
{
    bool operator()(const FunctionExpressionMultiOperatorImpl& l, const FunctionExpressionMultiOperatorImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const FunctionExpressionMinusImpl&>
{
    bool operator()(const FunctionExpressionMinusImpl& l, const FunctionExpressionMinusImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const FunctionExpressionFunctionImpl&>
{
    bool operator()(const FunctionExpressionFunctionImpl& l, const FunctionExpressionFunctionImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const FunctionExpressionImpl*>
{
    bool operator()(const FunctionExpressionImpl* l, const FunctionExpressionImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const FunctionSkeletonImpl*>
{
    bool operator()(const FunctionSkeletonImpl* l, const FunctionSkeletonImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const FunctionImpl*>
{
    bool operator()(const FunctionImpl* l, const FunctionImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const LiteralImpl*>
{
    bool operator()(const LiteralImpl* l, const LiteralImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const OptimizationMetricImpl*>
{
    bool operator()(const OptimizationMetricImpl* l, const OptimizationMetricImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const NumericFluentImpl*>
{
    bool operator()(const NumericFluentImpl* l, const NumericFluentImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const ObjectImpl*>
{
    bool operator()(const ObjectImpl* l, const ObjectImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const ParameterImpl*>
{
    bool operator()(const ParameterImpl* l, const ParameterImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const PredicateImpl*>
{
    bool operator()(const PredicateImpl* l, const PredicateImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const ProblemImpl*>
{
    bool operator()(const ProblemImpl* l, const ProblemImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const RequirementsImpl*>
{
    bool operator()(const RequirementsImpl* l, const RequirementsImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const TermObjectImpl&>
{
    bool operator()(const TermObjectImpl& l, const TermObjectImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const TermVariableImpl&>
{
    bool operator()(const TermVariableImpl& l, const TermVariableImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const TermImpl*>
{
    bool operator()(const TermImpl* l, const TermImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const TypeImpl*>
{
    bool operator()(const TypeImpl* l, const TypeImpl* r) const;
};

template<>
struct UniquePDDLEqualTo<const VariableImpl*>
{
    bool operator()(const VariableImpl* l, const VariableImpl* r) const;
};

}

#endif
