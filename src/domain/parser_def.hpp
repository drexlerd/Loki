#ifndef LOKI_SRC_DOMAIN_PARSER_DEF_HPP_
#define LOKI_SRC_DOMAIN_PARSER_DEF_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include "include/loki/domain/syntactic/ast.hpp"
#include "include/loki/domain/syntactic/error_handler.hpp"
#include "include/loki/domain/syntactic/parser.hpp"

#include "annotate_on_success.hpp"
#include "ast_adapted.hpp"


namespace loki::domain::parser {
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    using x3::lit;
    using x3::lexeme;
    using x3::eps;
    using x3::int_;
    using x3::double_;

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

    name_type const name = "name";
    variable_type const variable = "variable";
    number_type const number = "number";
    term_type const term = "term";
    undefined_type const undefined = "undefined";

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
    requirement_type const requirement = "requirement";

    type_type const type = "type";
    type_object_type const type_object = "type_object";
    type_either_type const type_either = "type_either";
    typed_list_of_names_recursively_type const typed_list_of_names_recursively = "typed_list_of_names_recursively";
    typed_list_of_names_type const typed_list_of_names = "typed_list_of_names";
    typed_list_of_variables_recursively_type const typed_list_of_variables_recursively = "typed_list_of_variables_recursively";
    typed_list_of_variables_type const typed_list_of_variables = "typed_list_of_variables";

    predicate_type const predicate = "predicate";
    atomic_formula_skeleton_type const atomic_formula_skeleton = "atomic_formula_skeleton";

    function_symbol_type const function_symbol = "function_symbol";
    function_term_type const function_term = "function_term";
    function_type_number_type const function_type_number = "function_type_number";
    function_type_type_type const function_type_type_ = "function_type_type";
    function_type_type const function_type = "function_type";
    atomic_function_skeleton_type const atomic_function_skeleton = "atomic_function_skeleton";
    function_typed_list_of_atomic_function_skeletons_recursively_type const function_typed_list_of_atomic_function_skeletons_recursively = "function_typed_list_of_atomic_function_skeletons_recursively";
    function_typed_list_of_atomic_function_skeletons_type const function_typed_list_of_atomic_function_skeletons = "function_typed_list_of_atomic_function_skeletons";

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

    effect_type const effect = "effect";
    effect_production_literal_type const effect_production_literal = "effect_production_literal";
    effect_production_numeric_fluent_type const effect_production_numeric_fluent = "effect_production_numeric_fluent";
    effect_production_object_fluent_type const effect_production_object_fluent = "effect_production_object_fluent";
    effect_production_type const effect_production = "effect_production";
    effect_conditional_forall_type const effect_conditional_forall = "effect_conditional_forall";
    effect_conditional_when_type const effect_conditional_when = "effect_conditional_when";
    effect_conditional_type const effect_conditional = "effect_conditional";
    action_symbol_type const action_symbol = "action_symbol";
    action_body_type const action_body = "action_body";
    action_type const action = "action";
    derived_predicate_type const derived_predicate = "derived_predicate";

    domain_name_type const domain_name = "domain_name";
    requirements_type const requirements = "requirements";
    types_type const types = "types";
    constants_type const constants = "constants";
    predicates_type const predicates = "predicates";
    functions_type const functions = "functions";
    constraints_type const constraints = "constraints";
    structure_type const structure = "structure";
    domain_type const domain = "domain";


    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////

    const auto name_def = lexeme[alpha >> *(alnum | char_('-') | char_('_'))];
    const auto variable_def = lexeme[char_('?') > name];
    const auto number_def = double_;
    const auto term_def = name | variable | function_term;
    const auto undefined_def = lit("undefined") >> x3::attr(ast::Undefined{});

    const auto requirement_strips_def = lit(":strips") >> x3::attr(ast::RequirementStrips{});
    const auto requirement_typing_def = lit(":typing") >> x3::attr(ast::RequirementTyping{});
    const auto requirement_negative_preconditions_def = lit(":negative-preconditions") >> x3::attr(ast::RequirementNegativePreconditions{});
    const auto requirement_disjunctive_preconditions_def = lit(":disjunctive-preconditions") >> x3::attr(ast::RequirementDisjunctivePreconditions{});
    const auto requirement_equality_def = lit(":equality") >> x3::attr(ast::RequirementEquality{});
    const auto requirement_existential_preconditions_def = lit(":existential-preconditions") >> x3::attr(ast::RequirementExistentialPreconditions{});
    const auto requirement_universal_preconditions_def = lit(":universal-preconditions") >> x3::attr(ast::RequirementUniversalPreconditions{});
    const auto requirement_quantified_preconditions_def = lit(":quantified-preconditions") >> x3::attr(ast::RequirementQuantifiedPreconditions{});
    const auto requirement_conditional_effects_def = lit(":conditional-effects") >> x3::attr(ast::RequirementConditionalEffects{});
    const auto requirement_fluents_def = lit(":fluents") >> x3::attr(ast::RequirementFluents{});
    const auto requirement_object_fluents_def = lit(":object-fluents") >> x3::attr(ast::RequirementObjectFluents{});
    const auto requirement_numeric_fluents_def = lit(":numeric-fluents") >> x3::attr(ast::RequirementNumericFluents{});
    const auto requirement_adl_def = lit(":adl") >> x3::attr(ast::RequirementAdl{});
    const auto requirement_durative_actions_def = lit(":durative-actions") >> x3::attr(ast::RequirementDurativeActions{});
    const auto requirement_derived_predicates_def = lit(":derived-predicates") >> x3::attr(ast::RequirementDerivedPredicates{});
    const auto requirement_timed_initial_literals_def = lit(":timed-initial-literals") >> x3::attr(ast::RequirementTimedInitialLiterals{});
    const auto requirement_preferences_def = lit(":preferences") >> x3::attr(ast::RequirementPreferences{});
    const auto requirement_constraints_def = lit(":constraints") >> x3::attr(ast::RequirementConstraints{});
    const auto requirement_def = requirement_strips | requirement_typing | requirement_negative_preconditions
        | requirement_disjunctive_preconditions | requirement_equality | requirement_existential_preconditions
        | requirement_universal_preconditions | requirement_quantified_preconditions | requirement_conditional_effects
        | requirement_fluents | requirement_object_fluents | requirement_numeric_fluents | requirement_adl
        | requirement_durative_actions | requirement_derived_predicates | requirement_timed_initial_literals
        | requirement_preferences | requirement_constraints;

    const auto type_def = name | type_object | type_either;
    const auto type_object_def = lit("object") >> x3::attr(ast::TypeObject{});
    const auto type_either_def = lit('(') >> lit("either") >> +type > lit(')');
    const auto typed_list_of_names_recursively_def = +name >> lit('-') > type > typed_list_of_names;
    const auto typed_list_of_names_def = typed_list_of_names_recursively | *name;
    const auto typed_list_of_variables_recursively_def = +variable >> lit('-') > type > typed_list_of_variables;
    const auto typed_list_of_variables_def = typed_list_of_variables_recursively | *variable;

    const auto predicate_def = name;
    const auto atomic_formula_skeleton_def = lit('(') > predicate > typed_list_of_variables > lit(')');

    const auto function_symbol_def = name;
    const auto function_term_def = lit('(') >> function_symbol > *term > lit(')');
    const auto function_type_number_def = number;
    const auto function_type_type__def = type;
    const auto function_type_def = function_type_number | function_type_type_;
    const auto atomic_function_skeleton_def = lit('(') > function_symbol > typed_list_of_variables > lit(')');
    const auto function_typed_list_of_atomic_function_skeletons_recursively_def = +atomic_function_skeleton >> lit('-') > function_type >> -function_typed_list_of_atomic_function_skeletons;
    const auto function_typed_list_of_atomic_function_skeletons_def = function_typed_list_of_atomic_function_skeletons_recursively | +atomic_function_skeleton;

    const auto atomic_formula_of_terms_predicate_def = lit('(') >> predicate > *term > lit(')');
    const auto atomic_formula_of_terms_equality_def = lit('(') >> lit('=') > term > term > lit(')');
    const auto atomic_formula_of_terms_def = atomic_formula_of_terms_equality | atomic_formula_of_terms_predicate;
    const auto atom_def = atomic_formula_of_terms;
    const auto negated_atom_def = lit('(') >> lit("not") > atomic_formula_of_terms > lit(')');
    const auto literal_def = negated_atom | atom;

    const auto multi_operator_mul_def = lit('*') >> x3::attr(ast::MultiOperatorMul{});
    const auto multi_operator_plus_def = lit('+') >> x3::attr(ast::MultiOperatorPlus{});
    const auto multi_operator_def = multi_operator_mul | multi_operator_plus;
    const auto binary_operator_minus_def = lit('-') >> x3::attr(ast::BinaryOperatorMinus{});
    const auto binary_operator_div_def = lit('/') >> x3::attr(ast::BinaryOperatorDiv{});
    const auto binary_operator_def = binary_operator_minus | binary_operator_div | multi_operator;

    const auto binary_comparator_greater_def = lit('>') >> x3::attr(ast::BinaryComparatorGreater{});
    const auto binary_comparator_less_def = lit('<') >> x3::attr(ast::BinaryComparatorLess{});
    const auto binary_comparator_equal_def = lit('=') >> x3::attr(ast::BinaryComparatorEqual{});
    const auto binary_comparator_greater_equal_def = lit(">=") >> x3::attr(ast::BinaryComparatorGreaterEqual{});
    const auto binary_comparator_less_equal_def = lit("<=") >> x3::attr(ast::BinaryComparatorLessEqual{});
    const auto binary_comparator_def = binary_comparator_greater | binary_comparator_less | binary_comparator_equal | binary_comparator_greater_equal | binary_comparator_less_equal;

    const auto function_head_def = (lit('(') >> function_symbol >> *term > lit(')')) | (function_symbol >> x3::attr(std::vector<ast::Term>{}));
    const auto function_expression_def = function_expression_binary_op | function_expression_minus | function_expression_head | function_expression_number;
    const auto function_expression_number_def = number;
    const auto function_expression_binary_op_def = lit('(') >> binary_operator >> function_expression >> function_expression > lit(')');
    const auto function_expression_minus_def = lit('(') >> lit('-') > function_expression > lit(')');
    const auto function_expression_head_def = function_head;

    const auto goal_descriptor_def = goal_descriptor_not | goal_descriptor_and | goal_descriptor_or | goal_descriptor_imply
        | goal_descriptor_exists | goal_descriptor_forall | goal_descriptor_function_comparison | goal_descriptor_literal | goal_descriptor_atom;
    const auto goal_descriptor_atom_def = atom;
    const auto goal_descriptor_literal_def = literal;
    const auto goal_descriptor_and_def = lit('(') >> lit("and") > *goal_descriptor > lit(')');
    const auto goal_descriptor_or_def = lit('(') >> lit("or") >> *goal_descriptor > lit(')');
    const auto goal_descriptor_not_def = lit('(') >> lit("not") > goal_descriptor > lit(')');
    const auto goal_descriptor_imply_def = lit('(') >> lit("imply") > goal_descriptor > goal_descriptor > lit(')');
    const auto goal_descriptor_exists_def = lit('(') >> lit("exists") > typed_list_of_variables > goal_descriptor > lit(')');
    const auto goal_descriptor_forall_def = lit('(') >> lit("forall") > typed_list_of_variables > goal_descriptor > lit(')');
    const auto goal_descriptor_function_comparison_def = lit('(') >> binary_comparator >> function_expression > function_expression > lit(')');

    const auto constraint_goal_descriptor_def = constraint_goal_descriptor_and | constraint_goal_descriptor_forall | constraint_goal_descriptor_at_end
        | constraint_goal_descriptor_always | constraint_goal_descriptor_sometime | constraint_goal_descriptor_within
        | constraint_goal_descriptor_at_most_once | constraint_goal_descriptor_sometime_after | constraint_goal_descriptor_sometime_before
        | constraint_goal_descriptor_always_within | constraint_goal_descriptor_hold_during | constraint_goal_descriptor_hold_after;
    const auto constraint_goal_descriptor_and_def = lit('(') >> lit("and") > *constraint_goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_forall_def = lit('(') >> lit("forall") > typed_list_of_variables > constraint_goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_at_end_def = lit('(') >> lit("at") >> lit("end") > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_always_def = lit('(') >> lit("always") > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_sometime_def = lit('(') >> lit("sometime") > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_within_def = lit('(') >> lit("within") > number > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_at_most_once_def = lit('(') >> lit("at-most-once") > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_sometime_after_def = lit('(') >> lit("sometime-after") > goal_descriptor > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_sometime_before_def = lit('(') >> lit("sometime-before") > goal_descriptor > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_always_within_def = lit('(') >> lit("always-within") > number > goal_descriptor > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_hold_during_def = lit('(') >> lit("hold-during") > number > number > goal_descriptor > lit(')');
    const auto constraint_goal_descriptor_hold_after_def = lit('(') >> lit("hold-after") > number > goal_descriptor > lit(')');

    const auto precondition_goal_descriptor_def = precondition_goal_descriptor_and | precondition_goal_descriptor_preference | precondition_goal_descriptor_forall | precondition_goal_descriptor_simple;
    const auto preference_name_def = name;
    const auto precondition_goal_descriptor_simple_def = goal_descriptor;
    const auto precondition_goal_descriptor_and_def = lit('(') >> lit("and") >> *precondition_goal_descriptor > lit(')');
    const auto precondition_goal_descriptor_preference_def = lit('(') >> lit("preference") > preference_name > goal_descriptor > lit(')');
    const auto precondition_goal_descriptor_forall_def = lit('(') >> lit("forall") > typed_list_of_variables > precondition_goal_descriptor > lit(')');

    const auto assign_operator_assign_def = lit("assign") >> x3::attr(ast::AssignOperatorAssign{});
    const auto assign_operator_scale_up_def = lit("scale-up") >> x3::attr(ast::AssignOperatorScaleUp{});
    const auto assign_operator_scale_down_def = lit("scale-down") >> x3::attr(ast::AssignOperatorScaleDown{});
    const auto assign_operator_increase_def = lit("increase") >> x3::attr(ast::AssignOperatorIncrease{});
    const auto assign_operator_decrease_def = lit("decrease") >> x3::attr(ast::AssignOperatorDecrease{});
    const auto assign_operator_def = assign_operator_assign | assign_operator_scale_up | assign_operator_scale_down | assign_operator_increase | assign_operator_decrease;

    const auto effect_def = lit('(') >> lit("and") >> *effect >> lit(')') | effect_conditional | effect_production;
    const auto effect_production_literal_def = literal;
    const auto effect_production_numeric_fluent_def = lit('(') >> assign_operator >> function_head >> function_expression > lit(')');
    const auto effect_production_object_fluent_def = lit('(') >> function_term >> (term | undefined);
    const auto effect_production_def = effect_production_literal | effect_production_numeric_fluent | effect_production_object_fluent;
    const auto effect_conditional_forall_def = lit('(') >> lit("forall") > lit("(") > typed_list_of_variables > lit(')') > effect > lit(')');
    const auto effect_conditional_when_def = lit('(') >> lit("when") > goal_descriptor > effect > lit(')');
    const auto effect_conditional_def = effect_conditional_forall | effect_conditional_when;

    const auto action_symbol_def = name;
    const auto action_body_def = (lit(":precondition") > ((lit('(') >> lit(')')) | precondition_goal_descriptor))
                                >> (lit(":effect") > ((lit('(') >> lit(')')) | effect));
    const auto action_def = lit('(') >> lit(":action") > action_symbol
                                     > lit(":parameters") > lit('(') >> typed_list_of_variables > lit(')')
                                     >> action_body
                            >> lit(')');

    const auto derived_predicate_def = lit('(') >> typed_list_of_variables >> goal_descriptor > lit(')');

    const auto domain_name_def = lit('(') >> lit("domain") > name > lit(')');
    const auto requirements_def = lit('(') >> lit(":requirements") >> *requirement >> lit(')');
    const auto types_def = lit('(') >> lit(":types") >> typed_list_of_names > lit(')');
    const auto constants_def = lit('(') >> lit(":constants") >> typed_list_of_names > lit(')');
    const auto predicates_def = lit('(') >> lit(":predicates") >> *atomic_formula_skeleton > lit(')');
    const auto functions_def = lit('(') >> lit(":functions") >> *function_typed_list_of_atomic_function_skeletons > lit(')');
    const auto constraints_def = lit('(') >> lit(":constraints") > constraint_goal_descriptor > lit(')');
    const auto structure_def = action | derived_predicate;

    const auto domain_def =
        lit('(') > lit("define")
           > domain_name
           >> -requirements
           >> -types
           >> -constants
           >> -predicates
           >> -functions
           >> -constraints
           >> *structure
        > lit(')');

    BOOST_SPIRIT_DEFINE(name, variable, number, term, undefined)

    BOOST_SPIRIT_DEFINE(requirement_strips, requirement_typing, requirement_negative_preconditions,
        requirement_disjunctive_preconditions, requirement_equality, requirement_existential_preconditions,
        requirement_universal_preconditions, requirement_quantified_preconditions,
        requirement_conditional_effects, requirement_fluents, requirement_object_fluents,
        requirement_numeric_fluents, requirement_adl, requirement_durative_actions,
        requirement_derived_predicates, requirement_timed_initial_literals, requirement_preferences,
        requirement_constraints, requirement)

    BOOST_SPIRIT_DEFINE(type, type_object, type_either, typed_list_of_names_recursively,
        typed_list_of_names, typed_list_of_variables_recursively, typed_list_of_variables)

    BOOST_SPIRIT_DEFINE(predicate, atomic_formula_skeleton)

    BOOST_SPIRIT_DEFINE(function_symbol, function_term, function_type_number, function_type_type_, function_type,
        atomic_function_skeleton, function_typed_list_of_atomic_function_skeletons_recursively,
        function_typed_list_of_atomic_function_skeletons)

    BOOST_SPIRIT_DEFINE(atomic_formula_of_terms_predicate, atomic_formula_of_terms_equality,
        atomic_formula_of_terms, atom, negated_atom, literal)

    BOOST_SPIRIT_DEFINE(multi_operator_mul, multi_operator_plus, multi_operator,
        binary_operator_minus, binary_operator_div, binary_operator)

    BOOST_SPIRIT_DEFINE(binary_comparator_greater, binary_comparator_less, binary_comparator_equal,
        binary_comparator_greater_equal, binary_comparator_less_equal, binary_comparator)

    BOOST_SPIRIT_DEFINE(function_head, function_expression, function_expression_number,
        function_expression_binary_op, function_expression_minus, function_expression_head)

    BOOST_SPIRIT_DEFINE(goal_descriptor, goal_descriptor_atom, goal_descriptor_literal,
        goal_descriptor_and, goal_descriptor_or, goal_descriptor_not, goal_descriptor_imply,
        goal_descriptor_exists, goal_descriptor_forall, goal_descriptor_function_comparison)

    BOOST_SPIRIT_DEFINE(constraint_goal_descriptor, constraint_goal_descriptor_and,
        constraint_goal_descriptor_forall, constraint_goal_descriptor_at_end,
        constraint_goal_descriptor_always, constraint_goal_descriptor_sometime,
        constraint_goal_descriptor_within, constraint_goal_descriptor_at_most_once,
        constraint_goal_descriptor_sometime_after, constraint_goal_descriptor_sometime_before,
        constraint_goal_descriptor_always_within, constraint_goal_descriptor_hold_during,
        constraint_goal_descriptor_hold_after)

    BOOST_SPIRIT_DEFINE(preference_name, precondition_goal_descriptor, precondition_goal_descriptor_simple,
        precondition_goal_descriptor_and, precondition_goal_descriptor_preference, precondition_goal_descriptor_forall)

    BOOST_SPIRIT_DEFINE(assign_operator_assign, assign_operator_scale_up, assign_operator_scale_down,
        assign_operator_increase, assign_operator_decrease, assign_operator)

    BOOST_SPIRIT_DEFINE(effect, effect_production_literal, effect_production_numeric_fluent,
        effect_production_object_fluent, effect_production, effect_conditional_forall,
        effect_conditional_when, effect_conditional, action_symbol,
        action_body, action, derived_predicate)

    BOOST_SPIRIT_DEFINE(domain_name, requirements, types, constants, predicates, functions,
        constraints, structure, domain)


    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

    struct NameClass : x3::annotate_on_success {};
    struct VariableClass : x3::annotate_on_success {};
    struct NumberClass : x3::annotate_on_success {};
    struct TermClass : x3::annotate_on_success {};
    struct UndefinedClass : x3::annotate_on_success {};

    struct RequirementStripsClass : annotation_domain {};
    struct RequirementTypingClass : x3::annotate_on_success {};
    struct RequirementNegativePreconditionsClass : x3::annotate_on_success {};
    struct RequirementDisjunctivePreconditionsClass : x3::annotate_on_success {};
    struct RequirementEqualityClass : x3::annotate_on_success {};
    struct RequirementExistentialPreconditionsClass : x3::annotate_on_success {};
    struct RequirementUniversalPreconditionsClass : x3::annotate_on_success {};
    struct RequirementQuantifiedPreconditionsClass : x3::annotate_on_success {};
    struct RequirementConditionalEffectsClass : x3::annotate_on_success {};
    struct RequirementFluentsClass : x3::annotate_on_success {};
    struct RequirementObjectFluentsClass : x3::annotate_on_success {};
    struct RequirementNumericalFluentsClass : x3::annotate_on_success {};
    struct RequirementAdlClass : x3::annotate_on_success {};
    struct RequirementDurativeActionsClass : x3::annotate_on_success {};
    struct RequirementDerivedPredicatesClass : x3::annotate_on_success {};
    struct RequirementTimedInitialLiteralsClass : x3::annotate_on_success {};
    struct RequirementPreferencesClass : x3::annotate_on_success {};
    struct RequirementConstraintsClass : x3::annotate_on_success {};
    struct RequirementClass : x3::annotate_on_success {};

    struct TypeClass : x3::annotate_on_success {};
    struct TypeObjectClass : x3::annotate_on_success {};
    struct TypeEitherClass : x3::annotate_on_success {};
    struct TypedListOfNamesRecursivelyClass : x3::annotate_on_success {};
    struct TypedListOfNamesClass : x3::annotate_on_success {};
    struct TypedListOfVariablesRecursivelyClass : x3::annotate_on_success {};
    struct TypedListOfVariablesClass : x3::annotate_on_success {};

    struct PredicateClass : x3::annotate_on_success {};
    struct AtomicFormulaSkeletonClass : x3::annotate_on_success {};

    struct FunctionSymbolClass : x3::annotate_on_success {};
    struct FunctionTypeNumberClass : x3::annotate_on_success {};
    struct FunctionTypeObjectClass : x3::annotate_on_success {};
    struct FunctionTypeTypeClass : x3::annotate_on_success {};
    struct FunctionTypeClass : x3::annotate_on_success {};
    struct AtomicFunctionSkeletonClass : x3::annotate_on_success {};
    struct FunctionTypedListOfAtomicFunctionSkeletonsRecursivelyClass : x3::annotate_on_success {};
    struct FunctionTypedListOfAtomicFunctionSkeletonsClass : x3::annotate_on_success {};

    struct AtomicFormulaOfTermsPredicateClass : x3::annotate_on_success {};
    struct AtomicFormulaOfTermsEqualityClass : x3::annotate_on_success {};
    struct AtomicFormulaOfTermsClass : x3::annotate_on_success {};
    struct AtomClass : x3::annotate_on_success {};
    struct NegatedAtomClass : x3::annotate_on_success {};
    struct LiteralClass : x3::annotate_on_success {};

    struct MultiOperatorMulClass : x3::annotate_on_success {};
    struct MultiOperatorPlusClass : x3::annotate_on_success {};
    struct MultiOperatorClass : x3::annotate_on_success {};
    struct BinaryOperatorMinusClass : x3::annotate_on_success {};
    struct BinaryOperatorDivClass : x3::annotate_on_success {};
    struct BinaryOperatorClass : x3::annotate_on_success {};

    struct BinaryComparatorGreaterClass : x3::annotate_on_success {};
    struct BinaryComparatorLessClass : x3::annotate_on_success {};
    struct BinaryComparatorEqualClass : x3::annotate_on_success {};
    struct BinaryComparatorGreaterEqualClass : x3::annotate_on_success {};
    struct BinaryComparatorLessEqualClass : x3::annotate_on_success {};
    struct BinaryComparatorClass : x3::annotate_on_success {};

    struct FunctionExpressionClass : x3::annotate_on_success {};
    struct FunctionHeadClass : x3::annotate_on_success {};
    struct FunctionExpressionNumberClass : x3::annotate_on_success {};
    struct FunctionExpressionBinaryOpClass : x3::annotate_on_success {};
    struct FunctionExpressionMinusClass : x3::annotate_on_success {};
    struct FunctionExpressionHeadClass : x3::annotate_on_success {};

    struct GoalDescriptorClass : x3::annotate_on_success {};
    struct GoalDescriptorAtomClass : x3::annotate_on_success {};
    struct GoalDescriptorLiteralClass : x3::annotate_on_success {};
    struct GoalDescriptorAndClass : x3::annotate_on_success {};
    struct GoalDescriptorOrClass : x3::annotate_on_success {};
    struct GoalDescriptorNotClass : x3::annotate_on_success {};
    struct GoalDescriptorImplyClass : x3::annotate_on_success {};
    struct GoalDescriptorExistsClass : x3::annotate_on_success {};
    struct GoalDescriptorForallClass : x3::annotate_on_success {};
    struct GoalDescriptorFunctionComparisonClass : x3::annotate_on_success {};

    struct ConstraintGoalDescriptorClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorAndClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorForallClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorAtEndClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorAlwaysClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorSometimeClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorWithinClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorAtMostOnceClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorSometimeAfterClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorSometimeBeforeClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorAlwaysWithinClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorHoldDuringClass : x3::annotate_on_success {};
    struct ConstraintGoalDescriptorHoldAfterClass : x3::annotate_on_success {};

    struct PreferenceNameClass : x3::annotate_on_success {};
    struct PreconditionGoalDescriptorClass : x3::annotate_on_success {};
    struct PreconditionGoalDescriptorSimpleClass : x3::annotate_on_success {};
    struct PreconditionGoalDescriptorAndClass : x3::annotate_on_success {};
    struct PreconditionGoalDescriptorPreferenceClass : x3::annotate_on_success {};
    struct PreconditionGoalDescriptorForallClass : x3::annotate_on_success {};

    struct AssignOperatorAssignClass : x3::annotate_on_success {};
    struct AssignOperatorScaleUpClass : x3::annotate_on_success {};
    struct AssignOperatorScaleDownClass : x3::annotate_on_success {};
    struct AssignOperatorIncreaseClass : x3::annotate_on_success {};
    struct AssignOperatorDecreaseClass : x3::annotate_on_success {};
    struct AssignOperatorClass : x3::annotate_on_success {};

    struct EffectClass : x3::annotate_on_success {};
    struct EffectProductionLiteralClass : x3::annotate_on_success {};
    struct EffectProductionNumericFluentClass : x3::annotate_on_success {};
    struct EffectProductionObjectFluentClass : x3::annotate_on_success {};
    struct EffectProductionClass : x3::annotate_on_success {};
    struct EffectConditionalForallClass : x3::annotate_on_success {};
    struct EffectConditionalWhenClass : x3::annotate_on_success {};
    struct EffectConditionalClass : x3::annotate_on_success {};
    struct ActionSymbolClass : x3::annotate_on_success {};
    struct ActionBodyClass : x3::annotate_on_success {};
    struct ActionClass : x3::annotate_on_success {};
    struct DurativeActionClass : x3::annotate_on_success {};
    struct DerivedPredicateClass : x3::annotate_on_success {};

    struct DomainNameClass : x3::annotate_on_success {};
    struct RequirementsClass : x3::annotate_on_success {};
    struct TypesClass : x3::annotate_on_success {};
    struct ConstantsClass : x3::annotate_on_success {};
    struct PredicatesClass : annotation_domain {};
    struct FunctionsClass : x3::annotate_on_success {};
    struct ConstraintsClass : x3::annotate_on_success {};
    struct StructureClass : x3::annotate_on_success {};
    struct DomainClass : x3::annotate_on_success, error_handler_domain {};
}

namespace loki::domain
{
    parser::name_type const& name() {
        return parser::name;
    }
    parser::variable_type const& variable() {
        return parser::variable;
    }
    parser::number_type const& number() {
        return parser::number;
    }
    parser::term_type const& term() {
        return parser::term;
    }
    parser::undefined_type const& undefined() {
        return parser::undefined;
    }

    parser::requirement_strips_type const& requirement_strips() {
        return parser::requirement_strips;
    }
    parser::requirement_typing_type const& requirement_typing() {
        return parser::requirement_typing;
    }
    parser::requirement_negative_preconditions_type const& requirement_negative_preconditions() {
        return parser::requirement_negative_preconditions;
    }
    parser::requirement_disjunctive_preconditions_type const& requirement_disjunctive_preconditions() {
        return parser::requirement_disjunctive_preconditions;
    }
    parser::requirement_equality_type const& requirement_equality() {
        return parser::requirement_equality;
    }
    parser::requirement_existential_preconditions_type const& requirement_existential_preconditions() {
        return parser::requirement_existential_preconditions;
    }
    parser::requirement_universal_preconditions_type const& requirement_universal_preconditions() {
        return parser::requirement_universal_preconditions;
    }
    parser::requirement_quantified_preconditions_type const& requirement_quantified_preconditions() {
        return parser::requirement_quantified_preconditions;
    }
    parser::requirement_conditional_effects_type const& requirement_conditional_effects() {
        return parser::requirement_conditional_effects;
    }
    parser::requirement_fluents_type const& requirement_fluents() {
        return parser::requirement_fluents;
    }
    parser::requirement_object_fluents_type const& requirement_object_fluents() {
        return parser::requirement_object_fluents;
    }
    parser::requirement_numeric_fluents_type const& requirement_numeric_fluents() {
        return parser::requirement_numeric_fluents;
    }
    parser::requirement_adl_type const& requirement_adl() {
        return parser::requirement_adl;
    }
    parser::requirement_durative_actions_type const& requirement_durative_actions() {
        return parser::requirement_durative_actions;
    }
    parser::requirement_derived_predicates_type const& requirement_derived_predicates() {
        return parser::requirement_derived_predicates;
    }
    parser::requirement_timed_initial_literals_type const& requirement_timed_initial_literals() {
        return parser::requirement_timed_initial_literals;
    }
    parser::requirement_preferences_type const& requirement_preferences() {
        return parser::requirement_preferences;
    }
    parser::requirement_constraints_type const& requirement_constraints() {
        return parser::requirement_constraints;
    }
    parser::requirement_type const& requirement() {
        return parser::requirement;
    }

    parser::type_type const& type() {
        return parser::type;
    }
    parser::type_object_type const& type_object() {
        return parser::type_object;
    }
    parser::type_either_type const& type_either() {
        return parser::type_either;
    }
    parser::typed_list_of_names_recursively_type const& typed_list_of_names_recursively() {
        return parser::typed_list_of_names_recursively;
    }
    parser::typed_list_of_names_type const& typed_list_of_names() {
        return parser::typed_list_of_names;
    }
    parser::typed_list_of_variables_recursively_type const& typed_list_of_variables_recursively() {
        return parser::typed_list_of_variables_recursively;
    }
    parser::typed_list_of_variables_type const& typed_list_of_variables() {
        return parser::typed_list_of_variables;
    }

    parser::predicate_type const& predicate() {
        return parser::predicate;
    }
    parser::atomic_formula_skeleton_type const& atomic_formula_skeleton() {
        return parser::atomic_formula_skeleton;
    }

    parser::function_symbol_type const& function_symbol() {
        return parser::function_symbol;
    }
    parser::function_term_type const& function_term() {
        return parser::function_term;
    }
    parser::function_type_number_type const& function_type_number() {
        return parser::function_type_number;
    }
    parser::function_type_type_type const& function_type_type_() {
        return parser::function_type_type_;
    }
    parser::function_type_type const& function_type() {
        return parser::function_type;
    }
    parser::atomic_function_skeleton_type const& atomic_function_skeleton() {
        return parser::atomic_function_skeleton;
    }
    parser::function_typed_list_of_atomic_function_skeletons_recursively_type const& function_typed_list_of_atomic_function_skeletons_recursively() {
        return parser::function_typed_list_of_atomic_function_skeletons_recursively;
    }
    parser::function_typed_list_of_atomic_function_skeletons_type const& function_typed_list_of_atomic_function_skeletons() {
        return parser::function_typed_list_of_atomic_function_skeletons;
    }

    parser::atomic_formula_of_terms_predicate_type const& atomic_formula_of_terms_predicate() {
        return parser::atomic_formula_of_terms_predicate;
    }
    parser::atomic_formula_of_terms_equality_type const& atomic_formula_of_terms_equality() {
        return parser::atomic_formula_of_terms_equality;
    }
    parser::atomic_formula_of_terms_type const& atomic_formula_of_terms() {
        return parser::atomic_formula_of_terms;
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

    parser::multi_operator_mul_type const& multi_operator_mul() {
        return parser::multi_operator_mul;
    }
    parser::multi_operator_plus_type const& multi_operator_plus() {
        return parser::multi_operator_plus;
    }
    parser::multi_operator_type const& multi_operator() {
        return parser::multi_operator;
    }
    parser::binary_operator_minus_type const& binary_operator_minus() {
        return parser::binary_operator_minus;
    }
    parser::binary_operator_div_type const& binary_operator_div() {
        return parser::binary_operator_div;
    }
    parser::binary_operator_type const& binary_operator() {
        return parser::binary_operator;
    }

    parser::binary_comparator_greater_type const& binary_comparator_greater() {
        return parser::binary_comparator_greater;
    }
    parser::binary_comparator_less_type const& binary_comparator_less() {
        return parser::binary_comparator_less;
    }
    parser::binary_comparator_equal_type const& binary_comparator_equal() {
        return parser::binary_comparator_equal;
    }
    parser::binary_comparator_greater_equal_type const& binary_comparator_greater_equal() {
        return parser::binary_comparator_greater_equal;
    }
    parser::binary_comparator_less_equal_type const& binary_comparator_less_equal() {
        return parser::binary_comparator_less_equal;
    }
    parser::binary_comparator_type const& binary_comparator() {
        return parser::binary_comparator;
    }

    parser::function_expression_type const& function_expression() {
        return parser::function_expression;
    }
    parser::function_head_type const& function_head() {
        return parser::function_head;
    }
    parser::function_expression_number_type const& function_expression_number() {
        return parser::function_expression_number;
    }
    parser::function_expression_binary_op_type const& function_expression_binary_op() {
        return parser::function_expression_binary_op;
    }
    parser::function_expression_minus_type const& function_expression_minus() {
        return parser::function_expression_minus;
    }
    parser::function_expression_head_type const& function_expression_head() {
        return parser::function_expression_head;
    }

    parser::goal_descriptor_type const& goal_descriptor() {
        return parser::goal_descriptor;
    }
    parser::goal_descriptor_atom_type const& goal_descriptor_atom() {
        return parser::goal_descriptor_atom;
    }
    parser::goal_descriptor_literal_type const& goal_descriptor_literal() {
        return parser::goal_descriptor_literal;
    }
    parser::goal_descriptor_and_type const& goal_descriptor_and() {
        return parser::goal_descriptor_and;
    }
    parser::goal_descriptor_or_type const& goal_descriptor_or() {
        return parser::goal_descriptor_or;
    }
    parser::goal_descriptor_not_type const& goal_descriptor_not() {
        return parser::goal_descriptor_not;
    }
    parser::goal_descriptor_imply_type const& goal_descriptor_imply() {
        return parser::goal_descriptor_imply;
    }
    parser::goal_descriptor_exists_type const& goal_descriptor_exists() {
        return parser::goal_descriptor_exists;
    }
    parser::goal_descriptor_forall_type const& goal_descriptor_forall() {
        return parser::goal_descriptor_forall;
    }
    parser::goal_descriptor_function_comparison_type const& goal_descriptor_function_comparison() {
        return parser::goal_descriptor_function_comparison;
    }

    parser::constraint_goal_descriptor_type const& constraint_goal_descriptor() {
        return parser::constraint_goal_descriptor;
    }
    parser::constraint_goal_descriptor_and_type const& constraint_goal_descriptor_and() {
        return parser::constraint_goal_descriptor_and;
    }
    parser::constraint_goal_descriptor_forall_type const& constraint_goal_descriptor_forall() {
        return parser::constraint_goal_descriptor_forall;
    }
    parser::constraint_goal_descriptor_at_end_type const& constraint_goal_descriptor_at_end() {
        return parser::constraint_goal_descriptor_at_end;
    }
    parser::constraint_goal_descriptor_always_type const& constraint_goal_descriptor_always() {
        return parser::constraint_goal_descriptor_always;
    }
    parser::constraint_goal_descriptor_sometime_type const& constraint_goal_descriptor_sometime() {
        return parser::constraint_goal_descriptor_sometime;
    }
    parser::constraint_goal_descriptor_within_type const& constraint_goal_descriptor_within() {
        return parser::constraint_goal_descriptor_within;
    }
    parser::constraint_goal_descriptor_at_most_once_type const& constraint_goal_descriptor_at_most_once() {
        return parser::constraint_goal_descriptor_at_most_once;
    }
    parser::constraint_goal_descriptor_sometime_after_type const& constraint_goal_descriptor_sometime_after() {
        return parser::constraint_goal_descriptor_sometime_after;
    }
    parser::constraint_goal_descriptor_sometime_before_type const& constraint_goal_descriptor_sometime_before() {
        return parser::constraint_goal_descriptor_sometime_before;
    }
    parser::constraint_goal_descriptor_always_within_type const& constraint_goal_descriptor_always_within() {
        return parser::constraint_goal_descriptor_always_within;
    }
    parser::constraint_goal_descriptor_hold_during_type const& constraint_goal_descriptor_hold_during() {
        return parser::constraint_goal_descriptor_hold_during;
    }
    parser::constraint_goal_descriptor_hold_after_type const& constraint_goal_descriptor_hold_after() {
        return parser::constraint_goal_descriptor_hold_after;
    }

    parser::precondition_goal_descriptor_type const& precondition_goal_descriptor() {
        return parser::precondition_goal_descriptor;
    }
    parser::preference_name_type const& preference_name() {
        return parser::preference_name;
    }
    parser::precondition_goal_descriptor_simple_type const& precondition_goal_descriptor_simple() {
        return parser::precondition_goal_descriptor_simple;
    }
    parser::precondition_goal_descriptor_and_type const& precondition_goal_descriptor_and() {
        return parser::precondition_goal_descriptor_and;
    }
    parser::precondition_goal_descriptor_preference_type const& precondition_goal_descriptor_preference() {
        return parser::precondition_goal_descriptor_preference;
    }
    parser::precondition_goal_descriptor_forall_type const& precondition_goal_descriptor_forall() {
        return parser::precondition_goal_descriptor_forall;
    }

    parser::assign_operator_assign_type const& assign_operator_assign() {
        return parser::assign_operator_assign;
    }
    parser::assign_operator_scale_up_type const& assign_operator_scale_up() {
        return parser::assign_operator_scale_up;
    }
    parser::assign_operator_scale_down_type const& assign_operator_scale_down() {
        return parser::assign_operator_scale_down;
    }
    parser::assign_operator_increase_type const& assign_operator_increase() {
        return parser::assign_operator_increase;
    }
    parser::assign_operator_decrease_type const& assign_operator_decrease() {
        return parser::assign_operator_decrease;
    }
    parser::assign_operator_type const& assign_operator() {
        return parser::assign_operator;
    }

    parser::effect_type const& effect() {
        return parser::effect;
    }
    parser::effect_production_literal_type const& effect_production_literal() {
        return parser::effect_production_literal;
    }
    parser::effect_production_numeric_fluent_type const& effect_production_numeric_fluent() {
        return parser::effect_production_numeric_fluent;
    }
    parser::effect_production_object_fluent_type const& effect_production_object_fluent() {
        return parser::effect_production_object_fluent;
    }
    parser::effect_production_type const& effect_production() {
        return parser::effect_production;
    }
    parser::effect_conditional_forall_type const& effect_conditional_forall() {
        return parser::effect_conditional_forall;
    }
    parser::effect_conditional_when_type const& effect_conditional_when() {
        return parser::effect_conditional_when;
    }
    parser::effect_conditional_type const& effect_conditional() {
        return parser::effect_conditional;
    }
    parser::action_symbol_type const& action_symbol() {
        return parser::action_symbol;
    }
    parser::action_body_type const& action_body() {
        return parser::action_body;
    }
    parser::action_type const& action() {
        return parser::action;
    }
    parser::derived_predicate_type const& derived_predicate() {
        return parser::derived_predicate;
    }

    parser::domain_name_type const& domain_name() {
        return parser::domain_name;
    }
    parser::requirements_type const& requirements() {
        return parser::requirements;
    }
    parser::types_type const& types() {
        return parser::types;
    }
    parser::constants_type const& constants() {
        return parser::constants;
    }
    parser::predicates_type const& predicates() {
        return parser::predicates;
    }
    parser::functions_type const& functions() {
        return parser::functions;
    }
    parser::constraints_type const& constraints() {
        return parser::constraints;
    }
    parser::structure_type const& structure() {
        return parser::structure;
    }
    parser::domain_type const& domain() {
        return parser::domain;
    }
}

#endif