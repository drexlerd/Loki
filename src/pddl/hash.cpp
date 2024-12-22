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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "loki/details/pddl/hash.hpp"

#include "loki/details/pddl/action.hpp"
#include "loki/details/pddl/atom.hpp"
#include "loki/details/pddl/axiom.hpp"
#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/domain.hpp"
#include "loki/details/pddl/effects.hpp"
#include "loki/details/pddl/function.hpp"
#include "loki/details/pddl/function_expressions.hpp"
#include "loki/details/pddl/function_skeleton.hpp"
#include "loki/details/pddl/literal.hpp"
#include "loki/details/pddl/metric.hpp"
#include "loki/details/pddl/numeric_fluent.hpp"
#include "loki/details/pddl/object.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/pddl/position.hpp"
#include "loki/details/pddl/predicate.hpp"
#include "loki/details/pddl/problem.hpp"
#include "loki/details/pddl/requirements.hpp"
#include "loki/details/pddl/term.hpp"
#include "loki/details/pddl/type.hpp"
#include "loki/details/pddl/variable.hpp"
#include "loki/details/utils/collections.hpp"
#include "loki/details/utils/hash.hpp"

size_t std::hash<loki::ObserverPtr<const loki::ActionImpl>>::operator()(loki::ObserverPtr<const loki::ActionImpl> e) const
{
    return loki::hash_combine(e->get_name(), loki::get_sorted_vector(e->get_parameters()), e->get_condition(), e->get_effect());
}

size_t std::hash<loki::ObserverPtr<const loki::AtomImpl>>::operator()(loki::ObserverPtr<const loki::AtomImpl> e) const
{
    return loki::hash_combine(e->get_predicate(), e->get_terms());
}

size_t std::hash<loki::ObserverPtr<const loki::AxiomImpl>>::operator()(loki::ObserverPtr<const loki::AxiomImpl> e) const
{
    return loki::hash_combine(e->get_derived_predicate_name(), loki::get_sorted_vector(e->get_parameters()), e->get_condition());
}

size_t std::hash<loki::ObserverPtr<const loki::ConditionLiteralImpl>>::operator()(loki::ObserverPtr<const loki::ConditionLiteralImpl> e) const
{
    return loki::hash_combine(e->get_literal());
}

size_t std::hash<loki::ObserverPtr<const loki::ConditionAndImpl>>::operator()(loki::ObserverPtr<const loki::ConditionAndImpl> e) const
{
    return loki::hash_combine(loki::get_sorted_vector(e->get_conditions()));
}

size_t std::hash<loki::ObserverPtr<const loki::ConditionOrImpl>>::operator()(loki::ObserverPtr<const loki::ConditionOrImpl> e) const
{
    return loki::hash_combine(loki::get_sorted_vector(e->get_conditions()));
}

size_t std::hash<loki::ObserverPtr<const loki::ConditionNotImpl>>::operator()(loki::ObserverPtr<const loki::ConditionNotImpl> e) const
{
    return loki::hash_combine(e->get_condition());
}

size_t std::hash<loki::ObserverPtr<const loki::ConditionImplyImpl>>::operator()(loki::ObserverPtr<const loki::ConditionImplyImpl> e) const
{
    return loki::hash_combine(e->get_condition_left(), e->get_condition_right());
}

size_t std::hash<loki::ObserverPtr<const loki::ConditionExistsImpl>>::operator()(loki::ObserverPtr<const loki::ConditionExistsImpl> e) const
{
    return loki::hash_combine(loki::get_sorted_vector(e->get_parameters()), e->get_condition());
}

size_t std::hash<loki::ObserverPtr<const loki::ConditionForallImpl>>::operator()(loki::ObserverPtr<const loki::ConditionForallImpl> e) const
{
    return loki::hash_combine(loki::get_sorted_vector(e->get_parameters()), e->get_condition());
}

size_t std::hash<loki::ObserverPtr<const loki::ConditionImpl>>::operator()(loki::ObserverPtr<const loki::ConditionImpl> e) const
{
    return loki::hash_combine(e->get_condition());
}

size_t std::hash<loki::ObserverPtr<const loki::DomainImpl>>::operator()(loki::ObserverPtr<const loki::DomainImpl> e) const
{
    return loki::hash_combine(e->get_name(),
                              e->get_requirements(),
                              loki::get_sorted_vector(e->get_types()),
                              loki::get_sorted_vector(e->get_constants()),
                              loki::get_sorted_vector(e->get_predicates()),
                              loki::get_sorted_vector(e->get_functions()),
                              loki::get_sorted_vector(e->get_actions()),
                              loki::get_sorted_vector(e->get_axioms()));
}

size_t std::hash<loki::ObserverPtr<const loki::EffectLiteralImpl>>::operator()(loki::ObserverPtr<const loki::EffectLiteralImpl> e) const
{
    return loki::hash_combine(e->get_literal());
}

size_t std::hash<loki::ObserverPtr<const loki::EffectAndImpl>>::operator()(loki::ObserverPtr<const loki::EffectAndImpl> e) const
{
    return loki::hash_combine(loki::get_sorted_vector(e->get_effects()));
}

size_t std::hash<loki::ObserverPtr<const loki::EffectNumericImpl>>::operator()(loki::ObserverPtr<const loki::EffectNumericImpl> e) const
{
    return loki::hash_combine(e->get_assign_operator(), e->get_function(), e->get_function_expression());
}

size_t std::hash<loki::ObserverPtr<const loki::EffectCompositeForallImpl>>::operator()(loki::ObserverPtr<const loki::EffectCompositeForallImpl> e) const
{
    return loki::hash_combine(e->get_effect(), loki::get_sorted_vector(e->get_parameters()));
}

size_t std::hash<loki::ObserverPtr<const loki::EffectCompositeWhenImpl>>::operator()(loki::ObserverPtr<const loki::EffectCompositeWhenImpl> e) const
{
    return loki::hash_combine(e->get_condition(), e->get_effect());
}

size_t std::hash<loki::ObserverPtr<const loki::EffectCompositeOneofImpl>>::operator()(loki::ObserverPtr<const loki::EffectCompositeOneofImpl> e) const
{
    return loki::hash_combine(loki::get_sorted_vector(e->get_effects()));
}

size_t std::hash<loki::ObserverPtr<const loki::EffectImpl>>::operator()(loki::ObserverPtr<const loki::EffectImpl> e) const
{
    return loki::hash_combine(e->get_effect());
}

size_t std::hash<loki::ObserverPtr<const loki::FunctionExpressionNumberImpl>>::operator()(loki::ObserverPtr<const loki::FunctionExpressionNumberImpl> e) const
{
    return loki::hash_combine(e->get_number());
}

size_t std::hash<loki::ObserverPtr<const loki::FunctionExpressionBinaryOperatorImpl>>::operator()(
    loki::ObserverPtr<const loki::FunctionExpressionBinaryOperatorImpl> e) const
{
    return loki::hash_combine(e->get_binary_operator(), e->get_left_function_expression(), e->get_right_function_expression());
}

size_t std::hash<loki::ObserverPtr<const loki::FunctionExpressionMultiOperatorImpl>>::operator()(
    loki::ObserverPtr<const loki::FunctionExpressionMultiOperatorImpl> e) const
{
    return loki::hash_combine(e->get_multi_operator(), loki::get_sorted_vector(e->get_function_expressions()));
}

size_t std::hash<loki::ObserverPtr<const loki::FunctionExpressionMinusImpl>>::operator()(loki::ObserverPtr<const loki::FunctionExpressionMinusImpl> e) const
{
    return loki::hash_combine(e->get_function_expression());
}

size_t
std::hash<loki::ObserverPtr<const loki::FunctionExpressionFunctionImpl>>::operator()(loki::ObserverPtr<const loki::FunctionExpressionFunctionImpl> e) const
{
    return loki::hash_combine(e->get_function());
}

size_t std::hash<loki::ObserverPtr<const loki::FunctionExpressionImpl>>::operator()(loki::ObserverPtr<const loki::FunctionExpressionImpl> e) const
{
    return loki::hash_combine(e->get_function_expression());
}

size_t std::hash<loki::ObserverPtr<const loki::FunctionSkeletonImpl>>::operator()(loki::ObserverPtr<const loki::FunctionSkeletonImpl> e) const
{
    return loki::hash_combine(e->get_name(), e->get_type(), loki::get_sorted_vector(e->get_parameters()));
}

size_t std::hash<loki::ObserverPtr<const loki::FunctionImpl>>::operator()(loki::ObserverPtr<const loki::FunctionImpl> e) const
{
    return loki::hash_combine(e->get_function_skeleton(), e->get_terms());
}

size_t std::hash<loki::ObserverPtr<const loki::LiteralImpl>>::operator()(loki::ObserverPtr<const loki::LiteralImpl> e) const
{
    return loki::hash_combine(e->is_negated(), e->get_atom());
}

size_t std::hash<loki::ObserverPtr<const loki::OptimizationMetricImpl>>::operator()(loki::ObserverPtr<const loki::OptimizationMetricImpl> e) const
{
    return loki::hash_combine(e->get_optimization_metric(), e->get_function_expression());
}

size_t std::hash<loki::ObserverPtr<const loki::NumericFluentImpl>>::operator()(loki::ObserverPtr<const loki::NumericFluentImpl> e) const
{
    return loki::hash_combine(e->get_number(), e->get_function());
}

size_t std::hash<loki::ObserverPtr<const loki::ObjectImpl>>::operator()(loki::ObserverPtr<const loki::ObjectImpl> e) const
{
    return loki::hash_combine(e->get_name(), loki::get_sorted_vector(e->get_bases()));
}

size_t std::hash<loki::ObserverPtr<const loki::ParameterImpl>>::operator()(loki::ObserverPtr<const loki::ParameterImpl> e) const
{
    return loki::hash_combine(e->get_variable(), loki::get_sorted_vector(e->get_bases()));
}

size_t std::hash<loki::ObserverPtr<const loki::PredicateImpl>>::operator()(loki::ObserverPtr<const loki::PredicateImpl> e) const
{
    return loki::hash_combine(e->get_name(), loki::get_sorted_vector(e->get_parameters()));
}

size_t std::hash<loki::ObserverPtr<const loki::ProblemImpl>>::operator()(loki::ObserverPtr<const loki::ProblemImpl> e) const
{
    return loki::hash_combine(e->get_name(),
                              e->get_domain(),
                              loki::get_sorted_vector(e->get_objects()),
                              loki::get_sorted_vector(e->get_derived_predicates()),
                              loki::get_sorted_vector(e->get_initial_literals()),
                              loki::get_sorted_vector(e->get_numeric_fluents()),
                              e->get_goal_condition(),
                              e->get_optimization_metric());
}

size_t std::hash<loki::ObserverPtr<const loki::RequirementsImpl>>::operator()(loki::ObserverPtr<const loki::RequirementsImpl> e) const
{
    return loki::hash_combine(e->get_requirements());
}

size_t std::hash<loki::ObserverPtr<const loki::TermImpl>>::operator()(loki::ObserverPtr<const loki::TermImpl> e) const
{
    return loki::hash_combine(e->get_object_or_variable());
}

size_t std::hash<loki::ObserverPtr<const loki::TypeImpl>>::operator()(loki::ObserverPtr<const loki::TypeImpl> e) const
{
    return loki::hash_combine(e->get_name(), loki::get_sorted_vector(e->get_bases()));
}

size_t std::hash<loki::ObserverPtr<const loki::VariableImpl>>::operator()(loki::ObserverPtr<const loki::VariableImpl> e) const
{
    return loki::hash_combine(e->get_name());
}
