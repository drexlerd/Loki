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

#include "loki/ast/ast.hpp"
#include "loki/utils/printer.hpp"

namespace loki
{

/**
 * Domain
 */
// create string representations from ast nodes.
extern std::string parse_text(const ast::Name& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Variable& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Number& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::FunctionSymbol& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Term& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Predicate& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::RequirementStrips& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementTyping& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementNegativePreconditions& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementDisjunctivePreconditions& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementEquality& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementExistentialPreconditions& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementUniversalPreconditions& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementQuantifiedPreconditions& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementConditionalEffects& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementFluents& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementObjectFluents& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementNumericFluents& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementAdl& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementDurativeActions& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementDerivedPredicates& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementTimedInitialLiterals& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementPreferences& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementConstraints& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::RequirementActionCosts& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Requirement& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::Type& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::TypeObject& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::TypeNumber& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::TypeEither& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::TypedListOfNamesRecursively& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::TypedListOfNames& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::TypedListOfVariablesRecursively& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::TypedListOfVariables& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::AtomicFormulaSkeleton& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::AtomicFunctionSkeletonTotalCost& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::AtomicFunctionSkeletonGeneral& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::AtomicFunctionSkeleton& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::FunctionTypedListOfAtomicFunctionSkeletons& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::AtomicFormulaOfTermsPredicate& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::AtomicFormulaOfTermsEquality& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::AtomicFormulaOfTerms& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Atom& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::NegatedAtom& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Literal& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::MultiOperatorMul& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::MultiOperatorPlus& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::MultiOperator& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::BinaryOperatorMinus& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::BinaryOperatorDiv& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::BinaryOperator& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::BinaryComparatorGreater& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::BinaryComparatorLess& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::BinaryComparatorEqual& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::BinaryComparatorGreaterEqual& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::BinaryComparatorLessEqual& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::BinaryComparator& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::FunctionHead& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::FunctionExpression& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::FunctionExpressionNumber& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::FunctionExpressionBinaryOp& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::FunctionExpressionMinus& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::FunctionExpressionHead& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::GoalDescriptor& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorAtom& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorLiteral& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorAnd& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorOr& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorNot& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorImply& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorExists& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorForall& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::GoalDescriptorFunctionComparison& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::ConstraintGoalDescriptor& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorAnd& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorForall& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorAtEnd& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorAlways& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorSometime& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorWithin& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorAtMostOnce& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorSometimeAfter& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorSometimeBefore& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorAlwaysWithin& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorHoldDuring& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::ConstraintGoalDescriptorHoldAfter& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::PreferenceName& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::PreconditionGoalDescriptor& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::PreconditionGoalDescriptorSimple& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::PreconditionGoalDescriptorAnd& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::PreconditionGoalDescriptorPreference& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::PreconditionGoalDescriptorForall& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::AssignOperatorAssign& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::AssignOperatorScaleUp& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::AssignOperatorScaleDown& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::AssignOperatorIncrease& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::AssignOperatorDecrease& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::AssignOperator& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::Effect& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::EffectProductionLiteral& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::EffectProductionNumericFluentTotalCost& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::EffectProductionNumericFluentGeneral& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::EffectProduction& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::EffectConditionalForall& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::EffectConditionalWhen& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::EffectConditional& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::ActionSymbol& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::ActionBody& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::Action& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::DerivedPredicate& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::DomainName& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Requirements& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Types& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Constants& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Predicates& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Functions& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Constraints& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Structure& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Domain& node, const FormattingOptions& options = {});

/**
 * Problem
 */

// create string representations from ast nodes.
extern std::string parse_text(const ast::BasicFunctionTerm& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::AtomicFormulaOfNamesPredicate& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::AtomicFormulaOfNamesEquality& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::AtomicFormulaOfNames& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::GroundAtom& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::NegatedGroundAtom& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::GroundLiteral& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::InitialElementLiteral& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::InitialElementTimedLiterals& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::InitialElementNumericFluentsTotalCost& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::InitialElementNumericFluentsGeneral& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::InitialElement& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::MetricFunctionExpression& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::MetricFunctionExpressionNumber& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::MetricFunctionExpressionBinaryOperator& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::MetricFunctionExpressionMultiOperator& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::MetricFunctionExpressionMinus& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::MetricFunctionExpressionBasicFunctionTerm& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::MetricFunctionExpressionTotalTime& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::MetricFunctionExpressionPreferences& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::OptimizationMinimize& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::OptimizationMaximize& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Optimization& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::MetricSpecificationTotalCost& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::MetricSpecificationGeneral& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::PreferenceConstraintGoalDescriptor& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::PreferenceConstraintGoalDescriptorAnd& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::PreferenceConstraintGoalDescriptorForall& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::PreferenceConstraintGoalDescriptorPreference& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::PreferenceConstraintGoalDescriptorSimple& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::ProblemName& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::ProblemDomainName& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Objects& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Initial& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::Goal& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::ProblemConstraints& node, const FormattingOptions& options = {});
extern std::string parse_text(const ast::MetricSpecification& node, const FormattingOptions& options = {});

extern std::string parse_text(const ast::Problem& node, const FormattingOptions& options = {});
}

#endif