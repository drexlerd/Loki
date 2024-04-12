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

#ifndef LOKI_SRC_DOMAIN_AST_AST_ADAPTED_HPP_
#define LOKI_SRC_DOMAIN_AST_AST_ADAPTED_HPP_

#include "loki/ast/ast.hpp"

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

// We need to tell fusion about our rexpr and rexpr_key_value
// to make them a first-class fusion citizens

/**
 * Domain
 */
BOOST_FUSION_ADAPT_STRUCT(loki::ast::Name, characters)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::Variable, characters)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::FunctionSymbol, name)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::Predicate, name)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::Number, value)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::TypeEither, types)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::TypedListOfNamesRecursively, names, type, typed_list_of_names)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::TypedListOfVariablesRecursively, variables, type, typed_list_of_variables)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::AtomicFormulaSkeleton, predicate, typed_list_of_variables)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::AtomicFunctionSkeletonTotalCost, function_symbol)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::AtomicFunctionSkeletonGeneral, function_symbol, arguments)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively,
                          atomic_function_skeletons,
                          function_type,
                          function_typed_list_of_atomic_function_skeletons)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::AtomicFormulaOfTermsPredicate, predicate, terms)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::AtomicFormulaOfTermsEquality, term_left, term_right)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::Atom, atomic_formula_of_terms)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::NegatedAtom, atomic_formula_of_terms)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::FunctionHead, function_symbol, terms)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::FunctionExpressionNumber, number)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::FunctionExpressionBinaryOp, binary_operator, function_expression_left, function_expression_right)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::FunctionExpressionMinus, function_expression)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::FunctionExpressionHead, function_head)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::GoalDescriptorAtom, atom)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::GoalDescriptorLiteral, literal)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::GoalDescriptorAnd, goal_descriptors)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::GoalDescriptorOr, goal_descriptors)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::GoalDescriptorNot, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::GoalDescriptorImply, goal_descriptor_left, goal_descriptor_right)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::GoalDescriptorExists, typed_list_of_variables, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::GoalDescriptorForall, typed_list_of_variables, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::GoalDescriptorFunctionComparison, binary_comparator, function_expression_left, function_expression_right)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::ConstraintGoalDescriptorAnd, constraint_goal_descriptors)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::ConstraintGoalDescriptorForall, typed_list_of_variables, constraint_goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::ConstraintGoalDescriptorAtEnd, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::ConstraintGoalDescriptorAlways, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::ConstraintGoalDescriptorSometime, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::ConstraintGoalDescriptorWithin, number, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::ConstraintGoalDescriptorAtMostOnce, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::ConstraintGoalDescriptorSometimeAfter, goal_descriptor_left, goal_descriptor_right)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::ConstraintGoalDescriptorSometimeBefore, goal_descriptor_left, goal_descriptor_right)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::ConstraintGoalDescriptorAlwaysWithin, number, goal_descriptor_left, goal_descriptor_right)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::ConstraintGoalDescriptorHoldDuring, number_left, number_right, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::ConstraintGoalDescriptorHoldAfter, number, goal_descriptor)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::PreferenceName, name)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::PreconditionGoalDescriptorSimple, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::PreconditionGoalDescriptorAnd, precondition_goal_descriptors)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::PreconditionGoalDescriptorPreference, preference_name, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::PreconditionGoalDescriptorForall, typed_list_of_variables, precondition_goal_descriptor)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::EffectProductionLiteral, literal)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::EffectProductionNumericFluentTotalCost, assign_operator_increase, function_symbol_total_cost, numeric_term)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::EffectProductionNumericFluentGeneral, assign_operator, function_head, function_expression)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::EffectConditionalForall, typed_list_of_variables, effect)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::EffectConditionalWhen, goal_descriptor, effect)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::ActionSymbol, name)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::ActionBody, precondition_goal_descriptor, effect)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::Action, action_symbol, typed_list_of_variables, action_body)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::Axiom, literal, goal_descriptor)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::DomainName, name)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::Requirements, requirements)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::Types, typed_list_of_names)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::Constants, typed_list_of_names)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::Predicates, atomic_formula_skeletons)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::DerivedPredicates, atomic_formula_skeletons)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::Functions, function_types_list_of_atomic_function_skeletons)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::Constraints, constraint_goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::Domain, domain_name, requirements, types, constants, predicates, derived_predicates, functions, constraints, structures)

/**
 * Problem
 */
BOOST_FUSION_ADAPT_STRUCT(loki::ast::BasicFunctionTerm, function_symbol, names)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::AtomicFormulaOfNamesPredicate, predicate, names)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::AtomicFormulaOfNamesEquality, name_left, name_right)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::GroundAtom, atomic_formula_of_names)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::NegatedGroundAtom, atomic_formula_of_names)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::InitialElementLiteral, literal)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::InitialElementTimedLiterals, number, literal)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::InitialElementNumericFluentsTotalCost, function_symbol_total_cost, number)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::InitialElementNumericFluentsGeneral, basic_function_term, number)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::MetricFunctionExpressionNumber, number)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::MetricFunctionExpressionBinaryOperator, binary_operator, metric_function_expression_left, metric_function_expression_right)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::MetricFunctionExpressionMultiOperator,
                          multi_operator,
                          metric_function_expression_first,
                          metric_function_expression_remaining)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::MetricFunctionExpressionMinus, metric_function_expression)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::MetricFunctionExpressionBasicFunctionTerm, basic_function_term)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::MetricFunctionExpressionPreferences, preference_name)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::PreferenceConstraintGoalDescriptorAnd, preference_constraint_goal_descriptors)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::PreferenceConstraintGoalDescriptorForall, typed_list_of_variables, preference_constraint_goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::PreferenceConstraintGoalDescriptorPreference, preference_name, constraint_goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::PreferenceConstraintGoalDescriptorSimple, constraint_goal_descriptor)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::MetricSpecificationTotalCost, optimization_minimize, function_symbol_total_cost)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::MetricSpecificationGeneral, optimization, metric_function_expression)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::ProblemName, name)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::ProblemDomainName, name)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::Objects, typed_list_of_names)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::Initial, initial_elements)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::Goal, precondition_goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::ast::ProblemConstraints, preference_constraint_goal_descriptor)

BOOST_FUSION_ADAPT_STRUCT(loki::ast::Problem,
                          problem_name,
                          domain_name,
                          requirements,
                          objects,
                          derived_predicates,
                          initial,
                          goal,
                          constraints,
                          metric_specification,
                          axioms)

#endif
