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
#include "loki/details/utils/segmented_repository.hpp"

#include <boost/hana.hpp>

namespace loki
{

template<typename T>
using SegmentedPDDLRepository = SegmentedRepository<T, UniquePDDLHasher<const T*>, UniquePDDLEqualTo<const T*>>;

using RequirementsRepository = SegmentedPDDLRepository<RequirementsImpl>;
using TypeRepository = SegmentedPDDLRepository<TypeImpl>;
using VariableRepository = SegmentedPDDLRepository<VariableImpl>;
using TermRepository = SegmentedPDDLRepository<TermImpl>;
using ObjectRepository = SegmentedPDDLRepository<ObjectImpl>;
using AtomRepository = SegmentedPDDLRepository<AtomImpl>;
using LiteralRepository = SegmentedPDDLRepository<LiteralImpl>;
using ParameterRepository = SegmentedPDDLRepository<ParameterImpl>;
using PredicateRepository = SegmentedPDDLRepository<PredicateImpl>;
using FunctionExpressionNumberRepository = SegmentedPDDLRepository<FunctionExpressionNumberImpl>;
using FunctionExpressionBinaryOperatorRepository = SegmentedPDDLRepository<FunctionExpressionBinaryOperatorImpl>;
using FunctionExpressionMultiOperatorRepository = SegmentedPDDLRepository<FunctionExpressionMultiOperatorImpl>;
using FunctionExpressionMinusRepository = SegmentedPDDLRepository<FunctionExpressionMinusImpl>;
using FunctionExpressionFunctionRepository = SegmentedPDDLRepository<FunctionExpressionFunctionImpl>;
using FunctionExpressionRepository = SegmentedPDDLRepository<FunctionExpressionImpl>;
using FunctionRepository = SegmentedPDDLRepository<FunctionImpl>;
using FunctionSkeletonRepository = SegmentedPDDLRepository<FunctionSkeletonImpl>;
using ConditionLiteralRepository = SegmentedPDDLRepository<ConditionLiteralImpl>;
using ConditionAndRepository = SegmentedPDDLRepository<ConditionAndImpl>;
using ConditionOrRepository = SegmentedPDDLRepository<ConditionOrImpl>;
using ConditionNotRepository = SegmentedPDDLRepository<ConditionNotImpl>;
using ConditionImplyRepository = SegmentedPDDLRepository<ConditionImplyImpl>;
using ConditionExistsRepository = SegmentedPDDLRepository<ConditionExistsImpl>;
using ConditionForallRepository = SegmentedPDDLRepository<ConditionForallImpl>;
using ConditionRepository = SegmentedPDDLRepository<ConditionImpl>;
using EffectLiteralRepository = SegmentedPDDLRepository<EffectLiteralImpl>;
using EffectAndRepository = SegmentedPDDLRepository<EffectAndImpl>;
using EffectNumericRepository = SegmentedPDDLRepository<EffectNumericImpl>;
using EffectCompositeForallRepository = SegmentedPDDLRepository<EffectCompositeForallImpl>;
using EffectCompositeWhenRepository = SegmentedPDDLRepository<EffectCompositeWhenImpl>;
using EffectCompositeOneofRepository = SegmentedPDDLRepository<EffectCompositeOneofImpl>;
using EffectRepository = SegmentedPDDLRepository<EffectImpl>;
using ActionRepository = SegmentedPDDLRepository<ActionImpl>;
using AxiomRepository = SegmentedPDDLRepository<AxiomImpl>;
using OptimizationMetricRepository = SegmentedPDDLRepository<OptimizationMetricImpl>;
using NumericFluentRepository = SegmentedPDDLRepository<NumericFluentImpl>;
using DomainRepository = SegmentedPDDLRepository<DomainImpl>;
using ProblemRepository = SegmentedPDDLRepository<ProblemImpl>;

using PDDLTypeToRepository =
    boost::hana::map<boost::hana::pair<boost::hana::type<RequirementsImpl>, RequirementsRepository>,
                     boost::hana::pair<boost::hana::type<TypeImpl>, TypeRepository>,
                     boost::hana::pair<boost::hana::type<VariableImpl>, VariableRepository>,
                     boost::hana::pair<boost::hana::type<TermImpl>, TermRepository>,
                     boost::hana::pair<boost::hana::type<ObjectImpl>, ObjectRepository>,
                     boost::hana::pair<boost::hana::type<AtomImpl>, AtomRepository>,
                     boost::hana::pair<boost::hana::type<LiteralImpl>, LiteralRepository>,
                     boost::hana::pair<boost::hana::type<ParameterImpl>, ParameterRepository>,
                     boost::hana::pair<boost::hana::type<PredicateImpl>, PredicateRepository>,
                     boost::hana::pair<boost::hana::type<FunctionExpressionNumberImpl>, FunctionExpressionNumberRepository>,
                     boost::hana::pair<boost::hana::type<FunctionExpressionBinaryOperatorImpl>, FunctionExpressionBinaryOperatorRepository>,
                     boost::hana::pair<boost::hana::type<FunctionExpressionMultiOperatorImpl>, FunctionExpressionMultiOperatorRepository>,
                     boost::hana::pair<boost::hana::type<FunctionExpressionMinusImpl>, FunctionExpressionMinusRepository>,
                     boost::hana::pair<boost::hana::type<FunctionExpressionFunctionImpl>, FunctionExpressionFunctionRepository>,
                     boost::hana::pair<boost::hana::type<FunctionExpressionImpl>, FunctionExpressionRepository>,
                     boost::hana::pair<boost::hana::type<FunctionImpl>, FunctionRepository>,
                     boost::hana::pair<boost::hana::type<FunctionSkeletonImpl>, FunctionSkeletonRepository>,
                     boost::hana::pair<boost::hana::type<ConditionLiteralImpl>, ConditionLiteralRepository>,
                     boost::hana::pair<boost::hana::type<ConditionAndImpl>, ConditionAndRepository>,
                     boost::hana::pair<boost::hana::type<ConditionOrImpl>, ConditionOrRepository>,
                     boost::hana::pair<boost::hana::type<ConditionNotImpl>, ConditionNotRepository>,
                     boost::hana::pair<boost::hana::type<ConditionImplyImpl>, ConditionImplyRepository>,
                     boost::hana::pair<boost::hana::type<ConditionExistsImpl>, ConditionExistsRepository>,
                     boost::hana::pair<boost::hana::type<ConditionForallImpl>, ConditionForallRepository>,
                     boost::hana::pair<boost::hana::type<ConditionImpl>, ConditionRepository>,
                     boost::hana::pair<boost::hana::type<EffectLiteralImpl>, EffectLiteralRepository>,
                     boost::hana::pair<boost::hana::type<EffectAndImpl>, EffectAndRepository>,
                     boost::hana::pair<boost::hana::type<EffectNumericImpl>, EffectNumericRepository>,
                     boost::hana::pair<boost::hana::type<EffectCompositeForallImpl>, EffectCompositeForallRepository>,
                     boost::hana::pair<boost::hana::type<EffectCompositeWhenImpl>, EffectCompositeWhenRepository>,
                     boost::hana::pair<boost::hana::type<EffectCompositeOneofImpl>, EffectCompositeOneofRepository>,
                     boost::hana::pair<boost::hana::type<EffectImpl>, EffectRepository>,
                     boost::hana::pair<boost::hana::type<ActionImpl>, ActionRepository>,
                     boost::hana::pair<boost::hana::type<AxiomImpl>, AxiomRepository>,
                     boost::hana::pair<boost::hana::type<OptimizationMetricImpl>, OptimizationMetricRepository>,
                     boost::hana::pair<boost::hana::type<NumericFluentImpl>, NumericFluentRepository>,
                     boost::hana::pair<boost::hana::type<DomainImpl>, DomainRepository>,
                     boost::hana::pair<boost::hana::type<ProblemImpl>, ProblemRepository>>;

extern PDDLTypeToRepository create_default_pddl_type_to_repository();

/// @brief Collection of factories for the unique creation of PDDL objects.
class PDDLRepositories
{
private:
    PDDLTypeToRepository m_repositories;

public:
    PDDLRepositories();
    PDDLRepositories(const PDDLRepositories& other) = delete;
    PDDLRepositories& operator=(const PDDLRepositories& other) = delete;
    PDDLRepositories(PDDLRepositories&& other);
    PDDLRepositories& operator=(PDDLRepositories&& other);

    Requirements get_or_create_requirements(RequirementEnumSet requirement_set);

    Type get_or_create_type(std::string name, TypeList bases);

    Variable get_or_create_variable(std::string name);

    Term get_or_create_term(Variable variable);

    Term get_or_create_term(Object object);

    Object get_or_create_object(std::string name, TypeList types);

    Atom get_or_create_atom(Predicate predicate, TermList terms);

    Literal get_or_create_literal(bool is_negated, Atom atom);

    Parameter get_or_create_parameter(Variable variable, TypeList types);

    Predicate get_or_create_predicate(std::string name, ParameterList parameters);

    FunctionExpressionNumber get_or_create_function_expression_number(double number);

    FunctionExpressionBinaryOperator get_or_create_function_expression_binary_operator(BinaryOperatorEnum binary_operator,
                                                                                       FunctionExpression left_function_expression,
                                                                                       FunctionExpression right_function_expression);

    FunctionExpressionMultiOperator get_or_create_function_expression_multi_operator(MultiOperatorEnum multi_operator,
                                                                                     FunctionExpressionList function_expressions_);

    FunctionExpressionMinus get_or_create_function_expression_minus(FunctionExpression function_expression);

    FunctionExpressionFunction get_or_create_function_expression_function(Function function);

    FunctionExpression get_or_create_function_expression(FunctionExpressionNumber fexpr_number);

    FunctionExpression get_or_create_function_expression(FunctionExpressionBinaryOperator fexpr_binary_op);

    FunctionExpression get_or_create_function_expression(FunctionExpressionMultiOperator fexpr_multi_op);

    FunctionExpression get_or_create_function_expression(FunctionExpressionMinus fexpr_minus);

    FunctionExpression get_or_create_function_expression(FunctionExpressionFunction fexpr_function);

    Function get_or_create_function(FunctionSkeleton function_skeleton, TermList terms);

    FunctionSkeleton get_or_create_function_skeleton(std::string name, ParameterList parameters, Type type);

    ConditionLiteral get_or_create_condition_literal(Literal literal);

    ConditionAnd get_or_create_condition_and(ConditionList conditions_);

    ConditionOr get_or_create_condition_or(ConditionList conditions_);

    ConditionNot get_or_create_condition_not(Condition condition);

    ConditionImply get_or_create_condition_imply(Condition condition_left, Condition condition_right);

    ConditionExists get_or_create_condition_exists(ParameterList parameters, Condition condition);

    ConditionForall get_or_create_condition_forall(ParameterList parameters, Condition condition);

    Condition get_or_create_condition(ConditionLiteral condition);

    Condition get_or_create_condition(ConditionAnd condition);

    Condition get_or_create_condition(ConditionOr condition);

    Condition get_or_create_condition(ConditionNot condition);

    Condition get_or_create_condition(ConditionImply condition);

    Condition get_or_create_condition(ConditionExists condition);

    Condition get_or_create_condition(ConditionForall condition);

    EffectLiteral get_or_create_effect_literal(Literal literal);

    EffectAnd get_or_create_effect_and(EffectList effects);

    EffectNumeric get_or_create_effect_numeric(AssignOperatorEnum assign_operator, Function function, FunctionExpression function_expression);

    EffectCompositeForall get_or_create_effect_composite_forall(ParameterList parameters, Effect effect);

    EffectCompositeWhen get_or_create_effect_composite_when(Condition condition, Effect effect);

    EffectCompositeOneof get_or_create_effect_composite_oneof(EffectList effects);

    Effect get_or_create_effect(EffectLiteral effect);

    Effect get_or_create_effect(EffectAnd effect);

    Effect get_or_create_effect(EffectNumeric effect);

    Effect get_or_create_effect(EffectCompositeForall effect);

    Effect get_or_create_effect(EffectCompositeWhen effect);

    Effect get_or_create_effect(EffectCompositeOneof effect);

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