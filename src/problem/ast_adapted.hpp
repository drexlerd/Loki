#ifndef LOKI_SRC_PROBLEM_AST_ADAPTED_HPP_
#define LOKI_SRC_PROBLEM_AST_ADAPTED_HPP_

#include "include/loki/problem/ast.hpp"

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

// We need to tell fusion about our rexpr and rexpr_key_value
// to make them a first-class fusion citizens
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::BasicFunctionTermArityGreaterZero, function_symbol, names)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::BasicFunctionTermArityZero, function_symbol)

BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::AtomicFormulaOfNamesPredicate, predicate, names)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::AtomicFormulaOfNamesEquality, name_left, name_right)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::Atom, atomic_formula_of_names)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::NegatedAtom, atomic_formula_of_names)

BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::InitialElaborationLiteral, literal)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::InitialElaborationTimedLiterals, number, literal)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::InitialElaborationNumericFluents, function_head, number)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::InitialElaborationObjectFluents, basic_function_term, name)

BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::MetricFunctionExpressionBinaryOperator, binary_operator, metric_function_expression_left, metric_function_expression_right)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::MetricFunctionExpressionMultiOperator, binary_operator, metric_function_expression_first, metric_function_expression_remaining)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::MetricFunctionExpressionMinus, metric_function_expression)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::MetricFunctionExpressionNumber, number)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::MetricFunctionExpressionBasicFunctionTerm, basic_function_term)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::MetricFunctionExpressionPreferences, preference_name)

BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::PreferenceConstraintGoalDescriptorAnd, preference_constraint_goal_descriptors)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::PreferenceConstraintGoalDescriptorForall, typed_list_of_variables, preference_constraint_goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::PreferenceConstraintGoalDescriptorPreference, preference_name, constraint_goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::PreferenceConstraintGoalDescriptorSimple, constraint_goal_descriptor)

BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::ProblemName, name)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::DomainName, name)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::Objects, typed_list_of_names)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::Initial, initial_elaboration)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::Goal, precondition_goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::Constraints, preference_constraint_goal_descriptor)
BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::MetricSpecification, optimization, metric_function_expression)

BOOST_FUSION_ADAPT_STRUCT(loki::problem::ast::Problem, problem_name, domain_name, requirements, objects, initial, goal, constraints, metric_specification)


#endif
