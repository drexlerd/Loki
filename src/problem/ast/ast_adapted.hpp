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

#ifndef LOKI_SRC_PROBLEM_AST_AST_ADAPTED_HPP_
#define LOKI_SRC_PROBLEM_AST_AST_ADAPTED_HPP_

#include "../../../include/loki/problem/ast/ast.hpp"

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

// We need to tell fusion about our rexpr and rexpr_key_value
// to make them a first-class fusion citizens
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::BasicFunctionTerm, function_symbol, names)

BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::AtomicFormulaOfNamesPredicate, predicate, names)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::AtomicFormulaOfNamesEquality, name_left, name_right)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::Atom, atomic_formula_of_names)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::NegatedAtom, atomic_formula_of_names)

BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::InitialElementLiteral, literal)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::InitialElementTimedLiterals, number, literal)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::InitialElementNumericFluents, function_head, number)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::InitialElementObjectFluents, basic_function_term, name)

BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::MetricFunctionExpressionNumber, number)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::MetricFunctionExpressionBinaryOperator, binary_operator, metric_function_expression_left, metric_function_expression_right)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::MetricFunctionExpressionMultiOperator, binary_operator, metric_function_expression_first, metric_function_expression_remaining)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::MetricFunctionExpressionMinus, metric_function_expression)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::MetricFunctionExpressionBasicFunctionTerm, basic_function_term)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::MetricFunctionExpressionPreferences, preference_name)

BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::PreferenceConstraintGoalDescriptorAnd, preference_constraint_goal_descriptors)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::PreferenceConstraintGoalDescriptorForall, typed_list_of_variables, preference_constraint_goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::PreferenceConstraintGoalDescriptorPreference, preference_name, constraint_goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::PreferenceConstraintGoalDescriptorSimple, constraint_goal_descriptor)

BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::ProblemName, name)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::DomainName, name)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::Objects, typed_list_of_names)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::Initial, initial_elements)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::Goal, precondition_goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::Constraints, preference_constraint_goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::MetricSpecification, optimization, metric_function_expression)

BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::Problem, problem_name, domain_name, requirements, objects, initial, goal, constraints, metric_specification)


#endif
