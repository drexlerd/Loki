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

#include "loki/details/pddl/repositories.hpp"

#include <algorithm>

namespace loki
{

HanaRepositories& Repositories::get_hana_repositories() { return m_repositories; }
const HanaRepositories& Repositories::get_hana_repositories() const { return m_repositories; }

Requirements Repositories::get_or_create_requirements(RequirementEnumSet requirement_set)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<RequirementsImpl> {}).get_or_create(std::move(requirement_set));
}

Type Repositories::get_or_create_type(std::string name, TypeList bases)
{
    std::sort(bases.begin(), bases.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_name() < rhs->get_name(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<TypeImpl> {}).get_or_create(std::move(name), std::move(bases));
}

Variable Repositories::get_or_create_variable(std::string name)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<VariableImpl> {}).get_or_create(std::move(name));
}

Object Repositories::get_or_create_object(std::string name, TypeList types)
{
    std::sort(types.begin(), types.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_name() < rhs->get_name(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ObjectImpl> {}).get_or_create(std::move(name), std::move(types));
}

Term Repositories::get_or_create_term(Variable variable) { return boost::hana::at_key(m_repositories, boost::hana::type<TermImpl> {}).get_or_create(variable); }
Term Repositories::get_or_create_term(Object object) { return boost::hana::at_key(m_repositories, boost::hana::type<TermImpl> {}).get_or_create(object); }

Parameter Repositories::get_or_create_parameter(Variable variable, TypeList types)
{
    std::sort(types.begin(), types.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_name() < rhs->get_name(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ParameterImpl> {}).get_or_create(std::move(variable), std::move(types));
}

Predicate Repositories::get_or_create_predicate(std::string name, ParameterList parameters)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<PredicateImpl> {}).get_or_create(std::move(name), std::move(parameters));
}

Atom Repositories::get_or_create_atom(Predicate predicate, TermList terms)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<AtomImpl> {}).get_or_create(std::move(predicate), std::move(terms));
}

Literal Repositories::get_or_create_literal(bool polarity, Atom atom)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<LiteralImpl> {}).get_or_create(std::move(polarity), std::move(atom));
}

FunctionExpressionNumber Repositories::get_or_create_function_expression_number(double number)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionNumberImpl> {}).get_or_create(number);
}
FunctionExpressionBinaryOperator Repositories::get_or_create_function_expression_binary_operator(BinaryOperatorEnum binary_operator,
                                                                                                 FunctionExpression left_function_expression,
                                                                                                 FunctionExpression right_function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionBinaryOperatorImpl> {})
        .get_or_create(binary_operator, std::move(left_function_expression), std::move(right_function_expression));
}
FunctionExpressionMultiOperator Repositories::get_or_create_function_expression_multi_operator(MultiOperatorEnum multi_operator,
                                                                                               FunctionExpressionList function_expressions)
{
    // TODO: do we need a canonical sort here? I guess duplicates do not hurt here
    std::sort(function_expressions.begin(), function_expressions.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionMultiOperatorImpl> {})
        .get_or_create(multi_operator, std::move(function_expressions));
}
FunctionExpressionMinus Repositories::get_or_create_function_expression_minus(FunctionExpression function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionMinusImpl> {}).get_or_create(std::move(function_expression));
}
FunctionExpressionFunction Repositories::get_or_create_function_expression_function(Function function)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionFunctionImpl> {}).get_or_create(std::move(function));
}
FunctionExpression Repositories::get_or_create_function_expression(FunctionExpressionNumber fexpr)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr);
}
FunctionExpression Repositories::get_or_create_function_expression(FunctionExpressionBinaryOperator fexpr)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr);
}
FunctionExpression Repositories::get_or_create_function_expression(FunctionExpressionMultiOperator fexpr)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr);
}
FunctionExpression Repositories::get_or_create_function_expression(FunctionExpressionMinus fexpr)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr);
}
FunctionExpression Repositories::get_or_create_function_expression(FunctionExpressionFunction fexpr)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr);
}

Function Repositories::get_or_create_function(FunctionSkeleton function_skeleton, TermList terms)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionImpl> {}).get_or_create(std::move(function_skeleton), std::move(terms));
}

FunctionValue Repositories::get_or_create_function_value(Function function, double number)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionValueImpl> {}).get_or_create(std::move(function), std::move(number));
}

FunctionSkeleton Repositories::get_or_create_function_skeleton(std::string name, ParameterList parameters, Type type)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionSkeletonImpl> {})
        .get_or_create(std::move(name), std::move(parameters), std::move(type));
}

ConditionLiteral Repositories::get_or_create_condition_literal(Literal literal)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionLiteralImpl> {}).get_or_create(std::move(literal));
}
ConditionAnd Repositories::get_or_create_condition_and(ConditionList conditions)
{
    // TODO: do we need a canonical sort here? I guess duplicates do not hurt here
    std::sort(conditions.begin(), conditions.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionAndImpl> {}).get_or_create(std::move(conditions));
}
ConditionOr Repositories::get_or_create_condition_or(ConditionList conditions)
{
    // TODO: do we need a canonical sort here? I guess duplicates do not hurt here
    std::sort(conditions.begin(), conditions.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionOrImpl> {}).get_or_create(std::move(conditions));
}
ConditionNot Repositories::get_or_create_condition_not(Condition condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionNotImpl> {}).get_or_create(std::move(condition));
}
ConditionImply Repositories::get_or_create_condition_imply(Condition left_condition, Condition right_condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImplyImpl> {}).get_or_create(std::move(left_condition), std::move(right_condition));
}
ConditionExists Repositories::get_or_create_condition_exists(ParameterList parameters, Condition condition)
{
    // TODO: do we need a canonical sort here? I guess duplicates do not hurt here
    std::sort(parameters.begin(), parameters.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionExistsImpl> {}).get_or_create(std::move(parameters), std::move(condition));
}

ConditionForall Repositories::get_or_create_condition_forall(ParameterList parameters, Condition condition)
{
    // TODO: do we need a canonical sort here? I guess duplicates do not hurt here
    std::sort(parameters.begin(), parameters.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionForallImpl> {}).get_or_create(std::move(parameters), std::move(condition));
}
ConditionNumericConstraint Repositories::get_or_create_condition_numeric_constraint(BinaryComparatorEnum binary_comparator,
                                                                                    FunctionExpression left_function_expression,
                                                                                    FunctionExpression right_function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionNumericConstraintImpl> {})
        .get_or_create(std::move(binary_comparator), std::move(left_function_expression), std::move(right_function_expression));
}
Condition Repositories::get_or_create_condition(ConditionLiteral condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition Repositories::get_or_create_condition(ConditionAnd condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition Repositories::get_or_create_condition(ConditionOr condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition Repositories::get_or_create_condition(ConditionNot condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition Repositories::get_or_create_condition(ConditionImply condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition Repositories::get_or_create_condition(ConditionExists condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition Repositories::get_or_create_condition(ConditionForall condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}
Condition Repositories::get_or_create_condition(ConditionNumericConstraint condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}

EffectLiteral Repositories::get_or_create_effect_literal(Literal literal)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectLiteralImpl> {}).get_or_create(std::move(literal));
}
EffectAnd Repositories::get_or_create_effect_and(EffectList effects)
{
    // TODO: do we need a canonical sort here? I guess duplicates do not hurt here
    std::sort(effects.begin(), effects.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<EffectAndImpl> {}).get_or_create(std::move(effects));
}
EffectNumeric Repositories::get_or_create_effect_numeric(AssignOperatorEnum assign_operator, Function function, FunctionExpression function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectNumericImpl> {})
        .get_or_create(std::move(assign_operator), std::move(function), std::move(function_expression));
}
EffectCompositeForall Repositories::get_or_create_effect_composite_forall(ParameterList parameters, Effect effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectCompositeForallImpl> {}).get_or_create(std::move(parameters), std::move(effect));
}
EffectCompositeWhen Repositories::get_or_create_effect_composite_when(Condition condition, Effect effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectCompositeWhenImpl> {}).get_or_create(std::move(condition), std::move(effect));
}
EffectCompositeOneof Repositories::get_or_create_effect_composite_oneof(EffectList effects)
{
    // TODO: do we need a canonical sort here? I guess duplicates do not hurt here
    std::sort(effects.begin(), effects.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<EffectCompositeOneofImpl> {}).get_or_create(std::move(effects));
}
EffectCompositeProbabilistic Repositories::get_or_create_effect_composite_probabilistic(EffectDistribution effects)
{
    // TODO: do we need a canonical sort here? I guess duplicates do not hurt here
    std::sort(effects.begin(), effects.end(), [](const auto& lhs, const auto& rhs) { return lhs.second->get_index() < rhs.second->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<EffectCompositeProbabilisticImpl> {}).get_or_create(std::move(effects));
}
Effect Repositories::get_or_create_effect(EffectLiteral effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}
Effect Repositories::get_or_create_effect(EffectAnd effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}
Effect Repositories::get_or_create_effect(EffectNumeric effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}
Effect Repositories::get_or_create_effect(EffectCompositeForall effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}
Effect Repositories::get_or_create_effect(EffectCompositeWhen effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}
Effect Repositories::get_or_create_effect(EffectCompositeOneof effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}
Effect Repositories::get_or_create_effect(EffectCompositeProbabilistic effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}

Action Repositories::get_or_create_action(std::string name,
                                          size_t original_arity,
                                          ParameterList parameters,
                                          std::optional<Condition> condition,
                                          std::optional<Effect> effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ActionImpl> {})
        .get_or_create(std::move(name), std::move(original_arity), std::move(parameters), std::move(condition), std::move(effect));
}

Axiom Repositories::get_or_create_axiom(ParameterList parameters, Literal subtyped_literal, Condition condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<AxiomImpl> {}).get_or_create(std::move(parameters), subtyped_literal, condition);
}

OptimizationMetric Repositories::get_or_create_optimization_metric(OptimizationMetricEnum optimization_metric, FunctionExpression function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<OptimizationMetricImpl> {})
        .get_or_create(std::move(optimization_metric), std::move(function_expression));
}
}
