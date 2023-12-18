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

#ifndef LOKI_SRC_PROBLEM_AST_PARSER_HPP_
#define LOKI_SRC_PROBLEM_AST_PARSER_HPP_

#include "../../../include/loki/problem/ast/ast.hpp"

#include <boost/spirit/home/x3.hpp>


namespace loki::problem {
    namespace x3 = boost::spirit::x3;

    ///////////////////////////////////////////////////////////////////////////
    // parser public interface for testing
    ///////////////////////////////////////////////////////////////////////////
    namespace parser {
        struct BasicFunctionTermClass;

        struct AtomicFormulaOfNamesPredicateClass;
        struct AtomicFormulaOfNamesEqualityClass;
        struct AtomicFormulaOfNamesClass;
        struct AtomClass;
        struct NegatedAtomClass;
        struct LiteralClass;

        struct InitialElementLiteralClass;
        struct InitialElementTimedLiteralsClass;
        struct InitialElementNumericFluentsClass;
        struct InitialElementObjectFluentsClass;
        struct InitialElementClass;

        struct MetricFunctionExpressionClass;
        struct MetricFunctionExpressionNumberClass;
        struct MetricFunctionExpressionBinaryOperatorClass;
        struct MetricFunctionExpressionMultiOperatorClass;
        struct MetricFunctionExpressionMinusClass;
        struct MetricFunctionExpressionBasicFunctionTermClass;
        struct MetricFunctionExpressionTotalTimeClass;
        struct MetricFunctionExpressionPreferencesClass;

        struct OptimizationMinimizeClass;
        struct OptimizationMaximizeClass;
        struct OptimizationClass;

        struct PreferenceConstraintGoalDescriptorClass;
        struct PreferenceConstraintGoalDescriptorAndClass;
        struct PreferenceConstraintGoalDescriptorForallClass;
        struct PreferenceConstraintGoalDescriptorPreferenceClass;
        struct PreferenceConstraintGoalDescriptorSimpleClass;

        struct ProblemKeywordClass;
        struct ProblemNameClass;
        struct DomainNameClass;
        struct ObjectsClass;
        struct InitialClass;
        struct GoalClass;
        struct ConstraintsClass;
        struct MetricSpecificationClass;


        typedef x3::rule<BasicFunctionTermClass, ast::BasicFunctionTerm> basic_function_term_type;

        typedef x3::rule<AtomicFormulaOfNamesPredicateClass, ast::AtomicFormulaOfNamesPredicate> atomic_formula_of_names_predicate_type;
        typedef x3::rule<AtomicFormulaOfNamesEqualityClass, ast::AtomicFormulaOfNamesEquality> atomic_formula_of_names_equality_type;
        typedef x3::rule<AtomicFormulaOfNamesClass, ast::AtomicFormulaOfNames> atomic_formula_of_names_type;
        typedef x3::rule<AtomClass, ast::Atom> atom_type;
        typedef x3::rule<NegatedAtomClass, ast::NegatedAtom> negated_atom_type;
        typedef x3::rule<LiteralClass, ast::Literal> literal_type;

        typedef x3::rule<InitialElementLiteralClass, ast::InitialElementLiteral> initial_element_literals_type;
        typedef x3::rule<InitialElementTimedLiteralsClass, ast::InitialElementTimedLiterals> initial_element_timed_literals_type;
        typedef x3::rule<InitialElementNumericFluentsClass, ast::InitialElementNumericFluents> initial_element_numeric_fluents_type;
        typedef x3::rule<InitialElementObjectFluentsClass, ast::InitialElementObjectFluents> initial_element_object_fluents_type;
        typedef x3::rule<InitialElementClass, ast::InitialElement> initial_element_type;

        typedef x3::rule<MetricFunctionExpressionClass, ast::MetricFunctionExpression> metric_function_expression_type;
        typedef x3::rule<MetricFunctionExpressionNumberClass, ast::MetricFunctionExpressionNumber> metric_function_expression_number_type;
        typedef x3::rule<MetricFunctionExpressionBinaryOperatorClass, ast::MetricFunctionExpressionBinaryOperator> metric_function_expression_binary_operator_type;
        typedef x3::rule<MetricFunctionExpressionMultiOperatorClass, ast::MetricFunctionExpressionMultiOperator> metric_function_expression_multi_operator_type;
        typedef x3::rule<MetricFunctionExpressionMinusClass, ast::MetricFunctionExpressionMinus> metric_function_expression_minus_type;
        typedef x3::rule<MetricFunctionExpressionBasicFunctionTermClass, ast::MetricFunctionExpressionBasicFunctionTerm> metric_function_expression_basic_function_term_type;
        typedef x3::rule<MetricFunctionExpressionTotalTimeClass, ast::MetricFunctionExpressionTotalTime> metric_function_expression_total_time_type;
        typedef x3::rule<MetricFunctionExpressionPreferencesClass, ast::MetricFunctionExpressionPreferences> metric_function_expression_preferences_type;

        typedef x3::rule<OptimizationMinimizeClass, ast::OptimizationMinimize> optimization_minimize_type;
        typedef x3::rule<OptimizationMaximizeClass, ast::OptimizationMaximize> optimization_maximize_type;
        typedef x3::rule<OptimizationClass, ast::Optimization> optimization_type;

        typedef x3::rule<PreferenceConstraintGoalDescriptorClass, ast::PreferenceConstraintGoalDescriptor> preference_constraint_goal_descriptor_type;
        typedef x3::rule<PreferenceConstraintGoalDescriptorAndClass, ast::PreferenceConstraintGoalDescriptorAnd> preference_constraint_goal_descriptor_and_type;
        typedef x3::rule<PreferenceConstraintGoalDescriptorForallClass, ast::PreferenceConstraintGoalDescriptorForall> preference_constraint_goal_descriptor_forall_type;
        typedef x3::rule<PreferenceConstraintGoalDescriptorPreferenceClass, ast::PreferenceConstraintGoalDescriptorPreference> preference_constraint_goal_descriptor_preference_type;
        typedef x3::rule<PreferenceConstraintGoalDescriptorSimpleClass, ast::PreferenceConstraintGoalDescriptorSimple> preference_constraint_goal_descriptor_simple_type;

        typedef x3::rule<ProblemKeywordClass, x3::unused_type> problem_keyword_type;
        typedef x3::rule<ProblemNameClass, ast::ProblemName> problem_name_type;
        typedef x3::rule<DomainNameClass, ast::DomainName> domain_name_type;
        typedef x3::rule<ObjectsClass, ast::Objects> objects_type;
        typedef x3::rule<InitialClass, ast::Initial> initial_type;
        typedef x3::rule<GoalClass, ast::Goal> goal_type;
        typedef x3::rule<ConstraintsClass, ast::Constraints> constraints_type;
        typedef x3::rule<MetricSpecificationClass, ast::MetricSpecification> metric_specification_type;


        BOOST_SPIRIT_DECLARE(basic_function_term_type)

        BOOST_SPIRIT_DECLARE(atomic_formula_of_names_predicate_type, atomic_formula_of_names_equality_type,
            atomic_formula_of_names_type, atom_type, negated_atom_type, literal_type)

        BOOST_SPIRIT_DECLARE(initial_element_literals_type, initial_element_timed_literals_type,
            initial_element_numeric_fluents_type, initial_element_object_fluents_type,
            initial_element_type)

        BOOST_SPIRIT_DECLARE(metric_function_expression_type,
            metric_function_expression_binary_operator_type,
            metric_function_expression_multi_operator_type,
            metric_function_expression_minus_type, metric_function_expression_number_type,
            metric_function_expression_basic_function_term_type,
            metric_function_expression_total_time_type, metric_function_expression_preferences_type)

        BOOST_SPIRIT_DECLARE(optimization_minimize_type, optimization_maximize_type, optimization_type)

        BOOST_SPIRIT_DECLARE(preference_constraint_goal_descriptor_type,
            preference_constraint_goal_descriptor_and_type,
            preference_constraint_goal_descriptor_forall_type,
            preference_constraint_goal_descriptor_preference_type,
            preference_constraint_goal_descriptor_simple_type)

        BOOST_SPIRIT_DECLARE(problem_keyword_type, problem_name_type, domain_name_type, objects_type,
            initial_type, goal_type, constraints_type, metric_specification_type)
    }


    parser::basic_function_term_type const& basic_function_term();

    parser::atomic_formula_of_names_predicate_type const& atomic_formula_of_names_predicate();
    parser::atomic_formula_of_names_equality_type const& atomic_formula_of_names_equality();
    parser::atomic_formula_of_names_type const& atomic_formula_of_names();
    parser::atom_type const& atom();
    parser::negated_atom_type const& negated_atom();
    parser::literal_type const& literal();

    parser::initial_element_literals_type const& initial_element_literals();
    parser::initial_element_timed_literals_type const& initial_element_timed_literals();
    parser::initial_element_numeric_fluents_type const& initial_element_numeric_fluents();
    parser::initial_element_object_fluents_type const& initial_element_object_fluents();
    parser::initial_element_type const& initial_element();

    parser::metric_function_expression_type const& metric_function_expression();
    parser::metric_function_expression_number_type const& metric_function_expression_number();
    parser::metric_function_expression_binary_operator_type const& metric_function_expression_binary_operator();
    parser::metric_function_expression_multi_operator_type const& metric_function_expression_multi_operator();
    parser::metric_function_expression_minus_type const& metric_function_expression_minus();
    parser::metric_function_expression_basic_function_term_type const& metric_function_expression_basic_function_term();
    parser::metric_function_expression_total_time_type const& metric_function_expression_total_time();
    parser::metric_function_expression_preferences_type const& metric_function_expression_preferences();

    parser::optimization_minimize_type const& optimization_minimize();
    parser::optimization_maximize_type const& optimization_maximize();
    parser::optimization_type const& optimization();

    parser::preference_constraint_goal_descriptor_type const& preference_constraint_goal_descriptor();
    parser::preference_constraint_goal_descriptor_and_type const& preference_constraint_goal_descriptor_and();
    parser::preference_constraint_goal_descriptor_forall_type const& preference_constraint_goal_descriptor_forall();
    parser::preference_constraint_goal_descriptor_preference_type const& preference_constraint_goal_descriptor_preference();
    parser::preference_constraint_goal_descriptor_simple_type const& preference_constraint_goal_descriptor_simple();

    parser::problem_keyword_type const& problem_keyword();
    parser::problem_name_type const& problem_name();
    parser::domain_name_type const& domain_name();
    parser::objects_type const& objects();
    parser::initial_type const& initial();
    parser::goal_type const& goal();
    parser::constraints_type const& constraints();
    parser::metric_specification_type const& metric_specification();
}

#endif
