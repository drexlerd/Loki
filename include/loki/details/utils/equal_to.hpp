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

#ifndef LOKI_INCLUDE_LOKI_UTILS_EQUAL_TO_HPP_
#define LOKI_INCLUDE_LOKI_UTILS_EQUAL_TO_HPP_

#include "loki/details/pddl/declarations.hpp"
#include "loki/details/utils/concepts.hpp"

#include <functional>

namespace loki
{

/// @brief `EqualTo` is a comparator that should be implemented to work in the context where
/// all objects of a custom type `T` are uniquely created and uncopieable.
/// This ensures that pointers can simply be compared and do not need to be dereferenced.
/// @tparam T the type of the object to compare.
template<typename T>
struct EqualTo
{
    bool operator()(const T& l, const T& r) const { return std::equal_to<T>()(l, r); }
};

/// Spezialization for std::variant.
template<typename... Ts>
struct EqualTo<std::variant<Ts...>>
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
                    return EqualTo<decltype(lhs)>()(lhs, rhs);
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
struct EqualTo<const ActionImpl*>
{
    bool operator()(const ActionImpl* l, const ActionImpl* r) const;
};

template<>
struct EqualTo<const AtomImpl*>
{
    bool operator()(const AtomImpl* l, const AtomImpl* r) const;
};

template<>
struct EqualTo<const AxiomImpl*>
{
    bool operator()(const AxiomImpl* l, const AxiomImpl* r) const;
};

template<>
struct EqualTo<const ConditionLiteralImpl&>
{
    bool operator()(const ConditionLiteralImpl& l, const ConditionLiteralImpl& r) const;
};

template<>
struct EqualTo<const ConditionAndImpl&>
{
    bool operator()(const ConditionAndImpl& l, const ConditionAndImpl& r) const;
};

template<>
struct EqualTo<const ConditionOrImpl&>
{
    bool operator()(const ConditionOrImpl& l, const ConditionOrImpl& r) const;
};

template<>
struct EqualTo<const ConditionNotImpl&>
{
    bool operator()(const ConditionNotImpl& l, const ConditionNotImpl& r) const;
};

template<>
struct EqualTo<const ConditionImplyImpl&>
{
    bool operator()(const ConditionImplyImpl& l, const ConditionImplyImpl& r) const;
};

template<>
struct EqualTo<const ConditionExistsImpl&>
{
    bool operator()(const ConditionExistsImpl& l, const ConditionExistsImpl& r) const;
};

template<>
struct EqualTo<const ConditionForallImpl&>
{
    bool operator()(const ConditionForallImpl& l, const ConditionForallImpl& r) const;
};

template<>
struct EqualTo<const ConditionImpl*>
{
    bool operator()(const ConditionImpl* l, const ConditionImpl* r) const;
};

template<>
struct EqualTo<const DomainImpl*>
{
    bool operator()(const DomainImpl* l, const DomainImpl* r) const;
};

template<>
struct EqualTo<const EffectLiteralImpl&>
{
    bool operator()(const EffectLiteralImpl& l, const EffectLiteralImpl& r) const;
};

template<>
struct EqualTo<const EffectAndImpl&>
{
    bool operator()(const EffectAndImpl& l, const EffectAndImpl& r) const;
};

template<>
struct EqualTo<const EffectNumericImpl&>
{
    bool operator()(const EffectNumericImpl& l, const EffectNumericImpl& r) const;
};

template<>
struct EqualTo<const EffectConditionalForallImpl&>
{
    bool operator()(const EffectConditionalForallImpl& l, const EffectConditionalForallImpl& r) const;
};

template<>
struct EqualTo<const EffectConditionalWhenImpl&>
{
    bool operator()(const EffectConditionalWhenImpl& l, const EffectConditionalWhenImpl& r) const;
};

template<>
struct EqualTo<const EffectImpl*>
{
    bool operator()(const EffectImpl* l, const EffectImpl* r) const;
};

template<>
struct EqualTo<const FunctionExpressionNumberImpl&>
{
    bool operator()(const FunctionExpressionNumberImpl& l, const FunctionExpressionNumberImpl& r) const;
};

template<>
struct EqualTo<const FunctionExpressionBinaryOperatorImpl&>
{
    bool operator()(const FunctionExpressionBinaryOperatorImpl& l, const FunctionExpressionBinaryOperatorImpl& r) const;
};

template<>
struct EqualTo<const FunctionExpressionMultiOperatorImpl&>
{
    bool operator()(const FunctionExpressionMultiOperatorImpl& l, const FunctionExpressionMultiOperatorImpl& r) const;
};

template<>
struct EqualTo<const FunctionExpressionMinusImpl&>
{
    bool operator()(const FunctionExpressionMinusImpl& l, const FunctionExpressionMinusImpl& r) const;
};

template<>
struct EqualTo<const FunctionExpressionFunctionImpl&>
{
    bool operator()(const FunctionExpressionFunctionImpl& l, const FunctionExpressionFunctionImpl& r) const;
};

template<>
struct EqualTo<const FunctionExpressionImpl*>
{
    bool operator()(const FunctionExpressionImpl* l, const FunctionExpressionImpl* r) const;
};

template<>
struct EqualTo<const FunctionImpl*>
{
    bool operator()(const FunctionImpl* l, const FunctionImpl* r) const;
};

template<>
struct EqualTo<const LiteralImpl*>
{
    bool operator()(const LiteralImpl* l, const LiteralImpl* r) const;
};

template<>
struct EqualTo<const OptimizationMetricImpl*>
{
    bool operator()(const OptimizationMetricImpl* l, const OptimizationMetricImpl* r) const;
};

template<>
struct EqualTo<const NumericFluentImpl*>
{
    bool operator()(const NumericFluentImpl* l, const NumericFluentImpl* r) const;
};

template<>
struct EqualTo<const ObjectImpl*>
{
    bool operator()(const ObjectImpl* l, const ObjectImpl* r) const;
};

template<>
struct EqualTo<const ParameterImpl*>
{
    bool operator()(const ParameterImpl* l, const ParameterImpl* r) const;
};

template<>
struct EqualTo<const PredicateImpl*>
{
    bool operator()(const PredicateImpl* l, const PredicateImpl* r) const;
};

template<>
struct EqualTo<const ProblemImpl*>
{
    bool operator()(const ProblemImpl* l, const ProblemImpl* r) const;
};

template<>
struct EqualTo<const RequirementsImpl*>
{
    bool operator()(const RequirementsImpl* l, const RequirementsImpl* r) const;
};

template<>
struct EqualTo<const TermObjectImpl&>
{
    bool operator()(const TermObjectImpl& l, const TermObjectImpl& r) const;
};

template<>
struct EqualTo<const TermVariableImpl&>
{
    bool operator()(const TermVariableImpl& l, const TermVariableImpl& r) const;
};

template<>
struct EqualTo<const TermImpl*>
{
    bool operator()(const TermImpl* l, const TermImpl* r) const;
};

template<>
struct EqualTo<const TypeImpl*>
{
    bool operator()(const TypeImpl* l, const TypeImpl* r) const;
};

template<>
struct EqualTo<const VariableImpl*>
{
    bool operator()(const VariableImpl* l, const VariableImpl* r) const;
};
}

#endif
