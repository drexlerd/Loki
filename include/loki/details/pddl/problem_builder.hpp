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
#include "loki/details/pddl/problem_repositories.hpp"
#include "loki/details/utils/filesystem.hpp"

#include <optional>
#include <string>

namespace loki
{

class ProblemBuilder
{
private:
    HanaProblemRepositories m_repositories;

    Domain m_domain;  ///< Immutable planning domain
    std::optional<fs::path> m_filepath;
    std::string m_name;
    Requirements m_requirements;
    std::unordered_map<std::string, Object> m_domain_and_problem_objects;
    std::unordered_map<std::string, Predicate> m_domain_and_problem_predicates;
    LiteralSet m_initial_literals;
    std::unordered_map<Function, FunctionValue> m_initial_function_values;
    std::optional<Condition> m_goal_condition;
    std::optional<OptimizationMetric> m_optimization_metric;
    AxiomSet m_domain_and_problem_axioms;

public:
    /// @brief Create a `ProblemBuilder` for a given `Domain`.
    /// Copies parts elements from the `Domain` into the `HanaProblemRepositories` and problem specific information to ensure indexing schemas.
    /// @param domain
    explicit ProblemBuilder(Domain domain);

    /// @brief Finalizes the `Problem` and returns it.
    /// The `ProblemBuilder` is in an invalid state afterwards.
    /// @return the resulting `Problem`.
    Problem get_result(size_t problem_index);

    /**
     * Unique construction.
     */

    Requirements get_or_create_requirements(RequirementEnumSet requirements);

    Variable get_or_create_variable(std::string name);

    Object get_or_create_object(std::string name, TypeList types);

    Term get_or_create_term(Variable variable);
    Term get_or_create_term(Object object);

    Parameter get_or_create_parameter(Variable variable, TypeList types);

    Predicate get_or_create_predicate(std::string name, ParameterList parameters);

    Atom get_or_create_atom(Predicate predicate, TermList terms);

    Literal get_or_create_literal(bool is_negated, Atom atom);

    FunctionExpressionNumber get_or_create_function_expression_number(double number);
    FunctionExpressionBinaryOperator get_or_create_function_expression_binary_operator(BinaryOperatorEnum binary_operator,
                                                                                       FunctionExpression left_function_expression,
                                                                                       FunctionExpression right_function_expression);
    FunctionExpressionMultiOperator get_or_create_function_expression_multi_operator(MultiOperatorEnum multi_operator,
                                                                                     FunctionExpressionList function_expressions_);
    FunctionExpressionMinus get_or_create_function_expression_minus(FunctionExpression function_expression);

    FunctionExpressionFunction get_or_create_function_expression_function(Function function);
    FunctionExpression get_or_create_function_expression(FunctionExpressionNumber fexpr);
    FunctionExpression get_or_create_function_expression(FunctionExpressionBinaryOperator fexpr);
    FunctionExpression get_or_create_function_expression(FunctionExpressionMultiOperator fexpr);
    FunctionExpression get_or_create_function_expression(FunctionExpressionMinus fexpr);
    FunctionExpression get_or_create_function_expression(FunctionExpressionFunction fexpr);
    FunctionValue get_or_create_function_value(Function function, double number);

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
     * Get and modify components of the problem.
     */

    std::optional<fs::path>& get_filepath();
    std::string& get_name();
    Requirements& get_requirements();
    std::unordered_map<std::string, Object>& get_objects();
    std::unordered_map<std::string, Predicate>& get_predicates();
    LiteralSet& get_initial_literals();
    std::unordered_map<Function, FunctionValue>& get_function_values();
    std::optional<Condition>& get_goal_condition();
    std::optional<OptimizationMetric>& get_optimization_metric();
    AxiomSet& get_axioms();
};
}

#endif