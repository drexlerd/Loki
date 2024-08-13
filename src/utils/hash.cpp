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

#include "loki/details/utils/hash.hpp"

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
size_t Hasher<const ActionImpl*>::operator()(const ActionImpl* e) const
{
    return HashCombiner()(e->get_name(), get_sorted_vector(e->get_parameters()), e->get_condition(), e->get_effect());
}

size_t Hasher<const AtomImpl*>::operator()(const AtomImpl* e) const { return HashCombiner()(e->get_predicate(), e->get_terms()); }

size_t Hasher<const AxiomImpl*>::operator()(const AxiomImpl* e) const
{
    return HashCombiner()(e->get_derived_predicate_name(), get_sorted_vector(e->get_parameters()), e->get_condition());
}

size_t Hasher<const ConditionLiteralImpl&>::operator()(const ConditionLiteralImpl& e) const { return HashCombiner()(e.get_literal()); }

size_t Hasher<const ConditionAndImpl&>::operator()(const ConditionAndImpl& e) const { return HashCombiner()(get_sorted_vector(e.get_conditions())); }

size_t Hasher<const ConditionOrImpl&>::operator()(const ConditionOrImpl& e) const { return HashCombiner()(get_sorted_vector(e.get_conditions())); }

size_t Hasher<const ConditionNotImpl&>::operator()(const ConditionNotImpl& e) const { return HashCombiner()(e.get_condition()); }

size_t Hasher<const ConditionImplyImpl&>::operator()(const ConditionImplyImpl& e) const
{
    return HashCombiner()(e.get_condition_left(), e.get_condition_right());
}

size_t Hasher<const ConditionExistsImpl&>::operator()(const ConditionExistsImpl& e) const
{
    return HashCombiner()(get_sorted_vector(e.get_parameters()), e.get_condition());
}

size_t Hasher<const ConditionForallImpl&>::operator()(const ConditionForallImpl& e) const
{
    return HashCombiner()(get_sorted_vector(e.get_parameters()), e.get_condition());
}

size_t Hasher<const ConditionImpl*>::operator()(const ConditionImpl* e) const
{
    return std::visit([](const auto& arg) { return Hasher<decltype(arg)>()(arg); }, *e);
}

size_t Hasher<const DomainImpl*>::operator()(const DomainImpl* e) const
{
    return HashCombiner()(e->get_name(),
                          e->get_requirements(),
                          get_sorted_vector(e->get_types()),
                          get_sorted_vector(e->get_constants()),
                          get_sorted_vector(e->get_predicates()),
                          get_sorted_vector(e->get_functions()),
                          get_sorted_vector(e->get_actions()),
                          get_sorted_vector(e->get_axioms()));
}

size_t Hasher<const EffectLiteralImpl&>::operator()(const EffectLiteralImpl& e) const { return HashCombiner()(e.get_literal()); }

size_t Hasher<const EffectAndImpl&>::operator()(const EffectAndImpl& e) const { return HashCombiner()(get_sorted_vector(e.get_effects())); }

size_t Hasher<const EffectNumericImpl&>::operator()(const EffectNumericImpl& e) const
{
    return HashCombiner()(e.get_assign_operator(), e.get_function(), e.get_function_expression());
}

size_t Hasher<const EffectConditionalForallImpl&>::operator()(const EffectConditionalForallImpl& e) const
{
    return HashCombiner()(e.get_effect(), get_sorted_vector(e.get_parameters()));
}

size_t Hasher<const EffectConditionalWhenImpl&>::operator()(const EffectConditionalWhenImpl& e) const
{
    return HashCombiner()(e.get_condition(), e.get_effect());
}

size_t Hasher<const EffectImpl*>::operator()(const EffectImpl* e) const
{
    return std::visit([](const auto& arg) { return Hasher<decltype(arg)>()(arg); }, *e);
}

size_t Hasher<const FunctionExpressionNumberImpl&>::operator()(const FunctionExpressionNumberImpl& e) const { return HashCombiner()(e.get_number()); }

size_t Hasher<const FunctionExpressionBinaryOperatorImpl&>::operator()(const FunctionExpressionBinaryOperatorImpl& e) const
{
    return HashCombiner()(e.get_binary_operator(), e.get_left_function_expression(), e.get_right_function_expression());
}

size_t Hasher<const FunctionExpressionMultiOperatorImpl&>::operator()(const FunctionExpressionMultiOperatorImpl& e) const
{
    return HashCombiner()(e.get_multi_operator(), get_sorted_vector(e.get_function_expressions()));
}

size_t Hasher<const FunctionExpressionMinusImpl&>::operator()(const FunctionExpressionMinusImpl& e) const
{
    return HashCombiner()(e.get_function_expression());
}

size_t Hasher<const FunctionExpressionFunctionImpl&>::operator()(const FunctionExpressionFunctionImpl& e) const { return HashCombiner()(e.get_function()); }

size_t Hasher<const FunctionExpressionImpl*>::operator()(const FunctionExpressionImpl* e) const
{
    return std::visit([](const auto& arg) { return Hasher<decltype(arg)>()(arg); }, *e);
}

size_t Hasher<const FunctionSkeletonImpl*>::operator()(const FunctionSkeletonImpl* e) const
{
    return HashCombiner()(e->get_name(), e->get_type(), get_sorted_vector(e->get_parameters()));
}

size_t Hasher<const FunctionImpl*>::operator()(const FunctionImpl* e) const { return HashCombiner()(e->get_function_skeleton(), e->get_terms()); }

size_t Hasher<const LiteralImpl*>::operator()(const LiteralImpl* e) const { return HashCombiner()(e->is_negated(), e->get_atom()); }

size_t Hasher<const OptimizationMetricImpl*>::operator()(const OptimizationMetricImpl* e) const
{
    return HashCombiner()(e->get_optimization_metric(), e->get_function_expression());
}

size_t Hasher<const NumericFluentImpl*>::operator()(const NumericFluentImpl* e) const { return HashCombiner()(e->get_number(), e->get_function()); }

size_t Hasher<const ObjectImpl*>::operator()(const ObjectImpl* e) const { return HashCombiner()(e->get_name(), get_sorted_vector(e->get_bases())); }

size_t Hasher<const ParameterImpl*>::operator()(const ParameterImpl* e) const { return HashCombiner()(e->get_variable(), get_sorted_vector(e->get_bases())); }

size_t Hasher<const PredicateImpl*>::operator()(const PredicateImpl* e) const { return HashCombiner()(e->get_name(), get_sorted_vector(e->get_parameters())); }

size_t Hasher<const ProblemImpl*>::operator()(const ProblemImpl* e) const
{
    return HashCombiner()(e->get_name(),
                          e->get_domain(),
                          get_sorted_vector(e->get_objects()),
                          get_sorted_vector(e->get_derived_predicates()),
                          get_sorted_vector(e->get_initial_literals()),
                          get_sorted_vector(e->get_numeric_fluents()),
                          e->get_goal_condition(),
                          e->get_optimization_metric());
}

size_t Hasher<const RequirementsImpl*>::operator()(const RequirementsImpl* e) const { return HashCombiner()(e->get_requirements()); }

size_t Hasher<const TermObjectImpl&>::operator()(const TermObjectImpl& e) const { return HashCombiner()(e.get_object()); }

size_t Hasher<const TermVariableImpl&>::operator()(const TermVariableImpl& e) const { return HashCombiner()(e.get_variable()); }

size_t Hasher<const TermImpl*>::operator()(const TermImpl* e) const
{
    return std::visit([](const auto& arg) { return Hasher<decltype(arg)>()(arg); }, *e);
}

size_t Hasher<const TypeImpl*>::operator()(const TypeImpl* e) const { return HashCombiner()(e->get_name(), get_sorted_vector(e->get_bases())); }

size_t Hasher<const VariableImpl*>::operator()(const VariableImpl* e) const { return HashCombiner()(e->get_name()); }
}
