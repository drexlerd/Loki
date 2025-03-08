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

#ifndef LOKI_INCLUDE_LOKI_AST_AST_HPP_
#define LOKI_INCLUDE_LOKI_AST_AST_HPP_

#include <boost/optional.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <map>
#include <sstream>
#include <vector>

namespace loki::ast
{
///////////////////////////////////////////////////////////////////////////
//  The AST
///////////////////////////////////////////////////////////////////////////
namespace x3 = boost::spirit::x3;

/**
 * Domain
 */
struct Name;
struct Variable;
struct FunctionSymbol;
struct Term;
struct Number;
struct Predicate;

struct RequirementStrips;
struct RequirementTyping;
struct RequirementNegativePreconditions;
struct RequirementDisjunctivePreconditions;
struct RequirementEquality;
struct RequirementExistentialPreconditions;
struct RequirementUniversalPreconditions;
struct RequirementQuantifiedPreconditions;
struct RequirementConditionalEffects;
struct RequirementFluents;         // PDDL 3.1 = :object-fluents + :numeric-fluents
struct RequirementObjectFluents;   // PDDL 3.1
struct RequirementFunctionValues;  // PDDL 3.1
struct RequirementAdl;
struct RequirementDurativeActions;
struct RequirementDerivedPredicates;
struct RequirementTimedInitialLiterals;
struct RequirementPreferences;
struct RequirementConstraints;
struct RequirementActionCosts;
struct RequirementNonDeterministic;
struct RequirementProbabilisticEffects;
struct Requirement;

struct Type;
struct TypeObject;
struct TypeNumber;
struct TypeEither;
struct TypedListOfNamesRecursively;  // :typing
struct TypedListOfNames;
struct TypedListOfVariablesRecursively;  // :typing
struct TypedListOfVariables;

struct AtomicFormulaSkeleton;

struct AtomicFunctionSkeletonTotalCost;
struct AtomicFunctionSkeletonGeneral;
struct AtomicFunctionSkeleton;
struct FunctionTypedListOfAtomicFunctionSkeletonsRecursively;
struct FunctionTypedListOfAtomicFunctionSkeletons;

struct AtomicFormulaOfTermsPredicate;
struct AtomicFormulaOfTermsEquality;  // :equality
struct AtomicFormulaOfTerms;
struct Atom;
struct NegatedAtom;
struct Literal;

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
struct FunctionExpression;  // :numeric-fluents
struct FunctionExpressionNumber;
struct FunctionExpressionBinaryOp;
struct FunctionExpressionMinus;
struct FunctionExpressionHead;

struct GoalDescriptor;
struct GoalDescriptorAtom;
struct GoalDescriptorLiteral;  // :negative-preconditions
struct GoalDescriptorAnd;
struct GoalDescriptorOr;                  // :disjunctive-preconditions
struct GoalDescriptorNot;                 // :disjunctive-preconditions
struct GoalDescriptorImply;               // :disjunctive-preconditions
struct GoalDescriptorExists;              // :existential-preconditions
struct GoalDescriptorForall;              // :universal-preconditions
struct GoalDescriptorFunctionComparison;  // :numeric-fluents

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
struct PreconditionGoalDescriptorPreference;  // :preferences
struct PreconditionGoalDescriptorForall;      // :universal-preconditions

struct AssignOperatorAssign;
struct AssignOperatorScaleUp;
struct AssignOperatorScaleDown;
struct AssignOperatorIncrease;
struct AssignOperatorDecrease;
struct AssignOperator;

struct Effect;
struct NumberAndEffect;
struct EffectProductionLiteral;
struct EffectProductionNumeric;
struct EffectProduction;
struct EffectCompositeForall;
struct EffectCompositeWhen;
struct EffectCompositeOneof;
struct EffectCompositeProbabilistic;
struct EffectComposite;

struct ActionSymbol;
struct ActionBody;

struct Action;
struct Axiom;  // :derived-predicates

struct DomainName;
struct Requirements;
struct Types;  // : typing
struct Constants;
struct Predicates;
struct Functions;
struct Constraints;  // :constraints
struct Structure;
struct Domain;

/**
 * Problem
 */
struct GroundFunction;

struct AtomicFormulaOfNamesPredicate;
struct AtomicFormulaOfNamesEquality;  // :equality
struct AtomicFormulaOfNames;
struct Atom;
struct NegatedAtom;
struct Literal;

struct InitialElementLiteral;
struct InitialElementTimedLiteral;   // :timed-initial-literals
struct InitialElementFunctionValue;  // :numeric-fluents
struct InitialElement;

struct MetricFunctionExpression;
struct MetricFunctionExpressionNumber;
struct MetricFunctionExpressionBinaryOperator;
struct MetricFunctionExpressionMultiOperator;
struct MetricFunctionExpressionMinus;
struct MetricFunctionExpressionBasicFunctionTerm;
struct MetricFunctionExpressionTotalTime;
struct MetricFunctionExpressionPreferences;  // :preferences

struct OptimizationMinimize;
struct OptimizationMaximize;
struct Optimization;
struct MetricSpecificationTotalCost;
struct MetricSpecificationGeneral;

struct PreferenceConstraintGoalDescriptor;
struct PreferenceConstraintGoalDescriptorAnd;
struct PreferenceConstraintGoalDescriptorForall;      // :universal-preconditions
struct PreferenceConstraintGoalDescriptorPreference;  // :preferences
struct PreferenceConstraintGoalDescriptorSimple;

struct ProblemName;
struct DomainName;
struct Objects;
struct Initial;
struct Goal;
struct Constraints;          // :constraints
struct MetricSpecification;  // :numeric-fluents

struct Problem;

/**
 * Domain
 */

/* <name> */
struct Name : x3::position_tagged
{
    std::string characters;
};

/* <variable> */
struct Variable : x3::position_tagged
{
    std::string characters;
};

/* <function-symbol> */
struct FunctionSymbol : x3::position_tagged
{
    Name name;
};

/* <term> */
struct Term : x3::position_tagged, x3::variant<Name, Variable>
{
    using base_type::base_type;
    using base_type::operator=;
};

/* <number> */
struct Number : x3::position_tagged
{
    double value;
};

/* <predicate-symbol> */
struct Predicate : x3::position_tagged
{
    Name name;
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

struct RequirementFunctionValues : x3::position_tagged
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

struct RequirementActionCosts : x3::position_tagged
{
};

struct RequirementNonDeterministic : x3::position_tagged
{
};

struct RequirementProbabilisticEffects : x3::position_tagged
{
};

struct Requirement :
    x3::position_tagged,
    x3::variant<RequirementStrips,
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
                RequirementFunctionValues,
                RequirementAdl,
                RequirementDurativeActions,
                RequirementDerivedPredicates,
                RequirementTimedInitialLiterals,
                RequirementPreferences,
                RequirementConstraints,
                RequirementActionCosts,
                RequirementNonDeterministic,
                RequirementProbabilisticEffects>
{
    using base_type = x3::variant<RequirementStrips,
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
                                  RequirementFunctionValues,
                                  RequirementAdl,
                                  RequirementDurativeActions,
                                  RequirementDerivedPredicates,
                                  RequirementTimedInitialLiterals,
                                  RequirementPreferences,
                                  RequirementConstraints,
                                  RequirementActionCosts,
                                  RequirementNonDeterministic,
                                  RequirementProbabilisticEffects>;

    using base_type::base_type;
    using base_type::operator=;
};

struct Requirements : x3::position_tagged
{
    std::vector<Requirement> requirements;
};

/* <typed list (name)> */
struct Type : x3::position_tagged, x3::variant<Name, x3::forward_ast<TypeObject>, x3::forward_ast<TypeNumber>, x3::forward_ast<TypeEither>>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct TypeObject : x3::position_tagged
{
};

struct TypeNumber : x3::position_tagged
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

struct TypedListOfNames :
    x3::position_tagged,
    x3::variant<std::vector<Name>,  // base type is object
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

struct TypedListOfVariables : x3::position_tagged, x3::variant<std::vector<Variable>, TypedListOfVariablesRecursively>
{
    using base_type::base_type;
    using base_type::operator=;
};

/* <atomic function skeleton> */
struct AtomicFormulaSkeleton : x3::position_tagged
{
    Predicate predicate;
    TypedListOfVariables typed_list_of_variables;
};

/* <function typed list (atomic function skeleton)> */
struct AtomicFunctionSkeletonTotalCost : x3::position_tagged
{
    FunctionSymbol function_symbol;
};

struct AtomicFunctionSkeletonGeneral : x3::position_tagged
{
    FunctionSymbol function_symbol;
    TypedListOfVariables arguments;
};

struct AtomicFunctionSkeleton : x3::position_tagged, x3::variant<AtomicFunctionSkeletonTotalCost, AtomicFunctionSkeletonGeneral>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct FunctionTypedListOfAtomicFunctionSkeletonsRecursively : x3::position_tagged
{
    std::vector<AtomicFunctionSkeleton> atomic_function_skeletons;
    TypeNumber function_type;
    boost::optional<x3::forward_ast<FunctionTypedListOfAtomicFunctionSkeletons>> function_typed_list_of_atomic_function_skeletons;
};

struct FunctionTypedListOfAtomicFunctionSkeletons :
    x3::position_tagged,
    x3::variant<std::vector<AtomicFunctionSkeleton>,  // :numeric-fluents and deprecated (https://ipc08.icaps-conference.org/deterministic/PddlExtension.html)
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
    Term left_term;
    Term right_term;
};

struct AtomicFormulaOfTerms : x3::position_tagged, x3::variant<AtomicFormulaOfTermsPredicate, AtomicFormulaOfTermsEquality>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct Atom : x3::position_tagged
{
    AtomicFormulaOfTerms atomic_formula_of_terms;
};

struct NegatedAtom : x3::position_tagged
{
    AtomicFormulaOfTerms atomic_formula_of_terms;
};

struct Literal : x3::position_tagged, x3::variant<Atom, NegatedAtom>
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

struct MultiOperator : x3::position_tagged, x3::variant<MultiOperatorMul, MultiOperatorPlus>
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

struct BinaryOperator : x3::position_tagged, x3::variant<BinaryOperatorMinus, BinaryOperatorDiv, MultiOperator>
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

struct BinaryComparator :
    x3::position_tagged,
    x3::variant<BinaryComparatorGreater, BinaryComparatorLess, BinaryComparatorEqual, BinaryComparatorGreaterEqual, BinaryComparatorLessEqual>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct FunctionHead : x3::position_tagged
{
    FunctionSymbol function_symbol;
    std::vector<Term> terms;
};

struct FunctionExpression :
    x3::position_tagged,
    x3::variant<x3::forward_ast<FunctionExpressionNumber>,
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
    FunctionExpression left_function_expression;
    FunctionExpression right_function_expression;
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
struct GoalDescriptor :
    x3::position_tagged,
    x3::variant<x3::forward_ast<GoalDescriptorAtom>,
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
    Atom atom;
};

struct GoalDescriptorLiteral : x3::position_tagged
{
    Literal literal;
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
    FunctionExpression left_function_expression;
    FunctionExpression right_function_expression;
};

/* <con-GD> */
struct ConstraintGoalDescriptor :
    x3::position_tagged,
    x3::variant<x3::forward_ast<ConstraintGoalDescriptorAnd>,
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

struct PreconditionGoalDescriptor :
    x3::position_tagged,
    x3::variant<x3::forward_ast<PreconditionGoalDescriptorSimple>,
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

struct AssignOperator :
    x3::position_tagged,
    x3::variant<AssignOperatorAssign, AssignOperatorScaleUp, AssignOperatorScaleDown, AssignOperatorIncrease, AssignOperatorDecrease>
{
    using base_type::base_type;
    using base_type::operator=;
};

/* <effect> */
struct Effect : x3::position_tagged, x3::variant<x3::forward_ast<EffectProduction>, x3::forward_ast<EffectComposite>, std::vector<Effect>>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct NumberAndEffect : x3::position_tagged
{
    Number number;
    Effect effect;
};

// Production effects
struct EffectProductionLiteral : x3::position_tagged
{
    Literal literal;
};

struct EffectProductionNumeric : x3::position_tagged
{
    AssignOperator assign_operator;
    FunctionHead function_head;
    FunctionExpression function_expression;
};

struct EffectProduction : x3::position_tagged, x3::variant<EffectProductionLiteral, EffectProductionNumeric>
{
    using base_type::base_type;
    using base_type::operator=;
};

// Composite effects
struct EffectCompositeForall : x3::position_tagged
{
    TypedListOfVariables typed_list_of_variables;
    Effect effect;
};

struct EffectCompositeWhen : x3::position_tagged
{
    GoalDescriptor goal_descriptor;
    Effect effect;
};

struct EffectCompositeOneof : x3::position_tagged
{
    std::vector<Effect> possibilities;
};

struct EffectCompositeProbabilistic : x3::position_tagged
{
    std::vector<NumberAndEffect> possibilities;
};

struct EffectComposite : x3::position_tagged, x3::variant<EffectCompositeForall, EffectCompositeWhen, EffectCompositeOneof, EffectCompositeProbabilistic>
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

/* <derived-def> */
struct Axiom : x3::position_tagged
{
    AtomicFormulaSkeleton atomic_formula_skeleton;
    GoalDescriptor goal_descriptor;
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
struct Structure : x3::position_tagged, x3::variant<Action, Axiom>
{
    using base_type::base_type;
    using base_type::operator=;
};

/* <domain> */
struct DomainName : x3::position_tagged
{
    Name name;
};

struct Domain : x3::position_tagged
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

/**
 * Problem
 */

struct GroundFunction : x3::position_tagged
{
    FunctionSymbol function_symbol;
    std::vector<Name> names;
};

/* Atomic formulas */
struct AtomicFormulaOfNamesPredicate : x3::position_tagged
{
    Predicate predicate;
    std::vector<Name> names;
};

struct AtomicFormulaOfNamesEquality : x3::position_tagged
{
    Name name_left;
    Name name_right;
};

struct AtomicFormulaOfNames : x3::position_tagged, x3::variant<AtomicFormulaOfNamesPredicate, AtomicFormulaOfNamesEquality>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct GroundAtom : x3::position_tagged
{
    AtomicFormulaOfNames atomic_formula_of_names;
};

struct NegatedGroundAtom : x3::position_tagged
{
    AtomicFormulaOfNames atomic_formula_of_names;
};

struct GroundLiteral : x3::position_tagged, x3::variant<GroundAtom, NegatedGroundAtom>
{
    using base_type::base_type;
    using base_type::operator=;
};

/* <init-el> */
struct InitialElementLiteral : x3::position_tagged
{
    GroundLiteral literal;
};

struct InitialElementTimedLiteral : x3::position_tagged
{
    Number number;
    GroundLiteral literal;
};

struct InitialElementFunctionValue : x3::position_tagged
{
    GroundFunction ground_function;
    Number number;
};

struct InitialElement : x3::position_tagged, x3::variant<InitialElementLiteral, InitialElementTimedLiteral, InitialElementFunctionValue>
{
    using base_type::base_type;
    using base_type::operator=;
};

/* <metric-f-exp> */
struct MetricFunctionExpression :
    x3::position_tagged,
    x3::variant<x3::forward_ast<MetricFunctionExpressionNumber>,
                x3::forward_ast<MetricFunctionExpressionBinaryOperator>,
                x3::forward_ast<MetricFunctionExpressionMultiOperator>,
                x3::forward_ast<MetricFunctionExpressionMinus>,
                x3::forward_ast<MetricFunctionExpressionBasicFunctionTerm>,
                x3::forward_ast<MetricFunctionExpressionTotalTime>,
                x3::forward_ast<MetricFunctionExpressionPreferences>>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct MetricFunctionExpressionNumber : x3::position_tagged
{
    Number number;
};

struct MetricFunctionExpressionBinaryOperator : x3::position_tagged
{
    BinaryOperator binary_operator;
    MetricFunctionExpression metric_left_function_expression;
    MetricFunctionExpression metric_right_function_expression;
};

struct MetricFunctionExpressionMultiOperator : x3::position_tagged
{
    MultiOperator multi_operator;
    MetricFunctionExpression metric_function_expression_first;
    std::vector<MetricFunctionExpression> metric_function_expression_remaining;
};

struct MetricFunctionExpressionMinus : x3::position_tagged
{
    MetricFunctionExpression metric_function_expression;
};

struct MetricFunctionExpressionBasicFunctionTerm : x3::position_tagged
{
    GroundFunction ground_function;
};

struct MetricFunctionExpressionTotalTime : x3::position_tagged
{
};

struct MetricFunctionExpressionPreferences : x3::position_tagged
{
    PreferenceName preference_name;
};

/* <optimization> */
struct OptimizationMinimize : x3::position_tagged
{
};

struct OptimizationMaximize : x3::position_tagged
{
};

struct Optimization : x3::position_tagged, x3::variant<OptimizationMinimize, OptimizationMaximize>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct MetricSpecificationTotalCost : x3::position_tagged
{
    OptimizationMinimize optimization_minimize;
    FunctionSymbol function_symbol_total_cost;
};

struct MetricSpecificationGeneral : x3::position_tagged
{
    Optimization optimization;
    MetricFunctionExpression metric_function_expression;
};

/* <pref-con-GD> */
struct PreferenceConstraintGoalDescriptor :
    x3::position_tagged,
    x3::variant<x3::forward_ast<PreferenceConstraintGoalDescriptorSimple>,
                x3::forward_ast<PreferenceConstraintGoalDescriptorPreference>,
                x3::forward_ast<PreferenceConstraintGoalDescriptorAnd>,
                x3::forward_ast<PreferenceConstraintGoalDescriptorForall>>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct PreferenceConstraintGoalDescriptorAnd : x3::position_tagged
{
    std::vector<PreferenceConstraintGoalDescriptor> preference_constraint_goal_descriptors;
};

struct PreferenceConstraintGoalDescriptorForall : x3::position_tagged
{
    TypedListOfVariables typed_list_of_variables;
    PreferenceConstraintGoalDescriptor preference_constraint_goal_descriptor;
};

struct PreferenceConstraintGoalDescriptorPreference : x3::position_tagged
{
    boost::optional<PreferenceName> preference_name;
    ConstraintGoalDescriptor constraint_goal_descriptor;
};

struct PreferenceConstraintGoalDescriptorSimple : x3::position_tagged
{
    ConstraintGoalDescriptor constraint_goal_descriptor;
};

/* <problem> */
struct ProblemName : x3::position_tagged
{
    Name name;
};

struct ProblemDomainName : x3::position_tagged
{
    Name name;
};

struct Objects : x3::position_tagged
{
    TypedListOfNames typed_list_of_names;
};

struct Initial : x3::position_tagged
{
    std::vector<InitialElement> initial_elements;
};

struct Goal : x3::position_tagged
{
    PreconditionGoalDescriptor precondition_goal_descriptor;
};

struct ProblemConstraints : x3::position_tagged
{
    PreferenceConstraintGoalDescriptor preference_constraint_goal_descriptor;
};

struct MetricSpecification : x3::position_tagged, x3::variant<MetricSpecificationTotalCost, MetricSpecificationGeneral>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct Problem : x3::position_tagged
{
    ProblemName problem_name;
    ProblemDomainName domain_name;
    boost::optional<Requirements> requirements;
    boost::optional<Objects> objects;
    boost::optional<Predicates> derived_predicates;
    boost::optional<Initial> initial;
    boost::optional<Goal> goal;
    boost::optional<ProblemConstraints> constraints;
    boost::optional<MetricSpecification> metric_specification;
    std::vector<Structure> structures;
};
}

#endif
