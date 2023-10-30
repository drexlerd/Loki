#ifndef LOKI_SRC_DOMAIN_PARSER_DEF_HPP_
#define LOKI_SRC_DOMAIN_PARSER_DEF_HPP_

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#include "include/loki/domain/ast.hpp"
#include "include/loki/domain/error_handler.hpp"
#include "include/loki/domain/parser.hpp"

#include "ast_adapted.hpp"
#include "parser.hpp"


namespace loki::domain::parser {
    namespace x3 = boost::spirit::x3;
    namespace ascii = boost::spirit::x3::ascii;

    using x3::lit;
    using x3::lexeme;
    using x3::eps;
    using x3::int_;

    using ascii::alpha;
    using ascii::alnum;
    using ascii::char_;
    using ascii::string;

    ///////////////////////////////////////////////////////////////////////////
    // Rule IDs
    ///////////////////////////////////////////////////////////////////////////

    struct NameClass;
    struct VariableClass;
    struct NumberClass;
    struct TermClass;

    struct StripsRequirementClass;
    struct TypingRequirementClass;
    struct RequirementClass;

    struct FluentTypeClass;
    struct EitherTypeClass;
    struct TypeClass;
    struct TypedListOfNamesRecursivelyClass;
    struct TypedListOfNamesClass;
    struct TypedListOfVariablesRecursivelyClass;
    struct TypedListOfVariablesClass;

    struct PredicateClass;
    struct AtomicFormulaSkeletonClass;

    struct FunctionSymbolClass;
    struct FunctionTypeClass;
    struct AtomicFunctionSkeletonClass;
    struct FunctionTypedListOfAtomicFunctionSkeletonsRecursivelyClass;
    struct FunctionTypedListOfAtomicFunctionSkeletonsClass;

    struct AtomicFormulaOfTermsClass;
    struct AtomClass;
    struct NegatedAtomClass;
    struct LiteralClass;

    struct MultiOperatorMulClass;
    struct MultiOperatorPlusClass;
    struct MultiOperatorClass;
    struct BinaryOperatorMinusClass;
    struct BinaryOperatorDivClass;
    struct BinaryOperatorClass;

    struct BinaryComparatorGreaterClass;
    struct BinaryComparatorLessClass;
    struct BinaryComparatorEqualClass;
    struct BinaryComparatorGreaterEqualClass;
    struct BinaryComparatorLessEqualClass;
    struct BinaryComparatorClass;

    struct FunctionHeadClass;
    struct FunctionExpressionNumberClass;
    struct FunctionExpressionBinaryOpClass;
    struct FunctionExpressionMinusClass;
    struct FunctionExpressionHeadClass;
    struct FunctionExpressionClass;

    struct GoalDescriptorAtomClass;
    struct GoalDescriptorLiteralClass;
    struct GoalDescriptorAndClass;
    struct GoalDescriptorOrClass;
    struct GoalDescriptorNotClass;
    struct GoalDescriptorImplyClass;
    struct GoalDescriptorExistsClass;
    struct GoalDescriptorForallClass;
    struct GoalDescriptorFunctionComparisonClass;
    struct GoalDescriptorClass;

    struct ConstraintGoalDescriptorAndClass;
    struct ConstraintGoalDescriptorForallClass;
    struct ConstraintGoalDescriptorAtEndClass;
    struct ConstraintGoalDescriptorAlwaysClass;
    struct ConstraintGoalDescriptorSometimeClass;
    struct ConstraintGoalDescriptorWithinClass;
    struct ConstraintGoalDescriptorAtMostOnceClass;
    struct ConstraintGoalDescriptorSometimeAfterClass;
    struct ConstraintGoalDescriptorSometimeBeforeClass;
    struct ConstraintGoalDescriptorAlwaysWithinClass;
    struct ConstraintGoalDescriptorHoldDuringClass;
    struct ConstraintGoalDescriptorHoldAfterClass;
    struct ConstraintGoalDescriptorClass;

    struct PreferenceNameClass;
    struct PreconditionGoalDescriptorSimpleClass;
    struct PreconditionGoalDescriptorAndClass;
    struct PreconditionGoalDescriptorPreferenceClass;
    struct PreconditionGoalDescriptorForallClass;
    struct PreconditionGoalDescriptorClass;

    struct AssignOperatorAssignClass;
    struct AssignOperatorScaleUpClass;
    struct AssignOperatorScaleDownClass;
    struct AssignOperatorIncreaseClass;
    struct AssignOperatorDecreaseClass;
    struct AssignOperatorClass;

    struct SimpleEffectLiteralClass;
    struct SimpleEffectFluentClass;
    struct SimpleEffectClass;
    struct ConditionalEffectForallClass;
    struct ConditionalEffectWhenClass;
    struct ConditionalEffectClass;
    struct EffectClass;

    struct ActionSymbolClass;
    struct ActionBodyClass;

    struct ActionClass;
    // TODO
    struct DurativeActionClass;
    struct DerivedPredicateClass;

    struct DomainNameClass;
    struct RequirementsClass;
    struct TypesClass;
    struct ConstantsClass;
    struct PredicatesClass;
    struct FunctionsClass;
    struct ConstraintsClass;
    struct StructureClass;


    ///////////////////////////////////////////////////////////////////////////
    // Rules
    ///////////////////////////////////////////////////////////////////////////

    x3::rule<NameClass, ast::Name> const
        name = "name";
    x3::rule<VariableClass, ast::Variable> const
        variable = "variable";
    x3::rule<NumberClass, ast::Number> const
        number = "number";
    x3::rule<TermClass, ast::Term> const
        term = "term";

    x3::rule<StripsRequirementClass, ast::StripsRequirement> const
        strips_requirement = "strips_requirement";
    x3::rule<TypingRequirementClass, ast::TypingRequirement> const
        typing_requirement = "typing_requirement";
    x3::rule<RequirementClass, ast::Requirement> const
        requirement = "requirement";

    x3::rule<FluentTypeClass, ast::FluentType> const
        fluent_type = "fluent_type";
    x3::rule<EitherTypeClass, ast::EitherType> const
        either_type = "either_type";
    x3::rule<TypeClass, ast::Type> const
        type = "type";
    x3::rule<TypedListOfNamesRecursivelyClass, ast::TypedListOfNamesRecursively> const
        typed_list_of_names_recursively = "typed_list_of_names_recursively";
    x3::rule<TypedListOfNamesClass, ast::TypedListOfNames> const
        typed_list_of_names = "typed_list_of_names";

    x3::rule<TypedListOfVariablesRecursivelyClass, ast::TypedListOfVariablesRecursively> const
        typed_list_of_variables_recursively = "typed_list_of_variables_recursively";
    x3::rule<TypedListOfVariablesClass, ast::TypedListOfVariables> const
        typed_list_of_variables = "typed_list_of_variables";

    x3::rule<PredicateClass, ast::Predicate> const
        predicate = "predicate";
    x3::rule<AtomicFormulaSkeletonClass, ast::AtomicFormulaSkeleton> const
        atomic_formula_skeleton = "atomic_formula_skeleton";

    x3::rule<FunctionSymbolClass, ast::FunctionSymbol> const
        function_symbol = "function_symbol";
    x3::rule<FunctionTypeClass, ast::FunctionType> const
        function_type = "function_type";
    x3::rule<AtomicFunctionSkeletonClass, ast::AtomicFunctionSkeleton> const
        atomic_function_skeleton = "atomic_function_skeleton";
    x3::rule<FunctionTypedListOfAtomicFunctionSkeletonsRecursivelyClass, ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively> const
        function_typed_list_of_atomic_function_skeletons_recursively = "function_typed_list_of_atomic_function_skeletons_recursively";
    x3::rule<FunctionTypedListOfAtomicFunctionSkeletonsClass, ast::FunctionTypedListOfAtomicFunctionSkeletons> const
        function_typed_list_of_atomic_function_skeletons = "function_typed_list_of_atomic_function_skeletons";

    x3::rule<AtomicFormulaOfTermsClass, ast::AtomicFormulaOfTerms> const
        atomic_formula_of_terms = "atomic_formula_of_terms";
    x3::rule<AtomClass, ast::Atom> const
        atom = "atom";
    x3::rule<NegatedAtomClass, ast::NegatedAtom> const
        negated_atom = "negated_atom";
    x3::rule<LiteralClass, ast::Literal> const
        literal = "literal";

    x3::rule<MultiOperatorMulClass, ast::MultiOperatorMul> const
        multi_operator_mul = "multi_operator_mul";
    x3::rule<MultiOperatorPlusClass, ast::MultiOperatorPlus> const
        multi_operator_plus = "multi_operator_plus";
    x3::rule<MultiOperatorClass, ast::MultiOperator> const
        multi_operator = "multi_operator";
    x3::rule<BinaryOperatorMinusClass, ast::BinaryOperatorMinus> const
        binary_operator_minus = "binary_operator_minus";
    x3::rule<BinaryOperatorDivClass, ast::BinaryOperatorDiv> const
        binary_operator_div = "binary_operator_div";
    x3::rule<BinaryOperatorClass, ast::BinaryOperator> const
        binary_operator = "binary_operator";

    x3::rule<BinaryComparatorGreaterClass, ast::BinaryComparatorGreater> const
        binary_comparator_greater = "binary_comparator_greater";
    x3::rule<BinaryComparatorLessClass, ast::BinaryComparatorLess> const
        binary_comparator_less = "binary_comparator_less";
    x3::rule<BinaryComparatorEqualClass, ast::BinaryComparatorEqual> const
        binary_comparator_equal = "binary_comparator_equal";
    x3::rule<BinaryComparatorGreaterEqualClass, ast::BinaryComparatorGreaterEqual> const
        binary_comparator_greater_equal = "binary_comparator_greater_equal";
    x3::rule<BinaryComparatorLessEqualClass, ast::BinaryComparatorLessEqual> const
        binary_comparator_less_equal = "binary_comparator_less_equal";
    x3::rule<BinaryComparatorClass, ast::BinaryComparator> const
        binary_comparator = "binary_comparator";

    x3::rule<FunctionHeadClass, ast::FunctionHead> const
        function_head = "function_head";
    x3::rule<FunctionExpressionNumberClass, ast::FunctionExpressionNumber> const
        function_expression_number = "function_expression_number";
    x3::rule<FunctionExpressionBinaryOpClass, ast::FunctionExpressionBinaryOp> const
        function_expression_binary_op = "function_expression_binary_op";
    x3::rule<FunctionExpressionMinusClass, ast::FunctionExpressionMinus> const
        function_expression_minus = "function_expression_minus";
    x3::rule<FunctionExpressionHeadClass, ast::FunctionExpressionHead> const
        function_expression_head = "function_expression_head";
    x3::rule<FunctionExpressionClass, ast::FunctionExpression> const
        function_expression = "function_expression";

    x3::rule<GoalDescriptorAtomClass, ast::GoalDescriptorAtom> const
        goal_descriptor_atom = "goal_descriptor_atom";
    x3::rule<GoalDescriptorLiteralClass, ast::GoalDescriptorLiteral> const
        goal_descriptor_literal = "goal_descriptor_literal";
    x3::rule<GoalDescriptorAndClass, ast::GoalDescriptorAnd> const
        goal_descriptor_and = "goal_descriptor_and";
    x3::rule<GoalDescriptorOrClass, ast::GoalDescriptorOr> const
        goal_descriptor_or = "goal_descriptor_or";
    x3::rule<GoalDescriptorNotClass, ast::GoalDescriptorNot> const
        goal_descriptor_not = "goal_descriptor_not";
    x3::rule<GoalDescriptorImplyClass, ast::GoalDescriptorImply> const
        goal_descriptor_imply = "goal_descriptor_imply";
    x3::rule<GoalDescriptorExistsClass, ast::GoalDescriptorExists> const
        goal_descriptor_exists = "goal_descriptor_exists";
    x3::rule<GoalDescriptorForallClass, ast::GoalDescriptorForall> const
        goal_descriptor_forall = "goal_descriptor_forall";
    x3::rule<GoalDescriptorFunctionComparisonClass, ast::GoalDescriptorFunctionComparison> const
        goal_descriptor_function_comparison = "goal_descriptor_function_comparison";
    x3::rule<GoalDescriptorClass, ast::GoalDescriptor> const
        goal_descriptor = "goal_descriptor";

    x3::rule<ConstraintGoalDescriptorAndClass, ast::ConstraintGoalDescriptorAnd> const
        constraint_goal_descriptor_and = "constraint_goal_descriptor_and";
    x3::rule<ConstraintGoalDescriptorForallClass, ast::ConstraintGoalDescriptorForall> const
        constraint_goal_descriptor_forall = "constraint_goal_descriptor_forall";
    x3::rule<ConstraintGoalDescriptorAtEndClass, ast::ConstraintGoalDescriptorAtEnd> const
        constraint_goal_descriptor_at_end = "constraint_goal_descriptor_at_end";
    x3::rule<ConstraintGoalDescriptorAlwaysClass, ast::ConstraintGoalDescriptorAlways> const
        constraint_goal_descriptor_always = "constraint_goal_descriptor_always";
    x3::rule<ConstraintGoalDescriptorSometimeClass, ast::ConstraintGoalDescriptorSometime> const
        constraint_goal_descriptor_sometime = "constraint_goal_descriptor_sometime";
    x3::rule<ConstraintGoalDescriptorWithinClass, ast::ConstraintGoalDescriptorWithin> const
        constraint_goal_descriptor_within = "constraint_goal_descriptor_within";
    x3::rule<ConstraintGoalDescriptorAtMostOnceClass, ast::ConstraintGoalDescriptorAtMostOnce> const
        constraint_goal_descriptor_at_most_once = "constraint_goal_descriptor_at_most_once";
    x3::rule<ConstraintGoalDescriptorSometimeAfterClass, ast::ConstraintGoalDescriptorSometimeAfter> const
        constraint_goal_descriptor_sometime_after = "constraint_goal_descriptor_sometime_after";
    x3::rule<ConstraintGoalDescriptorSometimeBeforeClass, ast::ConstraintGoalDescriptorSometimeBefore> const
        constraint_goal_descriptor_sometime_before = "constraint_goal_descriptor_sometime_before";
    x3::rule<ConstraintGoalDescriptorAlwaysWithinClass, ast::ConstraintGoalDescriptorAlwaysWithin> const
        constraint_goal_descriptor_always_within = "constraint_goal_descriptor_always_within";
    x3::rule<ConstraintGoalDescriptorHoldDuringClass, ast::ConstraintGoalDescriptorHoldDuring> const
        constraint_goal_descriptor_hold_during = "constraint_goal_descriptor_hold_during";
    x3::rule<ConstraintGoalDescriptorHoldAfterClass, ast::ConstraintGoalDescriptorHoldAfter> const
        constraint_goal_descriptor_hold_after = "constraint_goal_descriptor_hold_after";
    x3::rule<ConstraintGoalDescriptorClass, ast::ConstraintGoalDescriptor> const
        constraint_goal_descriptor = "constraint_goal_descriptor";

    x3::rule<PreferenceNameClass, ast::PreferenceName> const
        preference_name = "preference_name";
    x3::rule<PreconditionGoalDescriptorSimpleClass, ast::PreconditionGoalDescriptorSimple> const
        precondition_goal_descriptor_simple = "precondition_goal_descriptor_simple";
    x3::rule<PreconditionGoalDescriptorAndClass, ast::PreconditionGoalDescriptorAnd> const
        precondition_goal_descriptor_and = "precondition_goal_descriptor_and";
    x3::rule<PreconditionGoalDescriptorPreferenceClass, ast::PreconditionGoalDescriptorPreference> const
        precondition_goal_descriptor_preference = "precondition_goal_descriptor_preference";
    x3::rule<PreconditionGoalDescriptorForallClass, ast::PreconditionGoalDescriptorForall> const
        precondition_goal_descriptor_forall = "precondition_goal_descriptor_forall";
    x3::rule<PreconditionGoalDescriptorClass, ast::PreconditionGoalDescriptor> const
        precondition_goal_descriptor = "precondition_goal_descriptor";

    x3::rule<AssignOperatorAssignClass, ast::AssignOperatorAssign> const
        assign_operator_assign = "assign_operator_assign";
    x3::rule<AssignOperatorScaleUpClass, ast::AssignOperatorScaleUp> const
        assign_operator_scale_up = "assign_operator_scale_up";
    x3::rule<AssignOperatorScaleDownClass, ast::AssignOperatorScaleDown> const
        assign_operator_scale_down = "assign_operator_scale_down";
    x3::rule<AssignOperatorIncreaseClass, ast::AssignOperatorIncrease> const
        assign_operator_increase = "assign_operator_increase";
    x3::rule<AssignOperatorDecreaseClass, ast::AssignOperatorDecrease> const
        assign_operator_decrease = "assign_operator_decrease";
    x3::rule<AssignOperatorClass, ast::AssignOperator> const
        assign_operator = "assign_operator";

    x3::rule<SimpleEffectLiteralClass, ast::SimpleEffectLiteral> const
        simple_effect_literal = "simple_effect_literal";
    x3::rule<SimpleEffectFluentClass, ast::SimpleEffectFluent> const
        simple_effect_fluent = "simple_effect_fluent";
    x3::rule<SimpleEffectClass, ast::SimpleEffect> const
        simple_effect = "simple_effect";
    x3::rule<ConditionalEffectForallClass, ast::ConditionalEffectForall> const
        conditional_effect_forall = "conditional_effect_forall";
    x3::rule<ConditionalEffectWhenClass, ast::ConditionalEffectWhen> const
        conditional_effect_when = "conditional_effect_when";
    x3::rule<ConditionalEffectClass, ast::ConditionalEffect> const
        conditional_effect = "conditional_effect";
    x3::rule<EffectClass, ast::Effect> const
        effect = "effect";

    x3::rule<ActionSymbolClass, ast::ActionSymbol> const
        action_symbol = "action_symbol";
    x3::rule<ActionBodyClass, ast::ActionBody> const
        action_body = "action_body";

    x3::rule<ActionClass, ast::Action> const
        action = "action";

    x3::rule<DomainNameClass, ast::DomainName> const
        domain_name = "domain_name";
    x3::rule<RequirementsClass, ast::Requirements> const
        requirements = "requirements";
    x3::rule<TypesClass, ast::Types> const
        types = "types";
    x3::rule<ConstantsClass, ast::Constants> const
        constants = "constants";
    x3::rule<PredicatesClass, ast::Predicates> const
        predicates = "predicates";
    x3::rule<FunctionsClass, ast::Functions> const
        functions = "functions";
    x3::rule<ConstraintsClass, ast::Constraints> const
        constraints = "constraints";
    x3::rule<StructureClass, ast::Structure> const
        structure = "structure";
    domain_description_type const domain_description = "domain_description";


    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////

    const auto name_def = alpha >> lexeme[*(alnum | char_('-') | char_('_'))];
    const auto variable_def = char_('?') > name;
    const auto number_def = int_;
    const auto term_def = name | variable;

    const auto strips_requirement_def = lit(":strips") >> x3::attr(ast::StripsRequirement{});
    const auto typing_requirement_def = lit(":typing") >> x3::attr(ast::TypingRequirement{});
    const auto requirement_def = strips_requirement | typing_requirement;

    const auto fluent_type_def = lit('(') >> lit("fluent") > type > lit(')');
    const auto either_type_def = lit('(') >> lit("either") >> +type > lit(')');
    const auto type_def = name | fluent_type | either_type;
    const auto typed_list_of_names_recursively_def = +name > lit('-') > type >> typed_list_of_names_recursively;
    const auto typed_list_of_names_def = ((*name) | typed_list_of_names_recursively);
    const auto typed_list_of_variables_recursively_def = +variable > lit('-') > type >> typed_list_of_variables_recursively;
    const auto typed_list_of_variables_def = ((*variable) | typed_list_of_variables_recursively);

    const auto predicate_def = name;
    const auto atomic_formula_skeleton_def = predicate > typed_list_of_variables;

    const auto function_symbol_def = name;
    const auto function_type_def = number;
    const auto atomic_function_skeleton_def = lit('(') > function_symbol > typed_list_of_variables > lit(')');
    const auto function_typed_list_of_atomic_function_skeletons_recursively_def = +atomic_function_skeleton > lit('-') > function_type > function_typed_list_of_atomic_function_skeletons_recursively;
    const auto function_typed_list_of_atomic_function_skeletons_def = ((*atomic_function_skeleton_def) | function_typed_list_of_atomic_function_skeletons_recursively);

    const auto atomic_formula_of_terms_def = lit('(') >> predicate >> *term > lit(')');
    const auto atom_def = atomic_formula_of_terms;
    const auto negated_atom_def = lit('(') >> lit("not") >> atomic_formula_of_terms > lit(')');
    const auto literal_def = atom | negated_atom;

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

    const auto function_head_def = lit('(') >> function_symbol >> x3::attr(std::vector<ast::Term>{}) | (lit('(') >> function_symbol >> *term > lit(')'));
    const auto function_expression_number_def = number;
    const auto function_expression_binary_op_def = lit('(') >> binary_operator > function_expression > function_expression > lit(')');
    const auto function_expression_minus_def = lit('(') >> lit('-') > function_expression > lit(')');
    const auto function_expression_head_def = function_head;
    const auto function_expression_def = function_expression_number | function_expression_binary_op | function_expression_minus | function_expression_head;

    const auto goal_descriptor_atom_def = atom;
    const auto goal_descriptor_literal_def = literal;
    const auto goal_descriptor_and_def = lit('(') >> lit("and") >> *goal_descriptor > lit(')');
    const auto goal_descriptor_or_def = lit('(') >> lit("or") >> *goal_descriptor > lit(')');
    const auto goal_descriptor_not_def = lit('(') >> lit("not") > goal_descriptor > lit(')');
    const auto goal_descriptor_imply_def = lit('(') >> lit("imply") > goal_descriptor > goal_descriptor > lit(')');
    const auto goal_descriptor_exists_def = lit('(') >> lit("exists") > typed_list_of_variables > goal_descriptor > lit(')');
    const auto goal_descriptor_forall_def = lit('(') >> lit("forall") > typed_list_of_variables > goal_descriptor > lit(')');
    const auto goal_descriptor_function_comparison_def = lit('(') >> binary_comparator >> function_expression > function_expression > lit(')');
    const auto goal_descriptor_def = goal_descriptor_atom | goal_descriptor_literal | goal_descriptor_and | goal_descriptor_or
        | goal_descriptor_not | goal_descriptor_imply | goal_descriptor_exists | goal_descriptor_forall | goal_descriptor_function_comparison;

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
    const auto constraint_goal_descriptor_def = constraint_goal_descriptor_and | constraint_goal_descriptor_forall | constraint_goal_descriptor_at_end
        | constraint_goal_descriptor_always | constraint_goal_descriptor_sometime | constraint_goal_descriptor_within
        | constraint_goal_descriptor_at_most_once | constraint_goal_descriptor_sometime_after | constraint_goal_descriptor_sometime_before
        | constraint_goal_descriptor_always_within | constraint_goal_descriptor_hold_during | constraint_goal_descriptor_hold_after;

    const auto preference_name_def = name;
    const auto precondition_goal_descriptor_simple_def = goal_descriptor;
    const auto precondition_goal_descriptor_and_def = lit('(') >> *precondition_goal_descriptor > lit(')');
    const auto precondition_goal_descriptor_preference_def = lit('(') >> lit("preference") > preference_name > goal_descriptor > lit(')');
    const auto precondition_goal_descriptor_forall_def = lit('(') >> lit("forall") > typed_list_of_variables > precondition_goal_descriptor > lit(')');
    const auto precondition_goal_descriptor_def = precondition_goal_descriptor_simple | precondition_goal_descriptor_and | precondition_goal_descriptor_preference | precondition_goal_descriptor_forall;

    const auto assign_operator_assign_def = lit("assign") >> x3::attr(ast::AssignOperatorAssign{});
    const auto assign_operator_scale_up_def = lit("scale-up") >> x3::attr(ast::AssignOperatorScaleUp{});
    const auto assign_operator_scale_down_def = lit("scale-down") >> x3::attr(ast::AssignOperatorScaleDown{});
    const auto assign_operator_increase_def = lit("increase") >> x3::attr(ast::AssignOperatorIncrease{});
    const auto assign_operator_decrease_def = lit("decrease") >> x3::attr(ast::AssignOperatorDecrease{});
    const auto assign_operator_def = assign_operator_assign | assign_operator_scale_up | assign_operator_scale_down | assign_operator_increase | assign_operator_decrease;

    const auto simple_effect_literal_def = literal;
    const auto simple_effect_fluent_def = lit('(') >> assign_operator >> function_head >> function_expression > lit(')');
    const auto simple_effect_def = simple_effect_literal | simple_effect_fluent;
    const auto conditional_effect_forall_def = lit('(') >> lit("forall") >> typed_list_of_variables >> effect > lit(')');
    const auto conditional_effect_when_def = lit('(') >> lit("when") >> goal_descriptor >> effect > lit(')');
    const auto conditional_effect_def = conditional_effect_forall | conditional_effect_when;
    const auto effect_def = simple_effect | conditional_effect | *effect;

    const auto action_symbol_def = name;
    const auto action_body_def = -(lit(":precondition") >> (lit('(') >> lit(')')) | precondition_goal_descriptor)
                                >> -(lit(":effects") >> (lit('(') >> lit(')')) | effect);
    const auto action_def = lit('(') >> lit(":action") >> action_symbol
                                     >> lit(":parameters") >> lit('(') >> typed_list_of_variables >> lit(')')
                                     >> action_body
                            >> lit(')');

    const auto domain_name_def = lit('(') >> lit("domain") > name > lit(')');
    const auto requirements_def = lit('(') >> lit(":requirements") >> *requirement >> lit(')');
    const auto types_def = lit('(') >> lit(":types") >> typed_list_of_names > lit(')');
    const auto constants_def = lit('(') >> lit(":constants") >> typed_list_of_names > lit(')');
    const auto predicates_def = lit('(') >> lit(":predicates") >> *atomic_formula_skeleton > lit(')');
    const auto functions_def = lit('(') >> lit(":functions") >> *function_typed_list_of_atomic_function_skeletons > lit(')');
    const auto constraints_def = lit('(') >> lit(":constraints") > constraint_goal_descriptor > lit(')');
    const auto structure_def = action;

    const auto domain_description_def =
        lit('(') > lit("define")
           > domain_name
           >> -requirements
           >> types
           >> constants
           >> predicates
           >> -functions
           >> -constraints
           >> *structure
        > lit(')');

    BOOST_SPIRIT_DEFINE(
        name, variable, number, term,
        strips_requirement, typing_requirement, requirement, requirements,
        fluent_type, either_type, type, typed_list_of_names_recursively, typed_list_of_names, typed_list_of_variables_recursively, typed_list_of_variables,
        predicate, atomic_formula_skeleton,
        function_symbol, function_type, atomic_function_skeleton, function_typed_list_of_atomic_function_skeletons_recursively, function_typed_list_of_atomic_function_skeletons,
        atomic_formula_of_terms, atom, negated_atom, literal,
        multi_operator_mul, multi_operator_plus, multi_operator,
        binary_operator_minus, binary_operator_div, binary_operator,
        binary_comparator_greater, binary_comparator_less, binary_comparator_equal,
        binary_comparator_greater_equal, binary_comparator_less_equal, binary_comparator,
        function_head, function_expression_number, function_expression_binary_op,
        function_expression_minus, function_expression_head, function_expression)

    BOOST_SPIRIT_DEFINE(
        goal_descriptor_atom, goal_descriptor_literal, goal_descriptor_and, goal_descriptor_or,
        goal_descriptor_not, goal_descriptor_imply, goal_descriptor_exists, goal_descriptor_forall,
        goal_descriptor_function_comparison, goal_descriptor,
        constraint_goal_descriptor_and, constraint_goal_descriptor_forall, constraint_goal_descriptor_at_end,
        constraint_goal_descriptor_always, constraint_goal_descriptor_sometime, constraint_goal_descriptor_within,
        constraint_goal_descriptor_at_most_once, constraint_goal_descriptor_sometime_after,  constraint_goal_descriptor_sometime_before,
        constraint_goal_descriptor_always_within, constraint_goal_descriptor_hold_during, constraint_goal_descriptor_hold_after, constraint_goal_descriptor
    )

    BOOST_SPIRIT_DEFINE(
        preference_name, precondition_goal_descriptor_simple, precondition_goal_descriptor_and,
        precondition_goal_descriptor_preference, precondition_goal_descriptor_forall, precondition_goal_descriptor,
        assign_operator_assign, assign_operator_scale_up, assign_operator_scale_down,
        assign_operator_increase, assign_operator_decrease, assign_operator,
        simple_effect_literal, simple_effect_fluent, simple_effect, conditional_effect_forall,
        conditional_effect_when, conditional_effect, effect,
        action_symbol, action_body, action
    )

    BOOST_SPIRIT_DEFINE(
        domain_name, types, constants, predicates, functions, constraints, structure, domain_description
    )


    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

    struct NameClass : x3::annotate_on_success {};
    struct VariableClass : x3::annotate_on_success {};
    struct NumberClass : x3::annotate_on_success {};
    struct TermClass : x3::annotate_on_success {};

    struct StripsRequirementClass : x3::annotate_on_success {};
    struct TypingRequirementClass : x3::annotate_on_success {};
    struct RequirementClass : x3::annotate_on_success {};

    struct FluentTypeClass : x3::annotate_on_success {};
    struct EitherTypeClass : x3::annotate_on_success {};
    struct TypeClass : x3::annotate_on_success {};
    struct TypedListOfNamesRecursivelyClass : x3::annotate_on_success {};
    struct TypedListOfNamesClass : x3::annotate_on_success {};
    struct TypedListOfVariablesRecursivelyClass : x3::annotate_on_success {};
    struct TypedListOfVariablesClass : x3::annotate_on_success {};

    struct PredicateClass : x3::annotate_on_success {};
    struct AtomicFormulaSkeletonClass : x3::annotate_on_success {};

    struct FunctionSymbolClass : x3::annotate_on_success {};
    struct FunctionTypeClass : x3::annotate_on_success {};
    struct AtomicFunctionSkeletonClass : x3::annotate_on_success {};
    struct FunctionTypedListOfAtomicFunctionSkeletonsRecursivelyClass : x3::annotate_on_success {};
    struct FunctionTypedListOfAtomicFunctionSkeletonsClass : x3::annotate_on_success {};

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

    struct FunctionHeadClass : x3::annotate_on_success {};
    struct FunctionExpressionNumberClass : x3::annotate_on_success {};
    struct FunctionExpressionBinaryOpClass : x3::annotate_on_success {};
    struct FunctionExpressionMinusClass : x3::annotate_on_success {};
    struct FunctionExpressionHeadClass : x3::annotate_on_success {};
    struct FunctionExpressionClass : x3::annotate_on_success {};

    struct GoalDescriptorAtomClass : x3::annotate_on_success {};
    struct GoalDescriptorLiteralClass : x3::annotate_on_success {};
    struct GoalDescriptorAndClass : x3::annotate_on_success {};
    struct GoalDescriptorOrClass : x3::annotate_on_success {};
    struct GoalDescriptorNotClass : x3::annotate_on_success {};
    struct GoalDescriptorImplyClass : x3::annotate_on_success {};
    struct GoalDescriptorExistsClass : x3::annotate_on_success {};
    struct GoalDescriptorForallClass : x3::annotate_on_success {};
    struct GoalDescriptorFunctionComparisonClass : x3::annotate_on_success {};
    struct GoalDescriptorClass : x3::annotate_on_success {};

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
    struct ConstraintGoalDescriptorClass : x3::annotate_on_success {};

    struct PreferenceNameClass : x3::annotate_on_success {};
    struct PreconditionGoalDescriptorSimpleClass : x3::annotate_on_success {};
    struct PreconditionGoalDescriptorAndClass : x3::annotate_on_success {};
    struct PreconditionGoalDescriptorPreferenceClass : x3::annotate_on_success {};
    struct PreconditionGoalDescriptorForallClass : x3::annotate_on_success {};
    struct PreconditionGoalDescriptorClass : x3::annotate_on_success {};

    struct AssignOperatorAssignClass : x3::annotate_on_success {};
    struct AssignOperatorScaleUpClass : x3::annotate_on_success {};
    struct AssignOperatorScaleDownClass : x3::annotate_on_success {};
    struct AssignOperatorIncreaseClass : x3::annotate_on_success {};
    struct AssignOperatorDecreaseClass : x3::annotate_on_success {};
    struct AssignOperatorClass : x3::annotate_on_success {};

    struct SimpleEffectLiteralClass : x3::annotate_on_success {};
    struct SimpleEffectFluentClass : x3::annotate_on_success {};
    struct SimpleEffectClass : x3::annotate_on_success {};
    struct ConditionalEffectForallClass : x3::annotate_on_success {};
    struct ConditionalEffectWhenClass : x3::annotate_on_success {};
    struct ConditionalEffectClass : x3::annotate_on_success {};
    struct EffectClass : x3::annotate_on_success {};

    struct ActionSymbolClass : x3::annotate_on_success {};
    struct ActionBodyClass : x3::annotate_on_success {};

    struct ActionClass : x3::annotate_on_success {};
    struct DurativeActionClass : x3::annotate_on_success {};
    struct DerivedPredicateClass : x3::annotate_on_success {};

    struct DomainNameClass : x3::annotate_on_success {};
    struct RequirementsClass : x3::annotate_on_success {};
    struct TypesClass : x3::annotate_on_success {};
    struct ConstantsClass : x3::annotate_on_success {};
    struct PredicatesClass : x3::annotate_on_success {};
    struct FunctionsClass : x3::annotate_on_success {};
    struct ConstraintsClass : x3::annotate_on_success {};
    struct StructureClass : x3::annotate_on_success {};
    struct DomainDescriptionClass : x3::annotate_on_success, error_handler_domain {};
}

namespace loki::domain
{
    parser::domain_description_type const& domain_description() {
        return parser::domain_description;
    }
}

#endif