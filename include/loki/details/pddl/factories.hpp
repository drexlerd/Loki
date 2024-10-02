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
#include "loki/details/pddl/equal_to.hpp"
#include "loki/details/pddl/function.hpp"
#include "loki/details/pddl/function_expressions.hpp"
#include "loki/details/pddl/function_skeleton.hpp"
#include "loki/details/pddl/hash.hpp"
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
#include "loki/details/utils/unique_factory.hpp"
#include "loki/details/utils/variadic_container.hpp"

namespace loki
{

using RequirementsFactory = UniqueFactory<RequirementsImpl, UniquePDDLHasher<const RequirementsImpl*>, UniquePDDLEqualTo<const RequirementsImpl*>>;
using TypeFactory = UniqueFactory<TypeImpl, UniquePDDLHasher<const TypeImpl*>, UniquePDDLEqualTo<const TypeImpl*>>;
using VariableFactory = UniqueFactory<VariableImpl, UniquePDDLHasher<const VariableImpl*>, UniquePDDLEqualTo<const VariableImpl*>>;
using TermFactory = UniqueFactory<TermImpl, UniquePDDLHasher<const TermImpl*>, UniquePDDLEqualTo<const TermImpl*>>;
using ObjectFactory = UniqueFactory<ObjectImpl, UniquePDDLHasher<const ObjectImpl*>, UniquePDDLEqualTo<const ObjectImpl*>>;
using AtomFactory = UniqueFactory<AtomImpl, UniquePDDLHasher<const AtomImpl*>, UniquePDDLEqualTo<const AtomImpl*>>;
using LiteralFactory = UniqueFactory<LiteralImpl, UniquePDDLHasher<const LiteralImpl*>, UniquePDDLEqualTo<const LiteralImpl*>>;
using ParameterFactory = UniqueFactory<ParameterImpl, UniquePDDLHasher<const ParameterImpl*>, UniquePDDLEqualTo<const ParameterImpl*>>;
using PredicateFactory = UniqueFactory<PredicateImpl, UniquePDDLHasher<const PredicateImpl*>, UniquePDDLEqualTo<const PredicateImpl*>>;
using FunctionExpressionFactory =
    UniqueFactory<FunctionExpressionImpl, UniquePDDLHasher<const FunctionExpressionImpl*>, UniquePDDLEqualTo<const FunctionExpressionImpl*>>;
using FunctionFactory = UniqueFactory<FunctionImpl, UniquePDDLHasher<const FunctionImpl*>, UniquePDDLEqualTo<const FunctionImpl*>>;
using FunctionSkeletonFactory =
    UniqueFactory<FunctionSkeletonImpl, UniquePDDLHasher<const FunctionSkeletonImpl*>, UniquePDDLEqualTo<const FunctionSkeletonImpl*>>;
using ConditionFactory = UniqueFactory<ConditionImpl, UniquePDDLHasher<const ConditionImpl*>, UniquePDDLEqualTo<const ConditionImpl*>>;
using EffectFactory = UniqueFactory<EffectImpl, UniquePDDLHasher<const EffectImpl*>, UniquePDDLEqualTo<const EffectImpl*>>;
using ActionFactory = UniqueFactory<ActionImpl, UniquePDDLHasher<const ActionImpl*>, UniquePDDLEqualTo<const ActionImpl*>>;
using AxiomFactory = UniqueFactory<AxiomImpl, UniquePDDLHasher<const AxiomImpl*>, UniquePDDLEqualTo<const AxiomImpl*>>;
using OptimizationMetricFactory =
    UniqueFactory<OptimizationMetricImpl, UniquePDDLHasher<const OptimizationMetricImpl*>, UniquePDDLEqualTo<const OptimizationMetricImpl*>>;
using NumericFluentFactory = UniqueFactory<NumericFluentImpl, UniquePDDLHasher<const NumericFluentImpl*>, UniquePDDLEqualTo<const NumericFluentImpl*>>;
using DomainFactory = UniqueFactory<DomainImpl, UniquePDDLHasher<const DomainImpl*>, UniquePDDLEqualTo<const DomainImpl*>>;
using ProblemFactory = UniqueFactory<ProblemImpl, UniquePDDLHasher<const ProblemImpl*>, UniquePDDLEqualTo<const ProblemImpl*>>;

using VariadicPDDLConstructorFactory = VariadicContainer<RequirementsFactory,
                                                         TypeFactory,
                                                         VariableFactory,
                                                         TermFactory,
                                                         ObjectFactory,
                                                         AtomFactory,
                                                         LiteralFactory,
                                                         ParameterFactory,
                                                         PredicateFactory,
                                                         FunctionExpressionFactory,
                                                         FunctionFactory,
                                                         FunctionSkeletonFactory,
                                                         ConditionFactory,
                                                         EffectFactory,
                                                         ActionFactory,
                                                         AxiomFactory,
                                                         OptimizationMetricFactory,
                                                         NumericFluentFactory,
                                                         DomainFactory,
                                                         ProblemFactory>;

/// @brief Collection of factories for the unique creation of PDDL objects.
class PDDLFactories
{
private:
    VariadicPDDLConstructorFactory m_factories;

public:
    PDDLFactories();
    PDDLFactories(const PDDLFactories& other) = delete;
    PDDLFactories& operator=(const PDDLFactories& other) = delete;
    PDDLFactories(PDDLFactories&& other);
    PDDLFactories& operator=(PDDLFactories&& other);

    Requirements get_or_create_requirements(RequirementEnumSet requirement_set);

    Type get_or_create_type(std::string name, TypeList bases);

    Variable get_or_create_variable(std::string name);

    Term get_or_create_term_variable(Variable variable);

    Term get_or_create_term_object(Object object);

    Object get_or_create_object(std::string name, TypeList types);

    Atom get_or_create_atom(Predicate predicate, TermList terms);

    Literal get_or_create_literal(bool is_negated, Atom atom);

    Parameter get_or_create_parameter(Variable variable, TypeList types);

    Predicate get_or_create_predicate(std::string name, ParameterList parameters);

    FunctionExpression get_or_create_function_expression_number(double number);

    FunctionExpression get_or_create_function_expression_binary_operator(BinaryOperatorEnum binary_operator,
                                                                         FunctionExpression left_function_expression,
                                                                         FunctionExpression right_function_expression);

    FunctionExpression get_or_create_function_expression_multi_operator(MultiOperatorEnum multi_operator, FunctionExpressionList function_expressions_);

    FunctionExpression get_or_create_function_expression_minus(FunctionExpression function_expression);

    FunctionExpression get_or_create_function_expression_function(Function function);

    Function get_or_create_function(FunctionSkeleton function_skeleton, TermList terms);

    FunctionSkeleton get_or_create_function_skeleton(std::string name, ParameterList parameters, Type type);

    Condition get_or_create_condition_literal(Literal literal);

    Condition get_or_create_condition_and(ConditionList conditions_);

    Condition get_or_create_condition_or(ConditionList conditions_);

    Condition get_or_create_condition_not(Condition condition);

    Condition get_or_create_condition_imply(Condition condition_left, Condition condition_right);

    Condition get_or_create_condition_exists(ParameterList parameters, Condition condition);

    Condition get_or_create_condition_forall(ParameterList parameters, Condition condition);

    Effect get_or_create_effect_literal(Literal literal);

    Effect get_or_create_effect_and(EffectList effects);

    Effect get_or_create_effect_numeric(AssignOperatorEnum assign_operator, Function function, FunctionExpression function_expression);

    Effect get_or_create_effect_composite_forall(ParameterList parameters, Effect effect);

    Effect get_or_create_effect_composite_when(Condition condition, Effect effect);

    Effect get_or_create_effect_composite_oneof(EffectList effects);

    Action
    get_or_create_action(std::string name, size_t original_arity, ParameterList parameters, std::optional<Condition> condition, std::optional<Effect> effect);

    Axiom get_or_create_axiom(std::string derived_predicate_name, ParameterList parameters, Condition condition, size_t num_parameters_to_ground_head);

    OptimizationMetric get_or_create_optimization_metric(OptimizationMetricEnum metric, FunctionExpression function_expression);

    NumericFluent get_or_create_numeric_fluent(Function function, double number);

    Domain get_or_create_domain(std::optional<fs::path> filepath,
                                std::string name,
                                Requirements requirements,
                                TypeList types,
                                ObjectList constants,
                                PredicateList predicates,
                                FunctionSkeletonList functions,
                                ActionList actions,
                                AxiomList axioms);

    Problem get_or_create_problem(std::optional<fs::path> filepath,
                                  Domain domain,
                                  std::string name,
                                  Requirements requirements,
                                  ObjectList objects,
                                  PredicateList derived_predicates,
                                  LiteralList initial_literals,
                                  NumericFluentList numeric_fluents,
                                  std::optional<Condition> goal_condition,
                                  std::optional<OptimizationMetric> optimization_metric,
                                  AxiomList axioms);
};

// Here is a good place to define the `PDDLPositionCache` alias since we have all includes available.
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

}

#endif