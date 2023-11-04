#ifndef LOKI_INCLUDE_LOKI_DOMAIN_AST_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_AST_HPP_

#include <map>
#include <vector>

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/optional.hpp>

namespace loki::domain::ast
{
    ///////////////////////////////////////////////////////////////////////////
    //  The AST
    ///////////////////////////////////////////////////////////////////////////
    namespace x3 = boost::spirit::x3;

    struct Name;
    struct Variable;
    struct Number; // TODO: can also be float!
    struct Term;

    struct RequirementStrips;
    struct RequirementTyping;
    struct RequirementNegativePreconditions;
    struct RequirementDisjunctivePreconditions;
    struct RequirementEquality;
    struct RequirementExistentialPreconditions;
    struct RequirementUniversalPreconditions;
    struct RequirementQuantifiedPreconditions;
    struct RequirementConditionalEffects;
    struct RequirementFluents;        // PDDL 3.1 = :object-fluents + :numeric-fluents
    struct RequirementObjectFluents;  // PDDL 3.1
    struct RequirementNumericFluents; // PDDL 3.1
    struct RequirementAdl;
    struct RequirementDurativeActions;
    struct RequirementDerivedPredicates;
    struct RequirementTimedInitialLiterals;
    struct RequirementPreferences;
    struct RequirementConstraints;
    struct Requirement;

    struct Type; // We model fluent types as in fox-long-2003
    struct TypeObject;
    struct TypeEither; // We allow nested either
    struct TypedListOfNamesRecursively;
    struct TypedListOfNames;
    struct TypedListOfVariablesRecursively; // :typing
    struct TypedListOfVariables;

    struct Predicate;
    struct AtomicFormulaSkeleton;

    struct FunctionSymbol;
    struct FunctionTerm;
    struct FunctionTypeNumber;                                     // :numeric-fluents
    struct FunctionTypeType;                                       // :object-fluents :typing
    struct FunctionType;
    struct AtomicFunctionSkeleton;
    struct FunctionTypedListOfAtomicFunctionSkeletonsRecursively;
    struct FunctionTypedListOfAtomicFunctionSkeletons;

    struct AtomicFormulaOfTermsPredicate;
    struct AtomicFormulaOfTermsEquality;
    struct AtomicFormulaOfTerms;
    struct AtomOfTerms;
    struct NegatedAtomOfTerms;
    struct LiteralOfTerms;

    struct MultiOperatorMul;
    struct MultiOperatorPlus;
    struct MultiOperator;
    struct BinaryOperatorMinus;
    struct BinaryOperatorDiv;
    struct BinaryOperator;

    struct BinaryComparatorGreater;
    struct BinaryComparatorLess;
    struct BinaryComparatorEqual;
    struct BinaryComparatorGreaterEqual;
    struct BinaryComparatorLessEqual;
    struct BinaryComparator;

    struct FunctionHead;
    struct FunctionExpression;
    struct FunctionExpressionNumber;
    struct FunctionExpressionBinaryOp;
    struct FunctionExpressionMinus;
    struct FunctionExpressionHead;

    struct GoalDescriptor;
    struct GoalDescriptorAtom;
    struct GoalDescriptorLiteral; // :negative-preconditions
    struct GoalDescriptorAnd;
    struct GoalDescriptorOr;                 // :disjunctive-preconditions
    struct GoalDescriptorNot;                // :disjunctive-preconditions
    struct GoalDescriptorImply;              // :disjunctive-preconditions
    struct GoalDescriptorExists;             // :existential-preconditions
    struct GoalDescriptorForall;             // :universal-preconditions
    struct GoalDescriptorFunctionComparison; // :fluents

    struct ConstraintGoalDescriptor;
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

    struct PreferenceName;
    struct PreconditionGoalDescriptor;
    struct PreconditionGoalDescriptorSimple;
    struct PreconditionGoalDescriptorAnd;
    struct PreconditionGoalDescriptorPreference; // :preferences
    struct PreconditionGoalDescriptorForall;     // :universal-preconditions

    struct AssignOperatorAssign;
    struct AssignOperatorScaleUp;
    struct AssignOperatorScaleDown;
    struct AssignOperatorIncrease;
    struct AssignOperatorDecrease;
    struct AssignOperator;

    struct Effect;
    struct EffectProductionLiteral;
    struct EffectProductionNumericFluent;
    struct EffectProduction;
    struct EffectConditionalForall;
    struct EffectConditionalWhen;
    struct EffectConditional;

    struct ActionSymbol;
    struct ActionBody;

    struct Action;
    // TODO
    struct DurativeAction;   // :durative-actions
    struct DerivedPredicate; // :derived-predicates

    struct DomainName;
    struct Requirements;
    struct Types;
    struct Constants;
    struct Predicates;
    struct Functions;   // :fluents
    struct Constraints; // :constraints
    struct Structure;
    struct DomainDescription;

    /* <name> */
    struct Name : x3::position_tagged
    {
        char alpha;
        std::string suffix;
    };

    /* <variable> */
    struct Variable : x3::position_tagged
    {
        char question_mark;
        Name name;
    };

    /* <number> */
    struct Number : x3::position_tagged
    {
        double value;
    };

    /* <term> */
    struct Term : x3::position_tagged,
                  x3::variant<
                      Name,
                      Variable,
                      x3::forward_ast<FunctionTerm>>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    /* <require-def> */
    struct RequirementStrips : x3::position_tagged
    {
    };

    struct RequirementTyping : x3::position_tagged
    {
    };

    struct RequirementNegativePreconditions : x3::position_tagged
    {
    };

    struct RequirementDisjunctivePreconditions : x3::position_tagged
    {
    };

    struct RequirementEquality : x3::position_tagged
    {
    };

    struct RequirementExistentialPreconditions : x3::position_tagged
    {
    };

    struct RequirementUniversalPreconditions : x3::position_tagged
    {
    };

    struct RequirementQuantifiedPreconditions : x3::position_tagged
    {
    };

    struct RequirementConditionalEffects : x3::position_tagged
    {
    };

    struct RequirementFluents : x3::position_tagged
    {
    };

    struct RequirementObjectFluents : x3::position_tagged
    {
    };

    struct RequirementNumericFluents : x3::position_tagged
    {
    };

    struct RequirementAdl : x3::position_tagged
    {
    };

    struct RequirementDurativeActions : x3::position_tagged
    {
    };

    struct RequirementDerivedPredicates : x3::position_tagged
    {
    };

    struct RequirementTimedInitialLiterals : x3::position_tagged
    {
    };

    struct RequirementPreferences : x3::position_tagged
    {
    };

    struct RequirementConstraints : x3::position_tagged
    {
    };

    struct Requirement : x3::position_tagged,
                         x3::variant<
                             RequirementStrips,
                             RequirementTyping,
                             RequirementNegativePreconditions,
                             RequirementDisjunctivePreconditions,
                             RequirementEquality,
                             RequirementExistentialPreconditions,
                             RequirementUniversalPreconditions,
                             RequirementQuantifiedPreconditions,
                             RequirementConditionalEffects,
                             RequirementFluents,
                             RequirementObjectFluents,
                             RequirementNumericFluents,
                             RequirementAdl,
                             RequirementDurativeActions,
                             RequirementDerivedPredicates,
                             RequirementTimedInitialLiterals,
                             RequirementPreferences,
                             RequirementConstraints>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct Requirements : x3::position_tagged
    {
        std::vector<Requirement> requirements;
    };

    /* <typed list (name)> */
    struct Type : x3::position_tagged,
                  x3::variant<
                      Name,
                      x3::forward_ast<TypeObject>,
                      x3::forward_ast<TypeEither>>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct TypeObject : x3::position_tagged
    {
    };

    struct TypeEither : x3::position_tagged
    {
        std::vector<Type> types;
    };

    struct TypedListOfNamesRecursively : x3::position_tagged
    {
        std::vector<Name> names;
        Type type;
        x3::forward_ast<TypedListOfNames> typed_list_of_names;
    };

    struct TypedListOfNames : x3::position_tagged,
                              x3::variant<
                                  std::vector<Name>,
                                  TypedListOfNamesRecursively>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    /* <typed list (variable)> */
    struct TypedListOfVariablesRecursively : x3::position_tagged
    {
        std::vector<Variable> variables;
        Type type;
        x3::forward_ast<TypedListOfVariables> typed_list_of_variables;
    };

    struct TypedListOfVariables : x3::position_tagged,
                                  x3::variant<
                                      std::vector<Variable>,
                                      TypedListOfVariablesRecursively>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    /* <atomic function skeleton> */
    struct Predicate : x3::position_tagged
    {
        Name name;
    };

    struct AtomicFormulaSkeleton : x3::position_tagged
    {
        Predicate predicate;
        TypedListOfVariables typed_list_of_variables;
    };

    /* <function typed list (atomic function skeleton)> */
    struct FunctionSymbol : x3::position_tagged
    {
        Name name;
    };

    struct FunctionTerm : x3::position_tagged {
        FunctionSymbol function_symbol;
        std::vector<Term> terms;
    };

    struct FunctionTypeNumber : x3::position_tagged
    {
        Number number;
    };

    struct FunctionTypeType  : x3::position_tagged
    {
        Type type;
    };

    struct FunctionType : x3::position_tagged,
        x3::variant<
            FunctionTypeNumber,
            FunctionTypeType> {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct AtomicFunctionSkeleton : x3::position_tagged
    {
        FunctionSymbol function_symbol;
        TypedListOfVariables arguments;
    };

    struct FunctionTypedListOfAtomicFunctionSkeletonsRecursively : x3::position_tagged
    {
        std::vector<AtomicFunctionSkeleton> atomic_function_skeleton;
        FunctionType function_type;
        boost::optional<x3::forward_ast<FunctionTypedListOfAtomicFunctionSkeletons>> function_typed_list_of_atomic_function_skeletons;
    };

    struct FunctionTypedListOfAtomicFunctionSkeletons : x3::position_tagged,
        x3::variant<
            std::vector<AtomicFunctionSkeleton>,  // :numeric-fluents and deprecated (https://ipc08.icaps-conference.org/deterministic/PddlExtension.html)
            FunctionTypedListOfAtomicFunctionSkeletonsRecursively>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    /* Atomic formulas */
    struct AtomicFormulaOfTermsPredicate : x3::position_tagged
    {
        Predicate predicate;
        std::vector<Term> terms;
    };

    struct AtomicFormulaOfTermsEquality : x3::position_tagged
    {
        Term term_left;
        Term term_right;
    };

    struct AtomicFormulaOfTerms : x3::position_tagged,
                                  x3::variant<
                                      AtomicFormulaOfTermsPredicate,
                                      AtomicFormulaOfTermsEquality>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct AtomOfTerms : x3::position_tagged
    {
        AtomicFormulaOfTerms atomic_formula_of_terms;
    };

    struct NegatedAtomOfTerms : x3::position_tagged
    {
        AtomicFormulaOfTerms atomic_formula_of_terms;
    };

    struct LiteralOfTerms : x3::position_tagged,
                            x3::variant<
                                AtomOfTerms,
                                NegatedAtomOfTerms>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    /* Operators */
    struct MultiOperatorMul : x3::position_tagged
    {
    };

    struct MultiOperatorPlus : x3::position_tagged
    {
    };

    struct MultiOperator : x3::position_tagged,
                           x3::variant<
                               MultiOperatorMul,
                               MultiOperatorPlus>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct BinaryOperatorMinus : x3::position_tagged
    {
    };

    struct BinaryOperatorDiv : x3::position_tagged
    {
    };

    struct BinaryOperator : x3::position_tagged,
                            x3::variant<
                                BinaryOperatorMinus,
                                BinaryOperatorDiv,
                                MultiOperator>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct BinaryComparatorGreater : x3::position_tagged
    {
    };

    struct BinaryComparatorLess : x3::position_tagged
    {
    };

    struct BinaryComparatorEqual : x3::position_tagged
    {
    };

    struct BinaryComparatorGreaterEqual : x3::position_tagged
    {
    };

    struct BinaryComparatorLessEqual : x3::position_tagged
    {
    };

    struct BinaryComparator : x3::position_tagged,
                              x3::variant<
                                  BinaryComparatorGreater,
                                  BinaryComparatorLess,
                                  BinaryComparatorEqual,
                                  BinaryComparatorGreaterEqual,
                                  BinaryComparatorLessEqual>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct FunctionHead : x3::position_tagged
    {
        FunctionSymbol function_symbol;
        std::vector<Term> terms;
    };

    struct FunctionExpression : x3::position_tagged,
                                x3::variant<
                                    x3::forward_ast<FunctionExpressionNumber>,
                                    x3::forward_ast<FunctionExpressionBinaryOp>,
                                    x3::forward_ast<FunctionExpressionMinus>,
                                    x3::forward_ast<FunctionExpressionHead>>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct FunctionExpressionNumber : x3::position_tagged
    {
        Number number;
    };

    struct FunctionExpressionBinaryOp : x3::position_tagged
    {
        BinaryOperator binary_operator;
        FunctionExpression function_expression_left;
        FunctionExpression function_expression_right;
    };

    struct FunctionExpressionMinus : x3::position_tagged
    {
        FunctionExpression function_expression;
    };

    struct FunctionExpressionHead : x3::position_tagged
    {
        FunctionHead function_head;
    };

    /* Goal Descriptors */
    struct GoalDescriptor : x3::position_tagged,
                            x3::variant<
                                x3::forward_ast<GoalDescriptorAtom>,
                                x3::forward_ast<GoalDescriptorLiteral>,
                                x3::forward_ast<GoalDescriptorAnd>,
                                x3::forward_ast<GoalDescriptorOr>,
                                x3::forward_ast<GoalDescriptorNot>,
                                x3::forward_ast<GoalDescriptorImply>,
                                x3::forward_ast<GoalDescriptorExists>,
                                x3::forward_ast<GoalDescriptorForall>,
                                x3::forward_ast<GoalDescriptorFunctionComparison>>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct GoalDescriptorAtom : x3::position_tagged
    {
        AtomOfTerms atom_of_terms;
    };

    struct GoalDescriptorLiteral : x3::position_tagged
    {
        LiteralOfTerms literal_of_terms;
    };

    struct GoalDescriptorAnd : x3::position_tagged
    {
        std::vector<GoalDescriptor> goal_descriptors;
    };

    struct GoalDescriptorOr : x3::position_tagged
    {
        std::vector<GoalDescriptor> goal_descriptors;
    };

    struct GoalDescriptorNot : x3::position_tagged
    {
        GoalDescriptor goal_descriptor;
    };

    struct GoalDescriptorImply : x3::position_tagged
    {
        GoalDescriptor goal_descriptor_left;
        GoalDescriptor goal_descriptor_right;
    };

    struct GoalDescriptorExists : x3::position_tagged
    {
        TypedListOfVariables typed_list_of_variables;
        GoalDescriptor goal_descriptor;
    };

    struct GoalDescriptorForall : x3::position_tagged
    {
        TypedListOfVariables typed_list_of_variables;
        GoalDescriptor goal_descriptor;
    };

    struct GoalDescriptorFunctionComparison : x3::position_tagged
    {
        BinaryComparator binary_comparator;
        FunctionExpression function_expression_left;
        FunctionExpression function_expression_right;
    };

    /* <con-GD> */
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
                                          x3::forward_ast<ConstraintGoalDescriptorHoldAfter>>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct ConstraintGoalDescriptorAnd : x3::position_tagged
    {
        std::vector<ConstraintGoalDescriptor> constraint_goal_descriptors;
    };

    struct ConstraintGoalDescriptorForall : x3::position_tagged
    {
        TypedListOfVariables typed_list_of_variables;
        ConstraintGoalDescriptor constraint_goal_descriptor;
    };

    struct ConstraintGoalDescriptorAtEnd : x3::position_tagged
    {
        GoalDescriptor goal_descriptor;
    };

    struct ConstraintGoalDescriptorAlways : x3::position_tagged
    {
        GoalDescriptor goal_descriptor;
    };

    struct ConstraintGoalDescriptorSometime : x3::position_tagged
    {
        GoalDescriptor goal_descriptor;
    };

    struct ConstraintGoalDescriptorWithin : x3::position_tagged
    {
        Number number;
        GoalDescriptor goal_descriptor;
    };

    struct ConstraintGoalDescriptorAtMostOnce : x3::position_tagged
    {
        GoalDescriptor goal_descriptor;
    };

    struct ConstraintGoalDescriptorSometimeAfter : x3::position_tagged
    {
        GoalDescriptor goal_descriptor_left;
        GoalDescriptor goal_descriptor_right;
    };

    struct ConstraintGoalDescriptorSometimeBefore : x3::position_tagged
    {
        GoalDescriptor goal_descriptor_left;
        GoalDescriptor goal_descriptor_right;
    };

    struct ConstraintGoalDescriptorAlwaysWithin : x3::position_tagged
    {
        Number number;
        GoalDescriptor goal_descriptor_left;
        GoalDescriptor goal_descriptor_right;
    };

    struct ConstraintGoalDescriptorHoldDuring : x3::position_tagged
    {
        Number number_left;
        Number number_right;
        GoalDescriptor goal_descriptor;
    };

    struct ConstraintGoalDescriptorHoldAfter : x3::position_tagged
    {
        Number number;
        GoalDescriptor goal_descriptor;
    };

    /* <pre-GD> */
    struct PreferenceName : x3::position_tagged
    {
        Name name;
    };

    struct PreconditionGoalDescriptor : x3::position_tagged,
                                        x3::variant<
                                            x3::forward_ast<PreconditionGoalDescriptorSimple>,
                                            x3::forward_ast<PreconditionGoalDescriptorAnd>,
                                            x3::forward_ast<PreconditionGoalDescriptorPreference>,
                                            x3::forward_ast<PreconditionGoalDescriptorForall>>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct PreconditionGoalDescriptorSimple : x3::position_tagged
    {
        GoalDescriptor goal_descriptor;
    };

    struct PreconditionGoalDescriptorAnd : x3::position_tagged
    {
        std::vector<PreconditionGoalDescriptor> precondition_goal_descriptors;
    };

    struct PreconditionGoalDescriptorPreference : x3::position_tagged
    {
        PreferenceName preference_name;
        GoalDescriptor goal_descriptor;
    };

    struct PreconditionGoalDescriptorForall : x3::position_tagged
    {
        TypedListOfVariables typed_list_of_variables;
        PreconditionGoalDescriptor precondition_goal_descriptor;
    };

    /* <assign-op> */
    struct AssignOperatorAssign : x3::position_tagged
    {
    };

    struct AssignOperatorScaleUp : x3::position_tagged
    {
    };

    struct AssignOperatorScaleDown : x3::position_tagged
    {
    };

    struct AssignOperatorIncrease : x3::position_tagged
    {
    };

    struct AssignOperatorDecrease : x3::position_tagged
    {
    };

    struct AssignOperator : x3::position_tagged,
                            x3::variant<
                                AssignOperatorAssign,
                                AssignOperatorScaleUp,
                                AssignOperatorScaleDown,
                                AssignOperatorIncrease,
                                AssignOperatorDecrease>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    /* <effect> */
    // <p-effect>
    struct Effect : x3::position_tagged,
                    x3::variant<
                        x3::forward_ast<EffectProduction>,
                        x3::forward_ast<EffectConditional>,
                        x3::forward_ast<std::vector<Effect>>>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct EffectProductionLiteral : x3::position_tagged
    {
        LiteralOfTerms literal_of_terms;
    };

    struct EffectProductionNumericFluent : x3::position_tagged
    {
        AssignOperator assign_operator;
        FunctionExpressionHead function_expression_head;
        FunctionExpression function_expression;
    };

    struct EffectProduction : x3::position_tagged,
                              x3::variant<
                                  EffectProductionLiteral,
                                  EffectProductionNumericFluent>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    struct EffectConditionalForall : x3::position_tagged
    {
        TypedListOfVariables typed_list_of_variables;
        Effect effect;
    };

    struct EffectConditionalWhen : x3::position_tagged
    {
        GoalDescriptor goal_descriptor;
        Effect effect;
    };

    struct EffectConditional : x3::position_tagged,
                               x3::variant<
                                   EffectConditionalForall,
                                   EffectConditionalWhen>
    {
        using base_type::base_type;
        using base_type::operator=;
    };

    /* <action-def> */
    struct ActionSymbol : x3::position_tagged
    {
        Name name;
    };

    struct ActionBody : x3::position_tagged
    {
        boost::optional<PreconditionGoalDescriptor> precondition_goal_descriptor;
        boost::optional<Effect> effect;
    };

    struct Action : x3::position_tagged
    {
        ActionSymbol action_symbol;
        TypedListOfVariables typed_list_of_variables;
        ActionBody action_body;
    };

    /* <types-def> */
    struct Types : x3::position_tagged
    {
        TypedListOfNames typed_list_of_names;
    };

    /* <constants-def> */
    struct Constants : x3::position_tagged
    {
        TypedListOfNames typed_list_of_names;
    };

    /* <predicates-def> */
    struct Predicates : x3::position_tagged
    {
        std::vector<AtomicFormulaSkeleton> atomic_formula_skeletons;
    };

    /* <functions-def> */
    struct Functions : x3::position_tagged
    {
        FunctionTypedListOfAtomicFunctionSkeletons function_types_list_of_atomic_function_skeletons;
    };

    /* <constraints-def> */
    struct Constraints : x3::position_tagged
    {
        ConstraintGoalDescriptor constraint_goal_descriptor;
    };

    /* <structure-def> */
    struct Structure : x3::position_tagged
    {
        Action action;
        // TODO add other structure in a variant.
    };

    /* <domain> */
    struct DomainName : x3::position_tagged
    {
        Name name;
    };

    struct DomainDescription : x3::position_tagged
    {
        DomainName domain_name;
        boost::optional<Requirements> requirements;
        boost::optional<Types> types;
        boost::optional<Constants> constants;
        boost::optional<Predicates> predicates;
        boost::optional<Functions> functions;
        boost::optional<Constraints> constraints;
        std::vector<Structure> structures;
    };
}

#endif