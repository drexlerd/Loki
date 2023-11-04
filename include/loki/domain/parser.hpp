#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PARSER_HPP_

#include <boost/spirit/home/x3.hpp>

#include "ast.hpp"


namespace loki::domain
{
    namespace x3 = boost::spirit::x3;

    ///////////////////////////////////////////////////////////////////////////
    // parser public interface
    ///////////////////////////////////////////////////////////////////////////
    namespace parser {
        struct NameClass;
        struct VariableClass;
        struct NumberClass;
        struct TermClass;
        struct UndefinedClass;

        struct RequirementStripsClass;
        struct RequirementTypingClass;
        struct RequirementNegativePreconditionsClass;
        struct RequirementDisjunctivePreconditionsClass;
        struct RequirementEqualityClass;
        struct RequirementExistentialPreconditionsClass;
        struct RequirementUniversalPreconditionsClass;
        struct RequirementQuantifiedPreconditionsClass;
        struct RequirementConditionalEffectsClass;
        struct RequirementFluentsClass;
        struct RequirementObjectFluentsClass;
        struct RequirementNumericFluentsClass;
        struct RequirementAdlClass;
        struct RequirementDurativeActionsClass;
        struct RequirementDerivedPredicatesClass;
        struct RequirementTimedInitialLiteralsClass;
        struct RequirementPreferencesClass;
        struct RequirementConstraintsClass;
        struct RequirementClass;

        struct TypeClass;
        struct TypeObjectClass;
        struct TypeEitherClass;
        struct TypedListOfNamesRecursivelyClass;
        struct TypedListOfNamesClass;
        struct TypedListOfVariablesRecursivelyClass;
        struct TypedListOfVariablesClass;

        struct PredicateClass;
        struct AtomicFormulaSkeletonClass;

        struct FunctionSymbolClass;
        struct FunctionTermClass;
        struct FunctionTypeNumberClass;
        struct FunctionTypeTypeClass;
        struct FunctionTypeClass;
        struct AtomicFunctionSkeletonClass;
        struct FunctionTypedListOfAtomicFunctionSkeletonsRecursivelyClass;
        struct FunctionTypedListOfAtomicFunctionSkeletonsClass;

        struct AtomicFormulaOfTermsPredicateClass;
        struct AtomicFormulaOfTermsEqualityClass;
        struct AtomicFormulaOfTermsClass;
        struct AtomOfTermsClass;
        struct NegatedAtomOfTermsClass;
        struct LiteralOfTermsClass;

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
        struct FunctionExpressionClass;
        struct FunctionExpressionNumberClass;
        struct FunctionExpressionBinaryOpClass;
        struct FunctionExpressionMinusClass;
        struct FunctionExpressionHeadClass;

        struct GoalDescriptorClass;
        struct GoalDescriptorAtomClass;
        struct GoalDescriptorLiteralClass;
        struct GoalDescriptorAndClass;
        struct GoalDescriptorOrClass;
        struct GoalDescriptorNotClass;
        struct GoalDescriptorImplyClass;
        struct GoalDescriptorExistsClass;
        struct GoalDescriptorForallClass;
        struct GoalDescriptorFunctionComparisonClass;

        struct ConstraintGoalDescriptorClass;
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

        struct PreferenceNameClass;
        struct PreconditionGoalDescriptorClass;
        struct PreconditionGoalDescriptorSimpleClass;
        struct PreconditionGoalDescriptorAndClass;
        struct PreconditionGoalDescriptorPreferenceClass;
        struct PreconditionGoalDescriptorForallClass;

        struct AssignOperatorAssignClass;
        struct AssignOperatorScaleUpClass;
        struct AssignOperatorScaleDownClass;
        struct AssignOperatorIncreaseClass;
        struct AssignOperatorDecreaseClass;
        struct AssignOperatorClass;

        struct EffectClass;
        struct EffectProductionLiteralClass;
        struct EffectProductionNumericFluentClass;
        struct EffectProductionObjectFluentClass;
        struct EffectProductionClass;
        struct EffectConditionalForallClass;
        struct EffectConditionalWhenClass;
        struct EffectConditionalClass;
        struct ActionSymbolClass;
        struct ActionBodyClass;
        struct ActionClass;
        struct DurativeActionClass;  // TODO
        struct DerivedPredicateClass;

        struct DomainNameClass;
        struct RequirementsClass;
        struct TypesClass;
        struct ConstantsClass;
        struct PredicatesClass;
        struct FunctionsClass;
        struct ConstraintsClass;
        struct StructureClass;
        struct DomainDescriptionClass;


        typedef x3::rule<NameClass, ast::Name> name_type;
        typedef x3::rule<VariableClass, ast::Variable> variable_type;
        typedef x3::rule<NumberClass, ast::Number> number_type;
        typedef x3::rule<TermClass, ast::Term> term_type;
        typedef x3::rule<UndefinedClass, ast::Undefined> undefined_type;

        typedef x3::rule<RequirementStripsClass, ast::RequirementStrips> requirement_strips_type;
        typedef x3::rule<RequirementTypingClass, ast::RequirementTyping> requirement_typing_type;
        typedef x3::rule<RequirementNegativePreconditionsClass, ast::RequirementNegativePreconditions> requirement_negative_preconditions_type;
        typedef x3::rule<RequirementDisjunctivePreconditionsClass, ast::RequirementDisjunctivePreconditions> requirement_disjunctive_preconditions_type;
        typedef x3::rule<RequirementEqualityClass, ast::RequirementEquality> requirement_equality_type;
        typedef x3::rule<RequirementExistentialPreconditionsClass, ast::RequirementExistentialPreconditions> requirement_existential_preconditions_type;
        typedef x3::rule<RequirementUniversalPreconditionsClass, ast::RequirementUniversalPreconditions> requirement_universal_preconditions_type;
        typedef x3::rule<RequirementQuantifiedPreconditionsClass, ast::RequirementQuantifiedPreconditions> requirement_quantified_preconditions_type;
        typedef x3::rule<RequirementConditionalEffectsClass, ast::RequirementConditionalEffects> requirement_conditional_effects_type;
        typedef x3::rule<RequirementFluentsClass, ast::RequirementFluents> requirement_fluents_type;
        typedef x3::rule<RequirementObjectFluentsClass, ast::RequirementObjectFluents> requirement_object_fluents_type;
        typedef x3::rule<RequirementNumericFluentsClass, ast::RequirementNumericFluents> requirement_numeric_fluents_type;
        typedef x3::rule<RequirementAdlClass, ast::RequirementAdl> requirement_adl_type;
        typedef x3::rule<RequirementDurativeActionsClass, ast::RequirementDurativeActions> requirement_durative_actions_type;
        typedef x3::rule<RequirementDerivedPredicatesClass, ast::RequirementDerivedPredicates> requirement_derived_predicates_type;
        typedef x3::rule<RequirementTimedInitialLiteralsClass, ast::RequirementTimedInitialLiterals> requirement_timed_initial_literals_type;
        typedef x3::rule<RequirementPreferencesClass, ast::RequirementPreferences> requirement_preferences_type;
        typedef x3::rule<RequirementConstraintsClass, ast::RequirementConstraints> requirement_constraints_type;
        typedef x3::rule<RequirementClass, ast::Requirement> requirement_type;

        typedef x3::rule<TypeClass, ast::Type> type_type;
        typedef x3::rule<TypeObjectClass, ast::TypeObject> type_object_type;
        typedef x3::rule<TypeEitherClass, ast::TypeEither> type_either_type;
        typedef x3::rule<TypedListOfNamesRecursivelyClass, ast::TypedListOfNamesRecursively> typed_list_of_names_recursively_type;
        typedef x3::rule<TypedListOfNamesClass, ast::TypedListOfNames> typed_list_of_names_type;
        typedef x3::rule<TypedListOfVariablesRecursivelyClass, ast::TypedListOfVariablesRecursively> typed_list_of_variables_recursively_type;
        typedef x3::rule<TypedListOfVariablesClass, ast::TypedListOfVariables> typed_list_of_variables_type;

        typedef x3::rule<PredicateClass, ast::Predicate> predicate_type;
        typedef x3::rule<AtomicFormulaSkeletonClass, ast::AtomicFormulaSkeleton> atomic_formula_skeleton_type;

        typedef x3::rule<FunctionSymbolClass, ast::FunctionSymbol> function_symbol_type;
        typedef x3::rule<FunctionTermClass, ast::FunctionTerm> function_term_type;
        typedef x3::rule<FunctionTypeNumberClass, ast::FunctionTypeNumber> function_type_number_type;
        typedef x3::rule<FunctionTypeTypeClass, ast::FunctionTypeType> function_type_type_type;
        typedef x3::rule<FunctionTypeClass, ast::FunctionType> function_type_type;
        typedef x3::rule<AtomicFunctionSkeletonClass, ast::AtomicFunctionSkeleton> atomic_function_skeleton_type;
        typedef x3::rule<FunctionTypedListOfAtomicFunctionSkeletonsRecursivelyClass, ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively> function_typed_list_of_atomic_function_skeletons_recursively_type;
        typedef x3::rule<FunctionTypedListOfAtomicFunctionSkeletonsClass, ast::FunctionTypedListOfAtomicFunctionSkeletons> function_typed_list_of_atomic_function_skeletons_type;

        typedef x3::rule<AtomicFormulaOfTermsPredicateClass, ast::AtomicFormulaOfTermsPredicate> atomic_formula_of_terms_predicate_type;
        typedef x3::rule<AtomicFormulaOfTermsEqualityClass, ast::AtomicFormulaOfTermsEquality> atomic_formula_of_terms_equality_type;
        typedef x3::rule<AtomicFormulaOfTermsClass, ast::AtomicFormulaOfTerms> atomic_formula_of_terms_type;
        typedef x3::rule<AtomOfTermsClass, ast::AtomOfTerms> atom_of_terms_type;
        typedef x3::rule<NegatedAtomOfTermsClass, ast::NegatedAtomOfTerms> negated_atom_of_terms_type;
        typedef x3::rule<LiteralOfTermsClass, ast::LiteralOfTerms> literal_of_terms_type;

        typedef x3::rule<MultiOperatorMulClass, ast::MultiOperatorMul> multi_operator_mul_type;
        typedef x3::rule<MultiOperatorPlusClass, ast::MultiOperatorPlus> multi_operator_plus_type;
        typedef x3::rule<MultiOperatorClass, ast::MultiOperator> multi_operator_type;
        typedef x3::rule<BinaryOperatorMinusClass, ast::BinaryOperatorMinus> binary_operator_minus_type;
        typedef x3::rule<BinaryOperatorDivClass, ast::BinaryOperatorDiv> binary_operator_div_type;
        typedef x3::rule<BinaryOperatorClass, ast::BinaryOperator> binary_operator_type;

        typedef x3::rule<BinaryComparatorGreaterClass, ast::BinaryComparatorGreater> binary_comparator_greater_type;
        typedef x3::rule<BinaryComparatorLessClass, ast::BinaryComparatorLess> binary_comparator_less_type;
        typedef x3::rule<BinaryComparatorEqualClass, ast::BinaryComparatorEqual> binary_comparator_equal_type;
        typedef x3::rule<BinaryComparatorGreaterEqualClass, ast::BinaryComparatorGreaterEqual> binary_comparator_greater_equal_type;
        typedef x3::rule<BinaryComparatorLessEqualClass, ast::BinaryComparatorLessEqual> binary_comparator_less_equal_type;
        typedef x3::rule<BinaryComparatorClass, ast::BinaryComparator> binary_comparator_type;

        typedef x3::rule<FunctionExpressionClass, ast::FunctionExpression> function_expression_type;
        typedef x3::rule<FunctionHeadClass, ast::FunctionHead> function_head_type;
        typedef x3::rule<FunctionExpressionNumberClass, ast::FunctionExpressionNumber> function_expression_number_type;
        typedef x3::rule<FunctionExpressionBinaryOpClass, ast::FunctionExpressionBinaryOp> function_expression_binary_op_type;
        typedef x3::rule<FunctionExpressionMinusClass, ast::FunctionExpressionMinus> function_expression_minus_type;
        typedef x3::rule<FunctionExpressionHeadClass, ast::FunctionExpressionHead> function_expression_head_type;

        typedef x3::rule<GoalDescriptorClass, ast::GoalDescriptor> goal_descriptor_type;
        typedef x3::rule<GoalDescriptorAtomClass, ast::GoalDescriptorAtom> goal_descriptor_atom_type;
        typedef x3::rule<GoalDescriptorLiteralClass, ast::GoalDescriptorLiteral> goal_descriptor_literal_type;
        typedef x3::rule<GoalDescriptorAndClass, ast::GoalDescriptorAnd> goal_descriptor_and_type;
        typedef x3::rule<GoalDescriptorOrClass, ast::GoalDescriptorOr> goal_descriptor_or_type;
        typedef x3::rule<GoalDescriptorNotClass, ast::GoalDescriptorNot> goal_descriptor_not_type;
        typedef x3::rule<GoalDescriptorImplyClass, ast::GoalDescriptorImply> goal_descriptor_imply_type;
        typedef x3::rule<GoalDescriptorExistsClass, ast::GoalDescriptorExists> goal_descriptor_exists_type;
        typedef x3::rule<GoalDescriptorForallClass, ast::GoalDescriptorForall> goal_descriptor_forall_type;
        typedef x3::rule<GoalDescriptorFunctionComparisonClass, ast::GoalDescriptorFunctionComparison> goal_descriptor_function_comparison_type;

        typedef x3::rule<ConstraintGoalDescriptorClass, ast::ConstraintGoalDescriptor> constraint_goal_descriptor_type;
        typedef x3::rule<ConstraintGoalDescriptorAndClass, ast::ConstraintGoalDescriptorAnd> constraint_goal_descriptor_and_type;
        typedef x3::rule<ConstraintGoalDescriptorForallClass, ast::ConstraintGoalDescriptorForall> constraint_goal_descriptor_forall_type;
        typedef x3::rule<ConstraintGoalDescriptorAtEndClass, ast::ConstraintGoalDescriptorAtEnd> constraint_goal_descriptor_at_end_type;
        typedef x3::rule<ConstraintGoalDescriptorAlwaysClass, ast::ConstraintGoalDescriptorAlways> constraint_goal_descriptor_always_type;
        typedef x3::rule<ConstraintGoalDescriptorSometimeClass, ast::ConstraintGoalDescriptorSometime> constraint_goal_descriptor_sometime_type;
        typedef x3::rule<ConstraintGoalDescriptorWithinClass, ast::ConstraintGoalDescriptorWithin> constraint_goal_descriptor_within_type;
        typedef x3::rule<ConstraintGoalDescriptorAtMostOnceClass, ast::ConstraintGoalDescriptorAtMostOnce> constraint_goal_descriptor_at_most_once_type;
        typedef x3::rule<ConstraintGoalDescriptorSometimeAfterClass, ast::ConstraintGoalDescriptorSometimeAfter> constraint_goal_descriptor_sometime_after_type;
        typedef x3::rule<ConstraintGoalDescriptorSometimeBeforeClass, ast::ConstraintGoalDescriptorSometimeBefore> constraint_goal_descriptor_sometime_before_type;
        typedef x3::rule<ConstraintGoalDescriptorAlwaysWithinClass, ast::ConstraintGoalDescriptorAlwaysWithin> constraint_goal_descriptor_always_within_type;
        typedef x3::rule<ConstraintGoalDescriptorHoldDuringClass, ast::ConstraintGoalDescriptorHoldDuring> constraint_goal_descriptor_hold_during_type;
        typedef x3::rule<ConstraintGoalDescriptorHoldAfterClass, ast::ConstraintGoalDescriptorHoldAfter> constraint_goal_descriptor_hold_after_type;

        typedef x3::rule<PreconditionGoalDescriptorClass, ast::PreconditionGoalDescriptor> precondition_goal_descriptor_type;
        typedef x3::rule<PreferenceNameClass, ast::PreferenceName> preference_name_type;
        typedef x3::rule<PreconditionGoalDescriptorSimpleClass, ast::PreconditionGoalDescriptorSimple> precondition_goal_descriptor_simple_type;
        typedef x3::rule<PreconditionGoalDescriptorAndClass, ast::PreconditionGoalDescriptorAnd> precondition_goal_descriptor_and_type;
        typedef x3::rule<PreconditionGoalDescriptorPreferenceClass, ast::PreconditionGoalDescriptorPreference> precondition_goal_descriptor_preference_type;
        typedef x3::rule<PreconditionGoalDescriptorForallClass, ast::PreconditionGoalDescriptorForall> precondition_goal_descriptor_forall_type;

        typedef x3::rule<AssignOperatorAssignClass, ast::AssignOperatorAssign> assign_operator_assign_type;
        typedef x3::rule<AssignOperatorScaleUpClass, ast::AssignOperatorScaleUp> assign_operator_scale_up_type;
        typedef x3::rule<AssignOperatorScaleDownClass, ast::AssignOperatorScaleDown> assign_operator_scale_down_type;
        typedef x3::rule<AssignOperatorIncreaseClass, ast::AssignOperatorIncrease> assign_operator_increase_type;
        typedef x3::rule<AssignOperatorDecreaseClass, ast::AssignOperatorDecrease> assign_operator_decrease_type;
        typedef x3::rule<AssignOperatorClass, ast::AssignOperator> assign_operator_type;

        typedef x3::rule<EffectClass, ast::Effect> effect_type;
        typedef x3::rule<EffectProductionLiteralClass, ast::EffectProductionLiteral> effect_production_literal_type;
        typedef x3::rule<EffectProductionNumericFluentClass, ast::EffectProductionNumericFluent> effect_production_numeric_fluent_type;
        typedef x3::rule<EffectProductionObjectFluentClass, ast::EffectProductionObjectFluent> effect_production_object_fluent_type;
        typedef x3::rule<EffectProductionClass, ast::EffectProduction> effect_production_type;
        typedef x3::rule<EffectConditionalForallClass, ast::EffectConditionalForall> effect_conditional_forall_type;
        typedef x3::rule<EffectConditionalWhenClass, ast::EffectConditionalWhen> effect_conditional_when_type;
        typedef x3::rule<EffectConditionalClass, ast::EffectConditional> effect_conditional_type;
        typedef x3::rule<ActionSymbolClass, ast::ActionSymbol> action_symbol_type;
        typedef x3::rule<ActionBodyClass, ast::ActionBody> action_body_type;
        typedef x3::rule<ActionClass, ast::Action> action_type;

        typedef x3::rule<DerivedPredicateClass, ast::DerivedPredicate> derived_predicate_type;

        typedef x3::rule<DomainNameClass, ast::DomainName> domain_name_type;
        typedef x3::rule<RequirementsClass, ast::Requirements> requirements_type;
        typedef x3::rule<TypesClass, ast::Types> types_type;
        typedef x3::rule<ConstantsClass, ast::Constants> constants_type;
        typedef x3::rule<PredicatesClass, ast::Predicates> predicates_type;
        typedef x3::rule<FunctionsClass, ast::Functions> functions_type;
        typedef x3::rule<ConstraintsClass, ast::Constraints> constraints_type;
        typedef x3::rule<StructureClass, ast::Structure> structure_type;
        typedef x3::rule<DomainDescriptionClass, ast::DomainDescription> domain_description_type;


        BOOST_SPIRIT_DECLARE(name_type, variable_type, number_type, term_type, undefined_type)

        BOOST_SPIRIT_DECLARE(
            requirement_strips_type, requirement_typing_type, requirement_negative_preconditions_type, requirement_disjunctive_preconditions_type,
            requirement_equality_type, requirement_existential_preconditions_type, requirement_universal_preconditions_type,
            requirement_quantified_preconditions_type, requirement_conditional_effects_type, requirement_fluents_type,
            requirement_object_fluents_type, requirement_numeric_fluents_type, requirement_adl_type, requirement_durative_actions_type,
            requirement_derived_predicates_type, requirement_timed_initial_literals_type, requirement_preferences_type, requirement_constraints_type, requirement_type)

        BOOST_SPIRIT_DECLARE(
            type_type, type_object_type, type_either_type, typed_list_of_names_recursively_type, typed_list_of_names_type,
            typed_list_of_variables_recursively_type, typed_list_of_variables_type)

        BOOST_SPIRIT_DECLARE(predicate_type, atomic_formula_skeleton_type)

        BOOST_SPIRIT_DECLARE(function_symbol_type, function_term_type, function_type_number_type, function_type_type_type, function_type_type, atomic_function_skeleton_type, function_typed_list_of_atomic_function_skeletons_recursively_type, function_typed_list_of_atomic_function_skeletons_type)

        BOOST_SPIRIT_DECLARE(atomic_formula_of_terms_type, atom_of_terms_type, negated_atom_of_terms_type, literal_of_terms_type)

        BOOST_SPIRIT_DECLARE(multi_operator_mul_type, multi_operator_plus_type, multi_operator_type,
            binary_operator_minus_type, binary_operator_div_type, binary_operator_type)

        BOOST_SPIRIT_DECLARE(binary_comparator_greater_type, binary_comparator_less_type,
            binary_comparator_equal_type, binary_comparator_greater_equal_type,
            binary_comparator_less_equal_type, binary_comparator_type)

        BOOST_SPIRIT_DECLARE(function_expression_type, function_head_type, function_expression_number_type,
            function_expression_number_type, function_expression_binary_op_type,
            function_expression_minus_type, function_expression_head_type)

        BOOST_SPIRIT_DECLARE(goal_descriptor_type, goal_descriptor_atom_type, goal_descriptor_literal_type,
            goal_descriptor_and_type, goal_descriptor_or_type, goal_descriptor_not_type,
            goal_descriptor_imply_type, goal_descriptor_exists_type, goal_descriptor_forall_type,
            goal_descriptor_function_comparison_type)

        BOOST_SPIRIT_DECLARE(constraint_goal_descriptor_type, constraint_goal_descriptor_and_type,
            constraint_goal_descriptor_forall_type, constraint_goal_descriptor_at_end_type,
            constraint_goal_descriptor_always_type, constraint_goal_descriptor_sometime_type,
            constraint_goal_descriptor_within_type, constraint_goal_descriptor_at_most_once_type,
            constraint_goal_descriptor_sometime_after_type, constraint_goal_descriptor_sometime_before_type,
            constraint_goal_descriptor_always_within_type, constraint_goal_descriptor_hold_during_type,
            constraint_goal_descriptor_hold_after_type)

        BOOST_SPIRIT_DECLARE(precondition_goal_descriptor_type, preference_name_type,
           precondition_goal_descriptor_simple_type, precondition_goal_descriptor_and_type,
           precondition_goal_descriptor_preference_type, precondition_goal_descriptor_forall_type)

        BOOST_SPIRIT_DECLARE(assign_operator_assign_type, assign_operator_scale_up_type, assign_operator_scale_down_type,
            assign_operator_increase_type, assign_operator_decrease_type, assign_operator_type)

        BOOST_SPIRIT_DECLARE(effect_type, effect_production_literal_type, effect_production_numeric_fluent_type,
            effect_production_object_fluent_type, effect_production_type, effect_conditional_forall_type,
            effect_conditional_when_type, effect_conditional_type, action_symbol_type, action_body_type, action_type,
            derived_predicate_type)

        BOOST_SPIRIT_DECLARE(domain_name_type, requirements_type, types_type, constants_type,
            predicates_type, functions_type, constraints_type, structure_type, domain_description_type)
    }

    parser::name_type const& name();
    parser::variable_type const& variable();
    parser::number_type const& number();
    parser::term_type const& term();
    parser::undefined_type const& undefined();

    parser::requirement_strips_type const& requirement_strips();
    parser::requirement_typing_type const& requirement_typing();
    parser::requirement_negative_preconditions_type const& requirement_negative_preconditions();
    parser::requirement_disjunctive_preconditions_type const& requirement_disjunctive_preconditions();
    parser::requirement_equality_type const& requirement_equality();
    parser::requirement_existential_preconditions_type const& requirement_existential_preconditions();
    parser::requirement_universal_preconditions_type const& requirement_universal_preconditions();
    parser::requirement_quantified_preconditions_type const& requirement_quantified_preconditions();
    parser::requirement_conditional_effects_type const& requirement_conditional_effects();
    parser::requirement_fluents_type const& requirement_fluents();
    parser::requirement_object_fluents_type const& requirement_object_fluents();
    parser::requirement_numeric_fluents_type const& requirement_numeric_fluents();
    parser::requirement_adl_type const& requirement_adl();
    parser::requirement_durative_actions_type const& requirement_durative_actions();
    parser::requirement_derived_predicates_type const& requirement_derived_predicates();
    parser::requirement_timed_initial_literals_type const& requirement_timed_initial_literals();
    parser::requirement_preferences_type const& requirement_preferences();
    parser::requirement_constraints_type const& requirement_constraints();
    parser::requirement_type const& requirement();

    parser::type_type const& type();
    parser::type_object_type const& type_object();
    parser::type_either_type const& type_either();
    parser::typed_list_of_names_recursively_type const& typed_list_of_names_recursively();
    parser::typed_list_of_names_type const& typed_list_of_names();
    parser::typed_list_of_variables_recursively_type const& typed_list_of_variables_recursively();
    parser::typed_list_of_variables_type const& typed_list_of_variables();

    parser::predicate_type const& predicate();
    parser::atomic_formula_skeleton_type const& atomic_formula_skeleton();

    parser::multi_operator_mul_type const& multi_operator_mul();
    parser::multi_operator_plus_type const& multi_operator_plus();
    parser::multi_operator_type const& multi_operator();
    parser::binary_operator_minus_type const& binary_operator_minus();
    parser::binary_operator_div_type const& binary_operator_div();
    parser::binary_operator_type const& binary_operator();

    parser::binary_comparator_greater_type const& binary_comparator_greater();
    parser::binary_comparator_less_type const& binary_comparator_less();
    parser::binary_comparator_equal_type const& binary_comparator_equal();
    parser::binary_comparator_greater_equal_type const& binary_comparator_greater_equal();
    parser::binary_comparator_less_equal_type const& binary_comparator_less_equal();
    parser::binary_comparator_type const& binary_comparator();

    parser::function_symbol_type const& function_symbol();
    parser::function_term_type const& function_term();
    parser::function_type_number_type const& function_type_number();
    parser::function_type_type_type const& function_type_type_();
    parser::function_type_type const& function_type();
    parser::atomic_function_skeleton_type const& atomic_function_skeleton();
    parser::function_typed_list_of_atomic_function_skeletons_recursively_type const& function_typed_list_of_atomic_function_skeletons_recursively();
    parser::function_typed_list_of_atomic_function_skeletons_type const& function_typed_list_of_atomic_function_skeletons();

    parser::atomic_formula_of_terms_predicate_type const& atomic_formula_of_terms_predicate();
    parser::atomic_formula_of_terms_equality_type const& atomic_formula_of_terms_equality();
    parser::atomic_formula_of_terms_type const& atomic_formula_of_terms();
    parser::atom_of_terms_type const& atom_of_terms();
    parser::negated_atom_of_terms_type const& negated_atom_of_terms();
    parser::literal_of_terms_type const& literal_of_terms();


    parser::function_expression_type const& function_expression();
    parser::function_head_type const& function_head();
    parser::function_expression_number_type const& function_expression_number();
    parser::function_expression_binary_op_type const& function_expression_binary_op();
    parser::function_expression_minus_type const& function_expression_minus();
    parser::function_expression_head_type const& function_expression_head();

    parser::goal_descriptor_type const& goal_descriptor();
    parser::goal_descriptor_atom_type const& goal_descriptor_atom();
    parser::goal_descriptor_literal_type const& goal_descriptor_literal();
    parser::goal_descriptor_and_type const& goal_descriptor_and();
    parser::goal_descriptor_or_type const& goal_descriptor_or();
    parser::goal_descriptor_not_type const& goal_descriptor_not();
    parser::goal_descriptor_imply_type const& goal_descriptor_imply();
    parser::goal_descriptor_exists_type const& goal_descriptor_exists();
    parser::goal_descriptor_forall_type const& goal_descriptor_forall();
    parser::goal_descriptor_function_comparison_type const& goal_descriptor_function_comparison();

    parser::constraint_goal_descriptor_type const& constraint_goal_descriptor();
    parser::constraint_goal_descriptor_and_type const& constraint_goal_descriptor_and();
    parser::constraint_goal_descriptor_forall_type const& constraint_goal_descriptor_forall();
    parser::constraint_goal_descriptor_at_end_type const& constraint_goal_descriptor_at_end();
    parser::constraint_goal_descriptor_always_type const& constraint_goal_descriptor_always();
    parser::constraint_goal_descriptor_sometime_type const& constraint_goal_descriptor_sometime();
    parser::constraint_goal_descriptor_within_type const& constraint_goal_descriptor_within();
    parser::constraint_goal_descriptor_at_most_once_type const& constraint_goal_descriptor_at_most_once();
    parser::constraint_goal_descriptor_sometime_after_type const& constraint_goal_descriptor_sometime_after();
    parser::constraint_goal_descriptor_sometime_before_type const& constraint_goal_descriptor_sometime_before();
    parser::constraint_goal_descriptor_always_within_type const& constraint_goal_descriptor_always_within();
    parser::constraint_goal_descriptor_hold_during_type const& constraint_goal_descriptor_hold_during();
    parser::constraint_goal_descriptor_hold_after_type const& constraint_goal_descriptor_hold_after();

    parser::precondition_goal_descriptor_type const& precondition_goal_descriptor();
    parser::preference_name_type const& preference_name();
    parser::precondition_goal_descriptor_simple_type const& precondition_goal_descriptor_simple();
    parser::precondition_goal_descriptor_and_type const& precondition_goal_descriptor_and();
    parser::precondition_goal_descriptor_preference_type const& precondition_goal_descriptor_preference();
    parser::precondition_goal_descriptor_forall_type const& precondition_goal_descriptor_forall();

    parser::assign_operator_assign_type const& assign_operator_assign();
    parser::assign_operator_scale_up_type const& assign_operator_scale_up();
    parser::assign_operator_scale_down_type const& assign_operator_scale_down();
    parser::assign_operator_increase_type const& assign_operator_increase();
    parser::assign_operator_decrease_type const& assign_operator_decrease();
    parser::assign_operator_type const& assign_operator();

    parser::effect_type const& effect();
    parser::effect_production_literal_type const& effect_production_literal();
    parser::effect_production_numeric_fluent_type const& effect_production_numeric_fluent();
    parser::effect_production_object_fluent_type const& effect_production_object_fluent();
    parser::effect_production_type const& effect_production();
    parser::effect_conditional_forall_type const& effect_conditional_forall();
    parser::effect_conditional_when_type const& effect_conditional_when();
    parser::effect_conditional_type const& effect_conditional();
    parser::action_symbol_type const& action_symbol();
    parser::action_body_type const& action_body();
    parser::action_type const& action();
    parser::derived_predicate_type const& derived_predicate();

    parser::domain_name_type const& domain_name();
    parser::requirements_type const& requirements();
    parser::types_type const& types();
    parser::constants_type const& constants();
    parser::predicates_type const& predicates();
    parser::functions_type const& functions();
    parser::constraints_type const& constraints();
    parser::structure_type const& structure();
    parser::domain_description_type const& domain_description();
}

#endif