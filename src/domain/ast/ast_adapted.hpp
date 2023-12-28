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

#include "../../../include/domain/ast/ast.hpp"

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

// We need to tell fusion about our rexpr and rexpr_key_value
// to make them a first-class fusion citizens
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Name, characters)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Variable, characters)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::FunctionSymbol, name)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Predicate, name)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Number, value)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::TypeEither, types)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::TypedListOfNamesRecursively, names, type, typed_list_of_names)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::TypedListOfVariablesRecursively, variables, type, typed_list_of_variables)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::AtomicFormulaSkeleton, predicate, typed_list_of_variables)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::FunctionTypeNumber, number)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::FunctionTypeType, type)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::AtomicFunctionSkeletonTotalCost, function_symbol)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::AtomicFunctionSkeletonGeneral, function_symbol, arguments)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively, atomic_function_skeletons, function_type, function_typed_list_of_atomic_function_skeletons)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::AtomicFormulaOfTermsPredicate, predicate, terms)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::AtomicFormulaOfTermsEquality, term_left, term_right)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Atom, atomic_formula_of_terms)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::NegatedAtom, atomic_formula_of_terms)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::FunctionHead, function_symbol, terms)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::FunctionExpressionNumber, number)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::FunctionExpressionBinaryOp, binary_operator, function_expression_left, function_expression_right)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::FunctionExpressionMinus, function_expression)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::FunctionExpressionHead, function_head)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::GoalDescriptorAtom, atom)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::GoalDescriptorLiteral, literal)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::GoalDescriptorAnd, goal_descriptors)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::GoalDescriptorOr, goal_descriptors)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::GoalDescriptorNot, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::GoalDescriptorImply, goal_descriptor_left, goal_descriptor_right)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::GoalDescriptorExists, typed_list_of_variables, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::GoalDescriptorForall, typed_list_of_variables, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::GoalDescriptorFunctionComparison, binary_comparator, function_expression_left, function_expression_right)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::ConstraintGoalDescriptorAnd, constraint_goal_descriptors)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::ConstraintGoalDescriptorForall, typed_list_of_variables, constraint_goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::ConstraintGoalDescriptorAtEnd, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::ConstraintGoalDescriptorAlways, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::ConstraintGoalDescriptorSometime, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::ConstraintGoalDescriptorWithin, number, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::ConstraintGoalDescriptorAtMostOnce, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::ConstraintGoalDescriptorSometimeAfter, goal_descriptor_left, goal_descriptor_right)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::ConstraintGoalDescriptorSometimeBefore, goal_descriptor_left, goal_descriptor_right)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::ConstraintGoalDescriptorAlwaysWithin, number, goal_descriptor_left, goal_descriptor_right)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::ConstraintGoalDescriptorHoldDuring, number_left, number_right, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::ConstraintGoalDescriptorHoldAfter, number, goal_descriptor)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::PreferenceName, name)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::PreconditionGoalDescriptorSimple, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::PreconditionGoalDescriptorAnd, precondition_goal_descriptors)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::PreconditionGoalDescriptorPreference, preference_name, goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::PreconditionGoalDescriptorForall, typed_list_of_variables, precondition_goal_descriptor)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::EffectProductionLiteral, literal)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::EffectProductionNumericFluentTotalCost, assign_operator_increase, function_symbol_total_cost, numeric_term)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::EffectProductionNumericFluentGeneral, assign_operator, function_head, function_expression)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::EffectConditionalForall, typed_list_of_variables, effect)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::EffectConditionalWhen, goal_descriptor, effect)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::ActionSymbol, name)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::ActionBody, precondition_goal_descriptor, effect)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Action, action_symbol, typed_list_of_variables, action_body)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::DerivedPredicate, typed_list_of_variables, goal_descriptor)

BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::DomainName, name)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Requirements, requirements)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Types, typed_list_of_names)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Constants, typed_list_of_names)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Predicates, atomic_formula_skeletons)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Functions, function_types_list_of_atomic_function_skeletons)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Constraints, constraint_goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::domain::ast::Domain, domain_name, requirements, types, constants, predicates, functions, constraints, structures)


#endif
