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

#ifndef LOKI_SRC_DOMAIN_AST_PARSER_DEF_HPP_
#define LOKI_SRC_DOMAIN_AST_PARSER_DEF_HPP_

#include "ast_adapted.hpp"
#include "loki/details/ast/ast.hpp"
#include "loki/details/ast/error_handler.hpp"
#include "loki/details/ast/parser.hpp"
#include "loki/details/ast/parser_wrapper.hpp"
#include "parser.hpp"

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

namespace loki::parser
{
namespace x3 = boost::spirit::x3;
namespace ascii = boost::spirit::x3::ascii;

using x3::double_;
using x3::eps;
using x3::int_;
using x3::lexeme;
using x3::lit;
using x3::no_skip;
using x3::raw;
using x3::string;

using ascii::alnum;
using ascii::alpha;
using ascii::char_;
using ascii::space;

///////////////////////////////////////////////////////////////////////////
// Rule IDs
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
// Rules
///////////////////////////////////////////////////////////////////////////

/**
 * Domain
 */
define_keyword_type const define_keyword = "define";
domain_keyword_type const domain_keyword = "domain";

name_type const name = "name";
variable_type const variable = "variable";
name_total_cost_type const name_total_cost = "name_total_cost";
function_symbol_total_cost_type const function_symbol_total_cost = "function_symbol_total_cost";
function_symbol_type const function_symbol = "function_symbol";
term_type const term = "term";
number_type const number = "number";
predicate_type const predicate = "predicate";

requirement_strips_type const requirement_strips = "requirement_strips";
requirement_typing_type const requirement_typing = "requirement_typing";
requirement_negative_preconditions_type const requirement_negative_preconditions = "requirement_negative_preconditions";
requirement_disjunctive_preconditions_type const requirement_disjunctive_preconditions = "requirement_disjunctive_preconditions";
requirement_equality_type const requirement_equality = "requirement_equality";
requirement_existential_preconditions_type const requirement_existential_preconditions = "requirement_existential_preconditions";
requirement_universal_preconditions_type const requirement_universal_preconditions = "requirement_universal_preconditions";
requirement_quantified_preconditions_type const requirement_quantified_preconditions = "requirement_quantified_preconditions";
requirement_conditional_effects_type const requirement_conditional_effects = "requirement_conditional_effects";
requirement_fluents_type const requirement_fluents = "requirement_fluents";
requirement_object_fluents_type const requirement_object_fluents = "requirement_object_fluents";
requirement_numeric_fluents_type const requirement_numeric_fluents = "requirement_numeric_fluents";
requirement_adl_type const requirement_adl = "requirement_adl";
requirement_durative_actions_type const requirement_durative_actions = "requirement_durative_actions";
requirement_derived_predicates_type const requirement_derived_predicates = "requirement_derived_predicates";
requirement_timed_initial_literals_type const requirement_timed_initial_literals = "requirement_timed_initial_literals";
requirement_preferences_type const requirement_preferences = "requirement_preferences";
requirement_constraints_type const requirement_constraints = "requirement_constraints";
requirement_action_costs_type const requirement_action_costs = "requirement_action_costs";
requirement_non_deterministic_type const requirement_non_deterministic = "requirement_non_deterministic";
requirement_type const requirement = "requirement";

type_type const type = "type";
type_object_type const type_object = "type_object";
type_number_type const type_number = "type_number";
type_either_type const type_either = "type_either";
typed_list_of_names_recursively_type const typed_list_of_names_recursively = "typed_list_of_names_recursively";
typed_list_of_names_type const typed_list_of_names = "typed_list_of_names";
typed_list_of_variables_recursively_type const typed_list_of_variables_recursively = "typed_list_of_variables_recursively";
typed_list_of_variables_type const typed_list_of_variables = "typed_list_of_variables";

atomic_formula_skeleton_type const atomic_formula_skeleton = "atomic_formula_skeleton";

atomic_function_skeleton_total_cost_type const atomic_function_skeleton_total_cost = "atomic_function_skeleton_total_cost";
atomic_function_skeleton_general_type const atomic_function_skeleton_general = "atomic_function_skeleton_general";
atomic_function_skeleton_type const atomic_function_skeleton = "atomic_function_skeleton";
function_typed_list_of_atomic_function_skeletons_recursively_type const function_typed_list_of_atomic_function_skeletons_recursively =
    "function_typed_list_of_atomic_function_skeletons_recursively";
function_typed_list_of_atomic_function_skeletons_type const function_typed_list_of_atomic_function_skeletons =
    "function_typed_list_of_atomic_function_skeletons";

atomic_formula_of_terms_predicate_type const atomic_formula_of_terms_predicate = "atomic_formula_of_terms_predicate";
atomic_formula_of_terms_equality_type const atomic_formula_of_terms_equality = "atomic_formula_of_terms_equality";
atomic_formula_of_terms_type const atomic_formula_of_terms = "atomic_formula_of_terms";
atom_type const atom = "atom";
negated_atom_type const negated_atom = "negated_atom";
literal_type const literal = "literal";

multi_operator_mul_type const multi_operator_mul = "multi_operator_mul";
multi_operator_plus_type const multi_operator_plus = "multi_operator_plus";
multi_operator_type const multi_operator = "multi_operator";
binary_operator_minus_type const binary_operator_minus = "binary_operator_minus";
binary_operator_div_type const binary_operator_div = "binary_operator_div";
binary_operator_type const binary_operator = "binary_operator";

binary_comparator_greater_type const binary_comparator_greater = "binary_comparator_greater";
binary_comparator_less_type const binary_comparator_less = "binary_comparator_less";
binary_comparator_equal_type const binary_comparator_equal = "binary_comparator_equal";
binary_comparator_greater_equal_type const binary_comparator_greater_equal = "binary_comparator_greater_equal";
binary_comparator_less_equal_type const binary_comparator_less_equal = "binary_comparator_less_equal";
binary_comparator_type const binary_comparator = "binary_comparator";

function_expression_type const function_expression = "function_expression";
function_head_type const function_head = "function_head";
function_expression_number_type const function_expression_number = "function_expression_number";
function_expression_binary_op_type const function_expression_binary_op = "function_expression_binary_op";
function_expression_minus_type const function_expression_minus = "function_expression_minus";
function_expression_head_type const function_expression_head = "function_expression_head";

goal_descriptor_type const goal_descriptor = "goal_descriptor";
goal_descriptor_atom_type const goal_descriptor_atom = "goal_descriptor_atom";
goal_descriptor_literal_type const goal_descriptor_literal = "goal_descriptor_literal";
goal_descriptor_and_type const goal_descriptor_and = "goal_descriptor_and";
goal_descriptor_or_type const goal_descriptor_or = "goal_descriptor_or";
goal_descriptor_not_type const goal_descriptor_not = "goal_descriptor_not";
goal_descriptor_imply_type const goal_descriptor_imply = "goal_descriptor_imply";
goal_descriptor_exists_type const goal_descriptor_exists = "goal_descriptor_exists";
goal_descriptor_forall_type const goal_descriptor_forall = "goal_descriptor_forall";
goal_descriptor_function_comparison_type const goal_descriptor_function_comparison = "goal_descriptor_function_comparison";

constraint_goal_descriptor_type const constraint_goal_descriptor = "constraint_goal_descriptor";
constraint_goal_descriptor_and_type const constraint_goal_descriptor_and = "constraint_goal_descriptor_and";
constraint_goal_descriptor_forall_type const constraint_goal_descriptor_forall = "constraint_goal_descriptor_forall";
constraint_goal_descriptor_at_end_type const constraint_goal_descriptor_at_end = "constraint_goal_descriptor_at_end";
constraint_goal_descriptor_always_type const constraint_goal_descriptor_always = "constraint_goal_descriptor_always";
constraint_goal_descriptor_sometime_type const constraint_goal_descriptor_sometime = "constraint_goal_descriptor_sometime";
constraint_goal_descriptor_within_type const constraint_goal_descriptor_within = "constraint_goal_descriptor_within";
constraint_goal_descriptor_at_most_once_type const constraint_goal_descriptor_at_most_once = "constraint_goal_descriptor_at_most_once";
constraint_goal_descriptor_sometime_after_type const constraint_goal_descriptor_sometime_after = "constraint_goal_descriptor_sometime_after";
constraint_goal_descriptor_sometime_before_type const constraint_goal_descriptor_sometime_before = "constraint_goal_descriptor_sometime_before";
constraint_goal_descriptor_always_within_type const constraint_goal_descriptor_always_within = "constraint_goal_descriptor_always_within";
constraint_goal_descriptor_hold_during_type const constraint_goal_descriptor_hold_during = "constraint_goal_descriptor_hold_during";
constraint_goal_descriptor_hold_after_type const constraint_goal_descriptor_hold_after = "constraint_goal_descriptor_hold_after";

precondition_goal_descriptor_type const precondition_goal_descriptor = "precondition_goal_descriptor";
preference_name_type const preference_name = "preference_name";
precondition_goal_descriptor_simple_type const precondition_goal_descriptor_simple = "precondition_goal_descriptor_simple";
precondition_goal_descriptor_and_type const precondition_goal_descriptor_and = "precondition_goal_descriptor_and";
precondition_goal_descriptor_preference_type const precondition_goal_descriptor_preference = "precondition_goal_descriptor_preference";
precondition_goal_descriptor_forall_type const precondition_goal_descriptor_forall = "precondition_goal_descriptor_forall";

assign_operator_assign_type const assign_operator_assign = "assign_operator_assign";
assign_operator_scale_up_type const assign_operator_scale_up = "assign_operator_scale_up";
assign_operator_scale_down_type const assign_operator_scale_down = "assign_operator_scale_down";
assign_operator_increase_type const assign_operator_increase = "assign_operator_increase";
assign_operator_decrease_type const assign_operator_decrease = "assign_operator_decrease";
assign_operator_type const assign_operator = "assign_operator";

numeric_term_type const numeric_term = "numeric_term";

effect_type const effect = "effect";
effect_production_literal_type const effect_production_literal = "effect_production_literal";
effect_production_numeric_fluent_total_cost_type const effect_production_numeric_fluent_total_cost = "effect_production_numeric_fluent_total_cost";
effect_production_numeric_fluent_general_type const effect_production_numeric_fluent_general = "effect_production_numeric_fluent_general";
effect_production_type const effect_production = "effect_production";
effect_conditional_forall_type const effect_conditional_forall = "effect_conditional_forall";
effect_conditional_when_type const effect_conditional_when = "effect_conditional_when";
effect_conditional_type const effect_conditional = "effect_conditional";
effect_numeric_fluent_total_cost_or_effect_type const effect_numeric_fluent_total_cost_or_effect = "effect_numeric_fluent_total_cost_or_effect";
effect_root_deterministic_type const effect_root_deterministic = "effect_root_deterministic";
effect_root_non_deterministic_type const effect_root_non_deterministic = "effect_root_non_deterministic";
effect_root_type const effect_root = "effect_root";
action_symbol_type const action_symbol = "action_symbol";
action_body_type const action_body = "action_body";
action_type const action = "action";
axiom_type const axiom = "axiom";

domain_name_type const domain_name = "domain_name";
requirements_type const requirements = "requirements";
types_type const types = "types";
constants_type const constants = "constants";
predicates_type const predicates = "predicates";
functions_type const functions = "functions";
constraints_type const constraints = "constraints";
structure_type const structure = "structure";
domain_type const domain = "domain";

/**
 * Problem
 */
basic_function_term_type const basic_function_term = "basic_function_term";

atomic_formula_of_names_predicate_type const atomic_formula_of_names_predicate = "atomic_formula_of_names_predicate";
atomic_formula_of_names_equality_type const atomic_formula_of_names_equality = "atomic_formula_of_names_equality";
atomic_formula_of_names_type const atomic_formula_of_names = "atomic_formula_of_names";
ground_atom_type const ground_atom = "ground_atom";
negated_ground_atom_type const negated_ground_atom = "negated_ground_atom";
ground_literal_type const ground_literal = "ground_literal";

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
preference_constraint_goal_descriptor_preference_type const preference_constraint_goal_descriptor_preference =
    "preference_constraint_goal_descriptor_preference";
preference_constraint_goal_descriptor_simple_type const preference_constraint_goal_descriptor_simple = "preference_constraint_goal_descriptor_simple";

problem_keyword_type const problem_keyword = "problem";
problem_name_type const problem_name = "problem_name";
problem_domain_name_type const problem_domain_name = "problem_domain_name";
objects_type const objects = "objects";
initial_type const initial = "initial";
goal_type const goal = "goal";
problem_constraints_type const problem_constraints = "problem_constraints";
metric_specification_type const metric_specification = "metric_specification";

problem_type const problem = "problem";

///////////////////////////////////////////////////////////////////////////
// Grammar
///////////////////////////////////////////////////////////////////////////

/**
 * Domain
 */
const auto name_def = raw[lexeme[alpha >> *(alnum | char_('-') | char_('_'))]];
const auto variable_def = raw[lexeme[char_('?') > name]];
const auto name_total_cost_def = keyword_string("total-cost");
const auto function_symbol_total_cost_def = name_total_cost;
const auto function_symbol_def = name;
const auto term_def = name | variable;
const auto number_def = double_;
const auto predicate_def = name;

const auto requirement_strips_def = keyword_lit(":strips") > x3::attr(ast::RequirementStrips {});
const auto requirement_typing_def = keyword_lit(":typing") > x3::attr(ast::RequirementTyping {});
const auto requirement_negative_preconditions_def = keyword_lit(":negative-preconditions") > x3::attr(ast::RequirementNegativePreconditions {});
const auto requirement_disjunctive_preconditions_def = keyword_lit(":disjunctive-preconditions") > x3::attr(ast::RequirementDisjunctivePreconditions {});
const auto requirement_equality_def = keyword_lit(":equality") > x3::attr(ast::RequirementEquality {});
const auto requirement_existential_preconditions_def = keyword_lit(":existential-preconditions") > x3::attr(ast::RequirementExistentialPreconditions {});
const auto requirement_universal_preconditions_def = keyword_lit(":universal-preconditions") > x3::attr(ast::RequirementUniversalPreconditions {});
const auto requirement_quantified_preconditions_def = keyword_lit(":quantified-preconditions") > x3::attr(ast::RequirementQuantifiedPreconditions {});
const auto requirement_conditional_effects_def = keyword_lit(":conditional-effects") > x3::attr(ast::RequirementConditionalEffects {});
const auto requirement_fluents_def = keyword_lit(":fluents") >> x3::attr(ast::RequirementFluents {});
const auto requirement_object_fluents_def = keyword_lit(":object-fluents") > x3::attr(ast::RequirementObjectFluents {});
const auto requirement_numeric_fluents_def = keyword_lit(":numeric-fluents") > x3::attr(ast::RequirementNumericFluents {});
const auto requirement_adl_def = keyword_lit(":adl") >> x3::attr(ast::RequirementAdl {});
const auto requirement_durative_actions_def = keyword_lit(":durative-actions") > x3::attr(ast::RequirementDurativeActions {});
const auto requirement_derived_predicates_def = keyword_lit(":derived-predicates") > x3::attr(ast::RequirementDerivedPredicates {});
const auto requirement_timed_initial_literals_def = keyword_lit(":timed-initial-literals") > x3::attr(ast::RequirementTimedInitialLiterals {});
const auto requirement_preferences_def = keyword_lit(":preferences") > x3::attr(ast::RequirementPreferences {});
const auto requirement_constraints_def = keyword_lit(":constraints") > x3::attr(ast::RequirementConstraints {});
const auto requirement_action_costs_def = keyword_lit(":action-costs") > x3::attr(ast::RequirementActionCosts {});
const auto requirement_non_deterministic_def = keyword_lit(":non-deterministic") > x3::attr(ast::RequirementNonDeterministic {});
const auto requirement_def = requirement_strips | requirement_typing | requirement_negative_preconditions | requirement_disjunctive_preconditions
                             | requirement_equality | requirement_existential_preconditions | requirement_universal_preconditions
                             | requirement_quantified_preconditions | requirement_conditional_effects | requirement_fluents | requirement_object_fluents
                             | requirement_numeric_fluents | requirement_adl | requirement_durative_actions | requirement_derived_predicates
                             | requirement_timed_initial_literals | requirement_preferences | requirement_constraints | requirement_action_costs | requirement_non_deterministic;

const auto type_def = type_object | type_number | type_either | name;
const auto type_object_def = keyword_lit("object") > x3::attr(ast::TypeObject {});
const auto type_number_def = keyword_lit("number") > x3::attr(ast::TypeNumber {});
const auto type_either_def = (lit('(') >> keyword_lit("either") > +type) > lit(')');
const auto typed_list_of_names_recursively_def = (+name >> lit('-')) > type > typed_list_of_names;
const auto typed_list_of_names_def = typed_list_of_names_recursively | *name;
const auto typed_list_of_variables_recursively_def = (+variable >> lit('-')) > type > typed_list_of_variables;
const auto typed_list_of_variables_def = typed_list_of_variables_recursively | *variable;

const auto atomic_formula_skeleton_def = lit('(') > predicate > typed_list_of_variables > lit(')');

const auto atomic_function_skeleton_total_cost_def = lit('(') >> function_symbol_total_cost > lit(')');
const auto atomic_function_skeleton_general_def = lit('(') > function_symbol > typed_list_of_variables > lit(')');
const auto atomic_function_skeleton_def = atomic_function_skeleton_total_cost | atomic_function_skeleton_general;
const auto function_typed_list_of_atomic_function_skeletons_recursively_def =
    (+atomic_function_skeleton >> lit('-')) > type_number > -function_typed_list_of_atomic_function_skeletons;
const auto function_typed_list_of_atomic_function_skeletons_def = function_typed_list_of_atomic_function_skeletons_recursively | +atomic_function_skeleton;

const auto atomic_formula_of_terms_predicate_def = (lit('(') >> predicate) > *term > lit(')');
const auto atomic_formula_of_terms_equality_def = (lit('(') >> lit('=')) >> term > term > lit(')');
const auto atomic_formula_of_terms_def = atomic_formula_of_terms_equality | atomic_formula_of_terms_predicate;
const auto atom_def = atomic_formula_of_terms;
const auto negated_atom_def = (lit('(') >> keyword_lit("not")) > atomic_formula_of_terms > lit(')');
const auto literal_def = negated_atom | atom;

const auto multi_operator_mul_def = lit('*') > x3::attr(ast::MultiOperatorMul {});
const auto multi_operator_plus_def = lit('+') > x3::attr(ast::MultiOperatorPlus {});
const auto multi_operator_def = multi_operator_mul | multi_operator_plus;
const auto binary_operator_minus_def = lit('-') > x3::attr(ast::BinaryOperatorMinus {});
const auto binary_operator_div_def = lit('/') > x3::attr(ast::BinaryOperatorDiv {});
const auto binary_operator_def = binary_operator_minus | binary_operator_div | multi_operator;

const auto binary_comparator_greater_def = lit('>') > x3::attr(ast::BinaryComparatorGreater {});
const auto binary_comparator_less_def = lit('<') > x3::attr(ast::BinaryComparatorLess {});
const auto binary_comparator_equal_def = lit('=') > x3::attr(ast::BinaryComparatorEqual {});
const auto binary_comparator_greater_equal_def = lit(">=") > x3::attr(ast::BinaryComparatorGreaterEqual {});
const auto binary_comparator_less_equal_def = lit("<=") > x3::attr(ast::BinaryComparatorLessEqual {});
const auto binary_comparator_def =
    binary_comparator_greater | binary_comparator_less | binary_comparator_equal | binary_comparator_greater_equal | binary_comparator_less_equal;

const auto function_head_def = ((lit('(') >> function_symbol > *term) > lit(')')) | (function_symbol > x3::attr(std::vector<ast::Term> {}));
const auto function_expression_def = function_expression_binary_op | function_expression_minus | function_expression_head | function_expression_number;
const auto function_expression_number_def = number;
// distinguishing unary from binary minus requires some more backtracking
const auto function_expression_binary_op_def = (lit('(') >> binary_operator >> function_expression >> function_expression) > lit(')');
const auto function_expression_minus_def = (lit('(') >> lit('-')) >> function_expression > lit(')');
const auto function_expression_head_def = function_head;

const auto goal_descriptor_def = goal_descriptor_not | goal_descriptor_and | goal_descriptor_or | goal_descriptor_imply | goal_descriptor_exists
                                 | goal_descriptor_forall | goal_descriptor_function_comparison | goal_descriptor_atom | goal_descriptor_literal;
const auto goal_descriptor_atom_def = atom;
const auto goal_descriptor_literal_def = literal;
const auto goal_descriptor_and_def = (lit('(') >> keyword_lit("and")) > *goal_descriptor > lit(')');
const auto goal_descriptor_or_def = (lit('(') >> keyword_lit("or") > *goal_descriptor) > lit(')');
const auto goal_descriptor_not_def = (lit('(') >> keyword_lit("not")) > goal_descriptor > lit(')');
const auto goal_descriptor_imply_def = (lit('(') >> keyword_lit("imply")) > goal_descriptor > goal_descriptor > lit(')');
const auto goal_descriptor_exists_def = (lit('(') >> keyword_lit("exists")) > lit('(') > typed_list_of_variables > lit(')') > goal_descriptor > lit(')');
const auto goal_descriptor_forall_def = (lit('(') >> keyword_lit("forall")) > lit('(') > typed_list_of_variables > lit(')') > goal_descriptor > lit(')');
const auto goal_descriptor_function_comparison_def = (lit('(') >> binary_comparator) >> function_expression > function_expression > lit(')');

const auto constraint_goal_descriptor_def = constraint_goal_descriptor_and | constraint_goal_descriptor_forall | constraint_goal_descriptor_at_end
                                            | constraint_goal_descriptor_always | constraint_goal_descriptor_sometime | constraint_goal_descriptor_within
                                            | constraint_goal_descriptor_at_most_once | constraint_goal_descriptor_sometime_after
                                            | constraint_goal_descriptor_sometime_before | constraint_goal_descriptor_always_within
                                            | constraint_goal_descriptor_hold_during | constraint_goal_descriptor_hold_after;
const auto constraint_goal_descriptor_and_def = (lit('(') >> keyword_lit("and")) > *constraint_goal_descriptor > lit(')');
const auto constraint_goal_descriptor_forall_def = (lit('(') >> keyword_lit("forall")) > lit('(') > typed_list_of_variables > lit(')')
                                                   > constraint_goal_descriptor > lit(')');
const auto constraint_goal_descriptor_at_end_def = (lit('(') >> keyword_lit("at") >> keyword_lit("end")) > goal_descriptor > lit(')');
const auto constraint_goal_descriptor_always_def = (lit('(') >> keyword_lit("always")) > goal_descriptor > lit(')');
const auto constraint_goal_descriptor_sometime_def = (lit('(') >> keyword_lit("sometime")) > goal_descriptor > lit(')');
const auto constraint_goal_descriptor_within_def = (lit('(') >> keyword_lit("within")) > number > goal_descriptor > lit(')');
const auto constraint_goal_descriptor_at_most_once_def = (lit('(') >> keyword_lit("at-most-once")) > goal_descriptor > lit(')');
const auto constraint_goal_descriptor_sometime_after_def = (lit('(') >> keyword_lit("sometime-after")) > goal_descriptor > goal_descriptor > lit(')');
const auto constraint_goal_descriptor_sometime_before_def = (lit('(') >> keyword_lit("sometime-before")) > goal_descriptor > goal_descriptor > lit(')');
const auto constraint_goal_descriptor_always_within_def = (lit('(') >> keyword_lit("always-within")) > number > goal_descriptor > goal_descriptor > lit(')');
const auto constraint_goal_descriptor_hold_during_def = (lit('(') >> keyword_lit("hold-during")) > number > number > goal_descriptor > lit(')');
const auto constraint_goal_descriptor_hold_after_def = (lit('(') >> keyword_lit("hold-after")) > number > goal_descriptor > lit(')');

const auto precondition_goal_descriptor_def =
    precondition_goal_descriptor_and | precondition_goal_descriptor_preference | precondition_goal_descriptor_forall | precondition_goal_descriptor_simple;
const auto preference_name_def = name;
const auto precondition_goal_descriptor_simple_def = goal_descriptor;
const auto precondition_goal_descriptor_and_def = (lit('(') >> keyword_lit("and") > *precondition_goal_descriptor) > lit(')');
const auto precondition_goal_descriptor_preference_def = (lit('(') >> keyword_lit("preference")) > preference_name > goal_descriptor > lit(')');
const auto precondition_goal_descriptor_forall_def = (lit('(') >> keyword_lit("forall")) > lit('(') > typed_list_of_variables > lit(')')
                                                     > precondition_goal_descriptor > lit(')');

const auto assign_operator_assign_def = keyword_lit("assign") > x3::attr(ast::AssignOperatorAssign {});
const auto assign_operator_scale_up_def = keyword_lit("scale-up") > x3::attr(ast::AssignOperatorScaleUp {});
const auto assign_operator_scale_down_def = keyword_lit("scale-down") > x3::attr(ast::AssignOperatorScaleDown {});
const auto assign_operator_increase_def = keyword_lit("increase") > x3::attr(ast::AssignOperatorIncrease {});
const auto assign_operator_decrease_def = keyword_lit("decrease") > x3::attr(ast::AssignOperatorDecrease {});
const auto assign_operator_def =
    assign_operator_assign | assign_operator_scale_up | assign_operator_scale_down | assign_operator_increase | assign_operator_decrease;

// For action cost effects only
const auto numeric_term_def = function_expression_number | function_expression_head;

const auto effect_root_deterministic_def = ((lit('(') >> keyword_lit("and")) > *effect_numeric_fluent_total_cost_or_effect > lit(')')) | effect_conditional
                             | effect_production | effect_production_numeric_fluent_total_cost;
const auto effect_root_non_deterministic_def = ((lit('(') >> keyword_lit("oneof")) > *effect_root_deterministic > lit(')'));
const auto effect_root_def = effect_root_non_deterministic | effect_root_deterministic;
const auto effect_def = ((lit('(') >> keyword_lit("and")) > *effect > lit(')')) | effect_conditional | effect_production;
const auto effect_numeric_fluent_total_cost_or_effect_def = effect_production_numeric_fluent_total_cost | effect;
const auto effect_production_literal_def = literal;
const auto effect_production_numeric_fluent_total_cost_def = (lit('(') >> assign_operator_increase >> lit('(') >> function_symbol_total_cost) > lit(')')
                                                             > numeric_term > lit(')');
const auto effect_production_numeric_fluent_general_def = (lit('(') >> assign_operator >> function_head >> function_expression) > lit(')');
const auto effect_production_def = effect_production_numeric_fluent_general | effect_production_literal;
const auto effect_conditional_forall_def = (lit('(') >> keyword_lit("forall")) > lit("(") > typed_list_of_variables > lit(')') > effect > lit(')');
const auto effect_conditional_when_def = (lit('(') >> keyword_lit("when")) > goal_descriptor > effect > lit(')');
const auto effect_conditional_def = effect_conditional_forall | effect_conditional_when;

const auto action_symbol_def = name;
const auto action_body_def = -(keyword_lit(":precondition") > ((lit('(') >> lit(')')) | precondition_goal_descriptor))
                             > -(keyword_lit(":effect") > ((lit('(') >> lit(')')) | effect_root));
const auto action_def = (lit('(') >> keyword_lit(":action")) > action_symbol > keyword_lit(":parameters") > lit('(') > typed_list_of_variables > lit(')')
                        > action_body > lit(')');

const auto axiom_def = (lit('(') >> keyword_lit(":derived")) > atomic_formula_skeleton > goal_descriptor > lit(')');

const auto define_keyword_def = keyword_lit("define");
const auto domain_keyword_def = keyword_lit("domain");
const auto domain_name_def = (lit('(') > domain_keyword) > name > lit(')');
const auto requirements_def = lit('(') >> keyword_lit(":requirements") > *requirement >> lit(')');
const auto types_def = (lit('(') >> keyword_lit(":types") > typed_list_of_names) > lit(')');
const auto constants_def = (lit('(') >> keyword_lit(":constants") > typed_list_of_names) > lit(')');
const auto predicates_def = (lit('(') >> keyword_lit(":predicates") > *atomic_formula_skeleton) > lit(')');
const auto functions_def = (lit('(') >> keyword_lit(":functions") > *function_typed_list_of_atomic_function_skeletons) > lit(')');
const auto constraints_def = (lit('(') >> keyword_lit(":constraints")) > constraint_goal_descriptor > lit(')');
const auto structure_def = action | axiom;

const auto domain_def = lit('(') > define_keyword > domain_name > -requirements > -types > -constants > -predicates > -functions > -constraints > *structure
                        > lit(')');

/**
 * Problem
 */

const auto basic_function_term_def = ((lit('(') > function_symbol > *name) > lit(')')) | (function_symbol > x3::attr(std::vector<ast::Name> {}));

const auto atomic_formula_of_names_predicate_def = (lit('(') >> predicate) > *name > lit(')');
const auto atomic_formula_of_names_equality_def = (lit('(') >> lit('=')) > name > name > lit(')');
const auto atomic_formula_of_names_def = atomic_formula_of_names_equality | atomic_formula_of_names_predicate;
const auto ground_atom_def = atomic_formula_of_names;
const auto negated_ground_atom_def = lit('(') >> keyword_lit("not") > atomic_formula_of_names > lit(')');
const auto ground_literal_def = negated_ground_atom | ground_atom;

const auto initial_element_literals_def = ground_literal;
const auto initial_element_timed_literals_def = (lit('(') >> lit("at") >> number) > ground_literal > lit(')');
const auto initial_element_numeric_fluents_total_cost_def = (lit('(') >> lit('=') >> lit('(') >> function_symbol_total_cost) > lit(')') > number > lit(')');
const auto initial_element_numeric_fluents_general_def = (lit('(') >> lit('=') >> basic_function_term) > number > lit(')');
const auto initial_element_def =
    initial_element_timed_literals | initial_element_numeric_fluents_total_cost | initial_element_numeric_fluents_general | initial_element_literals;

const auto metric_function_expression_binary_operator_def = (lit('(') >> binary_operator >> metric_function_expression >> metric_function_expression)
                                                            > lit(')');
const auto metric_function_expression_multi_operator_def = (lit('(') >> multi_operator >> metric_function_expression >> +metric_function_expression) > lit(')');
const auto metric_function_expression_minus_def = (lit('(') >> lit('-')) > metric_function_expression > lit(')');
const auto metric_function_expression_preferences_def = (lit('(') >> keyword_lit("is-violated")) > preference_name > lit(')');
const auto metric_function_expression_basic_function_term_def = basic_function_term;
const auto metric_function_expression_total_time_def = keyword_lit("total-time") > x3::attr(ast::MetricFunctionExpressionTotalTime {});
const auto metric_function_expression_number_def = number;
const auto metric_function_expression_def = metric_function_expression_binary_operator | metric_function_expression_multi_operator
                                            | metric_function_expression_minus | metric_function_expression_preferences
                                            | metric_function_expression_basic_function_term | metric_function_expression_total_time
                                            | metric_function_expression_number;

const auto optimization_minimize_def = keyword_lit("minimize") > x3::attr(ast::OptimizationMinimize {});
const auto optimization_maximize_def = keyword_lit("maximize") > x3::attr(ast::OptimizationMaximize {});
const auto optimization_def = optimization_minimize | optimization_maximize;
const auto metric_specification_total_cost_def = optimization_minimize >> lit('(') >> function_symbol_total_cost > lit(')');
const auto metric_specification_general_def = optimization >> metric_function_expression;

const auto preference_constraint_goal_descriptor_and_def = (lit('(') >> keyword_lit("and")) > *preference_constraint_goal_descriptor > lit(')');
const auto preference_constraint_goal_descriptor_forall_def = (lit('(') >> keyword_lit("forall")) > typed_list_of_variables
                                                              > preference_constraint_goal_descriptor > lit(')');
const auto preference_constraint_goal_descriptor_preference_def = (lit('(') >> keyword_lit("preference") > (-preference_name)) > constraint_goal_descriptor
                                                                  > lit(')');
const auto preference_constraint_goal_descriptor_simple_def = constraint_goal_descriptor;
const auto preference_constraint_goal_descriptor_def = preference_constraint_goal_descriptor_and | preference_constraint_goal_descriptor_forall
                                                       | preference_constraint_goal_descriptor_preference | preference_constraint_goal_descriptor_simple;

const auto problem_keyword_def = keyword_lit("problem");
const auto problem_name_def = lit('(') > problem_keyword > name > lit(')');
const auto problem_domain_name_def = (lit('(') >> keyword_lit(":domain")) > name > lit(')');
const auto objects_def = (lit('(') >> keyword_lit(":objects")) > typed_list_of_names > lit(')');
const auto initial_def = (lit('(') >> keyword_lit(":init")) > *initial_element > lit(')');
const auto goal_def = (lit('(') >> keyword_lit(":goal")) > precondition_goal_descriptor > lit(')');
const auto problem_constraints_def = (lit('(') >> keyword_lit(":constraints")) > preference_constraint_goal_descriptor > lit(')');
const auto metric_specification_def = (lit('(') >> keyword_lit(":metric")) > (metric_specification_total_cost | metric_specification_general) > lit(')');

const auto problem_def = lit('(') > define_keyword > problem_name > problem_domain_name > -requirements > -objects > -predicates > -initial > -goal
                         > -problem_constraints > -metric_specification > -(*axiom) > lit(')');

/**
 * Domain
 */
BOOST_SPIRIT_DEFINE(name, variable, name_total_cost, function_symbol_total_cost, function_symbol, term, number, predicate)

BOOST_SPIRIT_DEFINE(requirement_strips,
                    requirement_typing,
                    requirement_negative_preconditions,
                    requirement_disjunctive_preconditions,
                    requirement_equality,
                    requirement_existential_preconditions,
                    requirement_universal_preconditions,
                    requirement_quantified_preconditions,
                    requirement_conditional_effects,
                    requirement_fluents,
                    requirement_object_fluents,
                    requirement_numeric_fluents,
                    requirement_adl,
                    requirement_durative_actions,
                    requirement_derived_predicates,
                    requirement_timed_initial_literals,
                    requirement_preferences,
                    requirement_constraints,
                    requirement_action_costs,
                    requirement_non_deterministic,
                    requirement)

BOOST_SPIRIT_DEFINE(type,
                    type_object,
                    type_number,
                    type_either,
                    typed_list_of_names_recursively,
                    typed_list_of_names,
                    typed_list_of_variables_recursively,
                    typed_list_of_variables)

BOOST_SPIRIT_DEFINE(atomic_formula_skeleton)

BOOST_SPIRIT_DEFINE(atomic_function_skeleton_total_cost,
                    atomic_function_skeleton_general,
                    atomic_function_skeleton,
                    function_typed_list_of_atomic_function_skeletons_recursively,
                    function_typed_list_of_atomic_function_skeletons)

BOOST_SPIRIT_DEFINE(atomic_formula_of_terms_predicate, atomic_formula_of_terms_equality, atomic_formula_of_terms, atom, negated_atom, literal)

BOOST_SPIRIT_DEFINE(multi_operator_mul, multi_operator_plus, multi_operator, binary_operator_minus, binary_operator_div, binary_operator)

BOOST_SPIRIT_DEFINE(binary_comparator_greater,
                    binary_comparator_less,
                    binary_comparator_equal,
                    binary_comparator_greater_equal,
                    binary_comparator_less_equal,
                    binary_comparator)

BOOST_SPIRIT_DEFINE(function_head,
                    function_expression,
                    function_expression_number,
                    function_expression_binary_op,
                    function_expression_minus,
                    function_expression_head)

BOOST_SPIRIT_DEFINE(goal_descriptor,
                    goal_descriptor_atom,
                    goal_descriptor_literal,
                    goal_descriptor_and,
                    goal_descriptor_or,
                    goal_descriptor_not,
                    goal_descriptor_imply,
                    goal_descriptor_exists,
                    goal_descriptor_forall,
                    goal_descriptor_function_comparison)

BOOST_SPIRIT_DEFINE(constraint_goal_descriptor,
                    constraint_goal_descriptor_and,
                    constraint_goal_descriptor_forall,
                    constraint_goal_descriptor_at_end,
                    constraint_goal_descriptor_always,
                    constraint_goal_descriptor_sometime,
                    constraint_goal_descriptor_within,
                    constraint_goal_descriptor_at_most_once,
                    constraint_goal_descriptor_sometime_after,
                    constraint_goal_descriptor_sometime_before,
                    constraint_goal_descriptor_always_within,
                    constraint_goal_descriptor_hold_during,
                    constraint_goal_descriptor_hold_after)

BOOST_SPIRIT_DEFINE(preference_name,
                    precondition_goal_descriptor,
                    precondition_goal_descriptor_simple,
                    precondition_goal_descriptor_and,
                    precondition_goal_descriptor_preference,
                    precondition_goal_descriptor_forall)

BOOST_SPIRIT_DEFINE(assign_operator_assign,
                    assign_operator_scale_up,
                    assign_operator_scale_down,
                    assign_operator_increase,
                    assign_operator_decrease,
                    assign_operator)

BOOST_SPIRIT_DEFINE(numeric_term)

BOOST_SPIRIT_DEFINE(effect,
                    effect_production_literal,
                    effect_production_numeric_fluent_total_cost,
                    effect_production_numeric_fluent_general,
                    effect_production,
                    effect_conditional_forall,
                    effect_conditional_when,
                    effect_conditional,
                    effect_numeric_fluent_total_cost_or_effect,
                    effect_root_deterministic,
                    effect_root_non_deterministic,
                    effect_root,
                    action_symbol,
                    action_body,
                    action,
                    axiom)

BOOST_SPIRIT_DEFINE(define_keyword, domain_keyword, domain_name, requirements, types, constants, predicates, functions, constraints, structure, domain)

/**
 * Problem
 */
BOOST_SPIRIT_DEFINE(basic_function_term)

BOOST_SPIRIT_DEFINE(atomic_formula_of_names_predicate,
                    atomic_formula_of_names_equality,
                    atomic_formula_of_names,
                    ground_atom,
                    negated_ground_atom,
                    ground_literal)

BOOST_SPIRIT_DEFINE(initial_element_literals,
                    initial_element_timed_literals,
                    initial_element_numeric_fluents_total_cost,
                    initial_element_numeric_fluents_general,
                    initial_element)

BOOST_SPIRIT_DEFINE(metric_function_expression,
                    metric_function_expression_number,
                    metric_function_expression_binary_operator,
                    metric_function_expression_multi_operator,
                    metric_function_expression_minus,
                    metric_function_expression_basic_function_term,
                    metric_function_expression_total_time,
                    metric_function_expression_preferences)

BOOST_SPIRIT_DEFINE(optimization_minimize, optimization_maximize, optimization, metric_specification_total_cost, metric_specification_general)

BOOST_SPIRIT_DEFINE(preference_constraint_goal_descriptor,
                    preference_constraint_goal_descriptor_and,
                    preference_constraint_goal_descriptor_forall,
                    preference_constraint_goal_descriptor_preference,
                    preference_constraint_goal_descriptor_simple)

BOOST_SPIRIT_DEFINE(problem_keyword, problem_name, problem_domain_name, objects, initial, goal, problem_constraints, metric_specification, problem)

///////////////////////////////////////////////////////////////////////////
// Annotation and Error handling
///////////////////////////////////////////////////////////////////////////

/**
 * Domain
 */
struct NameClass : x3::annotate_on_success
{
};
struct VariableClass : x3::annotate_on_success
{
};
struct NameTotalCostClass : x3::annotate_on_success
{
};
struct FunctionSymbolTotalCostClass : x3::annotate_on_success
{
};
struct FunctionSymbolClass : x3::annotate_on_success
{
};
struct TermClass : x3::annotate_on_success
{
};
struct NumberClass : x3::annotate_on_success
{
};
struct PredicateClass : x3::annotate_on_success
{
};

struct RequirementStripsClass : x3::annotate_on_success
{
};
struct RequirementTypingClass : x3::annotate_on_success
{
};
struct RequirementNegativePreconditionsClass : x3::annotate_on_success
{
};
struct RequirementDisjunctivePreconditionsClass : x3::annotate_on_success
{
};
struct RequirementEqualityClass : x3::annotate_on_success
{
};
struct RequirementExistentialPreconditionsClass : x3::annotate_on_success
{
};
struct RequirementUniversalPreconditionsClass : x3::annotate_on_success
{
};
struct RequirementQuantifiedPreconditionsClass : x3::annotate_on_success
{
};
struct RequirementConditionalEffectsClass : x3::annotate_on_success
{
};
struct RequirementFluentsClass : x3::annotate_on_success
{
};
struct RequirementObjectFluentsClass : x3::annotate_on_success
{
};
struct RequirementNumericalFluentsClass : x3::annotate_on_success
{
};
struct RequirementAdlClass : x3::annotate_on_success
{
};
struct RequirementDurativeActionsClass : x3::annotate_on_success
{
};
struct RequirementTimedInitialLiteralsClass : x3::annotate_on_success
{
};
struct RequirementPreferencesClass : x3::annotate_on_success
{
};
struct RequirementConstraintsClass : x3::annotate_on_success
{
};
struct RequirementActionCostsClass : x3::annotate_on_success
{
};
struct RequirementNonDeterministic : x3::annotate_on_success
{
};
struct RequirementClass : x3::annotate_on_success
{
};

struct TypeClass : x3::annotate_on_success
{
};
struct TypeObjectClass : x3::annotate_on_success
{
};
struct TypeNumberClass : x3::annotate_on_success
{
};
struct TypeEitherClass : x3::annotate_on_success
{
};
struct TypedListOfNamesRecursivelyClass : x3::annotate_on_success
{
};
struct TypedListOfNamesClass : x3::annotate_on_success
{
};
struct TypedListOfVariablesRecursivelyClass : x3::annotate_on_success
{
};
struct TypedListOfVariablesClass : x3::annotate_on_success
{
};

struct AtomicFormulaSkeletonClass : x3::annotate_on_success
{
};

struct AtomicFunctionSkeletonTotalCostClass : x3::annotate_on_success
{
};
struct AtomicFunctionSkeletonGeneralClass : x3::annotate_on_success
{
};
struct AtomicFunctionSkeletonClass : x3::annotate_on_success
{
};
struct FunctionTypedListOfAtomicFunctionSkeletonsRecursivelyClass : x3::annotate_on_success
{
};
struct FunctionTypedListOfAtomicFunctionSkeletonsClass : x3::annotate_on_success
{
};

struct AtomicFormulaOfTermsPredicateClass : x3::annotate_on_success
{
};
struct AtomicFormulaOfTermsEqualityClass : x3::annotate_on_success
{
};
struct AtomicFormulaOfTermsClass : x3::annotate_on_success
{
};
struct AtomClass : x3::annotate_on_success
{
};
struct NegatedAtomClass : x3::annotate_on_success
{
};
struct LiteralClass : x3::annotate_on_success
{
};

struct MultiOperatorMulClass : x3::annotate_on_success
{
};
struct MultiOperatorPlusClass : x3::annotate_on_success
{
};
struct MultiOperatorClass : x3::annotate_on_success
{
};
struct BinaryOperatorMinusClass : x3::annotate_on_success
{
};
struct BinaryOperatorDivClass : x3::annotate_on_success
{
};
struct BinaryOperatorClass : x3::annotate_on_success
{
};

struct BinaryComparatorGreaterClass : x3::annotate_on_success
{
};
struct BinaryComparatorLessClass : x3::annotate_on_success
{
};
struct BinaryComparatorEqualClass : x3::annotate_on_success
{
};
struct BinaryComparatorGreaterEqualClass : x3::annotate_on_success
{
};
struct BinaryComparatorLessEqualClass : x3::annotate_on_success
{
};
struct BinaryComparatorClass : x3::annotate_on_success
{
};

struct FunctionExpressionClass : x3::annotate_on_success
{
};
struct FunctionHeadClass : x3::annotate_on_success
{
};
struct FunctionExpressionNumberClass : x3::annotate_on_success
{
};
struct FunctionExpressionBinaryOpClass : x3::annotate_on_success
{
};
struct FunctionExpressionMinusClass : x3::annotate_on_success
{
};
struct FunctionExpressionHeadClass : x3::annotate_on_success
{
};

struct GoalDescriptorClass : x3::annotate_on_success
{
};
struct GoalDescriptorAtomClass : x3::annotate_on_success
{
};
struct GoalDescriptorLiteralClass : x3::annotate_on_success
{
};
struct GoalDescriptorAndClass : x3::annotate_on_success
{
};
struct GoalDescriptorOrClass : x3::annotate_on_success
{
};
struct GoalDescriptorNotClass : x3::annotate_on_success
{
};
struct GoalDescriptorImplyClass : x3::annotate_on_success
{
};
struct GoalDescriptorExistsClass : x3::annotate_on_success
{
};
struct GoalDescriptorForallClass : x3::annotate_on_success
{
};
struct GoalDescriptorFunctionComparisonClass : x3::annotate_on_success
{
};

struct ConstraintGoalDescriptorClass : x3::annotate_on_success
{
};
struct ConstraintGoalDescriptorAndClass : x3::annotate_on_success
{
};
struct ConstraintGoalDescriptorForallClass : x3::annotate_on_success
{
};
struct ConstraintGoalDescriptorAtEndClass : x3::annotate_on_success
{
};
struct ConstraintGoalDescriptorAlwaysClass : x3::annotate_on_success
{
};
struct ConstraintGoalDescriptorSometimeClass : x3::annotate_on_success
{
};
struct ConstraintGoalDescriptorWithinClass : x3::annotate_on_success
{
};
struct ConstraintGoalDescriptorAtMostOnceClass : x3::annotate_on_success
{
};
struct ConstraintGoalDescriptorSometimeAfterClass : x3::annotate_on_success
{
};
struct ConstraintGoalDescriptorSometimeBeforeClass : x3::annotate_on_success
{
};
struct ConstraintGoalDescriptorAlwaysWithinClass : x3::annotate_on_success
{
};
struct ConstraintGoalDescriptorHoldDuringClass : x3::annotate_on_success
{
};
struct ConstraintGoalDescriptorHoldAfterClass : x3::annotate_on_success
{
};

struct PreferenceNameClass : x3::annotate_on_success
{
};
struct PreconditionGoalDescriptorClass : x3::annotate_on_success
{
};
struct PreconditionGoalDescriptorSimpleClass : x3::annotate_on_success
{
};
struct PreconditionGoalDescriptorAndClass : x3::annotate_on_success
{
};
struct PreconditionGoalDescriptorPreferenceClass : x3::annotate_on_success
{
};
struct PreconditionGoalDescriptorForallClass : x3::annotate_on_success
{
};

struct AssignOperatorAssignClass : x3::annotate_on_success
{
};
struct AssignOperatorScaleUpClass : x3::annotate_on_success
{
};
struct AssignOperatorScaleDownClass : x3::annotate_on_success
{
};
struct AssignOperatorIncreaseClass : x3::annotate_on_success
{
};
struct AssignOperatorDecreaseClass : x3::annotate_on_success
{
};
struct AssignOperatorClass : x3::annotate_on_success
{
};

struct NumericTermClass : x3::annotate_on_success
{
};

struct EffectClass : x3::annotate_on_success
{
};
struct EffectProductionLiteralClass : x3::annotate_on_success
{
};
struct EffectProductionNumericFluentTotalCostClass : x3::annotate_on_success
{
};
struct EffectProductionNumericFluentGeneralClass : x3::annotate_on_success
{
};
struct EffectProductionClass : x3::annotate_on_success
{
};
struct EffectConditionalForallClass : x3::annotate_on_success
{
};
struct EffectConditionalWhenClass : x3::annotate_on_success
{
};
struct EffectConditionalClass : x3::annotate_on_success
{
};
struct EffectNumericFluentTotalCostOrEffectClass : x3::annotate_on_success
{
};
struct EffectRootDeterministicClass : x3::annotate_on_success
{
};
struct EffectRootNonDeterministicClass : x3::annotate_on_success
{
};
struct EffectRootClass : x3::annotate_on_success
{
};

struct ActionSymbolClass : x3::annotate_on_success
{
};
struct ActionBodyClass : x3::annotate_on_success
{
};
struct ActionClass : x3::annotate_on_success
{
};
struct DurativeActionClass : x3::annotate_on_success
{
};
struct AxiomClass : x3::annotate_on_success
{
};

struct DomainNameClass : x3::annotate_on_success
{
};
struct RequirementsClass : x3::annotate_on_success
{
};
struct TypesClass : x3::annotate_on_success
{
};
struct ConstantsClass : x3::annotate_on_success
{
};
struct PredicatesClass : x3::annotate_on_success
{
};
struct FunctionsClass : x3::annotate_on_success
{
};
struct ConstraintsClass : x3::annotate_on_success
{
};
struct StructureClass : x3::annotate_on_success
{
};
struct DomainClass : x3::annotate_on_success, error_handler_base
{
};

/**
 * Problem
 */

struct BasicFunctionTermArityGreaterZeroClass : x3::annotate_on_success
{
};
struct BasicFunctionTermArityZeroClass : x3::annotate_on_success
{
};
struct BasicFunctionTermClass : x3::annotate_on_success
{
};

struct AtomicFormulaOfNamesPredicateClass : x3::annotate_on_success
{
};
struct AtomicFormulaOfNamesEqualityClass : x3::annotate_on_success
{
};
struct AtomicFormulaOfNamesClass : x3::annotate_on_success
{
};
struct GroundAtomClass : x3::annotate_on_success
{
};
struct NegatedGroundAtomClass : x3::annotate_on_success
{
};
struct GroundLiteralClass : x3::annotate_on_success
{
};

struct InitialElementLiteralClass : x3::annotate_on_success
{
};
struct InitialElementTimedLiteralsClass : x3::annotate_on_success
{
};
struct InitialElementNumericFluentsTotalCostClass : x3::annotate_on_success
{
};
struct InitialElementNumericFluentsGeneralClass : x3::annotate_on_success
{
};
struct InitialElementClass : x3::annotate_on_success
{
};

struct MetricFunctionExpressionClass : x3::annotate_on_success
{
};
struct MetricFunctionExpressionNumberClass : x3::annotate_on_success
{
};
struct MetricFunctionExpressionBinaryOperatorClass : x3::annotate_on_success
{
};
struct MetricFunctionExpressionMultiOperatorClass : x3::annotate_on_success
{
};
struct MetricFunctionExpressionMinusClass : x3::annotate_on_success
{
};
struct MetricFunctionExpressionBasicFunctionTermClass : x3::annotate_on_success
{
};
struct MetricFunctionExpressionTotalTimeClass : x3::annotate_on_success
{
};
struct MetricFunctionExpressionPreferencesClass : x3::annotate_on_success
{
};

struct OptimizationMinimizeClass : x3::annotate_on_success
{
};
struct OptimizationMaximizeClass : x3::annotate_on_success
{
};
struct OptimizationClass : x3::annotate_on_success
{
};
struct MetricSpecificationTotalCostClass : x3::annotate_on_success
{
};
struct MetricSpecificationGeneralClass : x3::annotate_on_success
{
};

struct PreferenceConstraintGoalDescriptorClass : x3::annotate_on_success
{
};
struct PreferenceConstraintGoalDescriptorAndClass : x3::annotate_on_success
{
};
struct PreferenceConstraintGoalDescriptorForallClass : x3::annotate_on_success
{
};
struct PreferenceConstraintGoalDescriptorPreferenceClass : x3::annotate_on_success
{
};
struct PreferenceConstraintGoalDescriptorSimpleClass : x3::annotate_on_success
{
};

struct ProblemKeywordClass : x3::annotate_on_success
{
};
struct ProblemNameClass : x3::annotate_on_success
{
};
struct ProblemDomainNameClass : x3::annotate_on_success
{
};
struct ObjectsClass : x3::annotate_on_success
{
};
struct InitialClass : x3::annotate_on_success
{
};
struct GoalClass : x3::annotate_on_success
{
};
struct ProblemConstraintsClass : x3::annotate_on_success
{
};
struct MetricSpecificationClass : x3::annotate_on_success
{
};

struct ProblemClass : x3::annotate_on_success, error_handler_base
{
};
}

namespace loki
{
/**
 * Domain
 */
parser::define_keyword_type const& define_keyword() { return parser::define_keyword; }
parser::domain_keyword_type const& domain_keyword() { return parser::domain_keyword; }

parser::name_type const& name() { return parser::name; }
parser::variable_type const& variable() { return parser::variable; }
parser::name_total_cost_type const& name_total_cost() { return parser::name_total_cost; }
parser::function_symbol_total_cost_type const& function_symbol_total_cost() { return parser::function_symbol_total_cost; }
parser::function_symbol_type const& function_symbol() { return parser::function_symbol; }
parser::term_type const& term() { return parser::term; }
parser::number_type const& number() { return parser::number; }
parser::predicate_type const& predicate() { return parser::predicate; }

parser::requirement_strips_type const& requirement_strips() { return parser::requirement_strips; }
parser::requirement_typing_type const& requirement_typing() { return parser::requirement_typing; }
parser::requirement_negative_preconditions_type const& requirement_negative_preconditions() { return parser::requirement_negative_preconditions; }
parser::requirement_disjunctive_preconditions_type const& requirement_disjunctive_preconditions() { return parser::requirement_disjunctive_preconditions; }
parser::requirement_equality_type const& requirement_equality() { return parser::requirement_equality; }
parser::requirement_existential_preconditions_type const& requirement_existential_preconditions() { return parser::requirement_existential_preconditions; }
parser::requirement_universal_preconditions_type const& requirement_universal_preconditions() { return parser::requirement_universal_preconditions; }
parser::requirement_quantified_preconditions_type const& requirement_quantified_preconditions() { return parser::requirement_quantified_preconditions; }
parser::requirement_conditional_effects_type const& requirement_conditional_effects() { return parser::requirement_conditional_effects; }
parser::requirement_fluents_type const& requirement_fluents() { return parser::requirement_fluents; }
parser::requirement_object_fluents_type const& requirement_object_fluents() { return parser::requirement_object_fluents; }
parser::requirement_numeric_fluents_type const& requirement_numeric_fluents() { return parser::requirement_numeric_fluents; }
parser::requirement_adl_type const& requirement_adl() { return parser::requirement_adl; }
parser::requirement_durative_actions_type const& requirement_durative_actions() { return parser::requirement_durative_actions; }
parser::requirement_derived_predicates_type const& requirement_derived_predicates() { return parser::requirement_derived_predicates; }
parser::requirement_timed_initial_literals_type const& requirement_timed_initial_literals() { return parser::requirement_timed_initial_literals; }
parser::requirement_preferences_type const& requirement_preferences() { return parser::requirement_preferences; }
parser::requirement_constraints_type const& requirement_constraints() { return parser::requirement_constraints; }
parser::requirement_action_costs_type const& requirement_action_costs() { return parser::requirement_action_costs; }
parser::requirement_non_deterministic_type const& requirement_non_deterministic() { return parser::requirement_non_deterministic; }
parser::requirement_type const& requirement() { return parser::requirement; }

parser::type_type const& type() { return parser::type; }
parser::type_object_type const& type_object() { return parser::type_object; }
parser::type_number_type const& type_number() { return parser::type_number; }
parser::type_either_type const& type_either() { return parser::type_either; }
parser::typed_list_of_names_recursively_type const& typed_list_of_names_recursively() { return parser::typed_list_of_names_recursively; }
parser::typed_list_of_names_type const& typed_list_of_names() { return parser::typed_list_of_names; }
parser::typed_list_of_variables_recursively_type const& typed_list_of_variables_recursively() { return parser::typed_list_of_variables_recursively; }
parser::typed_list_of_variables_type const& typed_list_of_variables() { return parser::typed_list_of_variables; }

parser::atomic_formula_skeleton_type const& atomic_formula_skeleton() { return parser::atomic_formula_skeleton; }

parser::atomic_function_skeleton_total_cost_type const& atomic_function_skeleton_total_cost() { return parser::atomic_function_skeleton_total_cost; }
parser::atomic_function_skeleton_general_type const& atomic_function_skeleton_general() { return parser::atomic_function_skeleton_general; }
parser::atomic_function_skeleton_type const& atomic_function_skeleton() { return parser::atomic_function_skeleton; }
parser::function_typed_list_of_atomic_function_skeletons_recursively_type const& function_typed_list_of_atomic_function_skeletons_recursively()
{
    return parser::function_typed_list_of_atomic_function_skeletons_recursively;
}
parser::function_typed_list_of_atomic_function_skeletons_type const& function_typed_list_of_atomic_function_skeletons()
{
    return parser::function_typed_list_of_atomic_function_skeletons;
}

parser::atomic_formula_of_terms_predicate_type const& atomic_formula_of_terms_predicate() { return parser::atomic_formula_of_terms_predicate; }
parser::atomic_formula_of_terms_equality_type const& atomic_formula_of_terms_equality() { return parser::atomic_formula_of_terms_equality; }
parser::atomic_formula_of_terms_type const& atomic_formula_of_terms() { return parser::atomic_formula_of_terms; }
parser::atom_type const& atom() { return parser::atom; }
parser::negated_atom_type const& negated_atom() { return parser::negated_atom; }
parser::literal_type const& literal() { return parser::literal; }

parser::multi_operator_mul_type const& multi_operator_mul() { return parser::multi_operator_mul; }
parser::multi_operator_plus_type const& multi_operator_plus() { return parser::multi_operator_plus; }
parser::multi_operator_type const& multi_operator() { return parser::multi_operator; }
parser::binary_operator_minus_type const& binary_operator_minus() { return parser::binary_operator_minus; }
parser::binary_operator_div_type const& binary_operator_div() { return parser::binary_operator_div; }
parser::binary_operator_type const& binary_operator() { return parser::binary_operator; }

parser::binary_comparator_greater_type const& binary_comparator_greater() { return parser::binary_comparator_greater; }
parser::binary_comparator_less_type const& binary_comparator_less() { return parser::binary_comparator_less; }
parser::binary_comparator_equal_type const& binary_comparator_equal() { return parser::binary_comparator_equal; }
parser::binary_comparator_greater_equal_type const& binary_comparator_greater_equal() { return parser::binary_comparator_greater_equal; }
parser::binary_comparator_less_equal_type const& binary_comparator_less_equal() { return parser::binary_comparator_less_equal; }
parser::binary_comparator_type const& binary_comparator() { return parser::binary_comparator; }

parser::function_expression_type const& function_expression() { return parser::function_expression; }
parser::function_head_type const& function_head() { return parser::function_head; }
parser::function_expression_number_type const& function_expression_number() { return parser::function_expression_number; }
parser::function_expression_binary_op_type const& function_expression_binary_op() { return parser::function_expression_binary_op; }
parser::function_expression_minus_type const& function_expression_minus() { return parser::function_expression_minus; }
parser::function_expression_head_type const& function_expression_head() { return parser::function_expression_head; }

parser::goal_descriptor_type const& goal_descriptor() { return parser::goal_descriptor; }
parser::goal_descriptor_atom_type const& goal_descriptor_atom() { return parser::goal_descriptor_atom; }
parser::goal_descriptor_literal_type const& goal_descriptor_literal() { return parser::goal_descriptor_literal; }
parser::goal_descriptor_and_type const& goal_descriptor_and() { return parser::goal_descriptor_and; }
parser::goal_descriptor_or_type const& goal_descriptor_or() { return parser::goal_descriptor_or; }
parser::goal_descriptor_not_type const& goal_descriptor_not() { return parser::goal_descriptor_not; }
parser::goal_descriptor_imply_type const& goal_descriptor_imply() { return parser::goal_descriptor_imply; }
parser::goal_descriptor_exists_type const& goal_descriptor_exists() { return parser::goal_descriptor_exists; }
parser::goal_descriptor_forall_type const& goal_descriptor_forall() { return parser::goal_descriptor_forall; }
parser::goal_descriptor_function_comparison_type const& goal_descriptor_function_comparison() { return parser::goal_descriptor_function_comparison; }

parser::constraint_goal_descriptor_type const& constraint_goal_descriptor() { return parser::constraint_goal_descriptor; }
parser::constraint_goal_descriptor_and_type const& constraint_goal_descriptor_and() { return parser::constraint_goal_descriptor_and; }
parser::constraint_goal_descriptor_forall_type const& constraint_goal_descriptor_forall() { return parser::constraint_goal_descriptor_forall; }
parser::constraint_goal_descriptor_at_end_type const& constraint_goal_descriptor_at_end() { return parser::constraint_goal_descriptor_at_end; }
parser::constraint_goal_descriptor_always_type const& constraint_goal_descriptor_always() { return parser::constraint_goal_descriptor_always; }
parser::constraint_goal_descriptor_sometime_type const& constraint_goal_descriptor_sometime() { return parser::constraint_goal_descriptor_sometime; }
parser::constraint_goal_descriptor_within_type const& constraint_goal_descriptor_within() { return parser::constraint_goal_descriptor_within; }
parser::constraint_goal_descriptor_at_most_once_type const& constraint_goal_descriptor_at_most_once()
{
    return parser::constraint_goal_descriptor_at_most_once;
}
parser::constraint_goal_descriptor_sometime_after_type const& constraint_goal_descriptor_sometime_after()
{
    return parser::constraint_goal_descriptor_sometime_after;
}
parser::constraint_goal_descriptor_sometime_before_type const& constraint_goal_descriptor_sometime_before()
{
    return parser::constraint_goal_descriptor_sometime_before;
}
parser::constraint_goal_descriptor_always_within_type const& constraint_goal_descriptor_always_within()
{
    return parser::constraint_goal_descriptor_always_within;
}
parser::constraint_goal_descriptor_hold_during_type const& constraint_goal_descriptor_hold_during() { return parser::constraint_goal_descriptor_hold_during; }
parser::constraint_goal_descriptor_hold_after_type const& constraint_goal_descriptor_hold_after() { return parser::constraint_goal_descriptor_hold_after; }

parser::precondition_goal_descriptor_type const& precondition_goal_descriptor() { return parser::precondition_goal_descriptor; }
parser::preference_name_type const& preference_name() { return parser::preference_name; }
parser::precondition_goal_descriptor_simple_type const& precondition_goal_descriptor_simple() { return parser::precondition_goal_descriptor_simple; }
parser::precondition_goal_descriptor_and_type const& precondition_goal_descriptor_and() { return parser::precondition_goal_descriptor_and; }
parser::precondition_goal_descriptor_preference_type const& precondition_goal_descriptor_preference()
{
    return parser::precondition_goal_descriptor_preference;
}
parser::precondition_goal_descriptor_forall_type const& precondition_goal_descriptor_forall() { return parser::precondition_goal_descriptor_forall; }

parser::assign_operator_assign_type const& assign_operator_assign() { return parser::assign_operator_assign; }
parser::assign_operator_scale_up_type const& assign_operator_scale_up() { return parser::assign_operator_scale_up; }
parser::assign_operator_scale_down_type const& assign_operator_scale_down() { return parser::assign_operator_scale_down; }
parser::assign_operator_increase_type const& assign_operator_increase() { return parser::assign_operator_increase; }
parser::assign_operator_decrease_type const& assign_operator_decrease() { return parser::assign_operator_decrease; }
parser::assign_operator_type const& assign_operator() { return parser::assign_operator; }

parser::numeric_term_type const& numeric_term() { return parser::numeric_term; }

parser::effect_type const& effect() { return parser::effect; }
parser::effect_production_literal_type const& effect_production_literal() { return parser::effect_production_literal; }
parser::effect_production_numeric_fluent_total_cost_type const& effect_production_numeric_fluent_total_cost()
{
    return parser::effect_production_numeric_fluent_total_cost;
}
parser::effect_production_numeric_fluent_general_type const& effect_production_numeric_fluent_general()
{
    return parser::effect_production_numeric_fluent_general;
}
parser::effect_production_type const& effect_production() { return parser::effect_production; }
parser::effect_conditional_forall_type const& effect_conditional_forall() { return parser::effect_conditional_forall; }
parser::effect_conditional_when_type const& effect_conditional_when() { return parser::effect_conditional_when; }
parser::effect_conditional_type const& effect_conditional() { return parser::effect_conditional; }
parser::effect_numeric_fluent_total_cost_or_effect_type const& effect_numeric_fluent_total_cost_or_effect()
{
    return parser::effect_numeric_fluent_total_cost_or_effect;
}
parser::effect_root_deterministic_type const& effect_root_deterministic() { return parser::effect_root_deterministic; }
parser::effect_root_non_deterministic_type const& effect_root_non_deterministic() { return parser::effect_root_non_deterministic; }
parser::effect_root_type const& effect_root() { return parser::effect_root; }

parser::action_symbol_type const& action_symbol() { return parser::action_symbol; }
parser::action_body_type const& action_body() { return parser::action_body; }
parser::action_type const& action() { return parser::action; }
parser::axiom_type const& axiom() { return parser::axiom; }

parser::domain_name_type const& domain_name() { return parser::domain_name; }
parser::requirements_type const& requirements() { return parser::requirements; }
parser::types_type const& types() { return parser::types; }
parser::constants_type const& constants() { return parser::constants; }
parser::predicates_type const& predicates() { return parser::predicates; }
parser::functions_type const& functions() { return parser::functions; }
parser::constraints_type const& constraints() { return parser::constraints; }
parser::structure_type const& structure() { return parser::structure; }
parser::domain_type const& domain() { return parser::domain; }

/**
 * Problem
 */
parser::basic_function_term_type const& basic_function_term() { return parser::basic_function_term; }

parser::atomic_formula_of_names_predicate_type const& atomic_formula_of_names_predicate() { return parser::atomic_formula_of_names_predicate; }
parser::atomic_formula_of_names_equality_type const& atomic_formula_of_names_equality() { return parser::atomic_formula_of_names_equality; }
parser::atomic_formula_of_names_type const& atomic_formula_of_names() { return parser::atomic_formula_of_names; }
parser::ground_atom_type const& ground_atom() { return parser::ground_atom; }
parser::negated_ground_atom_type const& negated_ground_atom() { return parser::negated_ground_atom; }
parser::ground_literal_type const& ground_literal() { return parser::ground_literal; }

parser::initial_element_literals_type const& initial_element_literals() { return parser::initial_element_literals; }
parser::initial_element_timed_literals_type const& initial_element_timed_literals() { return parser::initial_element_timed_literals; }
parser::initial_element_numeric_fluents_total_cost_type const& initial_element_numeric_fluents_total_cost()
{
    return parser::initial_element_numeric_fluents_total_cost;
}
parser::initial_element_numeric_fluents_general_type const& initial_element_numeric_fluents_general()
{
    return parser::initial_element_numeric_fluents_general;
}
parser::initial_element_type const& initial_element() { return parser::initial_element; }

parser::metric_function_expression_type const& metric_function_expression() { return parser::metric_function_expression; }
parser::metric_function_expression_number_type const& metric_function_expression_number() { return parser::metric_function_expression_number; }
parser::metric_function_expression_binary_operator_type const& metric_function_expression_binary_operator()
{
    return parser::metric_function_expression_binary_operator;
}
parser::metric_function_expression_multi_operator_type const& metric_function_expression_multi_operator()
{
    return parser::metric_function_expression_multi_operator;
}
parser::metric_function_expression_minus_type const& metric_function_expression_minus() { return parser::metric_function_expression_minus; }
parser::metric_function_expression_basic_function_term_type const& metric_function_expression_basic_function_term()
{
    return parser::metric_function_expression_basic_function_term;
}
parser::metric_function_expression_total_time_type const& metric_function_expression_total_time() { return parser::metric_function_expression_total_time; }
parser::metric_function_expression_preferences_type const& metric_function_expression_preferences() { return parser::metric_function_expression_preferences; }

parser::optimization_minimize_type const& optimization_minimize() { return parser::optimization_minimize; }
parser::optimization_maximize_type const& optimization_maximize() { return parser::optimization_maximize; }
parser::optimization_type const& optimization() { return parser::optimization; }
parser::metric_specification_total_cost_type const& metric_specification_total_cost() { return parser::metric_specification_total_cost; }
parser::metric_specification_general_type const& metric_specification_general() { return parser::metric_specification_general; }

parser::preference_constraint_goal_descriptor_type const& preference_constraint_goal_descriptor() { return parser::preference_constraint_goal_descriptor; }
parser::preference_constraint_goal_descriptor_and_type const& preference_constraint_goal_descriptor_and()
{
    return parser::preference_constraint_goal_descriptor_and;
}
parser::preference_constraint_goal_descriptor_forall_type const& preference_constraint_goal_descriptor_forall()
{
    return parser::preference_constraint_goal_descriptor_forall;
}
parser::preference_constraint_goal_descriptor_preference_type const& preference_constraint_goal_descriptor_preference()
{
    return parser::preference_constraint_goal_descriptor_preference;
}
parser::preference_constraint_goal_descriptor_simple_type const& preference_constraint_goal_descriptor_simple()
{
    return parser::preference_constraint_goal_descriptor_simple;
}

parser::problem_keyword_type const& problem_keyword() { return parser::problem_keyword; }
parser::problem_name_type const& problem_name() { return parser::problem_name; }
parser::problem_domain_name_type const& problem_domain_name() { return parser::problem_domain_name; }
parser::objects_type const& objects() { return parser::objects; }
parser::initial_type const& initial() { return parser::initial; }
parser::goal_type const& goal() { return parser::goal; }
parser::problem_constraints_type const& problem_constraints() { return parser::problem_constraints; }
parser::metric_specification_type const& metric_specification() { return parser::metric_specification; }
parser::problem_type const& problem() { return parser::problem; }
}

#endif
