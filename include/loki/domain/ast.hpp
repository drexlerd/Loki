#ifndef LOKI_INCLUDE_LOKI_DOMAIN_AST_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_AST_HPP_

#include <map>
#include <vector>

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>


namespace loki::domain::ast
{
    ///////////////////////////////////////////////////////////////////////////
    //  The AST
    ///////////////////////////////////////////////////////////////////////////
    namespace x3 = boost::spirit::x3;

    struct Name;
    struct Variable;
    struct Number;

    struct FluentType;
    struct EitherType;
    struct Type;
    struct TypedListOfNamesRecursively;
    struct TypedListOfNames;
    struct TypedListOfVariablesRecursively;
    struct TypedListOfVariables;

    struct AtomicFormulaSkeleton;

    struct AtomicFunctionSkeleton;
    struct FunctionSymbol;
    struct FunctionType;
    struct FunctionTypedListOfAtomicFunctionSkeletonsRecursively;
    struct FunctionTypedListOfAtomicFunctionSkeletons;

    struct DomainName;
    struct Requirements;
    struct Types;
    struct Constants;
    struct Predicates;
    struct Functions;
    struct DomainDescription;

    /* <name> */
    struct Name: x3::position_tagged {
        char alpha;
        std::string suffix;
    };


    /* <variable> */
    struct Variable: x3::position_tagged {
        char question_mark;
        Name name;
    };

    /* <number> */
    struct Number : x3::position_tagged {
        int value;
    };


    /* <require-def> */
    struct StripsRequirement : x3::position_tagged {
    };

    struct TypingRequirement : x3::position_tagged {
    };

    struct Requirement : x3::position_tagged,
        x3::variant<
            x3::forward_ast<StripsRequirement>,
            x3::forward_ast<TypingRequirement>> {
        // TODO: add more requirements
        using base_type::base_type;
        using base_type::operator=;
    };

    struct Requirements : x3::position_tagged {
        std::vector<Requirement> requirements;
    };


    /* <typed list (name)> */
    struct FluentType : x3::position_tagged {
        x3::forward_ast<Type> type;
    };

    struct EitherType : x3::position_tagged {
        std::vector<x3::forward_ast<Type>> types;
    };

    struct Type : x3::position_tagged,
        x3::variant<
            x3::forward_ast<Name>,
            x3::forward_ast<FluentType>,
            x3::forward_ast<EitherType>> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct TypedListOfNamesRecursively : x3::position_tagged {
        std::vector<Name> names;
        Type type;
        x3::forward_ast<TypedListOfNamesRecursively> typed_list_of_names_recursively;
    };

    struct TypedListOfNames : x3::position_tagged,
        x3::variant<
            x3::forward_ast<std::vector<Name>>,
            x3::forward_ast<TypedListOfNamesRecursively>> {
        using base_type::base_type;
        using base_type::operator=;
    };


    /* <typed list (variable)> */
    struct TypedListOfVariablesRecursively : x3::position_tagged {
        std::vector<Variable> variables;
        Type type;
        x3::forward_ast<TypedListOfVariablesRecursively> typed_list_of_variables_recursively;
    };

    struct TypedListOfVariables : x3::position_tagged,
        x3::variant<
            x3::forward_ast<std::vector<Variable>>,
            x3::forward_ast<TypedListOfVariablesRecursively>> {
        using base_type::base_type;
        using base_type::operator=;
    };


    /* <atomic function skeleton> */
    struct AtomicFormulaSkeleton : x3::position_tagged {
        Name name;
        TypedListOfVariables typed_list_of_variables;
    };

    /* <function typed list (atomic function skeleton)> */
    struct FunctionSymbol : x3::position_tagged {
        Name name;
    };

    struct FunctionType : x3::position_tagged {
        Number number;
    };

    struct AtomicFunctionSkeleton : x3::position_tagged {
        FunctionSymbol function_symbol;
        TypedListOfVariables arguments;
    };

    struct FunctionTypedListOfAtomicFunctionSkeletonsRecursively : x3::position_tagged {
        std::vector<AtomicFunctionSkeleton> atomic_function_skeleton;
        FunctionType function_type;
        x3::forward_ast<FunctionTypedListOfAtomicFunctionSkeletonsRecursively> function_typed_list_of_atomic_function_skeletons_recursively;
    };

    struct FunctionTypedListOfAtomicFunctionSkeletons : x3::position_tagged,
        x3::variant<
            x3::forward_ast<std::vector<AtomicFunctionSkeleton>>,
            x3::forward_ast<FunctionTypedListOfAtomicFunctionSkeletonsRecursively>> {
        using base_type::base_type;
        using base_type::operator=;
    };


    /* <types-def> */
    struct Types : x3::position_tagged {
        TypedListOfNames typed_list_of_names;
    };


    /* <constants-def> */
    struct Constants : x3::position_tagged {
        TypedListOfNames typed_list_of_names;
    };


    /* <predicates-def> */
    struct Predicates : x3::position_tagged {
        std::vector<AtomicFormulaSkeleton> atomic_formula_skeletons;
    };


    /* <functions-def> */
    struct Functions : x3::position_tagged {
        FunctionTypedListOfAtomicFunctionSkeletons function_types_list_of_atomic_function_skeletons;
    };


    /* <domain> */
    struct DomainName : x3::position_tagged {
        Name name;
    };

    struct DomainDescription : x3::position_tagged {
        DomainName domain_name;
        Requirements requirements;
        Types types;
        Constants constants;
        Predicates predicates;
        Functions functions;
    };
}


#endif