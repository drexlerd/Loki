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

        struct DerivedPredicateClass;

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

        typedef x3::rule<DerivedPredicateClass, ast::DerivedPredicate> derived_predicate_type;

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

        BOOST_SPIRIT_DECLARE(derived_predicate_type)

        BOOST_SPIRIT_DECLARE(domain_description_type)
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

    parser::derived_predicate_type const& derived_predicate();

    parser::domain_description_type const& domain_description();
}

#endif