/*
 * Copyright (C) 2023 Dominik Drexler
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

#ifndef LOKI_INCLUDE_LOKI_PDDL_FACTORIES_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_FACTORIES_HPP_

#include "loki/details/pddl/action.hpp"
#include "loki/details/pddl/atom.hpp"
#include "loki/details/pddl/axiom.hpp"
#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/domain.hpp"
#include "loki/details/pddl/effects.hpp"
#include "loki/details/pddl/factory.hpp"
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

namespace loki
{
// The segmented sizes are chosen sufficiently large to avoid
// to avoid allocations and for continuous storage.
// The values are just educated guesses based on the knowledge
// that cache line size is 64 Bytes.
using RequirementFactory = PDDLFactory<RequirementsImpl>;
using TypeFactory = PDDLFactory<TypeImpl>;
using VariableFactory = PDDLFactory<VariableImpl>;
using TermFactory = PDDLFactory<TermImpl>;
using ObjectFactory = PDDLFactory<ObjectImpl>;
using AtomFactory = PDDLFactory<AtomImpl>;
using LiteralFactory = PDDLFactory<LiteralImpl>;
using ParameterFactory = PDDLFactory<ParameterImpl>;
using PredicateFactory = PDDLFactory<PredicateImpl>;
using FunctionExpressionFactory = PDDLFactory<FunctionExpressionImpl>;
using FunctionFactory = PDDLFactory<FunctionImpl>;
using FunctionSkeletonFactory = PDDLFactory<FunctionSkeletonImpl>;
using ConditionFactory = PDDLFactory<ConditionImpl>;
using EffectFactory = PDDLFactory<EffectImpl>;
using ActionFactory = PDDLFactory<ActionImpl>;
using AxiomFactory = PDDLFactory<AxiomImpl>;
using OptimizationMetricFactory = PDDLFactory<OptimizationMetricImpl>;
using NumericFluentFactory = PDDLFactory<NumericFluentImpl>;
using DomainFactory = PDDLFactory<DomainImpl>;
using ProblemFactory = PDDLFactory<ProblemImpl>;

using PDDLPositionCache = PositionCache<RequirementsImpl,
                                        TypeImpl,
                                        VariableImpl,
                                        TermImpl,
                                        ObjectImpl,
                                        AtomImpl,
                                        LiteralImpl,
                                        ParameterImpl,
                                        PredicateImpl,
                                        FunctionExpressionImpl,
                                        FunctionImpl,
                                        FunctionSkeletonImpl,
                                        ConditionImpl,
                                        EffectImpl,
                                        ActionImpl,
                                        AxiomImpl,
                                        OptimizationMetricImpl,
                                        NumericFluentImpl,
                                        DomainImpl,
                                        ProblemImpl>;

/// @brief Collection of factories for the unique creation of PDDL objects.
class PDDLFactories
{
private:
    RequirementFactory requirements;
    TypeFactory types;
    VariableFactory variables;
    TermFactory terms;
    ObjectFactory objects;
    AtomFactory atoms;
    AtomFactory derived_atoms;
    LiteralFactory literals;
    ParameterFactory parameters;
    PredicateFactory predicates;
    FunctionExpressionFactory function_expressions;
    FunctionFactory functions;
    FunctionSkeletonFactory function_skeletons;
    ConditionFactory conditions;
    EffectFactory effects;
    ActionFactory actions;
    AxiomFactory axioms;
    OptimizationMetricFactory optimization_metrics;
    NumericFluentFactory numeric_fluents;
    DomainFactory domains;
    ProblemFactory problems;

public:
    PDDLFactories() :
        requirements(RequirementFactory(100)),
        types(TypeFactory(1000)),
        variables(VariableFactory(1000)),
        terms(TermFactory(1000)),
        objects(ObjectFactory(1000)),
        atoms(AtomFactory(1000)),
        derived_atoms(AtomFactory(1000)),
        literals(LiteralFactory(1000)),
        parameters(ParameterFactory(1000)),
        predicates(PredicateFactory(1000)),
        function_expressions(FunctionExpressionFactory(1000)),
        functions(FunctionFactory(1000)),
        function_skeletons(FunctionSkeletonFactory(1000)),
        conditions(ConditionFactory(1000)),
        effects(EffectFactory(1000)),
        actions(ActionFactory(100)),
        axioms(AxiomFactory(100)),
        optimization_metrics(OptimizationMetricFactory(10)),
        numeric_fluents(NumericFluentFactory(1000)),
        domains(DomainFactory(1)),
        problems(ProblemFactory(10))
    {
    }
    PDDLFactories(const PDDLFactories& other) = delete;
    PDDLFactories& operator=(const PDDLFactories& other) = delete;
    PDDLFactories(PDDLFactories&& other) = default;
    PDDLFactories& operator=(PDDLFactories&& other) = default;

    Requirements get_or_create_requirements(RequirementEnumSet requirement_set)
    {
        return requirements.get_or_create<RequirementsImpl>(std::move(requirement_set));
    }

    Type get_or_create_type(std::string name, TypeList bases) { return types.get_or_create<TypeImpl>(std::move(name), std::move(bases)); }

    Variable get_or_create_variable(std::string name) { return variables.get_or_create<VariableImpl>(std::move(name)); }

    Term get_or_create_term_variable(Variable variable) { return terms.get_or_create<TermVariableImpl>(std::move(variable)); }

    Term get_or_create_term_object(Object object) { return terms.get_or_create<TermObjectImpl>(std::move(object)); }

    Object get_or_create_object(std::string name, TypeList types) { return objects.get_or_create<ObjectImpl>(std::move(name), std::move(types)); }

    Atom get_or_create_atom(Predicate predicate, TermList terms) { return atoms.get_or_create<AtomImpl>(std::move(predicate), std::move(terms)); }

    Literal get_or_create_literal(bool is_negated, Atom atom) { return literals.get_or_create<LiteralImpl>(std::move(is_negated), std::move(atom)); }

    Parameter get_or_create_parameter(Variable variable, TypeList types)
    {
        return parameters.get_or_create<ParameterImpl>(std::move(variable), std::move(types));
    }

    Predicate get_or_create_predicate(std::string name, ParameterList parameters)
    {
        return predicates.get_or_create<PredicateImpl>(std::move(name), std::move(parameters));
    }

    FunctionExpression get_or_create_function_expression_number(double number)
    {
        return function_expressions.get_or_create<FunctionExpressionNumberImpl>(number);
    }

    FunctionExpression get_or_create_function_expression_binary_operator(BinaryOperatorEnum binary_operator,
                                                                         FunctionExpression left_function_expression,
                                                                         FunctionExpression right_function_expression)
    {
        return function_expressions.get_or_create<FunctionExpressionBinaryOperatorImpl>(binary_operator,
                                                                                        std::move(left_function_expression),
                                                                                        std::move(right_function_expression));
    }

    FunctionExpression get_or_create_function_expression_multi_operator(MultiOperatorEnum multi_operator, FunctionExpressionList function_expressions_)
    {
        return function_expressions.get_or_create<FunctionExpressionMultiOperatorImpl>(multi_operator, std::move(function_expressions_));
    }

    FunctionExpression get_or_create_function_expression_minus(FunctionExpression function_expression)
    {
        return function_expressions.get_or_create<FunctionExpressionMinusImpl>(std::move(function_expression));
    }

    FunctionExpression get_or_create_function_expression_function(Function function)
    {
        return function_expressions.get_or_create<FunctionExpressionFunctionImpl>(std::move(function));
    }

    Function get_or_create_function(FunctionSkeleton function_skeleton, TermList terms)
    {
        return functions.get_or_create<FunctionImpl>(std::move(function_skeleton), std::move(terms));
    }

    FunctionSkeleton get_or_create_function_skeleton(std::string name, ParameterList parameters, Type type)
    {
        return function_skeletons.get_or_create<FunctionSkeletonImpl>(std::move(name), std::move(parameters), std::move(type));
    }

    Condition get_or_create_condition_literal(Literal literal) { return conditions.get_or_create<ConditionLiteralImpl>(std::move(literal)); }

    Condition get_or_create_condition_and(ConditionList conditions_) { return conditions.get_or_create<ConditionAndImpl>(std::move(conditions_)); }

    Condition get_or_create_condition_or(ConditionList conditions_) { return conditions.get_or_create<ConditionOrImpl>(std::move(conditions_)); }

    Condition get_or_create_condition_not(Condition condition) { return conditions.get_or_create<ConditionNotImpl>(std::move(condition)); }

    Condition get_or_create_condition_imply(Condition condition_left, Condition condition_right)
    {
        return conditions.get_or_create<ConditionImplyImpl>(std::move(condition_left), std::move(condition_right));
    }

    Condition get_or_create_condition_exists(ParameterList parameters, Condition condition)
    {
        return conditions.get_or_create<ConditionExistsImpl>(std::move(parameters), std::move(condition));
    }

    Condition get_or_create_condition_forall(ParameterList parameters, Condition condition)
    {
        return conditions.get_or_create<ConditionForallImpl>(std::move(parameters), std::move(condition));
    }

    Effect get_or_create_effect_literal(Literal literal) { return effects.get_or_create<EffectLiteralImpl>(std::move(literal)); }

    Effect get_or_create_effect_and(EffectList effects_) { return effects.get_or_create<EffectAndImpl>(std::move(effects_)); }

    Effect get_or_create_effect_numeric(AssignOperatorEnum assign_operator, Function function, FunctionExpression function_expression)
    {
        return effects.get_or_create<EffectNumericImpl>(std::move(assign_operator), std::move(function), std::move(function_expression));
    }

    Effect get_or_create_effect_conditional_forall(ParameterList parameters, Effect effect)
    {
        return effects.get_or_create<EffectConditionalForallImpl>(std::move(parameters), std::move(effect));
    }

    Effect get_or_create_effect_conditional_when(Condition condition, Effect effect)
    {
        return effects.get_or_create<EffectConditionalWhenImpl>(std::move(condition), std::move(effect));
    }

    Action
    get_or_create_action(std::string name, size_t original_arity, ParameterList parameters, std::optional<Condition> condition, std::optional<Effect> effect)
    {
        return actions.get_or_create<ActionImpl>(std::move(name), std::move(original_arity), std::move(parameters), std::move(condition), std::move(effect));
    }

    Axiom get_or_create_axiom(std::string derived_predicate_name, ParameterList parameters, Condition condition, size_t num_parameters_to_ground_head)
    {
        return axioms.get_or_create<AxiomImpl>(std::move(derived_predicate_name), std::move(parameters), std::move(condition), num_parameters_to_ground_head);
    }

    OptimizationMetric get_or_create_optimization_metric(OptimizationMetricEnum metric, FunctionExpression function_expression)
    {
        return optimization_metrics.get_or_create<OptimizationMetricImpl>(std::move(metric), std::move(function_expression));
    }

    NumericFluent get_or_create_numeric_fluent(Function function, double number)
    {
        return numeric_fluents.get_or_create<NumericFluentImpl>(std::move(function), std::move(number));
    }

    Domain get_or_create_domain(std::string name,
                                Requirements requirements,
                                TypeList types,
                                ObjectList constants,
                                PredicateList predicates,
                                FunctionSkeletonList functions,
                                ActionList actions,
                                AxiomList axioms)
    {
        return domains.get_or_create<DomainImpl>(std::move(name),
                                                 std::move(requirements),
                                                 std::move(types),
                                                 std::move(constants),
                                                 std::move(predicates),
                                                 std::move(functions),
                                                 std::move(actions),
                                                 std::move(axioms));
    }

    Problem get_or_create_problem(Domain domain,
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
        return problems.get_or_create<ProblemImpl>(std::move(domain),
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
};

}

#endif