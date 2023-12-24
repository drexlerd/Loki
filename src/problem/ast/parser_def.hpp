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

#ifndef LOKI_SRC_PROBLEM_AST_PARSER_DEF_HPP_
#define LOKI_SRC_PROBLEM_AST_PARSER_DEF_HPP_

#include "ast_adapted.hpp"
#include "parser.hpp"

#include "../../domain/ast/parser.hpp"
#include "../../../include/loki/problem/ast/ast.hpp"
#include "../../../include/loki/problem/ast/error_handler.hpp"
#include "../../../include/loki/problem/ast/parser.hpp"
#include "../../../include/loki/domain/ast/parser.hpp"

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>


namespace loki::problem::parser {
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    using x3::lit;
    using x3::lexeme;
    using x3::eps;
    using x3::int_;
    using x3::double_;
    using x3::expect;
    using x3::no_skip;

    using ascii::alpha;
    using ascii::alnum;
    using ascii::char_;
    using ascii::string;

    ///////////////////////////////////////////////////////////////////////////
    // Rule IDs
    ///////////////////////////////////////////////////////////////////////////


    ///////////////////////////////////////////////////////////////////////////
    // Rules
    ///////////////////////////////////////////////////////////////////////////

    //domain_type const domain = "domain";

    basic_function_term_type const basic_function_term = "basic_function_term";

    atomic_formula_of_names_predicate_type const atomic_formula_of_names_predicate = "atomic_formula_of_names_predicate";
    atomic_formula_of_names_equality_type const atomic_formula_of_names_equality = "atomic_formula_of_names_equality";
    atomic_formula_of_names_type const atomic_formula_of_names = "atomic_formula_of_names";
    atom_type const atom = "atom";
    negated_atom_type const negated_atom = "negated_atom";
    literal_type const literal = "literal";

    initial_element_literals_type const initial_element_literals = "initial_element_literals";
    initial_element_timed_literals_type const initial_element_timed_literals = "initial_element_timed_literals";
    initial_element_numeric_fluents_total_cost_type const initial_element_numeric_fluents_total_cost = "initial_element_numeric_fluents_total_cost";
    initial_element_numeric_fluents_general_type const initial_element_numeric_fluents_general = "initial_element_numeric_fluents_general";
    initial_element_type const initial_element = "initial_element";

    metric_function_expression_type const metric_function_expression = "metric_function_expression";
    metric_function_expression_binary_operator_type const metric_function_expression_binary_operator = "metric_function_expression_binary_operator";
    metric_function_expression_multi_operator_type const metric_function_expression_multi_operator = "metric_function_expression_multi_operator";
    metric_function_expression_minus_type const metric_function_expression_minus = "metric_function_expression_minus";
    metric_function_expression_number_type const metric_function_expression_number = "metric_function_expression_number";
    metric_function_expression_basic_function_term_type const metric_function_expression_basic_function_term = "metric_function_expression_basic_function_term";
    metric_function_expression_total_time_type const metric_function_expression_total_time = "metric_function_expression_total_time";
    metric_function_expression_preferences_type const metric_function_expression_preferences = "metric_function_expression_preferences";

    optimization_minimize_type const optimization_minimize = "optimization_minimize";
    optimization_maximize_type const optimization_maximize = "optimization_maximize";
    optimization_type const optimization = "optimization";
    metric_specification_total_cost_type const metric_specification_total_cost = "metric_specification_total_cost";
    metric_specification_general_type const metric_specification_general = "metric_specification_general";

    preference_constraint_goal_descriptor_type const preference_constraint_goal_descriptor = "preference_constraint_goal_descriptor";
    preference_constraint_goal_descriptor_and_type const preference_constraint_goal_descriptor_and = "preference_constraint_goal_descriptor_and";
    preference_constraint_goal_descriptor_forall_type const preference_constraint_goal_descriptor_forall = "preference_constraint_goal_descriptor_forall";
    preference_constraint_goal_descriptor_preference_type const preference_constraint_goal_descriptor_preference = "preference_constraint_goal_descriptor_preference";
    preference_constraint_goal_descriptor_simple_type const preference_constraint_goal_descriptor_simple = "preference_constraint_goal_descriptor_simple";

    problem_keyword_type const problem_keyword = "problem";
    problem_name_type const problem_name = "problem_name";
    domain_name_type const domain_name = "domain_name";
    objects_type const objects = "objects";
    initial_type const initial = "initial";
    goal_type const goal = "goal";
    constraints_type const constraints = "constraints";
    metric_specification_type const metric_specification = "metric_specification";

    problem_type const problem = "problem";


    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////

    const auto basic_function_term_def = ((lit('(') > domain::function_symbol() > *domain::name()) > lit(')')) | (domain::function_symbol() > x3::attr(std::vector<domain::ast::Name>{}));

    const auto atomic_formula_of_names_predicate_def = (lit('(') >> domain::predicate()) > *domain::name() > lit(')');
    const auto atomic_formula_of_names_equality_def = (lit('(') >> lit('=')) > domain::name() > domain::name() > lit(')');
    const auto atomic_formula_of_names_def = atomic_formula_of_names_equality | atomic_formula_of_names_predicate;
    const auto atom_def = atomic_formula_of_names;
    const auto negated_atom_def = lit('(') >> domain::keyword_lit("not") > atomic_formula_of_names >> lit(')');
    const auto literal_def = atom | negated_atom;

    const auto initial_element_literals_def = literal;
    const auto initial_element_timed_literals_def = (lit('(') >> lit("at") >> domain::number()) > literal > lit(')');
    const auto initial_element_numeric_fluents_total_cost_def = (lit('(') >> lit('=') >> lit('(') >> domain::function_symbol_total_cost()) > lit(')') > domain::number() > lit(')');
    const auto initial_element_numeric_fluents_general_def = (lit('(') >> lit('=') >> basic_function_term) > domain::number() > lit(')');
    const auto initial_element_def = initial_element_timed_literals | initial_element_numeric_fluents_total_cost
        | initial_element_numeric_fluents_general | initial_element_literals;


    const auto metric_function_expression_binary_operator_def = (lit('(') >> domain::binary_operator() >> metric_function_expression >> metric_function_expression) > lit(')');
    const auto metric_function_expression_multi_operator_def = (lit('(') >> domain::multi_operator() >> metric_function_expression >> +metric_function_expression) > lit(')');
    const auto metric_function_expression_minus_def = (lit('(') >> lit('-')) > metric_function_expression > lit(')');
    const auto metric_function_expression_preferences_def = (lit('(') >> domain::keyword_lit("is-violated")) > domain::preference_name() > lit(')');
    const auto metric_function_expression_basic_function_term_def = basic_function_term;
    const auto metric_function_expression_total_time_def = domain::keyword_lit("total-time") > x3::attr(ast::MetricFunctionExpressionTotalTime{});
    const auto metric_function_expression_number_def = domain::number();
    const auto metric_function_expression_def = metric_function_expression_binary_operator
        | metric_function_expression_multi_operator | metric_function_expression_minus
        | metric_function_expression_preferences | metric_function_expression_basic_function_term
        | metric_function_expression_total_time | metric_function_expression_number;

    const auto optimization_minimize_def = domain::keyword_lit("minimize") > x3::attr(ast::OptimizationMinimize{});
    const auto optimization_maximize_def = domain::keyword_lit("maximize") > x3::attr(ast::OptimizationMaximize{});
    const auto optimization_def = optimization_minimize | optimization_maximize;
    const auto metric_specification_total_cost_def = optimization_minimize >> lit('(') >> domain::function_symbol_total_cost() > lit(')');
    const auto metric_specification_general_def = optimization >> metric_function_expression;
    
    const auto preference_constraint_goal_descriptor_and_def = (lit('(') >> domain::keyword_lit("and")) > *preference_constraint_goal_descriptor > lit(')');
    const auto preference_constraint_goal_descriptor_forall_def = (lit('(') >> domain::keyword_lit("forall")) > domain::typed_list_of_variables() > preference_constraint_goal_descriptor > lit(')');
    const auto preference_constraint_goal_descriptor_preference_def = (lit('(') >> domain::keyword_lit("preference") > (-domain::preference_name())) > domain::constraint_goal_descriptor() > lit(')');
    const auto preference_constraint_goal_descriptor_simple_def = domain::constraint_goal_descriptor();
    const auto preference_constraint_goal_descriptor_def = preference_constraint_goal_descriptor_and | preference_constraint_goal_descriptor_forall
        | preference_constraint_goal_descriptor_preference | preference_constraint_goal_descriptor_simple;

    const auto problem_keyword_def = domain::keyword_lit("problem");
    const auto problem_name_def = lit('(') > problem_keyword > domain::name() > lit(')');
    const auto domain_name_def = (lit('(') >> domain::keyword_lit(":domain")) > domain::name() > lit(')');
    const auto objects_def = (lit('(') >> domain::keyword_lit(":objects")) > domain::typed_list_of_names() > lit(')');
    const auto initial_def = (lit('(') >> domain::keyword_lit(":init")) > *initial_element > lit(')');
    const auto goal_def = (lit('(') >> domain::keyword_lit(":goal")) > domain::precondition_goal_descriptor() > lit(')');
    const auto constraints_def = (lit('(') >> domain::keyword_lit(":constraints")) > preference_constraint_goal_descriptor > lit(')');
    const auto metric_specification_def = (lit('(') >> domain::keyword_lit(":metric")) > (metric_specification_total_cost | metric_specification_general) > lit(')');

    const auto problem_def = lit('(') > domain::define_keyword()
            > problem_name
            > domain_name
            > -domain::requirements()
            > -objects
            > initial
            > goal
            > -constraints
            > -metric_specification
        > lit(')');


    BOOST_SPIRIT_DEFINE(basic_function_term)

    BOOST_SPIRIT_DEFINE(atomic_formula_of_names_predicate, atomic_formula_of_names_equality,
        atomic_formula_of_names, atom, negated_atom, literal)

    BOOST_SPIRIT_DEFINE(initial_element_literals, initial_element_timed_literals,
       initial_element_numeric_fluents_total_cost, initial_element_numeric_fluents_general,
       initial_element)

    BOOST_SPIRIT_DEFINE(metric_function_expression,
        metric_function_expression_number,
        metric_function_expression_binary_operator,
        metric_function_expression_multi_operator,
        metric_function_expression_minus,
        metric_function_expression_basic_function_term,
        metric_function_expression_total_time, metric_function_expression_preferences)

    BOOST_SPIRIT_DEFINE(optimization_minimize, optimization_maximize, optimization, metric_specification_total_cost, metric_specification_general)

    BOOST_SPIRIT_DEFINE(preference_constraint_goal_descriptor,
        preference_constraint_goal_descriptor_and,
        preference_constraint_goal_descriptor_forall,
        preference_constraint_goal_descriptor_preference,
        preference_constraint_goal_descriptor_simple)

    BOOST_SPIRIT_DEFINE(problem_keyword, problem_name, domain_name, objects,
        initial, goal, constraints, metric_specification, problem)

    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

    struct BasicFunctionTermArityGreaterZeroClass : x3::annotate_on_success {};
    struct BasicFunctionTermArityZeroClass : x3::annotate_on_success {};
    struct BasicFunctionTermClass : x3::annotate_on_success {};

    struct AtomicFormulaOfNamesPredicateClass : x3::annotate_on_success {};
    struct AtomicFormulaOfNamesEqualityClass : x3::annotate_on_success {};
    struct AtomicFormulaOfNamesClass : x3::annotate_on_success {};
    struct AtomClass : x3::annotate_on_success {};
    struct NegatedAtomClass : x3::annotate_on_success {};
    struct LiteralClass : x3::annotate_on_success {};

    struct InitialElementLiteralClass : x3::annotate_on_success {};
    struct InitialElementTimedLiteralsClass : x3::annotate_on_success {};
    struct InitialElementNumericFluentsTotalCostClass : x3::annotate_on_success {};
    struct InitialElementNumericFluentsGeneralClass : x3::annotate_on_success {};
    struct InitialElementClass : x3::annotate_on_success {};

    struct MetricFunctionExpressionClass : x3::annotate_on_success {};
    struct MetricFunctionExpressionNumberClass : x3::annotate_on_success {};
    struct MetricFunctionExpressionBinaryOperatorClass : x3::annotate_on_success {};
    struct MetricFunctionExpressionMultiOperatorClass : x3::annotate_on_success {};
    struct MetricFunctionExpressionMinusClass : x3::annotate_on_success {};
    struct MetricFunctionExpressionBasicFunctionTermClass : x3::annotate_on_success {};
    struct MetricFunctionExpressionTotalTimeClass : x3::annotate_on_success {};
    struct MetricFunctionExpressionPreferencesClass : x3::annotate_on_success {};

    struct OptimizationMinimizeClass : x3::annotate_on_success {};
    struct OptimizationMaximizeClass : x3::annotate_on_success {};
    struct OptimizationClass : x3::annotate_on_success {};
    struct MetricSpecificationTotalCostClass : x3::annotate_on_success {};
    struct MetricSpecificationGeneralClass : x3::annotate_on_success {};

    struct PreferenceConstraintGoalDescriptorClass : x3::annotate_on_success {};
    struct PreferenceConstraintGoalDescriptorAndClass : x3::annotate_on_success {};
    struct PreferenceConstraintGoalDescriptorForallClass : x3::annotate_on_success {};
    struct PreferenceConstraintGoalDescriptorPreferenceClass : x3::annotate_on_success {};
    struct PreferenceConstraintGoalDescriptorSimpleClass : x3::annotate_on_success {};

    struct ProblemNameClass : x3::annotate_on_success {};
    struct DomainNameClass : x3::annotate_on_success {};
    struct ObjectsClass : x3::annotate_on_success {};
    struct InitialClass : x3::annotate_on_success {};
    struct GoalClass : x3::annotate_on_success {};
    struct ConstraintsClass : x3::annotate_on_success {};
    struct MetricSpecificationClass : x3::annotate_on_success {};

    struct ProblemClass : x3::annotate_on_success, error_handler_problem {};
}

namespace loki::problem
{
    parser::basic_function_term_type const& basic_function_term() {
        return parser::basic_function_term;
    }

    parser::atomic_formula_of_names_predicate_type const& atomic_formula_of_names_predicate() {
        return parser::atomic_formula_of_names_predicate;
    }
    parser::atomic_formula_of_names_equality_type const& atomic_formula_of_names_equality() {
        return parser::atomic_formula_of_names_equality;
    }
    parser::atomic_formula_of_names_type const& atomic_formula_of_names() {
        return parser::atomic_formula_of_names;
    }
    parser::atom_type const& atom() {
        return parser::atom;
    }
    parser::negated_atom_type const& negated_atom() {
        return parser::negated_atom;
    }
    parser::literal_type const& literal() {
        return parser::literal;
    }

    parser::initial_element_literals_type const& initial_element_literals() {
        return parser::initial_element_literals;
    }
    parser::initial_element_timed_literals_type const& initial_element_timed_literals() {
        return parser::initial_element_timed_literals;
    }
    parser::initial_element_numeric_fluents_total_cost_type const& initial_element_numeric_fluents_total_cost() {
        return parser::initial_element_numeric_fluents_total_cost;
    }
    parser::initial_element_numeric_fluents_general_type const& initial_element_numeric_fluents_general() {
        return parser::initial_element_numeric_fluents_general;
    }
    parser::initial_element_type const& initial_element() {
        return parser::initial_element;
    }

    parser::metric_function_expression_type const& metric_function_expression() {
        return parser::metric_function_expression;
    }
    parser::metric_function_expression_number_type const& metric_function_expression_number() {
        return parser::metric_function_expression_number;
    }
    parser::metric_function_expression_binary_operator_type const& metric_function_expression_binary_operator() {
        return parser::metric_function_expression_binary_operator;
    }
    parser::metric_function_expression_multi_operator_type const& metric_function_expression_multi_operator() {
        return parser::metric_function_expression_multi_operator;
    }
    parser::metric_function_expression_minus_type const& metric_function_expression_minus() {
        return parser::metric_function_expression_minus;
    }
    parser::metric_function_expression_basic_function_term_type const& metric_function_expression_basic_function_term() {
        return parser::metric_function_expression_basic_function_term;
    }
    parser::metric_function_expression_total_time_type const& metric_function_expression_total_time() {
        return parser::metric_function_expression_total_time;
    }
    parser::metric_function_expression_preferences_type const& metric_function_expression_preferences() {
        return parser::metric_function_expression_preferences;
    }

    parser::optimization_minimize_type const& optimization_minimize() {
        return parser::optimization_minimize;
    }
    parser::optimization_maximize_type const& optimization_maximize() {
        return parser::optimization_maximize;
    }
    parser::optimization_type const& optimization() {
        return parser::optimization;
    }
    parser::metric_specification_total_cost_type const& metric_specification_total_cost() {
        return parser::metric_specification_total_cost;
    }
    parser::metric_specification_general_type const& metric_specification_general() {
        return parser::metric_specification_general;
    }

    parser::preference_constraint_goal_descriptor_type const& preference_constraint_goal_descriptor() {
        return parser::preference_constraint_goal_descriptor;
    }
    parser::preference_constraint_goal_descriptor_and_type const& preference_constraint_goal_descriptor_and() {
        return parser::preference_constraint_goal_descriptor_and;
    }
    parser::preference_constraint_goal_descriptor_forall_type const& preference_constraint_goal_descriptor_forall() {
        return parser::preference_constraint_goal_descriptor_forall;
    }
    parser::preference_constraint_goal_descriptor_preference_type const& preference_constraint_goal_descriptor_preference() {
        return parser::preference_constraint_goal_descriptor_preference;
    }
    parser::preference_constraint_goal_descriptor_simple_type const& preference_constraint_goal_descriptor_simple() {
        return parser::preference_constraint_goal_descriptor_simple;
    }

    parser::problem_keyword_type const& problem_keyword() {
        return parser::problem_keyword;
    }
    parser::problem_name_type const& problem_name() {
        return parser::problem_name;
    }
    parser::domain_name_type const& domain_name() {
        return parser::domain_name;
    }
    parser::objects_type const& objects() {
        return parser::objects;
    }
    parser::initial_type const& initial() {
        return parser::initial;
    }
    parser::goal_type const& goal() {
        return parser::goal;
    }
    parser::constraints_type const& constraints() {
        return parser::constraints;
    }
    parser::metric_specification_type const& metric_specification() {
        return parser::metric_specification;
    }
    parser::problem_type const& problem() {
        return parser::problem;
    }
}

#endif
