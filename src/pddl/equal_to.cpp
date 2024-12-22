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

#include "loki/details/pddl/equal_to.hpp"

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

bool std::equal_to<loki::ObserverPtr<const loki::ActionImpl>>::operator()(loki::ObserverPtr<const loki::ActionImpl> lhs,
                                                                          loki::ObserverPtr<const loki::ActionImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_name() == rhs->get_name()) && (loki::get_sorted_vector(lhs->get_parameters()) == loki::get_sorted_vector(rhs->get_parameters()))
               && (lhs->get_condition() == rhs->get_condition()) && (lhs->get_effect() == rhs->get_effect());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::AtomImpl>>::operator()(loki::ObserverPtr<const loki::AtomImpl> lhs,
                                                                        loki::ObserverPtr<const loki::AtomImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_predicate() == rhs->get_predicate()) && (lhs->get_terms() == rhs->get_terms());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::AxiomImpl>>::operator()(loki::ObserverPtr<const loki::AxiomImpl> lhs,
                                                                         loki::ObserverPtr<const loki::AxiomImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_derived_predicate_name() == rhs->get_derived_predicate_name())
               && (loki::get_sorted_vector(lhs->get_parameters()) == loki::get_sorted_vector(rhs->get_parameters()))
               && (lhs->get_condition() == rhs->get_condition());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::ConditionLiteralImpl>>::operator()(loki::ObserverPtr<const loki::ConditionLiteralImpl> lhs,
                                                                                    loki::ObserverPtr<const loki::ConditionLiteralImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_literal() == rhs->get_literal());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::ConditionAndImpl>>::operator()(loki::ObserverPtr<const loki::ConditionAndImpl> lhs,
                                                                                loki::ObserverPtr<const loki::ConditionAndImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (loki::get_sorted_vector(lhs->get_conditions()) == loki::get_sorted_vector(rhs->get_conditions()));
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::ConditionOrImpl>>::operator()(loki::ObserverPtr<const loki::ConditionOrImpl> lhs,
                                                                               loki::ObserverPtr<const loki::ConditionOrImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (loki::get_sorted_vector(lhs->get_conditions()) == loki::get_sorted_vector(rhs->get_conditions()));
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::ConditionNotImpl>>::operator()(loki::ObserverPtr<const loki::ConditionNotImpl> lhs,
                                                                                loki::ObserverPtr<const loki::ConditionNotImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_condition() == rhs->get_condition());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::ConditionImplyImpl>>::operator()(loki::ObserverPtr<const loki::ConditionImplyImpl> lhs,
                                                                                  loki::ObserverPtr<const loki::ConditionImplyImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_condition_left() == rhs->get_condition_left()) && (lhs->get_condition_right() == rhs->get_condition_right());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::ConditionExistsImpl>>::operator()(loki::ObserverPtr<const loki::ConditionExistsImpl> lhs,
                                                                                   loki::ObserverPtr<const loki::ConditionExistsImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_condition() == rhs->get_condition())
               && (loki::get_sorted_vector(lhs->get_parameters()) == loki::get_sorted_vector(rhs->get_parameters()));
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::ConditionForallImpl>>::operator()(loki::ObserverPtr<const loki::ConditionForallImpl> lhs,
                                                                                   loki::ObserverPtr<const loki::ConditionForallImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_condition() == rhs->get_condition())
               && (loki::get_sorted_vector(lhs->get_parameters()) == loki::get_sorted_vector(rhs->get_parameters()));
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::ConditionImpl>>::operator()(loki::ObserverPtr<const loki::ConditionImpl> lhs,
                                                                             loki::ObserverPtr<const loki::ConditionImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_condition() == rhs->get_condition());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::DomainImpl>>::operator()(loki::ObserverPtr<const loki::DomainImpl> lhs,
                                                                          loki::ObserverPtr<const loki::DomainImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_name() == rhs->get_name()) && (lhs->get_requirements() == rhs->get_requirements())
               && (loki::get_sorted_vector(lhs->get_types()) == loki::get_sorted_vector(rhs->get_types()))
               && (loki::get_sorted_vector(lhs->get_constants()) == loki::get_sorted_vector(rhs->get_constants()))
               && (loki::get_sorted_vector(lhs->get_predicates()) == loki::get_sorted_vector(rhs->get_predicates()))
               && (loki::get_sorted_vector(lhs->get_functions()) == loki::get_sorted_vector(rhs->get_functions()))
               && (loki::get_sorted_vector(lhs->get_actions()) == loki::get_sorted_vector(rhs->get_actions()))
               && (loki::get_sorted_vector(lhs->get_axioms()) == loki::get_sorted_vector(rhs->get_axioms()));
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::EffectLiteralImpl>>::operator()(loki::ObserverPtr<const loki::EffectLiteralImpl> lhs,
                                                                                 loki::ObserverPtr<const loki::EffectLiteralImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_literal() == rhs->get_literal());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::EffectAndImpl>>::operator()(loki::ObserverPtr<const loki::EffectAndImpl> lhs,
                                                                             loki::ObserverPtr<const loki::EffectAndImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (loki::get_sorted_vector(lhs->get_effects()) == loki::get_sorted_vector(rhs->get_effects()));
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::EffectNumericImpl>>::operator()(loki::ObserverPtr<const loki::EffectNumericImpl> lhs,
                                                                                 loki::ObserverPtr<const loki::EffectNumericImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_assign_operator() == rhs->get_assign_operator()) && (lhs->get_function() == rhs->get_function())
               && (lhs->get_function_expression() == rhs->get_function_expression());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::EffectCompositeForallImpl>>::operator()(loki::ObserverPtr<const loki::EffectCompositeForallImpl> lhs,
                                                                                         loki::ObserverPtr<const loki::EffectCompositeForallImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_effect() == rhs->get_effect()) && (loki::get_sorted_vector(lhs->get_parameters()) == loki::get_sorted_vector(rhs->get_parameters()));
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::EffectCompositeWhenImpl>>::operator()(loki::ObserverPtr<const loki::EffectCompositeWhenImpl> lhs,
                                                                                       loki::ObserverPtr<const loki::EffectCompositeWhenImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_condition() == rhs->get_condition()) && (lhs->get_effect() == rhs->get_effect());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::EffectCompositeOneofImpl>>::operator()(loki::ObserverPtr<const loki::EffectCompositeOneofImpl> lhs,
                                                                                        loki::ObserverPtr<const loki::EffectCompositeOneofImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (loki::get_sorted_vector(lhs->get_effects()) == loki::get_sorted_vector(rhs->get_effects()));
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::EffectImpl>>::operator()(loki::ObserverPtr<const loki::EffectImpl> lhs,
                                                                          loki::ObserverPtr<const loki::EffectImpl> rhs) const
{
    return lhs->get_effect() == rhs->get_effect();
}

bool std::equal_to<loki::ObserverPtr<const loki::FunctionExpressionNumberImpl>>::operator()(
    loki::ObserverPtr<const loki::FunctionExpressionNumberImpl> lhs,
    loki::ObserverPtr<const loki::FunctionExpressionNumberImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_number() == rhs->get_number());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::FunctionExpressionBinaryOperatorImpl>>::operator()(
    loki::ObserverPtr<const loki::FunctionExpressionBinaryOperatorImpl> lhs,
    loki::ObserverPtr<const loki::FunctionExpressionBinaryOperatorImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_binary_operator() == rhs->get_binary_operator()) && (lhs->get_left_function_expression() == rhs->get_left_function_expression())
               && (lhs->get_right_function_expression() == rhs->get_right_function_expression());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::FunctionExpressionMultiOperatorImpl>>::operator()(
    loki::ObserverPtr<const loki::FunctionExpressionMultiOperatorImpl> lhs,
    loki::ObserverPtr<const loki::FunctionExpressionMultiOperatorImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_multi_operator() == rhs->get_multi_operator())
               && (loki::get_sorted_vector(lhs->get_function_expressions()) == loki::get_sorted_vector(rhs->get_function_expressions()));
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::FunctionExpressionMinusImpl>>::operator()(loki::ObserverPtr<const loki::FunctionExpressionMinusImpl> lhs,
                                                                                           loki::ObserverPtr<const loki::FunctionExpressionMinusImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_function_expression() == rhs->get_function_expression());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::FunctionExpressionFunctionImpl>>::operator()(
    loki::ObserverPtr<const loki::FunctionExpressionFunctionImpl> lhs,
    loki::ObserverPtr<const loki::FunctionExpressionFunctionImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_function() == rhs->get_function());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::FunctionExpressionImpl>>::operator()(loki::ObserverPtr<const loki::FunctionExpressionImpl> lhs,
                                                                                      loki::ObserverPtr<const loki::FunctionExpressionImpl> rhs) const
{
    return lhs->get_function_expression() == rhs->get_function_expression();
}

bool std::equal_to<loki::ObserverPtr<const loki::FunctionSkeletonImpl>>::operator()(loki::ObserverPtr<const loki::FunctionSkeletonImpl> lhs,
                                                                                    loki::ObserverPtr<const loki::FunctionSkeletonImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_name() == rhs->get_name()) && (lhs->get_type() == rhs->get_type())
               && (loki::get_sorted_vector(lhs->get_parameters()) == loki::get_sorted_vector(rhs->get_parameters()));
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::FunctionImpl>>::operator()(loki::ObserverPtr<const loki::FunctionImpl> lhs,
                                                                            loki::ObserverPtr<const loki::FunctionImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_function_skeleton() == rhs->get_function_skeleton()) && (lhs->get_terms() == rhs->get_terms());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::LiteralImpl>>::operator()(loki::ObserverPtr<const loki::LiteralImpl> lhs,
                                                                           loki::ObserverPtr<const loki::LiteralImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->is_negated() == rhs->is_negated()) && (lhs->get_atom() == rhs->get_atom());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::OptimizationMetricImpl>>::operator()(loki::ObserverPtr<const loki::OptimizationMetricImpl> lhs,
                                                                                      loki::ObserverPtr<const loki::OptimizationMetricImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_optimization_metric() == rhs->get_optimization_metric()) && (lhs->get_function_expression() == rhs->get_function_expression());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::NumericFluentImpl>>::operator()(loki::ObserverPtr<const loki::NumericFluentImpl> lhs,
                                                                                 loki::ObserverPtr<const loki::NumericFluentImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_number() == rhs->get_number()) && (lhs->get_function() == rhs->get_function());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::ObjectImpl>>::operator()(loki::ObserverPtr<const loki::ObjectImpl> lhs,
                                                                          loki::ObserverPtr<const loki::ObjectImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_name() == rhs->get_name()) && (loki::get_sorted_vector(lhs->get_bases()) == loki::get_sorted_vector(rhs->get_bases()));
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::ParameterImpl>>::operator()(loki::ObserverPtr<const loki::ParameterImpl> lhs,
                                                                             loki::ObserverPtr<const loki::ParameterImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_variable() == rhs->get_variable()) && (loki::get_sorted_vector(lhs->get_bases()) == loki::get_sorted_vector(rhs->get_bases()));
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::PredicateImpl>>::operator()(loki::ObserverPtr<const loki::PredicateImpl> lhs,
                                                                             loki::ObserverPtr<const loki::PredicateImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_name() == rhs->get_name()) && (loki::get_sorted_vector(lhs->get_parameters()) == loki::get_sorted_vector(rhs->get_parameters()));
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::ProblemImpl>>::operator()(loki::ObserverPtr<const loki::ProblemImpl> lhs,
                                                                           loki::ObserverPtr<const loki::ProblemImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_name() == rhs->get_name()) && (lhs->get_domain() == rhs->get_domain())
               && (loki::get_sorted_vector(lhs->get_objects()) == loki::get_sorted_vector(rhs->get_objects()))
               && (loki::get_sorted_vector(lhs->get_derived_predicates()) == loki::get_sorted_vector(rhs->get_derived_predicates()))
               && (loki::get_sorted_vector(lhs->get_initial_literals()) == loki::get_sorted_vector(rhs->get_initial_literals()))
               && (loki::get_sorted_vector(lhs->get_numeric_fluents()) == loki::get_sorted_vector(rhs->get_numeric_fluents()))
               && (lhs->get_goal_condition() == rhs->get_goal_condition()) && (lhs->get_optimization_metric() == rhs->get_optimization_metric());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::RequirementsImpl>>::operator()(loki::ObserverPtr<const loki::RequirementsImpl> lhs,
                                                                                loki::ObserverPtr<const loki::RequirementsImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_requirements() == rhs->get_requirements());
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::TermImpl>>::operator()(loki::ObserverPtr<const loki::TermImpl> lhs,
                                                                        loki::ObserverPtr<const loki::TermImpl> rhs) const
{
    return lhs->get_object_or_variable() == rhs->get_object_or_variable();
}

bool std::equal_to<loki::ObserverPtr<const loki::TypeImpl>>::operator()(loki::ObserverPtr<const loki::TypeImpl> lhs,
                                                                        loki::ObserverPtr<const loki::TypeImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_name() == rhs->get_name()) && (loki::get_sorted_vector(lhs->get_bases()) == loki::get_sorted_vector(rhs->get_bases()));
    }
    return true;
}

bool std::equal_to<loki::ObserverPtr<const loki::VariableImpl>>::operator()(loki::ObserverPtr<const loki::VariableImpl> lhs,
                                                                            loki::ObserverPtr<const loki::VariableImpl> rhs) const
{
    if (lhs != rhs)
    {
        return (lhs->get_name() == rhs->get_name());
    }
    return true;
}
