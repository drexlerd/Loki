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
#include "loki/details/utils/observer_ptr.hpp"

/**
 * Specializations for PDDL
 */

template<>
struct std::equal_to<loki::ObserverPtr<const loki::ActionImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::ActionImpl> lhs, loki::ObserverPtr<const loki::ActionImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::AtomImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::AtomImpl> lhs, loki::ObserverPtr<const loki::AtomImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::AxiomImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::AxiomImpl> lhs, loki::ObserverPtr<const loki::AxiomImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::ConditionLiteralImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::ConditionLiteralImpl> lhs, loki::ObserverPtr<const loki::ConditionLiteralImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::ConditionAndImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::ConditionAndImpl> lhs, loki::ObserverPtr<const loki::ConditionAndImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::ConditionOrImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::ConditionOrImpl> lhs, loki::ObserverPtr<const loki::ConditionOrImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::ConditionNotImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::ConditionNotImpl> lhs, loki::ObserverPtr<const loki::ConditionNotImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::ConditionImplyImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::ConditionImplyImpl> lhs, loki::ObserverPtr<const loki::ConditionImplyImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::ConditionExistsImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::ConditionExistsImpl> lhs, loki::ObserverPtr<const loki::ConditionExistsImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::ConditionForallImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::ConditionForallImpl> lhs, loki::ObserverPtr<const loki::ConditionForallImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::ConditionImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::ConditionImpl> lhs, loki::ObserverPtr<const loki::ConditionImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::DomainImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::DomainImpl> lhs, loki::ObserverPtr<const loki::DomainImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::EffectLiteralImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::EffectLiteralImpl> lhs, loki::ObserverPtr<const loki::EffectLiteralImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::EffectAndImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::EffectAndImpl> lhs, loki::ObserverPtr<const loki::EffectAndImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::EffectNumericImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::EffectNumericImpl> lhs, loki::ObserverPtr<const loki::EffectNumericImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::EffectCompositeForallImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::EffectCompositeForallImpl> lhs, loki::ObserverPtr<const loki::EffectCompositeForallImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::EffectCompositeWhenImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::EffectCompositeWhenImpl> lhs, loki::ObserverPtr<const loki::EffectCompositeWhenImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::EffectCompositeOneofImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::EffectCompositeOneofImpl> lhs, loki::ObserverPtr<const loki::EffectCompositeOneofImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::EffectImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::EffectImpl> lhs, loki::ObserverPtr<const loki::EffectImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::FunctionExpressionNumberImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::FunctionExpressionNumberImpl> lhs, loki::ObserverPtr<const loki::FunctionExpressionNumberImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::FunctionExpressionBinaryOperatorImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::FunctionExpressionBinaryOperatorImpl> lhs,
                    loki::ObserverPtr<const loki::FunctionExpressionBinaryOperatorImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::FunctionExpressionMultiOperatorImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::FunctionExpressionMultiOperatorImpl> lhs,
                    loki::ObserverPtr<const loki::FunctionExpressionMultiOperatorImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::FunctionExpressionMinusImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::FunctionExpressionMinusImpl> lhs, loki::ObserverPtr<const loki::FunctionExpressionMinusImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::FunctionExpressionFunctionImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::FunctionExpressionFunctionImpl> lhs, loki::ObserverPtr<const loki::FunctionExpressionFunctionImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::FunctionExpressionImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::FunctionExpressionImpl> lhs, loki::ObserverPtr<const loki::FunctionExpressionImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::FunctionSkeletonImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::FunctionSkeletonImpl> lhs, loki::ObserverPtr<const loki::FunctionSkeletonImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::FunctionImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::FunctionImpl> lhs, loki::ObserverPtr<const loki::FunctionImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::LiteralImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::LiteralImpl> lhs, loki::ObserverPtr<const loki::LiteralImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::OptimizationMetricImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::OptimizationMetricImpl> lhs, loki::ObserverPtr<const loki::OptimizationMetricImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::NumericFluentImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::NumericFluentImpl> lhs, loki::ObserverPtr<const loki::NumericFluentImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::ObjectImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::ObjectImpl> lhs, loki::ObserverPtr<const loki::ObjectImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::ParameterImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::ParameterImpl> lhs, loki::ObserverPtr<const loki::ParameterImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::PredicateImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::PredicateImpl> lhs, loki::ObserverPtr<const loki::PredicateImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::ProblemImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::ProblemImpl> lhs, loki::ObserverPtr<const loki::ProblemImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::RequirementsImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::RequirementsImpl> lhs, loki::ObserverPtr<const loki::RequirementsImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::TermImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::TermImpl> lhs, loki::ObserverPtr<const loki::TermImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::TypeImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::TypeImpl> lhs, loki::ObserverPtr<const loki::TypeImpl> rhs) const;
};

template<>
struct std::equal_to<loki::ObserverPtr<const loki::VariableImpl>>
{
    bool operator()(loki::ObserverPtr<const loki::VariableImpl> lhs, loki::ObserverPtr<const loki::VariableImpl> rhs) const;
};

#endif
