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

#ifndef LOKI_INCLUDE_LOKI_PROBLEM_AST_AST_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_AST_AST_HPP_

#include "../../common/ast/ast.hpp"
#include "../../domain/ast/ast.hpp"

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/optional.hpp>

#include <map>
#include <vector>


namespace loki::problem::ast
{
    ///////////////////////////////////////////////////////////////////////////
    //  The AST
    ///////////////////////////////////////////////////////////////////////////
    namespace x3 = boost::spirit::x3;

    struct BasicFunctionTerm;

    struct AtomicFormulaOfNamesPredicate;
    struct AtomicFormulaOfNamesEquality;         // :equality
    struct AtomicFormulaOfNames;
    struct Atom;
    struct NegatedAtom;
    struct Literal;

    struct InitialElementLiteral;
    struct InitialElementTimedLiterals;      // :timed-initial-literals
    struct InitialElementNumericFluents;     // :numeric-fluents
    struct InitialElementObjectFluents;      // :object-fluents
    struct InitialElement;

    struct MetricFunctionExpression;
    struct MetricFunctionExpressionNumber;
    struct MetricFunctionExpressionBinaryOperator;
    struct MetricFunctionExpressionMultiOperator;
    struct MetricFunctionExpressionMinus;
    struct MetricFunctionExpressionBasicFunctionTerm;
    struct MetricFunctionExpressionTotalTime;
    struct MetricFunctionExpressionPreferences;  // :preferences

    struct OptimizationMinimize;
    struct OptimizationMaximize;
    struct Optimization;

    struct PreferenceConstraintGoalDescriptor;
    struct PreferenceConstraintGoalDescriptorAnd;
    struct PreferenceConstraintGoalDescriptorForall;       // :universal-preconditions
    struct PreferenceConstraintGoalDescriptorPreference;   // :preferences
    struct PreferenceConstraintGoalDescriptorSimple;

    struct ProblemName;
    struct DomainName;
    struct Objects;
    struct Initial;
    struct Goal;
    struct Constraints;                          // :constraints
    struct MetricSpecification;                  // :numeric-fluents

    struct Problem;


    /* <basic-function-term> */
    struct BasicFunctionTerm : x3::position_tagged {
        common::ast::FunctionSymbol function_symbol;
        std::vector<common::ast::Name> names;
    };


    /* Atomic formulas */
    struct AtomicFormulaOfNamesPredicate : x3::position_tagged
    {
        common::ast::Predicate predicate;
        std::vector<common::ast::Name> names;
    };

    struct AtomicFormulaOfNamesEquality : x3::position_tagged
    {
        common::ast::Name name_left;
        common::ast::Name name_right;
    };

    struct AtomicFormulaOfNames : x3::position_tagged,
                                  x3::variant<
                                      AtomicFormulaOfNamesPredicate,
                                      AtomicFormulaOfNamesEquality>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct Atom : x3::position_tagged
    {
        AtomicFormulaOfNames atomic_formula_of_names;
    };

    struct NegatedAtom : x3::position_tagged
    {
        AtomicFormulaOfNames atomic_formula_of_names;
    };

    struct Literal : x3::position_tagged,
        x3::variant<
            Atom,
            NegatedAtom>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    /* <init-el> */
    struct InitialElementLiteral : x3::position_tagged {
        Literal literal;
    };

    struct InitialElementTimedLiterals : x3::position_tagged {
        common::ast::Number number;
        Literal literal;
    };

    struct InitialElementNumericFluents : x3::position_tagged {
        BasicFunctionTerm basic_function_term;
        common::ast::Number number;
    };

    struct InitialElementObjectFluents : x3::position_tagged {
        BasicFunctionTerm basic_function_term;
        common::ast::Name name;
    };

    struct InitialElement : x3::position_tagged,
        x3::variant<
            InitialElementLiteral,
            InitialElementTimedLiterals,
            InitialElementNumericFluents,
            InitialElementObjectFluents> {
        using base_type::base_type;
        using base_type::operator=;
    };


    /* <metric-f-exp> */
    struct MetricFunctionExpression : x3::position_tagged,
        x3::variant<
            x3::forward_ast<MetricFunctionExpressionNumber>,
            x3::forward_ast<MetricFunctionExpressionBinaryOperator>,
            x3::forward_ast<MetricFunctionExpressionMultiOperator>,
            x3::forward_ast<MetricFunctionExpressionMinus>,
            x3::forward_ast<MetricFunctionExpressionBasicFunctionTerm>,
            x3::forward_ast<MetricFunctionExpressionTotalTime>,
            x3::forward_ast<MetricFunctionExpressionPreferences>> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct MetricFunctionExpressionNumber : x3::position_tagged {
        common::ast::Number number;
    };

    struct MetricFunctionExpressionBinaryOperator : x3::position_tagged {
        domain::ast::BinaryOperator binary_operator;
        MetricFunctionExpression metric_function_expression_left;
        MetricFunctionExpression metric_function_expression_right;
    };

    struct MetricFunctionExpressionMultiOperator : x3::position_tagged {
        domain::ast::BinaryOperator binary_operator;
        MetricFunctionExpression metric_function_expression_first;
        std::vector<MetricFunctionExpression> metric_function_expression_remaining;
    };

    struct MetricFunctionExpressionMinus : x3::position_tagged {
        MetricFunctionExpression metric_function_expression;
    };

    struct MetricFunctionExpressionBasicFunctionTerm : x3::position_tagged {
        BasicFunctionTerm basic_function_term;
    };

    struct MetricFunctionExpressionTotalTime : x3::position_tagged { };

    struct MetricFunctionExpressionPreferences : x3::position_tagged {
        domain::ast::PreferenceName preference_name;
    };


    /* <optimization> */
    struct OptimizationMinimize : x3::position_tagged { };

    struct OptimizationMaximize : x3::position_tagged { };

    struct Optimization : x3::position_tagged,
        x3::variant<
            OptimizationMinimize,
            OptimizationMaximize> {
        using base_type::base_type;
        using base_type::operator=;
    };


    /* <pref-con-GD> */
    struct PreferenceConstraintGoalDescriptor : x3::position_tagged,
        x3::variant<
            x3::forward_ast<PreferenceConstraintGoalDescriptorSimple>,
            x3::forward_ast<PreferenceConstraintGoalDescriptorPreference>,
            x3::forward_ast<PreferenceConstraintGoalDescriptorAnd>,
            x3::forward_ast<PreferenceConstraintGoalDescriptorForall>> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct PreferenceConstraintGoalDescriptorAnd : x3::position_tagged {
        std::vector<PreferenceConstraintGoalDescriptor> preference_constraint_goal_descriptors;
    };

    struct PreferenceConstraintGoalDescriptorForall : x3::position_tagged {
        domain::ast::TypedListOfVariables typed_list_of_variables;
        PreferenceConstraintGoalDescriptor preference_constraint_goal_descriptor;
    };

    struct PreferenceConstraintGoalDescriptorPreference : x3::position_tagged {
        boost::optional<domain::ast::PreferenceName> preference_name;
        domain::ast::ConstraintGoalDescriptor constraint_goal_descriptor;
    };

    struct PreferenceConstraintGoalDescriptorSimple : x3::position_tagged {
        domain::ast::ConstraintGoalDescriptor constraint_goal_descriptor;
    };


    /* <problem> */
    struct ProblemName : x3::position_tagged {
        common::ast::Name name;
    };

    struct DomainName : x3::position_tagged {
        common::ast::Name name;
    };

    struct Objects : x3::position_tagged {
        domain::ast::TypedListOfNames typed_list_of_names;
    };

    struct Initial : x3::position_tagged {
        std::vector<InitialElement> initial_elements;
    };

    struct Goal : x3::position_tagged {
        domain::ast::PreconditionGoalDescriptor precondition_goal_descriptor;
    };

    struct Constraints : x3::position_tagged {
        PreferenceConstraintGoalDescriptor preference_constraint_goal_descriptor;
    };

    struct MetricSpecification : x3::position_tagged {
        Optimization optimization;
        MetricFunctionExpression metric_function_expression;
    };

    struct Problem : x3::position_tagged {
        ProblemName problem_name;
        DomainName domain_name;
        boost::optional<domain::ast::Requirements> requirements;
        boost::optional<Objects> objects;
        Initial initial;
        Goal goal;
        boost::optional<Constraints> constraints;
        boost::optional<MetricSpecification> metric_specification;
    };

}

#endif