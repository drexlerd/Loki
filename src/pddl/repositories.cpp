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

namespace loki
{

PDDLTypeToRepository create_default_pddl_type_to_repository()
{
    return boost::hana::make_map(boost::hana::make_pair(boost::hana::type_c<RequirementsImpl>, RequirementsRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<TypeImpl>, TypeRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<VariableImpl>, VariableRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<TermImpl>, TermRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<ObjectImpl>, ObjectRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<AtomImpl>, AtomRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<LiteralImpl>, LiteralRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<ParameterImpl>, ParameterRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<PredicateImpl>, PredicateRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<FunctionExpressionImpl>, FunctionExpressionRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<FunctionImpl>, FunctionRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<FunctionSkeletonImpl>, FunctionSkeletonRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<ConditionImpl>, ConditionRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<EffectImpl>, EffectRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<ActionImpl>, ActionRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<AxiomImpl>, AxiomRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<OptimizationMetricImpl>, OptimizationMetricRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<NumericFluentImpl>, NumericFluentRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<DomainImpl>, DomainRepository {}),
                                 boost::hana::make_pair(boost::hana::type_c<ProblemImpl>, ProblemRepository {}));
}

PDDLRepositories::PDDLRepositories() : m_repositories(create_default_pddl_type_to_repository()) {}

PDDLRepositories::PDDLRepositories(PDDLRepositories&& other) = default;

PDDLRepositories& PDDLRepositories::operator=(PDDLRepositories&& other) = default;

Requirements PDDLRepositories::get_or_create_requirements(RequirementEnumSet requirement_set)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<RequirementsImpl> {}).template get_or_create<RequirementsImpl>(std::move(requirement_set));
}

Type PDDLRepositories::get_or_create_type(std::string name, TypeList bases)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<TypeImpl> {}).template get_or_create<TypeImpl>(std::move(name), std::move(bases));
}

Variable PDDLRepositories::get_or_create_variable(std::string name)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<VariableImpl> {}).template get_or_create<VariableImpl>(std::move(name));
}

Term PDDLRepositories::get_or_create_term_variable(Variable variable)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<TermImpl> {}).template get_or_create<TermVariableImpl>(std::move(variable));
}

Term PDDLRepositories::get_or_create_term_object(Object object)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<TermImpl> {}).template get_or_create<TermObjectImpl>(std::move(object));
}

Object PDDLRepositories::get_or_create_object(std::string name, TypeList types)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ObjectImpl> {}).template get_or_create<ObjectImpl>(std::move(name), std::move(types));
}

Atom PDDLRepositories::get_or_create_atom(Predicate predicate, TermList terms)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<AtomImpl> {}).template get_or_create<AtomImpl>(std::move(predicate), std::move(terms));
}

Literal PDDLRepositories::get_or_create_literal(bool is_negated, Atom atom)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<LiteralImpl> {}).template get_or_create<LiteralImpl>(std::move(is_negated), std::move(atom));
}

Parameter PDDLRepositories::get_or_create_parameter(Variable variable, TypeList types)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ParameterImpl> {})
        .template get_or_create<ParameterImpl>(std::move(variable), std::move(types));
}

Predicate PDDLRepositories::get_or_create_predicate(std::string name, ParameterList parameters)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<PredicateImpl> {})
        .template get_or_create<PredicateImpl>(std::move(name), std::move(parameters));
}

FunctionExpression PDDLRepositories::get_or_create_function_expression_number(double number)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {}).template get_or_create<FunctionExpressionNumberImpl>(number);
}

FunctionExpression PDDLRepositories::get_or_create_function_expression_binary_operator(BinaryOperatorEnum binary_operator,
                                                                                       FunctionExpression left_function_expression,
                                                                                       FunctionExpression right_function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {})
        .template get_or_create<FunctionExpressionBinaryOperatorImpl>(binary_operator,
                                                                      std::move(left_function_expression),
                                                                      std::move(right_function_expression));
}

FunctionExpression PDDLRepositories::get_or_create_function_expression_multi_operator(MultiOperatorEnum multi_operator,
                                                                                      FunctionExpressionList function_expressions_)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {})
        .template get_or_create<FunctionExpressionMultiOperatorImpl>(multi_operator, std::move(function_expressions_));
}

FunctionExpression PDDLRepositories::get_or_create_function_expression_minus(FunctionExpression function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {})
        .template get_or_create<FunctionExpressionMinusImpl>(std::move(function_expression));
}

FunctionExpression PDDLRepositories::get_or_create_function_expression_function(Function function)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionExpressionImpl> {})
        .template get_or_create<FunctionExpressionFunctionImpl>(std::move(function));
}

Function PDDLRepositories::get_or_create_function(FunctionSkeleton function_skeleton, TermList terms)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionImpl> {})
        .template get_or_create<FunctionImpl>(std::move(function_skeleton), std::move(terms));
}

FunctionSkeleton PDDLRepositories::get_or_create_function_skeleton(std::string name, ParameterList parameters, Type type)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<FunctionSkeletonImpl> {})
        .template get_or_create<FunctionSkeletonImpl>(std::move(name), std::move(parameters), std::move(type));
}

Condition PDDLRepositories::get_or_create_condition_literal(Literal literal)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).template get_or_create<ConditionLiteralImpl>(std::move(literal));
}

Condition PDDLRepositories::get_or_create_condition_and(ConditionList conditions_)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).template get_or_create<ConditionAndImpl>(std::move(conditions_));
}

Condition PDDLRepositories::get_or_create_condition_or(ConditionList conditions_)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).template get_or_create<ConditionOrImpl>(std::move(conditions_));
}

Condition PDDLRepositories::get_or_create_condition_not(Condition condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {}).template get_or_create<ConditionNotImpl>(std::move(condition));
}

Condition PDDLRepositories::get_or_create_condition_imply(Condition condition_left, Condition condition_right)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {})
        .template get_or_create<ConditionImplyImpl>(std::move(condition_left), std::move(condition_right));
}

Condition PDDLRepositories::get_or_create_condition_exists(ParameterList parameters, Condition condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {})
        .template get_or_create<ConditionExistsImpl>(std::move(parameters), std::move(condition));
}

Condition PDDLRepositories::get_or_create_condition_forall(ParameterList parameters, Condition condition)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ConditionImpl> {})
        .template get_or_create<ConditionForallImpl>(std::move(parameters), std::move(condition));
}

Effect PDDLRepositories::get_or_create_effect_literal(Literal literal)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).template get_or_create<EffectLiteralImpl>(std::move(literal));
}

Effect PDDLRepositories::get_or_create_effect_and(EffectList effects_)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).template get_or_create<EffectAndImpl>(std::move(effects_));
}

Effect PDDLRepositories::get_or_create_effect_numeric(AssignOperatorEnum assign_operator, Function function, FunctionExpression function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {})
        .template get_or_create<EffectNumericImpl>(std::move(assign_operator), std::move(function), std::move(function_expression));
}

Effect PDDLRepositories::get_or_create_effect_composite_forall(ParameterList parameters, Effect effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {})
        .template get_or_create<EffectCompositeForallImpl>(std::move(parameters), std::move(effect));
}

Effect PDDLRepositories::get_or_create_effect_composite_when(Condition condition, Effect effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {})
        .template get_or_create<EffectCompositeWhenImpl>(std::move(condition), std::move(effect));
}

Effect PDDLRepositories::get_or_create_effect_composite_oneof(EffectList effects_)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<EffectImpl> {}).template get_or_create<EffectCompositeOneofImpl>(std::move(effects_));
}

Action PDDLRepositories::get_or_create_action(std::string name,
                                              size_t original_arity,
                                              ParameterList parameters,
                                              std::optional<Condition> condition,
                                              std::optional<Effect> effect)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ActionImpl> {})
        .template get_or_create<ActionImpl>(std::move(name), std::move(original_arity), std::move(parameters), std::move(condition), std::move(effect));
}

Axiom PDDLRepositories::get_or_create_axiom(std::string derived_predicate_name,
                                            ParameterList parameters,
                                            Condition condition,
                                            size_t num_parameters_to_ground_head)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<AxiomImpl> {})
        .template get_or_create<AxiomImpl>(std::move(derived_predicate_name), std::move(parameters), std::move(condition), num_parameters_to_ground_head);
}

OptimizationMetric PDDLRepositories::get_or_create_optimization_metric(OptimizationMetricEnum metric, FunctionExpression function_expression)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<OptimizationMetricImpl> {})
        .template get_or_create<OptimizationMetricImpl>(std::move(metric), std::move(function_expression));
}

NumericFluent PDDLRepositories::get_or_create_numeric_fluent(Function function, double number)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<NumericFluentImpl> {})
        .template get_or_create<NumericFluentImpl>(std::move(function), std::move(number));
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
    return boost::hana::at_key(m_repositories, boost::hana::type<DomainImpl> {})
        .template get_or_create<DomainImpl>(std::move(filepath),
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
                                                NumericFluentList numeric_fluents,
                                                std::optional<Condition> goal_condition,
                                                std::optional<OptimizationMetric> optimization_metric,
                                                AxiomList axioms)
{
    return boost::hana::at_key(m_repositories, boost::hana::type<ProblemImpl> {})
        .template get_or_create<ProblemImpl>(std::move(filepath),
                                             std::move(domain),
                                             std::move(name),
                                             std::move(requirements),
                                             std::move(objects),
                                             std::move(derived_predicates),
                                             std::move(initial_literals),
                                             std::move(numeric_fluents),
                                             std::move(goal_condition),
                                             std::move(optimization_metric),
                                             std::move(axioms));
}
}
