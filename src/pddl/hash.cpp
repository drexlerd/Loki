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

namespace loki
{
size_t UniquePDDLHasher<Action>::operator()(Action e) const
{
    return UniquePDDLHashCombiner()(e->get_name(), get_sorted_vector(e->get_parameters()), e->get_condition(), e->get_effect());
}

size_t UniquePDDLHasher<Atom>::operator()(Atom e) const { return UniquePDDLHashCombiner()(e->get_predicate(), e->get_terms()); }

size_t UniquePDDLHasher<Axiom>::operator()(Axiom e) const
{
    return UniquePDDLHashCombiner()(e->get_derived_predicate_name(), get_sorted_vector(e->get_parameters()), e->get_condition());
}

size_t UniquePDDLHasher<ConditionLiteral>::operator()(ConditionLiteral e) const { return UniquePDDLHashCombiner()(e->get_literal()); }

size_t UniquePDDLHasher<ConditionAnd>::operator()(ConditionAnd e) const { return UniquePDDLHashCombiner()(get_sorted_vector(e->get_conditions())); }

size_t UniquePDDLHasher<ConditionOr>::operator()(ConditionOr e) const { return UniquePDDLHashCombiner()(get_sorted_vector(e->get_conditions())); }

size_t UniquePDDLHasher<ConditionNot>::operator()(ConditionNot e) const { return UniquePDDLHashCombiner()(e->get_condition()); }

size_t UniquePDDLHasher<ConditionImply>::operator()(ConditionImply e) const
{
    return UniquePDDLHashCombiner()(e->get_condition_left(), e->get_condition_right());
}

size_t UniquePDDLHasher<ConditionExists>::operator()(ConditionExists e) const
{
    return UniquePDDLHashCombiner()(get_sorted_vector(e->get_parameters()), e->get_condition());
}

size_t UniquePDDLHasher<ConditionForall>::operator()(ConditionForall e) const
{
    return UniquePDDLHashCombiner()(get_sorted_vector(e->get_parameters()), e->get_condition());
}

size_t UniquePDDLHasher<Condition>::operator()(Condition e) const { return UniquePDDLHashCombiner()(e->get_condition()); }

size_t UniquePDDLHasher<Domain>::operator()(Domain e) const
{
    return UniquePDDLHashCombiner()(e->get_name(),
                                    e->get_requirements(),
                                    get_sorted_vector(e->get_types()),
                                    get_sorted_vector(e->get_constants()),
                                    get_sorted_vector(e->get_predicates()),
                                    get_sorted_vector(e->get_functions()),
                                    get_sorted_vector(e->get_actions()),
                                    get_sorted_vector(e->get_axioms()));
}

size_t UniquePDDLHasher<EffectLiteral>::operator()(EffectLiteral e) const { return UniquePDDLHashCombiner()(e->get_literal()); }

size_t UniquePDDLHasher<EffectAnd>::operator()(EffectAnd e) const { return UniquePDDLHashCombiner()(get_sorted_vector(e->get_effects())); }

size_t UniquePDDLHasher<EffectNumeric>::operator()(EffectNumeric e) const
{
    return UniquePDDLHashCombiner()(e->get_assign_operator(), e->get_function(), e->get_function_expression());
}

size_t UniquePDDLHasher<EffectCompositeForall>::operator()(EffectCompositeForall e) const
{
    return UniquePDDLHashCombiner()(e->get_effect(), get_sorted_vector(e->get_parameters()));
}

size_t UniquePDDLHasher<EffectCompositeWhen>::operator()(EffectCompositeWhen e) const { return UniquePDDLHashCombiner()(e->get_condition(), e->get_effect()); }

size_t UniquePDDLHasher<EffectCompositeOneof>::operator()(EffectCompositeOneof e) const
{
    return UniquePDDLHashCombiner()(get_sorted_vector(e->get_effects()));
}

size_t UniquePDDLHasher<Effect>::operator()(Effect e) const { return UniquePDDLHashCombiner()(e->get_effect()); }

size_t UniquePDDLHasher<FunctionExpressionNumber>::operator()(FunctionExpressionNumber e) const { return UniquePDDLHashCombiner()(e->get_number()); }

size_t UniquePDDLHasher<FunctionExpressionBinaryOperator>::operator()(FunctionExpressionBinaryOperator e) const
{
    return UniquePDDLHashCombiner()(e->get_binary_operator(), e->get_left_function_expression(), e->get_right_function_expression());
}

size_t UniquePDDLHasher<FunctionExpressionMultiOperator>::operator()(FunctionExpressionMultiOperator e) const
{
    return UniquePDDLHashCombiner()(e->get_multi_operator(), get_sorted_vector(e->get_function_expressions()));
}

size_t UniquePDDLHasher<FunctionExpressionMinus>::operator()(FunctionExpressionMinus e) const { return UniquePDDLHashCombiner()(e->get_function_expression()); }

size_t UniquePDDLHasher<FunctionExpressionFunction>::operator()(FunctionExpressionFunction e) const { return UniquePDDLHashCombiner()(e->get_function()); }

size_t UniquePDDLHasher<FunctionExpression>::operator()(FunctionExpression e) const { return UniquePDDLHashCombiner()(e->get_function_expression()); }

size_t UniquePDDLHasher<FunctionSkeleton>::operator()(FunctionSkeleton e) const
{
    return UniquePDDLHashCombiner()(e->get_name(), e->get_type(), get_sorted_vector(e->get_parameters()));
}

size_t UniquePDDLHasher<Function>::operator()(Function e) const { return UniquePDDLHashCombiner()(e->get_function_skeleton(), e->get_terms()); }

size_t UniquePDDLHasher<Literal>::operator()(Literal e) const { return UniquePDDLHashCombiner()(e->is_negated(), e->get_atom()); }

size_t UniquePDDLHasher<OptimizationMetric>::operator()(OptimizationMetric e) const
{
    return UniquePDDLHashCombiner()(e->get_optimization_metric(), e->get_function_expression());
}

size_t UniquePDDLHasher<NumericFluent>::operator()(NumericFluent e) const { return UniquePDDLHashCombiner()(e->get_number(), e->get_function()); }

size_t UniquePDDLHasher<Object>::operator()(Object e) const { return UniquePDDLHashCombiner()(e->get_name(), get_sorted_vector(e->get_bases())); }

size_t UniquePDDLHasher<Parameter>::operator()(Parameter e) const { return UniquePDDLHashCombiner()(e->get_variable(), get_sorted_vector(e->get_bases())); }

size_t UniquePDDLHasher<Predicate>::operator()(Predicate e) const { return UniquePDDLHashCombiner()(e->get_name(), get_sorted_vector(e->get_parameters())); }

size_t UniquePDDLHasher<Problem>::operator()(Problem e) const
{
    return UniquePDDLHashCombiner()(e->get_name(),
                                    e->get_domain(),
                                    get_sorted_vector(e->get_objects()),
                                    get_sorted_vector(e->get_derived_predicates()),
                                    get_sorted_vector(e->get_initial_literals()),
                                    get_sorted_vector(e->get_numeric_fluents()),
                                    e->get_goal_condition(),
                                    e->get_optimization_metric());
}

size_t UniquePDDLHasher<Requirements>::operator()(Requirements e) const { return UniquePDDLHashCombiner()(e->get_requirements()); }

size_t UniquePDDLHasher<Term>::operator()(Term e) const { return UniquePDDLHashCombiner()(e->get_object_or_variable()); }

size_t UniquePDDLHasher<Type>::operator()(Type e) const { return UniquePDDLHashCombiner()(e->get_name(), get_sorted_vector(e->get_bases())); }

size_t UniquePDDLHasher<Variable>::operator()(Variable e) const { return UniquePDDLHashCombiner()(e->get_name()); }
}