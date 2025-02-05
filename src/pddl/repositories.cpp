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

PDDLTypeToRepository create_default_pddl_type_to_repository()
{
    return boost::hana::make_map(
        boost::hana::make_pair(boost::hana::type_c<RequirementsImpl>, RequirementsRepository {}),
        boost::hana::make_pair(boost::hana::type_c<TypeImpl>, TypeRepository {}),
        boost::hana::make_pair(boost::hana::type_c<VariableImpl>, VariableRepository {}),
        boost::hana::make_pair(boost::hana::type_c<TermImpl>, TermRepository {}),
        boost::hana::make_pair(boost::hana::type_c<ObjectImpl>, ObjectRepository {}),
        boost::hana::make_pair(boost::hana::type_c<AtomImpl>, AtomRepository {}),
        boost::hana::make_pair(boost::hana::type_c<LiteralImpl>, LiteralRepository {}),
        boost::hana::make_pair(boost::hana::type_c<ParameterImpl>, ParameterRepository {}),
        boost::hana::make_pair(boost::hana::type_c<PredicateImpl>, PredicateRepository {}),
        boost::hana::make_pair(boost::hana::type_c<FunctionExpressionNumberImpl>, FunctionExpressionNumberRepository {}),
        boost::hana::make_pair(boost::hana::type_c<FunctionExpressionBinaryOperatorImpl>, FunctionExpressionBinaryOperatorRepository {}),
        boost::hana::make_pair(boost::hana::type_c<FunctionExpressionMultiOperatorImpl>, FunctionExpressionMultiOperatorRepository {}),
        boost::hana::make_pair(boost::hana::type_c<FunctionExpressionMinusImpl>, FunctionExpressionMinusRepository {}),
        boost::hana::make_pair(boost::hana::type_c<FunctionExpressionFunctionImpl>, FunctionExpressionFunctionRepository {}),
        boost::hana::make_pair(boost::hana::type_c<FunctionExpressionImpl>, FunctionExpressionRepository {}),
        boost::hana::make_pair(boost::hana::type_c<FunctionImpl>, FunctionRepository {}),
        boost::hana::make_pair(boost::hana::type_c<FunctionSkeletonImpl>, FunctionSkeletonRepository {}),
        boost::hana::make_pair(boost::hana::type_c<ConditionLiteralImpl>, ConditionLiteralRepository {}),
        boost::hana::make_pair(boost::hana::type_c<ConditionAndImpl>, ConditionAndRepository {}),
        boost::hana::make_pair(boost::hana::type_c<ConditionOrImpl>, ConditionOrRepository {}),
        boost::hana::make_pair(boost::hana::type_c<ConditionNotImpl>, ConditionNotRepository {}),
        boost::hana::make_pair(boost::hana::type_c<ConditionImplyImpl>, ConditionImplyRepository {}),
        boost::hana::make_pair(boost::hana::type_c<ConditionExistsImpl>, ConditionExistsRepository {}),
        boost::hana::make_pair(boost::hana::type_c<ConditionForallImpl>, ConditionForallRepository {}),
        boost::hana::make_pair(boost::hana::type_c<ConditionNumericConstraintImpl>, ConditionNumericConstraintRepository {}),
        boost::hana::make_pair(boost::hana::type_c<ConditionImpl>, ConditionRepository {}),
        boost::hana::make_pair(boost::hana::type_c<EffectLiteralImpl>, EffectLiteralRepository {}),
        boost::hana::make_pair(boost::hana::type_c<EffectAndImpl>, EffectAndRepository {}),
        boost::hana::make_pair(boost::hana::type_c<EffectNumericImpl>, EffectNumericRepository {}),
        boost::hana::make_pair(boost::hana::type_c<EffectCompositeForallImpl>, EffectCompositeForallRepository {}),
        boost::hana::make_pair(boost::hana::type_c<EffectCompositeWhenImpl>, EffectCompositeWhenRepository {}),
        boost::hana::make_pair(boost::hana::type_c<EffectCompositeOneofImpl>, EffectCompositeOneofRepository {}),
        boost::hana::make_pair(boost::hana::type_c<EffectCompositeProbabilisticImpl>, EffectCompositeProbabilisticRepository {}),
        boost::hana::make_pair(boost::hana::type_c<EffectImpl>, EffectRepository {}),
        boost::hana::make_pair(boost::hana::type_c<ActionImpl>, ActionRepository {}),
        boost::hana::make_pair(boost::hana::type_c<AxiomImpl>, AxiomRepository {}),
        boost::hana::make_pair(boost::hana::type_c<OptimizationMetricImpl>, OptimizationMetricRepository {}),
        boost::hana::make_pair(boost::hana::type_c<FunctionValueImpl>, FunctionValueRepository {}),
        boost::hana::make_pair(boost::hana::type_c<DomainImpl>, DomainRepository {}),
        boost::hana::make_pair(boost::hana::type_c<ProblemImpl>, ProblemRepository {}));
}

PDDLRepositories::PDDLRepositories() : m_repositories(create_default_pddl_type_to_repository()) {}

PDDLRepositories::PDDLRepositories(PDDLRepositories&& other) = default;

PDDLRepositories& PDDLRepositories::operator=(PDDLRepositories&& other) = default;

Requirements PDDLRepositories::get_or_create_requirements(RequirementEnumSet requirement_set)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<RequirementsImpl> {}).get_or_create(std::move(requirement_set));
}

Type PDDLRepositories::get_or_create_type(std::string name, TypeList bases)
{
    std::sort(bases.begin(), bases.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<TypeImpl> {}).get_or_create(std::move(name), std::move(bases));
}

Variable PDDLRepositories::get_or_create_variable(std::string name)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<VariableImpl> {}).get_or_create(std::move(name));
}

Term PDDLRepositories::get_or_create_term(Variable variable)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<TermImpl> {}).get_or_create(variable);
}

Term PDDLRepositories::get_or_create_term(Object object) { return boost::hana::at_key(m_repositories, boost::hana::type<TermImpl> {}).get_or_create(object); }

Object PDDLRepositories::get_or_create_object(std::string name, TypeList types)
{
    std::sort(types.begin(), types.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ObjectImpl> {}).get_or_create(std::move(name), std::move(types));
}

Atom PDDLRepositories::get_or_create_atom(Predicate predicate, TermList terms)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<AtomImpl> {}).get_or_create(std::move(predicate), std::move(terms));
}

Literal PDDLRepositories::get_or_create_literal(bool is_negated, Atom atom)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<LiteralImpl> {}).get_or_create(std::move(is_negated), std::move(atom));
}

Parameter PDDLRepositories::get_or_create_parameter(Variable variable, TypeList types)
{
    std::sort(types.begin(), types.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ParameterImpl> {}).get_or_create(std::move(variable), std::move(types));
}

Predicate PDDLRepositories::get_or_create_predicate(std::string name, ParameterList parameters)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<PredicateImpl> {}).get_or_create(std::move(name), std::move(parameters));
}

FunctionExpressionNumber PDDLRepositories::get_or_create_function_expression_number(double number)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionNumberImpl> {}).get_or_create(number);
}

FunctionExpressionBinaryOperator PDDLRepositories::get_or_create_function_expression_binary_operator(BinaryOperatorEnum binary_operator,
                                                                                                     FunctionExpression left_function_expression,
                                                                                                     FunctionExpression right_function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionBinaryOperatorImpl> {})
        .get_or_create(binary_operator, std::move(left_function_expression), std::move(right_function_expression));
}

FunctionExpressionMultiOperator PDDLRepositories::get_or_create_function_expression_multi_operator(MultiOperatorEnum multi_operator,
                                                                                                   FunctionExpressionList function_expressions_)
{
    std::sort(function_expressions_.begin(), function_expressions_.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionMultiOperatorImpl> {})
        .get_or_create(multi_operator, std::move(function_expressions_));
}

FunctionExpressionMinus PDDLRepositories::get_or_create_function_expression_minus(FunctionExpression function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionMinusImpl> {}).get_or_create(std::move(function_expression));
}

FunctionExpressionFunction PDDLRepositories::get_or_create_function_expression_function(Function function)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionFunctionImpl> {}).get_or_create(std::move(function));
}

FunctionExpression PDDLRepositories::get_or_create_function_expression(FunctionExpressionNumber fexpr_number)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr_number);
}

FunctionExpression PDDLRepositories::get_or_create_function_expression(FunctionExpressionBinaryOperator fexpr_binary_op)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr_binary_op);
}

FunctionExpression PDDLRepositories::get_or_create_function_expression(FunctionExpressionMultiOperator fexpr_multi_op)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr_multi_op);
}

FunctionExpression PDDLRepositories::get_or_create_function_expression(FunctionExpressionMinus fexpr_minus)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr_minus);
}

FunctionExpression PDDLRepositories::get_or_create_function_expression(FunctionExpressionFunction fexpr_function)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).get_or_create(fexpr_function);
}

Function PDDLRepositories::get_or_create_function(FunctionSkeleton function_skeleton, TermList terms)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionImpl> {}).get_or_create(std::move(function_skeleton), std::move(terms));
}

FunctionSkeleton PDDLRepositories::get_or_create_function_skeleton(std::string name, ParameterList parameters, Type type)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionSkeletonImpl> {})
        .get_or_create(std::move(name), std::move(parameters), std::move(type));
}

ConditionLiteral PDDLRepositories::get_or_create_condition_literal(Literal literal)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionLiteralImpl> {}).get_or_create(std::move(literal));
}

ConditionAnd PDDLRepositories::get_or_create_condition_and(ConditionList conditions_)
{
    std::sort(conditions_.begin(), conditions_.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionAndImpl> {}).get_or_create(std::move(conditions_));
}

ConditionOr PDDLRepositories::get_or_create_condition_or(ConditionList conditions_)
{
    std::sort(conditions_.begin(), conditions_.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionOrImpl> {}).get_or_create(std::move(conditions_));
}

ConditionNot PDDLRepositories::get_or_create_condition_not(Condition condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionNotImpl> {}).get_or_create(std::move(condition));
}

ConditionImply PDDLRepositories::get_or_create_condition_imply(Condition condition_left, Condition condition_right)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImplyImpl> {}).get_or_create(std::move(condition_left), std::move(condition_right));
}

ConditionExists PDDLRepositories::get_or_create_condition_exists(ParameterList parameters, Condition condition)
{
    std::sort(parameters.begin(), parameters.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionExistsImpl> {}).get_or_create(std::move(parameters), std::move(condition));
}

ConditionForall PDDLRepositories::get_or_create_condition_forall(ParameterList parameters, Condition condition)
{
    std::sort(parameters.begin(), parameters.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionForallImpl> {}).get_or_create(std::move(parameters), std::move(condition));
}

ConditionNumericConstraint PDDLRepositories::get_or_create_condition_numeric_constraint(BinaryComparatorEnum binary_comparator,
                                                                                        FunctionExpression function_expression_left,
                                                                                        FunctionExpression function_expression_right)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionNumericConstraintImpl> {})
        .get_or_create(std::move(binary_comparator), std::move(function_expression_left), std::move(function_expression_right));
}

Condition PDDLRepositories::get_or_create_condition(ConditionLiteral condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}

Condition PDDLRepositories::get_or_create_condition(ConditionAnd condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}

Condition PDDLRepositories::get_or_create_condition(ConditionOr condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}

Condition PDDLRepositories::get_or_create_condition(ConditionNot condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}

Condition PDDLRepositories::get_or_create_condition(ConditionImply condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}

Condition PDDLRepositories::get_or_create_condition(ConditionExists condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}

Condition PDDLRepositories::get_or_create_condition(ConditionForall condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}

Condition PDDLRepositories::get_or_create_condition(ConditionNumericConstraint condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).get_or_create(condition);
}

EffectLiteral PDDLRepositories::get_or_create_effect_literal(Literal literal)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectLiteralImpl> {}).get_or_create(std::move(literal));
}

EffectAnd PDDLRepositories::get_or_create_effect_and(EffectList effects_)
{
    std::sort(effects_.begin(), effects_.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<EffectAndImpl> {}).get_or_create(std::move(effects_));
}

EffectNumeric PDDLRepositories::get_or_create_effect_numeric(AssignOperatorEnum assign_operator, Function function, FunctionExpression function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectNumericImpl> {})
        .get_or_create(std::move(assign_operator), std::move(function), std::move(function_expression));
}

EffectCompositeForall PDDLRepositories::get_or_create_effect_composite_forall(ParameterList parameters, Effect effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectCompositeForallImpl> {}).get_or_create(std::move(parameters), std::move(effect));
}

EffectCompositeWhen PDDLRepositories::get_or_create_effect_composite_when(Condition condition, Effect effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectCompositeWhenImpl> {}).get_or_create(std::move(condition), std::move(effect));
}

EffectCompositeOneof PDDLRepositories::get_or_create_effect_composite_oneof(EffectList effects_)
{
    std::sort(effects_.begin(), effects_.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<EffectCompositeOneofImpl> {}).get_or_create(std::move(effects_));
}

EffectCompositeProbabilistic PDDLRepositories::get_or_create_effect_composite_probabilistic(EffectDistribution effects_)
{
    std::sort(effects_.begin(), effects_.end(), [](const auto& lhs, const auto& rhs) { return lhs.second->get_index() < rhs.second->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<EffectCompositeProbabilisticImpl> {}).get_or_create(std::move(effects_));
}

Effect PDDLRepositories::get_or_create_effect(EffectLiteral effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}

Effect PDDLRepositories::get_or_create_effect(EffectAnd effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}

Effect PDDLRepositories::get_or_create_effect(EffectNumeric effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}

Effect PDDLRepositories::get_or_create_effect(EffectCompositeForall effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}

Effect PDDLRepositories::get_or_create_effect(EffectCompositeWhen effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}

Effect PDDLRepositories::get_or_create_effect(EffectCompositeOneof effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}

Effect PDDLRepositories::get_or_create_effect(EffectCompositeProbabilistic effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).get_or_create(effect);
}

Action PDDLRepositories::get_or_create_action(std::string name,
                                              size_t original_arity,
                                              ParameterList parameters,
                                              std::optional<Condition> condition,
                                              std::optional<Effect> effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ActionImpl> {})
        .get_or_create(std::move(name), std::move(original_arity), std::move(parameters), std::move(condition), std::move(effect));
}

Axiom PDDLRepositories::get_or_create_axiom(ParameterList parameters, Predicate derived_predicate, Literal subtyped_literal, Condition condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<AxiomImpl> {})
        .get_or_create(std::move(parameters), derived_predicate, subtyped_literal, condition);
}

OptimizationMetric PDDLRepositories::get_or_create_optimization_metric(OptimizationMetricEnum metric, FunctionExpression function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<OptimizationMetricImpl> {}).get_or_create(std::move(metric), std::move(function_expression));
}

FunctionValue PDDLRepositories::get_or_create_function_value(Function function, double number)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionValueImpl> {}).get_or_create(std::move(function), std::move(number));
}

Domain PDDLRepositories::get_or_create_domain(std::optional<fs::path> filepath,
                                              std::string name,
                                              Requirements requirements,
                                              TypeList types,
                                              ObjectList constants,
                                              PredicateList predicates,
                                              FunctionSkeletonList functions,
                                              ActionList actions,
                                              AxiomList axioms)
{
    std::sort(types.begin(), types.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });
    std::sort(constants.begin(), constants.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });
    std::sort(predicates.begin(), predicates.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });
    std::sort(functions.begin(), functions.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });
    std::sort(actions.begin(), actions.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });
    std::sort(axioms.begin(), axioms.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<DomainImpl> {})
        .get_or_create(std::move(filepath),
                       std::move(name),
                       std::move(requirements),
                       std::move(types),
                       std::move(constants),
                       std::move(predicates),
                       std::move(functions),
                       std::move(actions),
                       std::move(axioms));
}

Problem PDDLRepositories::get_or_create_problem(std::optional<fs::path> filepath,
                                                Domain domain,
                                                std::string name,
                                                Requirements requirements,
                                                ObjectList objects,
                                                PredicateList derived_predicates,
                                                LiteralList initial_literals,
                                                FunctionValueList function_values,
                                                std::optional<Condition> goal_condition,
                                                std::optional<OptimizationMetric> optimization_metric,
                                                AxiomList axioms)
{
    std::sort(objects.begin(), objects.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });
    std::sort(derived_predicates.begin(), derived_predicates.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });
    std::sort(initial_literals.begin(), initial_literals.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });
    std::sort(function_values.begin(), function_values.end(), [](const auto& lhs, const auto& rhs) { return lhs->get_index() < rhs->get_index(); });

    return boost::hana::at_key(m_repositories, boost::hana::type<ProblemImpl> {})
        .get_or_create(std::move(filepath),
                       std::move(domain),
                       std::move(name),
                       std::move(requirements),
                       std::move(objects),
                       std::move(derived_predicates),
                       std::move(initial_literals),
                       std::move(function_values),
                       std::move(goal_condition),
                       std::move(optimization_metric),
                       std::move(axioms));
}
}
