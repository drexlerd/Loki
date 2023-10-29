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
    struct Number;  // TODO: can also be float!
    struct Term;

    struct FluentType;                           // :fluents
    struct EitherType;
    struct Type;
    struct TypedListOfNamesRecursively;
    struct TypedListOfNames;
    struct TypedListOfVariablesRecursively;      // :typing
    struct TypedListOfVariables;

    struct Predicate;
    struct AtomicFormulaSkeleton;

    struct AtomicFunctionSkeleton;
    struct FunctionSymbol;
    struct FunctionType;
    struct FunctionTypedListOfAtomicFunctionSkeletonsRecursively;  // :typing
    struct FunctionTypedListOfAtomicFunctionSkeletons;

    struct Atom;
    struct NegatedAtom;
    struct Literal;
    struct AtomicFormulaOfTerms;

    struct GoalDescriptorAtomicFormula;
    struct GoalDescriptorLiteral;                // :negative-preconditions
    struct GoalDescriptorAnd;
    struct GoalDescriptorOr;                     // :disjunctive-preconditions
    struct GoalDescriptorNot;                    // :disjunctive-preconditions
    struct GoalDescriptorImply;                  // :disjunctive-preconditions
    struct GoalDescriptorExists;                 // :existential-preconditions
    struct GoalDescriptorForall;                 // :fluents
    struct GoalDescriptor;

    struct ConstraintGoalDescriptorAnd;
    struct ConstraintGoalDescriptorForall;
    struct ConstraintGoalDescriptorAtEnd;
    struct ConstraintGoalDescriptorAlways;
    struct ConstraintGoalDescriptorSometime;
    struct ConstraintGoalDescriptorWithin;
    struct ConstraintGoalDescriptorAtMostOnce;
    struct ConstraintGoalDescriptorSometimeAfter;
    struct ConstraintGoalDescriptorSometimeBefore;
    struct ConstraintGoalDescriptorAlwaysWithin;
    struct ConstraintGoalDescriptorHoldDuring;
    struct ConstraintGoalDescriptorHoldAfter;
    struct ConstraintGoalDescriptor;

    struct DomainName;
    struct Requirements;
    struct Types;
    struct Constants;
    struct Predicates;
    struct Functions;                            // :fluents
    struct Constraints;                          // :constraints
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

    /* <term> */
    struct Term : x3::position_tagged,
        x3::variant<
            x3::forward_ast<Name>,
            x3::forward_ast<Variable>> {
        using base_type::base_type;
        using base_type::operator=;
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
    struct Predicate : x3::position_tagged {
        Name name;
    };

    struct AtomicFormulaSkeleton : x3::position_tagged {
        Predicate predicate;
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


    /* Atomic formulas */
    struct AtomicFormulaOfTerms : x3::position_tagged {
        Predicate predicate;
        std::vector<Term> terms;
    };

    struct Atom : x3::position_tagged {
        AtomicFormulaOfTerms atomic_formula_of_terms;
    };

    struct NegatedAtom : x3::position_tagged {
        AtomicFormulaOfTerms atomic_formula_of_terms;
    };

    struct Literal : x3::position_tagged,
        x3::variant<
            x3::forward_ast<Atom>,
            x3::forward_ast<NegatedAtom>> {
        using base_type::base_type;
        using base_type::operator=;
    };


    /* Goal Descriptors */
    struct GoalDescriptorAtomicFormula : x3::position_tagged {
        AtomicFormulaOfTerms atomic_formula_of_terms;
    };

    struct GoalDescriptorLiteral : x3::position_tagged {
        Literal literal;
    };

    struct GoalDescriptorAnd : x3::position_tagged {
        std::vector<x3::forward_ast<GoalDescriptor>> goal_descriptors;
    };

    struct GoalDescriptorOr : x3::position_tagged {
        std::vector<x3::forward_ast<GoalDescriptor>> goal_descriptors;
    };

    struct GoalDescriptorNot : x3::position_tagged {
        x3::forward_ast<GoalDescriptor> goal_descriptor;
    };

    struct GoalDescriptorImply : x3::position_tagged {
        x3::forward_ast<GoalDescriptor> goal_descriptor_left;
        x3::forward_ast<GoalDescriptor> goal_descriptor_right;
    };

    struct GoalDescriptorExists : x3::position_tagged {
        TypedListOfVariables typed_list_of_variables;
        x3::forward_ast<GoalDescriptor> goal_descriptor;
    };

    struct GoalDescriptorForall : x3::position_tagged {
        TypedListOfVariables typed_list_of_variables;
        x3::forward_ast<GoalDescriptor> goal_descriptor;
    };

    struct GoalDescriptor : x3::position_tagged,
        x3::variant<
            x3::forward_ast<GoalDescriptorAtomicFormula>,
            x3::forward_ast<GoalDescriptorLiteral>,
            x3::forward_ast<GoalDescriptorAnd>,
            x3::forward_ast<GoalDescriptorOr>,
            x3::forward_ast<GoalDescriptorNot>,
            x3::forward_ast<GoalDescriptorImply>,
            x3::forward_ast<GoalDescriptorExists>,
            x3::forward_ast<GoalDescriptorForall>> {
        using base_type::base_type;
        using base_type::operator=;
    };


    /* Constraint Goal Descriptors */
    struct ConstraintGoalDescriptorAnd : x3::position_tagged {
        std::vector<x3::forward_ast<ConstraintGoalDescriptor>> constraint_goal_descriptors;
    };

    struct ConstraintGoalDescriptorForall : x3::position_tagged {
        TypedListOfVariables typed_list_of_variables;
        x3::forward_ast<ConstraintGoalDescriptor> constraint_goal_descriptor;
    };

    struct ConstraintGoalDescriptorAtEnd : x3::position_tagged {
        x3::forward_ast<GoalDescriptor> goal_descriptor;
    };

    struct ConstraintGoalDescriptorAlways : x3::position_tagged {
        x3::forward_ast<GoalDescriptor> goal_descriptor;
    };

    struct ConstraintGoalDescriptorSometime : x3::position_tagged {
        x3::forward_ast<GoalDescriptor> goal_descriptor;
    };

    struct ConstraintGoalDescriptorWithin : x3::position_tagged {
        Number number;
        x3::forward_ast<GoalDescriptor> goal_descriptor;
    };

    struct ConstraintGoalDescriptorAtMostOnce : x3::position_tagged {
        x3::forward_ast<GoalDescriptor> goal_descriptor;
    };

    struct ConstraintGoalDescriptorSometimeAfter : x3::position_tagged {
        x3::forward_ast<GoalDescriptor> goal_descriptor_left;
        x3::forward_ast<GoalDescriptor> goal_descriptor_right;
    };

    struct ConstraintGoalDescriptorSometimeBefore : x3::position_tagged {
        x3::forward_ast<GoalDescriptor> goal_descriptor_left;
        x3::forward_ast<GoalDescriptor> goal_descriptor_right;
    };

    struct ConstraintGoalDescriptorAlwaysWithin : x3::position_tagged {
        Number number;
        x3::forward_ast<GoalDescriptor> goal_descriptor_left;
        x3::forward_ast<GoalDescriptor> goal_descriptor_right;
    };

    struct ConstraintGoalDescriptorHoldDuring : x3::position_tagged {
        Number number_left;
        Number number_right;
        x3::forward_ast<GoalDescriptor> goal_descriptor;
    };

    struct ConstraintGoalDescriptorHoldAfter : x3::position_tagged {
        Number number;
        x3::forward_ast<GoalDescriptor> goal_descriptor;
    };

    struct ConstraintGoalDescriptor : x3::position_tagged,
        x3::variant<
            x3::forward_ast<ConstraintGoalDescriptorAnd>,
            x3::forward_ast<ConstraintGoalDescriptorForall>,
            x3::forward_ast<ConstraintGoalDescriptorAtEnd>,
            x3::forward_ast<ConstraintGoalDescriptorAlways>,
            x3::forward_ast<ConstraintGoalDescriptorSometime>,
            x3::forward_ast<ConstraintGoalDescriptorWithin>,
            x3::forward_ast<ConstraintGoalDescriptorAtMostOnce>,
            x3::forward_ast<ConstraintGoalDescriptorSometimeAfter>,
            x3::forward_ast<ConstraintGoalDescriptorSometimeBefore>,
            x3::forward_ast<ConstraintGoalDescriptorAlwaysWithin>,
            x3::forward_ast<ConstraintGoalDescriptorHoldDuring>,
            x3::forward_ast<ConstraintGoalDescriptorHoldAfter>> {
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


    /* <constraints-def> */
    struct Constraints : x3::position_tagged {
        ConstraintGoalDescriptor constraint_goal_descriptor;
    };


    /* <domain> */
    struct DomainName : x3::position_tagged {
        Name name;
    };

    struct DomainDescription : x3::position_tagged {
        DomainName domain_name;
        boost::optional<Requirements> requirements;
        boost::optional<Types> types;
        boost::optional<Constants> constants;
        boost::optional<Predicates> predicates;
        boost::optional<Functions> functions;
        boost::optional<Constraints> constraints;
    };
}


#endif