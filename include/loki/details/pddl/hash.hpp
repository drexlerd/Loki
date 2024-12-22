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
#include "loki/details/utils/observer_ptr.hpp"

/**
 * Specializations for PDDL
 */

template<>
struct std::hash<loki::ObserverPtr<const loki::ActionImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::ActionImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::AtomImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::AtomImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::AxiomImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::AxiomImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::ConditionLiteralImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::ConditionLiteralImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::ConditionAndImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::ConditionAndImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::ConditionOrImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::ConditionOrImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::ConditionNotImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::ConditionNotImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::ConditionImplyImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::ConditionImplyImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::ConditionExistsImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::ConditionExistsImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::ConditionForallImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::ConditionForallImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::ConditionImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::ConditionImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::DomainImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::DomainImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::EffectLiteralImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::EffectLiteralImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::EffectAndImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::EffectAndImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::EffectNumericImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::EffectNumericImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::EffectCompositeForallImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::EffectCompositeForallImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::EffectCompositeWhenImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::EffectCompositeWhenImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::EffectCompositeOneofImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::EffectCompositeOneofImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::EffectImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::EffectImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::FunctionExpressionNumberImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::FunctionExpressionNumberImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::FunctionExpressionBinaryOperatorImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::FunctionExpressionBinaryOperatorImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::FunctionExpressionMultiOperatorImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::FunctionExpressionMultiOperatorImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::FunctionExpressionMinusImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::FunctionExpressionMinusImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::FunctionExpressionFunctionImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::FunctionExpressionFunctionImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::FunctionExpressionImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::FunctionExpressionImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::FunctionSkeletonImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::FunctionSkeletonImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::FunctionImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::FunctionImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::LiteralImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::LiteralImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::OptimizationMetricImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::OptimizationMetricImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::NumericFluentImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::NumericFluentImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::ObjectImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::ObjectImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::ParameterImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::ParameterImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::PredicateImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::PredicateImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::ProblemImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::ProblemImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::RequirementsImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::RequirementsImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::TermImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::TermImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::TypeImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::TypeImpl> e) const;
};

template<>
struct std::hash<loki::ObserverPtr<const loki::VariableImpl>>
{
    size_t operator()(loki::ObserverPtr<const loki::VariableImpl> e) const;
};

#endif
