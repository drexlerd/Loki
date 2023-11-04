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