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
bool UniquePDDLEqualTo<Action>::operator()(Action l, Action r) const
{
    if (l != r)
    {
        return (l->get_name() == r->get_name()) && (get_sorted_vector(l->get_parameters()) == get_sorted_vector(r->get_parameters()))
               && (l->get_condition() == r->get_condition()) && (l->get_effect() == r->get_effect());
    }
    return true;
}

bool UniquePDDLEqualTo<Atom>::operator()(Atom l, Atom r) const
{
    if (l != r)
    {
        return (l->get_predicate() == r->get_predicate()) && (l->get_terms() == r->get_terms());
    }
    return true;
}

bool UniquePDDLEqualTo<Axiom>::operator()(Axiom l, Axiom r) const
{
    if (l != r)
    {
        return (l->get_derived_predicate_name() == r->get_derived_predicate_name())
               && (get_sorted_vector(l->get_parameters()) == get_sorted_vector(r->get_parameters())) && (l->get_condition() == r->get_condition());
    }
    return true;
}

bool UniquePDDLEqualTo<ConditionLiteral>::operator()(ConditionLiteral l, ConditionLiteral r) const
{
    if (l != r)
    {
        return (l->get_literal() == r->get_literal());
    }
    return true;
}

bool UniquePDDLEqualTo<ConditionAnd>::operator()(ConditionAnd l, ConditionAnd r) const
{
    if (l != r)
    {
        return (get_sorted_vector(l->get_conditions()) == get_sorted_vector(r->get_conditions()));
    }
    return true;
}

bool UniquePDDLEqualTo<ConditionOr>::operator()(ConditionOr l, ConditionOr r) const
{
    if (l != r)
    {
        return (get_sorted_vector(l->get_conditions()) == get_sorted_vector(r->get_conditions()));
    }
    return true;
}

bool UniquePDDLEqualTo<ConditionNot>::operator()(ConditionNot l, ConditionNot r) const
{
    if (l != r)
    {
        return (l->get_condition() == r->get_condition());
    }
    return true;
}

bool UniquePDDLEqualTo<ConditionImply>::operator()(ConditionImply l, ConditionImply r) const
{
    if (l != r)
    {
        return (l->get_condition_left() == r->get_condition_left()) && (l->get_condition_left() == r->get_condition_left());
    }
    return true;
}

bool UniquePDDLEqualTo<ConditionExists>::operator()(ConditionExists l, ConditionExists r) const
{
    if (l != r)
    {
        return (l->get_condition() == r->get_condition()) && (get_sorted_vector(l->get_parameters()) == get_sorted_vector(r->get_parameters()));
    }
    return true;
}

bool UniquePDDLEqualTo<ConditionForall>::operator()(ConditionForall l, ConditionForall r) const
{
    if (l != r)
    {
        return (l->get_condition() == r->get_condition()) && (get_sorted_vector(l->get_parameters()) == get_sorted_vector(r->get_parameters()));
    }
    return true;
}

bool UniquePDDLEqualTo<Condition>::operator()(Condition l, Condition r) const { return l->get_condition() == r->get_condition(); }

bool UniquePDDLEqualTo<Domain>::operator()(Domain l, Domain r) const
{
    if (l != r)
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

bool UniquePDDLEqualTo<EffectLiteral>::operator()(EffectLiteral l, EffectLiteral r) const
{
    if (l != r)
    {
        return (l->get_literal() == r->get_literal());
    }
    return true;
}

bool UniquePDDLEqualTo<EffectAnd>::operator()(EffectAnd l, EffectAnd r) const
{
    if (l != r)
    {
        return (get_sorted_vector(l->get_effects()) == get_sorted_vector(r->get_effects()));
    }
    return true;
}

bool UniquePDDLEqualTo<EffectNumeric>::operator()(EffectNumeric l, EffectNumeric r) const
{
    if (l != r)
    {
        return (l->get_assign_operator() == r->get_assign_operator()) && (l->get_function() == r->get_function())
               && (l->get_function_expression() == r->get_function_expression());
    }
    return true;
}

bool UniquePDDLEqualTo<EffectCompositeForall>::operator()(EffectCompositeForall l, EffectCompositeForall r) const
{
    if (l != r)
    {
        return (l->get_effect() == r->get_effect()) && (get_sorted_vector(l->get_parameters()) == get_sorted_vector(r->get_parameters()));
    }
    return true;
}

bool UniquePDDLEqualTo<EffectCompositeWhen>::operator()(EffectCompositeWhen l, EffectCompositeWhen r) const
{
    if (l != r)
    {
        return (l->get_condition() == r->get_condition()) && (l->get_effect() == r->get_effect());
    }
    return true;
}

bool UniquePDDLEqualTo<EffectCompositeOneof>::operator()(EffectCompositeOneof l, EffectCompositeOneof r) const
{
    if (l != r)
    {
        return (get_sorted_vector(l->get_effects()) == get_sorted_vector(r->get_effects()));
    }
    return true;
}

bool UniquePDDLEqualTo<Effect>::operator()(Effect l, Effect r) const { return l->get_effect() == r->get_effect(); }

bool UniquePDDLEqualTo<FunctionExpressionNumber>::operator()(FunctionExpressionNumber l, FunctionExpressionNumber r) const
{
    if (l != r)
    {
        return (l->get_number() == r->get_number());
    }
    return true;
}

bool UniquePDDLEqualTo<FunctionExpressionBinaryOperator>::operator()(FunctionExpressionBinaryOperator l, FunctionExpressionBinaryOperator r) const
{
    if (l != r)
    {
        return (l->get_binary_operator() == r->get_binary_operator()) && (l->get_left_function_expression() == r->get_left_function_expression())
               && (l->get_right_function_expression() == r->get_right_function_expression());
    }
    return true;
}

bool UniquePDDLEqualTo<FunctionExpressionMultiOperator>::operator()(FunctionExpressionMultiOperator l, FunctionExpressionMultiOperator r) const
{
    if (l != r)
    {
        return (l->get_multi_operator() == r->get_multi_operator())
               && (get_sorted_vector(l->get_function_expressions()) == get_sorted_vector(r->get_function_expressions()));
    }
    return true;
}

bool UniquePDDLEqualTo<FunctionExpressionMinus>::operator()(FunctionExpressionMinus l, FunctionExpressionMinus r) const
{
    if (l != r)
    {
        return (l->get_function_expression() == r->get_function_expression());
    }
    return true;
}

bool UniquePDDLEqualTo<FunctionExpressionFunction>::operator()(FunctionExpressionFunction l, FunctionExpressionFunction r) const
{
    if (l != r)
    {
        return (l->get_function() == r->get_function());
    }
    return true;
}

bool UniquePDDLEqualTo<FunctionExpression>::operator()(FunctionExpression l, FunctionExpression r) const
{
    return l->get_function_expression() == r->get_function_expression();
}

bool UniquePDDLEqualTo<FunctionSkeleton>::operator()(FunctionSkeleton l, FunctionSkeleton r) const
{
    if (l != r)
    {
        return (l->get_name() == r->get_name()) && (l->get_type() == r->get_type())
               && (get_sorted_vector(l->get_parameters()) == get_sorted_vector(r->get_parameters()));
    }
    return true;
}

bool UniquePDDLEqualTo<Function>::operator()(Function l, Function r) const
{
    if (l != r)
    {
        return (l->get_function_skeleton() == r->get_function_skeleton()) && (l->get_terms() == r->get_terms());
    }
    return true;
}

bool UniquePDDLEqualTo<Literal>::operator()(Literal l, Literal r) const
{
    if (l != r)
    {
        return (l->is_negated() == r->is_negated()) && (l->get_atom() == r->get_atom());
    }
    return true;
}

bool UniquePDDLEqualTo<OptimizationMetric>::operator()(OptimizationMetric l, OptimizationMetric r) const
{
    if (l != r)
    {
        return (l->get_optimization_metric() == r->get_optimization_metric()) && (l->get_function_expression() == r->get_function_expression());
    }
    return true;
}

bool UniquePDDLEqualTo<NumericFluent>::operator()(NumericFluent l, NumericFluent r) const
{
    if (l != r)
    {
        return (l->get_number() == r->get_number()) && (l->get_function() == r->get_function());
    }
    return true;
}

bool UniquePDDLEqualTo<Object>::operator()(Object l, Object r) const
{
    if (l != r)
    {
        return (l->get_name() == r->get_name()) && (get_sorted_vector(l->get_bases()) == get_sorted_vector(r->get_bases()));
    }
    return true;
}

bool UniquePDDLEqualTo<Parameter>::operator()(Parameter l, Parameter r) const
{
    if (l != r)
    {
        return (l->get_variable() == r->get_variable()) && (get_sorted_vector(l->get_bases()) == get_sorted_vector(r->get_bases()));
    }
    return true;
}

bool UniquePDDLEqualTo<Predicate>::operator()(Predicate l, Predicate r) const
{
    if (l != r)
    {
        return (l->get_name() == r->get_name()) && (get_sorted_vector(l->get_parameters()) == get_sorted_vector(r->get_parameters()));
    }
    return true;
}

bool UniquePDDLEqualTo<Problem>::operator()(Problem l, Problem r) const
{
    if (l != r)
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

bool UniquePDDLEqualTo<Requirements>::operator()(Requirements l, Requirements r) const
{
    if (l != r)
    {
        return (l->get_requirements() == r->get_requirements());
    }
    return true;
}

bool UniquePDDLEqualTo<Term>::operator()(Term l, Term r) const { return l->get_object_or_variable() == r->get_object_or_variable(); }

bool UniquePDDLEqualTo<Type>::operator()(Type l, Type r) const
{
    if (l != r)
    {
        return (l->get_name() == r->get_name()) && (get_sorted_vector(l->get_bases()) == get_sorted_vector(r->get_bases()));
    }
    return true;
}

bool UniquePDDLEqualTo<Variable>::operator()(Variable l, Variable r) const
{
    if (l != r)
    {
        return (l->get_name() == r->get_name());
    }
    return true;
}
}