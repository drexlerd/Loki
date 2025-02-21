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

#ifndef LOKI_INCLUDE_LOKI_PDDL_PROBLEM_BUILDER_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_PROBLEM_BUILDER_HPP_

#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/repositories.hpp"
#include "loki/details/utils/filesystem.hpp"

#include <optional>
#include <string>

namespace loki
{

using ProblemTypeToRepository =
    boost::hana::map<boost::hana::pair<boost::hana::type<RequirementsImpl>, RequirementsRepository>,
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
                     boost::hana::pair<boost::hana::type<ConditionNumericConstraintImpl>, ConditionNumericConstraintRepository>,
                     boost::hana::pair<boost::hana::type<ConditionImpl>, ConditionRepository>,
                     boost::hana::pair<boost::hana::type<AxiomImpl>, AxiomRepository>,
                     boost::hana::pair<boost::hana::type<OptimizationMetricImpl>, OptimizationMetricRepository>,
                     boost::hana::pair<boost::hana::type<FunctionValueImpl>, FunctionValueRepository>>;

class ProblemBuilder
{
private:
    ProblemTypeToRepository m_repositories;  ///< Memory
    std::shared_ptr<const Domain> m_domain;  ///< Immutable planning domain

    std::optional<fs::path> m_filepath;
    std::string m_name;

    /* Problem specific information. */
    Requirements m_requirements;

    std::unordered_set<Object> m_objects;
    std::unordered_set<Predicate> m_domain_and_problem_predicates;

    std::unordered_set<Literal> m_initial_literals;
    std::unordered_set<FunctionValue> m_initial_function_values;

    std::optional<Condition> m_goal_condition;

    std::optional<OptimizationMetric> m_optimization_metric;

    std::unordered_set<Axiom> m_domain_and_problem_axioms;

public:
    /// @brief Create a `ProblemBuilder` for a given `Domain`.
    /// Copies parts elements from the `Domain` into the `ProblemTypeToRepository` and problem specific information to ensure indexing schemas.
    /// @param domain
    explicit ProblemBuilder(std::shared_ptr<const Domain> domain);

    /// @brief Finalizes the `Problem` and returns it.
    /// The `ProblemBuilder` is in an invalid state afterwards.
    /// @return the resulting `Problem`.
    Problem get_result();

    ///////////////////////////////////
    /// Child-level
    ///////////////////////////////////

    /**
     * Unique construction.
     */

    Parameter get_or_create_parameter(Variable variable, TypeList types);

    Atom get_or_create_atom(Predicate predicate, TermList terms);

    FunctionExpressionNumber get_or_create_function_expression_number(double number);

    FunctionExpressionBinaryOperator get_or_create_function_expression_binary_operator(BinaryOperatorEnum binary_operator,
                                                                                       FunctionExpression left_function_expression,
                                                                                       FunctionExpression right_function_expression);

    FunctionExpressionMultiOperator get_or_create_function_expression_multi_operator(MultiOperatorEnum multi_operator,
                                                                                     FunctionExpressionList function_expressions_);

    FunctionExpressionMinus get_or_create_function_expression_minus(FunctionExpression function_expression);

    FunctionExpressionFunction get_or_create_function_expression_function(Function function);

    ConditionLiteral get_or_create_condition_literal(Literal literal);

    ConditionAnd get_or_create_condition_and(ConditionList conditions_);

    ConditionOr get_or_create_condition_or(ConditionList conditions_);

    ConditionNot get_or_create_condition_not(Condition condition);

    ConditionImply get_or_create_condition_imply(Condition condition_left, Condition condition_right);

    ConditionExists get_or_create_condition_exists(ParameterList parameters, Condition condition);

    ConditionForall get_or_create_condition_forall(ParameterList parameters, Condition condition);

    ConditionNumericConstraint get_or_create_condition_numeric_constraint(BinaryComparatorEnum binary_comparator,
                                                                          FunctionExpression function_expression_left,
                                                                          FunctionExpression function_expression_right);

    ///////////////////////////////////
    /// Top-level
    ///////////////////////////////////

    /**
     * Unique construction.
     */

    Requirements get_or_create_requirements(RequirementEnumSet requirements);

    Object get_or_create_object(std::string name, TypeList types);

    Predicate get_or_create_predicate(std::string name, ParameterList parameters);

    Literal get_or_create_literal(bool is_negated, Atom atom);

    FunctionExpression get_or_create_function_expression(FunctionExpressionNumber fexpr_number);
    FunctionExpression get_or_create_function_expression(FunctionExpressionBinaryOperator fexpr_binary_op);
    FunctionExpression get_or_create_function_expression(FunctionExpressionMultiOperator fexpr_multi_op);
    FunctionExpression get_or_create_function_expression(FunctionExpressionMinus fexpr_minus);
    FunctionExpression get_or_create_function_expression(FunctionExpressionFunction fexpr_function);
    FunctionValue get_or_create_function_value(Function function, double number);

    Condition get_or_create_condition(ConditionLiteral condition);
    Condition get_or_create_condition(ConditionAnd condition);
    Condition get_or_create_condition(ConditionOr condition);
    Condition get_or_create_condition(ConditionNot condition);
    Condition get_or_create_condition(ConditionImply condition);
    Condition get_or_create_condition(ConditionExists condition);
    Condition get_or_create_condition(ConditionForall condition);
    Condition get_or_create_condition(ConditionNumericConstraint condition);

    OptimizationMetric get_or_create_optimization_metric(OptimizationMetricEnum optimization_metric, FunctionExpression function_expression);

    Axiom get_or_create_axiom(ParameterList parameters, Literal subtyped_literal, Condition condition);

    /**
     * Top-level problem modifiers.
     * Return true iff the element was newly inserted into the problem and false otherwise.
     */

    bool set_requirements(Requirements requirements);
    bool add_object(Object object);
    bool add_predicate(Predicate predicate);
    bool add_initial_literal(Literal literal);
    bool add_initial_function_value(FunctionValue function_value);
    bool set_goal_condition(Condition condition);
    bool set_optimization_metric(OptimizationMetric optimization_metric);
    bool add_axiom(Axiom axiom);

    /**
     * Top-level problem accessors.
     * Return true iff the element exist in the problem and false otherwise.
     */

    bool contains_requirement(RequirementEnum requirement) const;
    bool contains_object(Object object) const;
    bool contains_predicate(Predicate predicate) const;
    bool contains_initial_literal(Literal literal) const;
    bool contains_function_value(FunctionValue function_value) const;
    bool has_goal_condition() const;
    bool has_optimization_metric() const;
    bool has_axiom(Axiom axiom) const;
};
}

#endif