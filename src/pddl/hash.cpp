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
size_t UniquePDDLHasher<const ActionImpl*>::operator()(const ActionImpl* e) const
{
    return UniquePDDLHashCombiner()(e->get_name(), get_sorted_vector(e->get_parameters()), e->get_condition(), e->get_effect());
}

size_t UniquePDDLHasher<const AtomImpl*>::operator()(const AtomImpl* e) const { return UniquePDDLHashCombiner()(e->get_predicate(), e->get_terms()); }

size_t UniquePDDLHasher<const AxiomImpl*>::operator()(const AxiomImpl* e) const
{
    return UniquePDDLHashCombiner()(e->get_derived_predicate_name(), get_sorted_vector(e->get_parameters()), e->get_condition());
}

size_t UniquePDDLHasher<const ConditionLiteralImpl&>::operator()(const ConditionLiteralImpl& e) const { return UniquePDDLHashCombiner()(e.get_literal()); }

size_t UniquePDDLHasher<const ConditionAndImpl&>::operator()(const ConditionAndImpl& e) const
{
    return UniquePDDLHashCombiner()(get_sorted_vector(e.get_conditions()));
}

size_t UniquePDDLHasher<const ConditionOrImpl&>::operator()(const ConditionOrImpl& e) const
{
    return UniquePDDLHashCombiner()(get_sorted_vector(e.get_conditions()));
}

size_t UniquePDDLHasher<const ConditionNotImpl&>::operator()(const ConditionNotImpl& e) const { return UniquePDDLHashCombiner()(e.get_condition()); }

size_t UniquePDDLHasher<const ConditionImplyImpl&>::operator()(const ConditionImplyImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_condition_left(), e.get_condition_right());
}

size_t UniquePDDLHasher<const ConditionExistsImpl&>::operator()(const ConditionExistsImpl& e) const
{
    return UniquePDDLHashCombiner()(get_sorted_vector(e.get_parameters()), e.get_condition());
}

size_t UniquePDDLHasher<const ConditionForallImpl&>::operator()(const ConditionForallImpl& e) const
{
    return UniquePDDLHashCombiner()(get_sorted_vector(e.get_parameters()), e.get_condition());
}

size_t UniquePDDLHasher<const ConditionImpl*>::operator()(const ConditionImpl* e) const
{
    return std::visit([](const auto& arg) { return UniquePDDLHasher<decltype(arg)>()(arg); }, *e);
}

size_t UniquePDDLHasher<const DomainImpl*>::operator()(const DomainImpl* e) const
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

size_t UniquePDDLHasher<const EffectLiteralImpl&>::operator()(const EffectLiteralImpl& e) const { return UniquePDDLHashCombiner()(e.get_literal()); }

size_t UniquePDDLHasher<const EffectAndImpl&>::operator()(const EffectAndImpl& e) const { return UniquePDDLHashCombiner()(get_sorted_vector(e.get_effects())); }

size_t UniquePDDLHasher<const EffectNumericImpl&>::operator()(const EffectNumericImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_assign_operator(), e.get_function(), e.get_function_expression());
}

size_t UniquePDDLHasher<const EffectCompositeForallImpl&>::operator()(const EffectCompositeForallImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_effect(), get_sorted_vector(e.get_parameters()));
}

size_t UniquePDDLHasher<const EffectCompositeWhenImpl&>::operator()(const EffectCompositeWhenImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_condition(), e.get_effect());
}

size_t UniquePDDLHasher<const EffectCompositeOneofImpl&>::operator()(const EffectCompositeOneofImpl& e) const
{
    return UniquePDDLHashCombiner()(get_sorted_vector(e.get_effects()));
}

size_t UniquePDDLHasher<const EffectImpl*>::operator()(const EffectImpl* e) const
{
    return std::visit([](const auto& arg) { return UniquePDDLHasher<decltype(arg)>()(arg); }, *e);
}

size_t UniquePDDLHasher<const FunctionExpressionNumberImpl&>::operator()(const FunctionExpressionNumberImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_number());
}

size_t UniquePDDLHasher<const FunctionExpressionBinaryOperatorImpl&>::operator()(const FunctionExpressionBinaryOperatorImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_binary_operator(), e.get_left_function_expression(), e.get_right_function_expression());
}

size_t UniquePDDLHasher<const FunctionExpressionMultiOperatorImpl&>::operator()(const FunctionExpressionMultiOperatorImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_multi_operator(), get_sorted_vector(e.get_function_expressions()));
}

size_t UniquePDDLHasher<const FunctionExpressionMinusImpl&>::operator()(const FunctionExpressionMinusImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_function_expression());
}

size_t UniquePDDLHasher<const FunctionExpressionFunctionImpl&>::operator()(const FunctionExpressionFunctionImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_function());
}

size_t UniquePDDLHasher<const FunctionExpressionImpl*>::operator()(const FunctionExpressionImpl* e) const
{
    return std::visit([](const auto& arg) { return UniquePDDLHasher<decltype(arg)>()(arg); }, *e);
}

size_t UniquePDDLHasher<const FunctionSkeletonImpl*>::operator()(const FunctionSkeletonImpl* e) const
{
    return UniquePDDLHashCombiner()(e->get_name(), e->get_type(), get_sorted_vector(e->get_parameters()));
}

size_t UniquePDDLHasher<const FunctionImpl*>::operator()(const FunctionImpl* e) const
{
    return UniquePDDLHashCombiner()(e->get_function_skeleton(), e->get_terms());
}

size_t UniquePDDLHasher<const LiteralImpl*>::operator()(const LiteralImpl* e) const { return UniquePDDLHashCombiner()(e->is_negated(), e->get_atom()); }

size_t UniquePDDLHasher<const OptimizationMetricImpl*>::operator()(const OptimizationMetricImpl* e) const
{
    return UniquePDDLHashCombiner()(e->get_optimization_metric(), e->get_function_expression());
}

size_t UniquePDDLHasher<const NumericFluentImpl*>::operator()(const NumericFluentImpl* e) const
{
    return UniquePDDLHashCombiner()(e->get_number(), e->get_function());
}

size_t UniquePDDLHasher<const ObjectImpl*>::operator()(const ObjectImpl* e) const
{
    return UniquePDDLHashCombiner()(e->get_name(), get_sorted_vector(e->get_bases()));
}

size_t UniquePDDLHasher<const ParameterImpl*>::operator()(const ParameterImpl* e) const
{
    return UniquePDDLHashCombiner()(e->get_variable(), get_sorted_vector(e->get_bases()));
}

size_t UniquePDDLHasher<const PredicateImpl*>::operator()(const PredicateImpl* e) const
{
    return UniquePDDLHashCombiner()(e->get_name(), get_sorted_vector(e->get_parameters()));
}

size_t UniquePDDLHasher<const ProblemImpl*>::operator()(const ProblemImpl* e) const
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

size_t UniquePDDLHasher<const RequirementsImpl*>::operator()(const RequirementsImpl* e) const { return UniquePDDLHashCombiner()(e->get_requirements()); }

size_t UniquePDDLHasher<const TermObjectImpl&>::operator()(const TermObjectImpl& e) const { return UniquePDDLHashCombiner()(e.get_object()); }

size_t UniquePDDLHasher<const TermVariableImpl&>::operator()(const TermVariableImpl& e) const { return UniquePDDLHashCombiner()(e.get_variable()); }

size_t UniquePDDLHasher<const TermImpl*>::operator()(const TermImpl* e) const
{
    return std::visit([](const auto& arg) { return UniquePDDLHasher<decltype(arg)>()(arg); }, *e);
}

size_t UniquePDDLHasher<const TypeImpl*>::operator()(const TypeImpl* e) const
{
    return UniquePDDLHashCombiner()(e->get_name(), get_sorted_vector(e->get_bases()));
}

size_t UniquePDDLHasher<const VariableImpl*>::operator()(const VariableImpl* e) const { return UniquePDDLHashCombiner()(e->get_name()); }
}