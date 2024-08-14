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

namespace loki
{
bool UniquePDDLEqualTo<const ActionImpl*>::operator()(const ActionImpl* l, const ActionImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_name() == r->get_name()) && (get_sorted_vector(l->get_parameters()) == get_sorted_vector(r->get_parameters()))
               && (l->get_condition() == r->get_condition()) && (l->get_effect() == r->get_effect());
    }
    return true;
}

bool UniquePDDLEqualTo<const AtomImpl*>::operator()(const AtomImpl* l, const AtomImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_predicate() == r->get_predicate()) && (l->get_terms() == r->get_terms());
    }
    return true;
}

bool UniquePDDLEqualTo<const AxiomImpl*>::operator()(const AxiomImpl* l, const AxiomImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_derived_predicate_name() == r->get_derived_predicate_name())
               && (get_sorted_vector(l->get_parameters()) == get_sorted_vector(r->get_parameters())) && (l->get_condition() == r->get_condition());
    }
    return true;
}

bool UniquePDDLEqualTo<const ConditionLiteralImpl&>::operator()(const ConditionLiteralImpl& l, const ConditionLiteralImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_literal() == r.get_literal());
    }
    return true;
}

bool UniquePDDLEqualTo<const ConditionAndImpl&>::operator()(const ConditionAndImpl& l, const ConditionAndImpl& r) const
{
    if (&l != &r)
    {
        return (get_sorted_vector(l.get_conditions()) == get_sorted_vector(r.get_conditions()));
    }
    return true;
}

bool UniquePDDLEqualTo<const ConditionOrImpl&>::operator()(const ConditionOrImpl& l, const ConditionOrImpl& r) const
{
    if (&l != &r)
    {
        return (get_sorted_vector(l.get_conditions()) == get_sorted_vector(r.get_conditions()));
    }
    return true;
}

bool UniquePDDLEqualTo<const ConditionNotImpl&>::operator()(const ConditionNotImpl& l, const ConditionNotImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_condition() == r.get_condition());
    }
    return true;
}

bool UniquePDDLEqualTo<const ConditionImplyImpl&>::operator()(const ConditionImplyImpl& l, const ConditionImplyImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_condition_left() == r.get_condition_left()) && (l.get_condition_left() == r.get_condition_left());
    }
    return true;
}

bool UniquePDDLEqualTo<const ConditionExistsImpl&>::operator()(const ConditionExistsImpl& l, const ConditionExistsImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_condition() == r.get_condition()) && (get_sorted_vector(l.get_parameters()) == get_sorted_vector(r.get_parameters()));
    }
    return true;
}

bool UniquePDDLEqualTo<const ConditionForallImpl&>::operator()(const ConditionForallImpl& l, const ConditionForallImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_condition() == r.get_condition()) && (get_sorted_vector(l.get_parameters()) == get_sorted_vector(r.get_parameters()));
    }
    return true;
}

bool UniquePDDLEqualTo<const ConditionImpl*>::operator()(const ConditionImpl* l, const ConditionImpl* r) const
{
    return UniquePDDLEqualTo<ConditionImpl>()(*l, *r);
}

bool UniquePDDLEqualTo<const DomainImpl*>::operator()(const DomainImpl* l, const DomainImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_name() == r->get_name()) && (l->get_requirements() == r->get_requirements())
               && (get_sorted_vector(l->get_types()) == get_sorted_vector(r->get_types()))
               && (get_sorted_vector(l->get_constants()) == get_sorted_vector(r->get_constants()))
               && (get_sorted_vector(l->get_predicates()) == get_sorted_vector(r->get_predicates()))
               && (get_sorted_vector(l->get_functions()) == get_sorted_vector(r->get_functions()))
               && (get_sorted_vector(l->get_actions()) == get_sorted_vector(r->get_actions()))
               && (get_sorted_vector(l->get_axioms()) == get_sorted_vector(r->get_axioms()));
    }
    return true;
}

bool UniquePDDLEqualTo<const EffectLiteralImpl&>::operator()(const EffectLiteralImpl& l, const EffectLiteralImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_literal() == r.get_literal());
    }
    return true;
}

bool UniquePDDLEqualTo<const EffectAndImpl&>::operator()(const EffectAndImpl& l, const EffectAndImpl& r) const
{
    if (&l != &r)
    {
        return (get_sorted_vector(l.get_effects()) == get_sorted_vector(r.get_effects()));
    }
    return true;
}

bool UniquePDDLEqualTo<const EffectNumericImpl&>::operator()(const EffectNumericImpl& l, const EffectNumericImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_assign_operator() == r.get_assign_operator()) && (l.get_function() == r.get_function())
               && (l.get_function_expression() == r.get_function_expression());
    }
    return true;
}

bool UniquePDDLEqualTo<const EffectConditionalForallImpl&>::operator()(const EffectConditionalForallImpl& l, const EffectConditionalForallImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_effect() == r.get_effect()) && (get_sorted_vector(l.get_parameters()) == get_sorted_vector(r.get_parameters()));
    }
    return true;
}

bool UniquePDDLEqualTo<const EffectConditionalWhenImpl&>::operator()(const EffectConditionalWhenImpl& l, const EffectConditionalWhenImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_condition() == r.get_condition()) && (l.get_effect() == r.get_effect());
    }
    return true;
}

bool UniquePDDLEqualTo<const EffectImpl*>::operator()(const EffectImpl* l, const EffectImpl* r) const { return UniquePDDLEqualTo<EffectImpl>()(*l, *r); }

bool UniquePDDLEqualTo<const FunctionExpressionNumberImpl&>::operator()(const FunctionExpressionNumberImpl& l, const FunctionExpressionNumberImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_number() == r.get_number());
    }
    return true;
}

bool UniquePDDLEqualTo<const FunctionExpressionBinaryOperatorImpl&>::operator()(const FunctionExpressionBinaryOperatorImpl& l,
                                                                                const FunctionExpressionBinaryOperatorImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_binary_operator() == r.get_binary_operator()) && (l.get_left_function_expression() == r.get_left_function_expression())
               && (l.get_right_function_expression() == r.get_right_function_expression());
    }
    return true;
}

bool UniquePDDLEqualTo<const FunctionExpressionMultiOperatorImpl&>::operator()(const FunctionExpressionMultiOperatorImpl& l,
                                                                               const FunctionExpressionMultiOperatorImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_multi_operator() == r.get_multi_operator())
               && (get_sorted_vector(l.get_function_expressions()) == get_sorted_vector(r.get_function_expressions()));
    }
    return true;
}

bool UniquePDDLEqualTo<const FunctionExpressionMinusImpl&>::operator()(const FunctionExpressionMinusImpl& l, const FunctionExpressionMinusImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_function_expression() == r.get_function_expression());
    }
    return true;
}

bool UniquePDDLEqualTo<const FunctionExpressionFunctionImpl&>::operator()(const FunctionExpressionFunctionImpl& l,
                                                                          const FunctionExpressionFunctionImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_function() == r.get_function());
    }
    return true;
}

bool UniquePDDLEqualTo<const FunctionExpressionImpl*>::operator()(const FunctionExpressionImpl* l, const FunctionExpressionImpl* r) const
{
    return UniquePDDLEqualTo<FunctionExpressionImpl>()(*l, *r);
}

bool UniquePDDLEqualTo<const FunctionSkeletonImpl*>::operator()(const FunctionSkeletonImpl* l, const FunctionSkeletonImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_name() == r->get_name()) && (l->get_type() == r->get_type())
               && (get_sorted_vector(l->get_parameters()) == get_sorted_vector(r->get_parameters()));
    }
    return true;
}

bool UniquePDDLEqualTo<const FunctionImpl*>::operator()(const FunctionImpl* l, const FunctionImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_function_skeleton() == r->get_function_skeleton()) && (l->get_terms() == r->get_terms());
    }
    return true;
}

bool UniquePDDLEqualTo<const LiteralImpl*>::operator()(const LiteralImpl* l, const LiteralImpl* r) const
{
    if (&l != &r)
    {
        return (l->is_negated() == r->is_negated()) && (l->get_atom() == r->get_atom());
    }
    return true;
}

bool UniquePDDLEqualTo<const OptimizationMetricImpl*>::operator()(const OptimizationMetricImpl* l, const OptimizationMetricImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_optimization_metric() == r->get_optimization_metric()) && (l->get_function_expression() == r->get_function_expression());
    }
    return true;
}

bool UniquePDDLEqualTo<const NumericFluentImpl*>::operator()(const NumericFluentImpl* l, const NumericFluentImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_number() == r->get_number()) && (l->get_function() == r->get_function());
    }
    return true;
}

bool UniquePDDLEqualTo<const ObjectImpl*>::operator()(const ObjectImpl* l, const ObjectImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_name() == r->get_name()) && (get_sorted_vector(l->get_bases()) == get_sorted_vector(r->get_bases()));
    }
    return true;
}

bool UniquePDDLEqualTo<const ParameterImpl*>::operator()(const ParameterImpl* l, const ParameterImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_variable() == r->get_variable()) && (get_sorted_vector(l->get_bases()) == get_sorted_vector(r->get_bases()));
    }
    return true;
}

bool UniquePDDLEqualTo<const PredicateImpl*>::operator()(const PredicateImpl* l, const PredicateImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_name() == r->get_name()) && (get_sorted_vector(l->get_parameters()) == get_sorted_vector(r->get_parameters()));
    }
    return true;
}

bool UniquePDDLEqualTo<const ProblemImpl*>::operator()(const ProblemImpl* l, const ProblemImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_name() == r->get_name()) && (l->get_domain() == r->get_domain())
               && (get_sorted_vector(l->get_objects()) == get_sorted_vector(r->get_objects()))
               && (get_sorted_vector(l->get_derived_predicates()) == get_sorted_vector(r->get_derived_predicates()))
               && (get_sorted_vector(l->get_initial_literals()) == get_sorted_vector(r->get_initial_literals()))
               && (get_sorted_vector(l->get_numeric_fluents()) == get_sorted_vector(r->get_numeric_fluents()))
               && (l->get_goal_condition() == r->get_goal_condition()) && (l->get_optimization_metric() == r->get_optimization_metric());
    }
    return true;
}

bool UniquePDDLEqualTo<const RequirementsImpl*>::operator()(const RequirementsImpl* l, const RequirementsImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_requirements() == r->get_requirements());
    }
    return true;
}

bool UniquePDDLEqualTo<const TermObjectImpl&>::operator()(const TermObjectImpl& l, const TermObjectImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_object() == r.get_object());
    }
    return true;
}

bool UniquePDDLEqualTo<const TermVariableImpl&>::operator()(const TermVariableImpl& l, const TermVariableImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_variable() == r.get_variable());
    }
    return true;
}

bool UniquePDDLEqualTo<const TermImpl*>::operator()(const TermImpl* l, const TermImpl* r) const { return UniquePDDLEqualTo<TermImpl>()(*l, *r); }

bool UniquePDDLEqualTo<const TypeImpl*>::operator()(const TypeImpl* l, const TypeImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_name() == r->get_name()) && (get_sorted_vector(l->get_bases()) == get_sorted_vector(r->get_bases()));
    }
    return true;
}

bool UniquePDDLEqualTo<const VariableImpl*>::operator()(const VariableImpl* l, const VariableImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_name() == r->get_name());
    }
    return true;
}
}