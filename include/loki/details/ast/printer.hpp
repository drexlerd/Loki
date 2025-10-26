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

struct DefaultFormatter
{
    int indent = 0;
    int add_indent = 0;
};

template<typename T>
concept Formatter = requires(T f) {
    { f.indent } -> std::convertible_to<std::size_t>;
    { f.add_indent } -> std::convertible_to<std::size_t>;
};

/**
 * Helpers to materialize strings
 */

template<Formatter F, typename T>
std::string string(const T& element, F formatter)
{
    std::stringstream ss;
    if constexpr (std::is_pointer_v<std::decay_t<decltype(element)>>)
        write(*element, formatter, ss);
    else
        write(element, formatter, ss);
    return ss.str();
}

template<Formatter F, std::ranges::input_range Range>
std::vector<std::string> strings(const Range& range, F formatter)
{
    auto result = std::vector<std::string> {};
    if constexpr (std::ranges::sized_range<Range>)
        result.reserve(std::ranges::size(range));
    for (const auto& element : range)
        result.push_back(string(element, formatter));
    return result;
}

/**
 * Domain
 */

template<Formatter T>
void write(const ast::Name& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Variable& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Number& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::FunctionSymbol& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Term& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Predicate& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::RequirementStrips& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementTyping& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementNegativePreconditions& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementDisjunctivePreconditions& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementEquality& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementExistentialPreconditions& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementUniversalPreconditions& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementQuantifiedPreconditions& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementConditionalEffects& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementFluents& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementObjectFluents& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementFunctionValues& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementAdl& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementDurativeActions& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementDerivedPredicates& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementTimedInitialLiterals& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementPreferences& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementConstraints& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementActionCosts& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementNonDeterministic& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::RequirementProbabilisticEffects& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Requirement& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Type& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::TypeObject& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::TypeNumber& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::TypeEither& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::TypedListOfNamesRecursively& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::TypedListOfNames& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::TypedListOfVariablesRecursively& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::TypedListOfVariables& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AtomicFormulaSkeleton& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AtomicFunctionSkeletonTotalCost& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::AtomicFunctionSkeletonGeneral& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::AtomicFunctionSkeleton& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::FunctionTypedListOfAtomicFunctionSkeletons& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AtomicFormulaOfTermsPredicate& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::AtomicFormulaOfTermsEquality& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::AtomicFormulaOfTerms& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Atom& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::NegatedAtom& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Literal& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::MultiOperatorMul& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::MultiOperatorPlus& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::MultiOperator& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::BinaryOperatorMinus& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::BinaryOperatorDiv& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::BinaryOperator& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::BinaryComparatorGreater& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::BinaryComparatorLess& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::BinaryComparatorEqual& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::BinaryComparatorGreaterEqual& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::BinaryComparatorLessEqual& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::BinaryComparator& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::FunctionHead& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::FunctionExpression& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::FunctionExpressionNumber& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::FunctionExpressionBinaryOp& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::FunctionExpressionMultiOp& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::FunctionExpressionMinus& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::FunctionExpressionHead& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::GoalDescriptor& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::GoalDescriptorAtom& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::GoalDescriptorLiteral& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::GoalDescriptorAnd& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::GoalDescriptorOr& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::GoalDescriptorNot& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::GoalDescriptorImply& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::GoalDescriptorExists& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::GoalDescriptorForall& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::GoalDescriptorFunctionComparison& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ConstraintGoalDescriptor& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::ConstraintGoalDescriptorAnd& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::ConstraintGoalDescriptorForall& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::ConstraintGoalDescriptorAtEnd& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::ConstraintGoalDescriptorAlways& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::ConstraintGoalDescriptorSometime& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::ConstraintGoalDescriptorWithin& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::ConstraintGoalDescriptorAtMostOnce& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::ConstraintGoalDescriptorSometimeAfter& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::ConstraintGoalDescriptorSometimeBefore& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::ConstraintGoalDescriptorAlwaysWithin& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::ConstraintGoalDescriptorHoldDuring& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::ConstraintGoalDescriptorHoldAfter& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::PreferenceName& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::PreconditionGoalDescriptor& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::PreconditionGoalDescriptorSimple& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::PreconditionGoalDescriptorAnd& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::PreconditionGoalDescriptorPreference& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::PreconditionGoalDescriptorForall& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AssignOperatorAssign& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::AssignOperatorScaleUp& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::AssignOperatorScaleDown& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::AssignOperatorIncrease& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::AssignOperatorDecrease& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::AssignOperator& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Effect& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::NumberAndEffect& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::EffectProductionLiteral& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::EffectProductionNumeric& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::EffectProduction& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::EffectCompositeForall& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::EffectCompositeWhen& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::EffectCompositeOneof& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::EffectCompositeProbabilistic& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::EffectComposite& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ActionSymbol& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::ActionBody& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Action& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Axiom& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::DomainName& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Requirements& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Types& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Constants& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Predicates& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Functions& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Constraints& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Structure& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Domain& node, T formatter, std::ostream& out);

/**
 * Problem
 */

template<Formatter T>
void write(const ast::GroundFunction& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AtomicFormulaOfNamesPredicate& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::AtomicFormulaOfNamesEquality& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::AtomicFormulaOfNames& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::GroundAtom& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::NegatedGroundAtom& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::GroundLiteral& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::InitialElementLiteral& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::InitialElementTimedLiteral& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::InitialElementFunctionValue& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::InitialElement& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::MetricFunctionExpression& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::MetricFunctionExpressionNumber& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::MetricFunctionExpressionBinaryOperator& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::MetricFunctionExpressionMultiOperator& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::MetricFunctionExpressionMinus& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::MetricFunctionExpressionBasicFunctionTerm& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::MetricFunctionExpressionTotalTime& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::MetricFunctionExpressionPreferences& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::OptimizationMinimize& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::OptimizationMaximize& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Optimization& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::MetricSpecificationTotalCost& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::MetricSpecificationGeneral& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::PreferenceConstraintGoalDescriptor& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::PreferenceConstraintGoalDescriptorAnd& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::PreferenceConstraintGoalDescriptorForall& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::PreferenceConstraintGoalDescriptorPreference& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::PreferenceConstraintGoalDescriptorSimple& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ProblemName& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::ProblemDomainName& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Objects& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Initial& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::Goal& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::ProblemConstraints& node, T formatter, std::ostream& out);
template<Formatter T>
void write(const ast::MetricSpecification& node, T formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Problem& node, T formatter, std::ostream& out);
}

#endif
