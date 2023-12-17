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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_AST_PRINTER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_AST_PRINTER_HPP_

#include "ast.hpp"
#include "../../common/printer.hpp"


namespace loki {

// create string representations from ast nodes.
extern std::string parse_text(const domain::ast::RequirementStrips& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementTyping& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementNegativePreconditions& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementDisjunctivePreconditions& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementEquality& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementExistentialPreconditions& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementUniversalPreconditions& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementQuantifiedPreconditions& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementConditionalEffects& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementFluents& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementObjectFluents& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementNumericFluents& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementAdl& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementDurativeActions& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementDerivedPredicates& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementTimedInitialLiterals& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementPreferences& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementConstraints& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::RequirementActionCosts& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::Requirement& node, const FormattingOptions& options={});

extern std::string parse_text(const domain::ast::Type& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::TypeEither& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::TypedListOfNamesRecursively& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::TypedListOfNames& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::TypedListOfVariablesRecursively& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::TypedListOfVariables& node, const FormattingOptions& options={});

extern std::string parse_text(const domain::ast::AtomicFormulaSkeleton& node, const FormattingOptions& options={});

extern std::string parse_text(const domain::ast::FunctionTypeNumber& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::FunctionTypeType& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::FunctionType& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::AtomicFunctionSkeleton& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::FunctionTypedListOfAtomicFunctionSkeletons& node, const FormattingOptions& options={});

extern std::string parse_text(const domain::ast::AtomicFormulaOfTermsPredicate& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::AtomicFormulaOfTermsEquality& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::AtomicFormulaOfTerms& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::Atom& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::NegatedAtom& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::Literal& node, const FormattingOptions& options={});

extern std::string parse_text(const domain::ast::MultiOperatorMul& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::MultiOperatorPlus& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::MultiOperator& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::BinaryOperatorMinus& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::BinaryOperatorDiv& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::BinaryOperator& node, const FormattingOptions& options={});

extern std::string parse_text(const domain::ast::BinaryComparatorGreater& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::BinaryComparatorLess& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::BinaryComparatorEqual& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::BinaryComparatorGreaterEqual& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::BinaryComparatorLessEqual& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::BinaryComparator& node, const FormattingOptions& options={});

extern std::string parse_text(const domain::ast::FunctionHead& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::FunctionExpression& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::FunctionExpressionNumber& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::FunctionExpressionBinaryOp& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::FunctionExpressionMinus& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::FunctionExpressionHead& node, const FormattingOptions& options={});

extern std::string parse_text(const domain::ast::GoalDescriptor& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::GoalDescriptorAtom& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::GoalDescriptorLiteral& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::GoalDescriptorAnd& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::GoalDescriptorOr& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::GoalDescriptorNot& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::GoalDescriptorImply& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::GoalDescriptorExists& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::GoalDescriptorForall& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::GoalDescriptorFunctionComparison& node, const FormattingOptions& options={});

extern std::string parse_text(const domain::ast::ConstraintGoalDescriptor& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::ConstraintGoalDescriptorAnd& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::ConstraintGoalDescriptorForall& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::ConstraintGoalDescriptorAtEnd& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::ConstraintGoalDescriptorAlways& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::ConstraintGoalDescriptorSometime& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::ConstraintGoalDescriptorWithin& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::ConstraintGoalDescriptorAtMostOnce& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::ConstraintGoalDescriptorSometimeAfter& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::ConstraintGoalDescriptorSometimeBefore& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::ConstraintGoalDescriptorAlwaysWithin& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::ConstraintGoalDescriptorHoldDuring& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::ConstraintGoalDescriptorHoldAfter& node, const FormattingOptions& options={});

extern std::string parse_text(const domain::ast::PreferenceName& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::PreconditionGoalDescriptor& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::PreconditionGoalDescriptorSimple& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::PreconditionGoalDescriptorAnd& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::PreconditionGoalDescriptorPreference& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::PreconditionGoalDescriptorForall& node, const FormattingOptions& options={});

extern std::string parse_text(const domain::ast::AssignOperatorAssign& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::AssignOperatorScaleUp& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::AssignOperatorScaleDown& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::AssignOperatorIncrease& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::AssignOperatorDecrease& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::AssignOperator& node, const FormattingOptions& options={});

extern std::string parse_text(const domain::ast::Effect& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::EffectProductionLiteral& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::EffectProductionNumericFluent& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::EffectProductionObjectFluent& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::EffectProduction& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::EffectConditionalForall& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::EffectConditionalWhen& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::EffectConditional& node, const FormattingOptions& options={});

extern std::string parse_text(const domain::ast::ActionSymbol& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::ActionBody& node, const FormattingOptions& options={});

extern std::string parse_text(const domain::ast::Action& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::DerivedPredicate& node, const FormattingOptions& options={});

extern std::string parse_text(const domain::ast::DomainName& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::Requirements& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::Types& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::Constants& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::Predicates& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::Functions& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::Constraints& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::Structure& node, const FormattingOptions& options={});
extern std::string parse_text(const domain::ast::Domain& node, const FormattingOptions& options={});

}

#endif