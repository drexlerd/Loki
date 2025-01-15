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

#ifndef LOKI_INCLUDE_LOKI_AST_PRINTER_HPP_
#define LOKI_INCLUDE_LOKI_AST_PRINTER_HPP_

#include "loki/details/ast/ast.hpp"

namespace loki
{

struct DefaultFormatterOptions
{
    // The indentation in the current level.
    int indent = 0;
    // The amount of indentation added per nesting
    int add_indent = 0;
};

/**
 * Domain
 */
// create string representations from ast nodes.
extern std::string parse_text(const ast::Name& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Variable& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Number& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::FunctionSymbol& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Term& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Predicate& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::RequirementStrips& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementTyping& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementNegativePreconditions& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementDisjunctivePreconditions& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementEquality& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementExistentialPreconditions& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementUniversalPreconditions& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementQuantifiedPreconditions& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementConditionalEffects& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementFluents& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementObjectFluents& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementFunctionValues& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementAdl& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementDurativeActions& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementDerivedPredicates& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementTimedInitialLiterals& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementPreferences& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementConstraints& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementActionCosts& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementNonDeterministic& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::RequirementProbabilisticEffects& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Requirement& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::Type& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::TypeObject& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::TypeNumber& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::TypeEither& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::TypedListOfNamesRecursively& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::TypedListOfNames& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::TypedListOfVariablesRecursively& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::TypedListOfVariables& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::AtomicFormulaSkeleton& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::AtomicFunctionSkeletonTotalCost& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::AtomicFunctionSkeletonGeneral& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::AtomicFunctionSkeleton& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::FunctionTypedListOfAtomicFunctionSkeletons& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::AtomicFormulaOfTermsPredicate& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::AtomicFormulaOfTermsEquality& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::AtomicFormulaOfTerms& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Atom& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::NegatedAtom& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Literal& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::MultiOperatorMul& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::MultiOperatorPlus& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::MultiOperator& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::BinaryOperatorMinus& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::BinaryOperatorDiv& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::BinaryOperator& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::BinaryComparatorGreater& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::BinaryComparatorLess& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::BinaryComparatorEqual& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::BinaryComparatorGreaterEqual& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::BinaryComparatorLessEqual& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::BinaryComparator& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::FunctionHead& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::FunctionExpression& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::FunctionExpressionNumber& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::FunctionExpressionBinaryOp& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::FunctionExpressionMinus& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::FunctionExpressionHead& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::GoalDescriptor& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorAtom& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorLiteral& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorAnd& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorOr& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorNot& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorImply& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorExists& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorForall& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorFunctionComparison& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::ConstraintGoalDescriptor& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorAnd& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorForall& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorAtEnd& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorAlways& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorSometime& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorWithin& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorAtMostOnce& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorSometimeAfter& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorSometimeBefore& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorAlwaysWithin& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorHoldDuring& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorHoldAfter& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::PreferenceName& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::PreconditionGoalDescriptor& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::PreconditionGoalDescriptorSimple& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::PreconditionGoalDescriptorAnd& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::PreconditionGoalDescriptorPreference& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::PreconditionGoalDescriptorForall& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::AssignOperatorAssign& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::AssignOperatorScaleUp& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::AssignOperatorScaleDown& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::AssignOperatorIncrease& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::AssignOperatorDecrease& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::AssignOperator& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::Effect& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::NumberAndEffect& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::EffectProductionLiteral& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::EffectProductionNumeric& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::EffectProduction& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::EffectCompositeForall& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::EffectCompositeWhen& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::EffectCompositeOneof& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::EffectCompositeProbabilistic& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::EffectComposite& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::ActionSymbol& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::ActionBody& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::Action& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Axiom& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::DomainName& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Requirements& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Types& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Constants& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Predicates& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Functions& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Constraints& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Structure& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Domain& node, const DefaultFormatterOptions& options = {});

/**
 * Problem
 */

// create string representations from ast nodes.
extern std::string parse_text(const ast::GroundFunction& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::AtomicFormulaOfNamesPredicate& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::AtomicFormulaOfNamesEquality& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::AtomicFormulaOfNames& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::GroundAtom& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::NegatedGroundAtom& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::GroundLiteral& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::InitialElementLiteral& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::InitialElementTimedLiteral& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::InitialElementFunctionValue& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::InitialElement& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::MetricFunctionExpression& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::MetricFunctionExpressionNumber& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::MetricFunctionExpressionBinaryOperator& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::MetricFunctionExpressionMultiOperator& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::MetricFunctionExpressionMinus& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::MetricFunctionExpressionBasicFunctionTerm& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::MetricFunctionExpressionTotalTime& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::MetricFunctionExpressionPreferences& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::OptimizationMinimize& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::OptimizationMaximize& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Optimization& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::MetricSpecificationTotalCost& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::MetricSpecificationGeneral& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::PreferenceConstraintGoalDescriptor& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::PreferenceConstraintGoalDescriptorAnd& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::PreferenceConstraintGoalDescriptorForall& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::PreferenceConstraintGoalDescriptorPreference& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::PreferenceConstraintGoalDescriptorSimple& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::ProblemName& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::ProblemDomainName& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Objects& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Initial& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::Goal& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::ProblemConstraints& node, const DefaultFormatterOptions& options = {});
extern std::string parse_text(const ast::MetricSpecification& node, const DefaultFormatterOptions& options = {});

extern std::string parse_text(const ast::Problem& node, const DefaultFormatterOptions& options = {});
}

#endif
