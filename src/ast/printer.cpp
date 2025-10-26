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

#include "loki/details/ast/printer.hpp"

#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <ranges>
#include <sstream>
#include <vector>

namespace loki
{

// Writer for boost::variant
template<Formatter T>
class NodeVisitorPrinter
{
private:
    T formatter;
    std::ostream& out;

public:
    NodeVisitorPrinter(T formatter, std::ostream& out) : formatter(formatter), out(out) {}

    template<typename Node>
    void operator()(const Node& node) const;
};

template<Formatter F, std::ranges::input_range Range>
void write(const Range& range, F formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Name& node, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "{}", node.characters);
}

template void write<DefaultFormatter>(const ast::Name& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Variable& node, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "{}", node.characters);
}

template void write<DefaultFormatter>(const ast::Variable& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::FunctionSymbol& node, T formatter, std::ostream& out)
{
    fmt::print(out, "{}", string(node.name, formatter));
}

template void write<DefaultFormatter>(const ast::FunctionSymbol& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Term& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::Term& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Predicate& node, T formatter, std::ostream& out)
{
    fmt::print(out, "{}", string(node.name, formatter));
}

template void write<DefaultFormatter>(const ast::Predicate& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Number& node, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "{}", node.value);
}

template void write<DefaultFormatter>(const ast::Number& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::RequirementStrips& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":strips");
}

template void write<DefaultFormatter>(const ast::RequirementStrips&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementTyping& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":typing");
}

template void write<DefaultFormatter>(const ast::RequirementTyping&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementNegativePreconditions& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":negative-preconditions");
}

template void write<DefaultFormatter>(const ast::RequirementNegativePreconditions&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementDisjunctivePreconditions& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":disjunctive-preconditions");
}

template void write<DefaultFormatter>(const ast::RequirementDisjunctivePreconditions&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementEquality& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":equality");
}

template void write<DefaultFormatter>(const ast::RequirementEquality&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementExistentialPreconditions& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":existential-preconditions");
}

template void write<DefaultFormatter>(const ast::RequirementExistentialPreconditions&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementUniversalPreconditions& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":universal-preconditions");
}

template void write<DefaultFormatter>(const ast::RequirementUniversalPreconditions&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementQuantifiedPreconditions& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":quantified-preconditions");
}

template void write<DefaultFormatter>(const ast::RequirementQuantifiedPreconditions&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementConditionalEffects& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":conditional-effects");
}

template void write<DefaultFormatter>(const ast::RequirementConditionalEffects&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementFluents& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":fluents");
}

template void write<DefaultFormatter>(const ast::RequirementFluents&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementObjectFluents& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":object-fluents");
}

template void write<DefaultFormatter>(const ast::RequirementObjectFluents&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementFunctionValues& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":numeric-fluents");
}

template void write<DefaultFormatter>(const ast::RequirementFunctionValues&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementAdl& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":adl");
}

template void write<DefaultFormatter>(const ast::RequirementAdl&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementDurativeActions& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":durative-actions");
}

template void write<DefaultFormatter>(const ast::RequirementDurativeActions&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementDerivedPredicates& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":derived-predicates");
}

template void write<DefaultFormatter>(const ast::RequirementDerivedPredicates&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementTimedInitialLiterals& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":timed-initial-literals");
}

template void write<DefaultFormatter>(const ast::RequirementTimedInitialLiterals&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementPreferences& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":preferences");
}

template void write<DefaultFormatter>(const ast::RequirementPreferences&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementConstraints& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":constraints");
}

template void write<DefaultFormatter>(const ast::RequirementConstraints&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementActionCosts& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":action-costs");
}

template void write<DefaultFormatter>(const ast::RequirementActionCosts&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementNonDeterministic& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":non-deterministic");
}

template void write<DefaultFormatter>(const ast::RequirementNonDeterministic&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::RequirementProbabilisticEffects& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ":probabilistic-effects");
}

template void write<DefaultFormatter>(const ast::RequirementProbabilisticEffects&, DefaultFormatter, std::ostream&);

template<Formatter T>
void write(const ast::Requirement& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::Requirement& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Type& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::Type& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::TypeObject& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "object");
}

template void write<DefaultFormatter>(const ast::TypeObject& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::TypeNumber& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "number");
}

template void write<DefaultFormatter>(const ast::TypeNumber& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::TypeEither& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(either {})", fmt::join(strings(node.types, formatter), " "));
}

template void write<DefaultFormatter>(const ast::TypeEither& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::TypedListOfNamesRecursively& node, T formatter, std::ostream& out)
{
    if (!node.names.empty())
    {
        fmt::print(out, "{} - {}", fmt::join(strings(node.names, formatter), " "), string(node.type, formatter));

        auto remainder = fmt::format("{}", string(node.typed_list_of_names, formatter));

        if (!remainder.empty())
            fmt::print(out, " {}", remainder);
    }
}

template void write<DefaultFormatter>(const ast::TypedListOfNamesRecursively& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::TypedListOfNames& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::TypedListOfNames& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::TypedListOfVariablesRecursively& node, T formatter, std::ostream& out)
{
    if (!node.variables.empty())
    {
        fmt::print(out, "{} - {}", fmt::join(strings(node.variables, formatter), " "), string(node.type, formatter));

        auto remainder = fmt::format("{}", string(node.typed_list_of_variables, formatter));

        if (!remainder.empty())
            fmt::print(out, " {}", remainder);
    }
}

template void write<DefaultFormatter>(const ast::TypedListOfVariablesRecursively& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::TypedListOfVariables& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::TypedListOfVariables& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AtomicFormulaSkeleton& node, T formatter, std::ostream& out)
{
    fmt::print(out, "({} {})", string(node.predicate, formatter), string(node.typed_list_of_variables, formatter));
}

template void write<DefaultFormatter>(const ast::AtomicFormulaSkeleton& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AtomicFunctionSkeletonTotalCost& node, T formatter, std::ostream& out)
{
    fmt::print(out, "({})", string(node.function_symbol, formatter));
}

template void write<DefaultFormatter>(const ast::AtomicFunctionSkeletonTotalCost& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AtomicFunctionSkeletonGeneral& node, T formatter, std::ostream& out)
{
    fmt::print(out, "({} {})", string(node.function_symbol, formatter), string(node.arguments, formatter));
}

template<Formatter T>
void write(const ast::AtomicFunctionSkeleton& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::AtomicFunctionSkeleton& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& node, T formatter, std::ostream& out)
{
    if (!node.atomic_function_skeletons.empty())
    {
        fmt::print(out, "{} - {}", fmt::join(strings(node.atomic_function_skeletons, formatter), " "), string(node.function_type, formatter));

        if (node.function_typed_list_of_atomic_function_skeletons.has_value())
            fmt::print(out, "\n{}", string(node.function_typed_list_of_atomic_function_skeletons.value(), formatter));
    }
}

template void write<DefaultFormatter>(const ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::FunctionTypedListOfAtomicFunctionSkeletons& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::FunctionTypedListOfAtomicFunctionSkeletons& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AtomicFormulaOfTermsPredicate& node, T formatter, std::ostream& out)
{
    fmt::print(out, "({} {})", string(node.predicate, formatter), fmt::join(strings(node.terms, formatter), " "));
}

template void write<DefaultFormatter>(const ast::AtomicFormulaOfTermsPredicate& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AtomicFormulaOfTermsEquality& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(= {} {})", string(node.left_term, formatter), string(node.right_term, formatter));
}

template void write<DefaultFormatter>(const ast::AtomicFormulaOfTermsEquality& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AtomicFormulaOfTerms& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::AtomicFormulaOfTerms& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Atom& node, T formatter, std::ostream& out)
{
    write(node.atomic_formula_of_terms, formatter, out);
}

template<Formatter T>
void write(const ast::NegatedAtom& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(not {})", string(node.atomic_formula_of_terms, formatter));
}

template<Formatter T>
void write(const ast::Literal& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::Literal& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::MultiOperatorMul&, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "*");
}

template void write<DefaultFormatter>(const ast::MultiOperatorMul&, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::MultiOperatorPlus&, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "+");
}

template void write<DefaultFormatter>(const ast::MultiOperatorPlus&, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::MultiOperator& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::MultiOperator& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::BinaryOperatorMinus&, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "-");
}

template void write<DefaultFormatter>(const ast::BinaryOperatorMinus&, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::BinaryOperatorDiv&, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "/");
}

template void write<DefaultFormatter>(const ast::BinaryOperatorDiv&, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::BinaryOperator& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::BinaryOperator& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::BinaryComparatorGreater&, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ">");
}

template void write<DefaultFormatter>(const ast::BinaryComparatorGreater& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::BinaryComparatorLess&, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "<");
}

template void write<DefaultFormatter>(const ast::BinaryComparatorLess& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::BinaryComparatorEqual&, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "=");
}

template void write<DefaultFormatter>(const ast::BinaryComparatorEqual& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::BinaryComparatorGreaterEqual&, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, ">=");
}

template void write<DefaultFormatter>(const ast::BinaryComparatorGreaterEqual& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::BinaryComparatorLessEqual&, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "<=");
}

template void write<DefaultFormatter>(const ast::BinaryComparatorLessEqual& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::BinaryComparator& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::BinaryComparator& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::FunctionHead& node, T formatter, std::ostream& out)
{
    if (node.terms.size() > 0)
        fmt::print(out, "({} {})", string(node.function_symbol, formatter), fmt::join(strings(node.terms, formatter), " "));
    else
        fmt::print(out, "({})", string(node.function_symbol, formatter));
}

template void write<DefaultFormatter>(const ast::FunctionHead& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::FunctionExpression& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::FunctionExpression& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::FunctionExpressionNumber& node, T formatter, std::ostream& out)
{
    write(node.number, formatter, out);
}

template void write<DefaultFormatter>(const ast::FunctionExpressionNumber& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::FunctionExpressionBinaryOp& node, T formatter, std::ostream& out)
{
    fmt::print(out,
               "({} {} {})",
               string(node.binary_operator, formatter),
               string(node.left_function_expression, formatter),
               string(node.right_function_expression, formatter));
}

template void write<DefaultFormatter>(const ast::FunctionExpressionBinaryOp& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::FunctionExpressionMultiOp& node, T formatter, std::ostream& out)
{
    fmt::print(out, "({} {})", string(node.multi_operator, formatter), fmt::join(strings(node.function_expressions, formatter), " "));
}

template void write<DefaultFormatter>(const ast::FunctionExpressionMultiOp& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::FunctionExpressionMinus& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(- {})", string(node.function_expression, formatter));
}

template void write<DefaultFormatter>(const ast::FunctionExpressionMinus& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::FunctionExpressionHead& node, T formatter, std::ostream& out)
{
    write(node.function_head, formatter, out);
}

template void write<DefaultFormatter>(const ast::FunctionExpressionHead& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::GoalDescriptor& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::GoalDescriptor& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::GoalDescriptorAtom& node, T formatter, std::ostream& out)
{
    write(node.atom, formatter, out);
}

template void write<DefaultFormatter>(const ast::GoalDescriptorAtom& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::GoalDescriptorLiteral& node, T formatter, std::ostream& out)
{
    write(node.literal, formatter, out);
}

template void write<DefaultFormatter>(const ast::GoalDescriptorLiteral& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::GoalDescriptorAnd& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(and {})", fmt::join(strings(node.goal_descriptors, formatter), " "));
}

template void write<DefaultFormatter>(const ast::GoalDescriptorAnd& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::GoalDescriptorOr& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(or {})", fmt::join(strings(node.goal_descriptors, formatter), " "));
}

template void write<DefaultFormatter>(const ast::GoalDescriptorOr& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::GoalDescriptorNot& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(not {})", string(node.goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::GoalDescriptorNot& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::GoalDescriptorImply& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(imply {} {})", string(node.goal_descriptor_left, formatter), string(node.goal_descriptor_right, formatter));
}

template void write<DefaultFormatter>(const ast::GoalDescriptorImply& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::GoalDescriptorExists& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(exists ({}) {})", string(node.typed_list_of_variables, formatter), string(node.goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::GoalDescriptorExists& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::GoalDescriptorForall& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(forall ({}) {})", string(node.typed_list_of_variables, formatter), string(node.goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::GoalDescriptorForall& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::GoalDescriptorFunctionComparison& node, T formatter, std::ostream& out)
{
    fmt::print(out,
               "({} {} {})",
               string(node.binary_comparator, formatter),
               string(node.left_function_expression, formatter),
               string(node.right_function_expression, formatter));
}

template void write<DefaultFormatter>(const ast::GoalDescriptorFunctionComparison& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ConstraintGoalDescriptor& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::ConstraintGoalDescriptor& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ConstraintGoalDescriptorAnd& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(and {})", fmt::join(strings(node.constraint_goal_descriptors, formatter), " "));
}

template void write<DefaultFormatter>(const ast::ConstraintGoalDescriptorAnd& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ConstraintGoalDescriptorForall& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(forall ({}) {})", string(node.typed_list_of_variables, formatter), string(node.constraint_goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::ConstraintGoalDescriptorForall& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ConstraintGoalDescriptorAtEnd& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(at end {})", string(node.goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::ConstraintGoalDescriptorAtEnd& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ConstraintGoalDescriptorAlways& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(always {})", string(node.goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::ConstraintGoalDescriptorAlways& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ConstraintGoalDescriptorSometime& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(sometime {})", string(node.goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::ConstraintGoalDescriptorSometime& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ConstraintGoalDescriptorWithin& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(within {} {})", string(node.number, formatter), string(node.goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::ConstraintGoalDescriptorWithin& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ConstraintGoalDescriptorAtMostOnce& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(at-most-once {})", string(node.goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::ConstraintGoalDescriptorAtMostOnce& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ConstraintGoalDescriptorSometimeAfter& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(sometime-after {} {})", string(node.goal_descriptor_left, formatter), string(node.goal_descriptor_right, formatter));
}

template void write<DefaultFormatter>(const ast::ConstraintGoalDescriptorSometimeAfter& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ConstraintGoalDescriptorSometimeBefore& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(sometime-before {} {})", string(node.goal_descriptor_left, formatter), string(node.goal_descriptor_right, formatter));
}

template void write<DefaultFormatter>(const ast::ConstraintGoalDescriptorSometimeBefore& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ConstraintGoalDescriptorAlwaysWithin& node, T formatter, std::ostream& out)
{
    fmt::print(out,
               "(always-within {} {} {})",
               string(node.number, formatter),
               string(node.goal_descriptor_left, formatter),
               string(node.goal_descriptor_right, formatter));
}

template void write<DefaultFormatter>(const ast::ConstraintGoalDescriptorAlwaysWithin& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ConstraintGoalDescriptorHoldDuring& node, T formatter, std::ostream& out)
{
    fmt::print(out,
               "(hold-during {} {} {})",
               string(node.number_left, formatter),
               string(node.number_right, formatter),
               string(node.goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::ConstraintGoalDescriptorHoldDuring& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ConstraintGoalDescriptorHoldAfter& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(hold-after {} {})", string(node.number, formatter), string(node.goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::ConstraintGoalDescriptorHoldAfter& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::PreferenceName& node, T formatter, std::ostream& out)
{
    write(node.name, formatter, out);
}

template void write<DefaultFormatter>(const ast::PreferenceName& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::PreconditionGoalDescriptor& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::PreconditionGoalDescriptor& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::PreconditionGoalDescriptorSimple& node, T formatter, std::ostream& out)
{
    write(node.goal_descriptor, formatter, out);
}

template void write<DefaultFormatter>(const ast::PreconditionGoalDescriptorSimple& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::PreconditionGoalDescriptorAnd& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(and {})", fmt::join(strings(node.precondition_goal_descriptors, formatter), " "));
}

template void write<DefaultFormatter>(const ast::PreconditionGoalDescriptorAnd& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::PreconditionGoalDescriptorPreference& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(preference {} {})", string(node.preference_name, formatter), string(node.goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::PreconditionGoalDescriptorPreference& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::PreconditionGoalDescriptorForall& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(forall ({}) {})", string(node.typed_list_of_variables, formatter), string(node.precondition_goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::PreconditionGoalDescriptorForall& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AssignOperatorAssign&, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "assign");
}

template void write<DefaultFormatter>(const ast::AssignOperatorAssign&, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AssignOperatorScaleUp&, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "scale-up");
}

template void write<DefaultFormatter>(const ast::AssignOperatorScaleUp&, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AssignOperatorScaleDown&, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "scale-down");
}

template void write<DefaultFormatter>(const ast::AssignOperatorScaleDown&, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AssignOperatorIncrease&, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "increase");
}

template void write<DefaultFormatter>(const ast::AssignOperatorIncrease&, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AssignOperatorDecrease&, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "decrease");
}

template void write<DefaultFormatter>(const ast::AssignOperatorDecrease&, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AssignOperator& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::AssignOperator& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Effect& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::Effect& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::NumberAndEffect& node, T formatter, std::ostream& out)
{
    fmt::print(out, "{} {}", string(node.number, formatter), string(node.effect, formatter));
}

template void write<DefaultFormatter>(const ast::NumberAndEffect& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::EffectProductionLiteral& node, T formatter, std::ostream& out)
{
    write(node.literal, formatter, out);
}

template void write<DefaultFormatter>(const ast::EffectProductionLiteral& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::EffectProductionNumeric& node, T formatter, std::ostream& out)
{
    fmt::print(out, "{} {}", string(node.assign_operator, formatter), string(node.function_head, formatter), string(node.function_expression, formatter));
}

template void write<DefaultFormatter>(const ast::EffectProductionNumeric& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::EffectProduction& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::EffectProduction& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::EffectCompositeForall& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(forall ({}) {})", string(node.typed_list_of_variables, formatter), string(node.effect, formatter));
}

template void write<DefaultFormatter>(const ast::EffectCompositeForall& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::EffectCompositeWhen& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(when {} {})", string(node.goal_descriptor, formatter), string(node.effect, formatter));
}

template void write<DefaultFormatter>(const ast::EffectCompositeWhen& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::EffectCompositeOneof& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(oneof {})", fmt::join(strings(node.possibilities, formatter), " "));
}

template void write<DefaultFormatter>(const ast::EffectCompositeOneof& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::EffectCompositeProbabilistic& node, T formatter, std::ostream& out)
{
    fmt::print(
        out,
        "(probabilistic {})",
        fmt::join(node.possibilities | std::views::transform([&](const auto& child_node) { return fmt::format("{}", string(child_node, formatter)); }), " "));
}

template void write<DefaultFormatter>(const ast::EffectCompositeProbabilistic& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::EffectComposite& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::EffectComposite& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ActionSymbol& node, T formatter, std::ostream& out)
{
    write(node.name, formatter, out);
}

template void write<DefaultFormatter>(const ast::ActionSymbol& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ActionBody& node, T formatter, std::ostream& out)
{
    auto indent = std::string(formatter.indent, ' ');
    fmt::print(
        out,
        "{}{}",
        node.precondition_goal_descriptor.has_value() ? indent + "(:precondition " + string(node.precondition_goal_descriptor.value(), formatter) + ")\n" : "",
        node.effect.has_value() ? indent + "(:effect " + string(node.effect.value(), formatter) + ")\n" : "");
}

template void write<DefaultFormatter>(const ast::ActionBody& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Action& node, T formatter, std::ostream& out)
{
    // Header
    fmt::print(out, "(:action {}\n", string(node.action_symbol, formatter));

    formatter.indent += formatter.add_indent;
    auto indent = std::string(formatter.indent, ' ');

    // Parameters
    fmt::print(out, "{}:parameters ({})\n", indent, string(node.typed_list_of_variables, formatter));

    // Body
    fmt::print(out, "{}", string(node.action_body, formatter));

    formatter.indent -= formatter.add_indent;
    indent = std::string(formatter.indent, ' ');

    // End action
    fmt::print(out, "{})", indent);
}

template void write<DefaultFormatter>(const ast::Action& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Axiom& node, T formatter, std::ostream& out)
{
    // Header line: "(:derived <pred> <params...\n"
    fmt::print(out, "(:derived ({} {})\n", string(node.atomic_formula_skeleton, formatter), string(node.goal_descriptor, formatter));

    formatter.indent += formatter.add_indent;
    auto indent = std::string(formatter.indent, ' ');

    // Conditions
    fmt::print(out, "{}{}\n", indent, string(node.goal_descriptor, formatter));

    formatter.indent -= formatter.add_indent;
    indent = std::string(formatter.indent, ' ');

    // End axiom
    fmt::print(out, "{})", indent);
}

template void write<DefaultFormatter>(const ast::Axiom& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::DomainName& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(domain {})", string(node.name, formatter));
}

template void write<DefaultFormatter>(const ast::DomainName& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Requirements& node, T formatter, std::ostream& out)
{
    fmt::print(
        out,
        "(:requirements {})",
        fmt::join(node.requirements | std::views::transform([&](const auto& child_node) { return fmt::format("{}", string(child_node, formatter)); }), " "));
}

template void write<DefaultFormatter>(const ast::Requirements& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Types& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(:types {})", string(node.typed_list_of_names, formatter));
}

template void write<DefaultFormatter>(const ast::Types& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Constants& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(:constants {})", string(node.typed_list_of_names, formatter));
}

template void write<DefaultFormatter>(const ast::Constants& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Predicates& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(:predicates {})", fmt::join(strings(node.atomic_formula_skeletons, formatter), " "));
}

template void write<DefaultFormatter>(const ast::Predicates& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Functions& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(:functions {})", string(node.function_types_list_of_atomic_function_skeletons, formatter));
}

template void write<DefaultFormatter>(const ast::Functions& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Constraints& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(:constraints {})", string(node.constraint_goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::Constraints& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Structure& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::Structure& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Domain& node, T formatter, std::ostream& out)
{
    auto indent = std::string(formatter.indent, ' ');

    // Header
    fmt::print(out, "{}(define (domain {})\n", indent, string(node.domain_name, formatter));

    formatter.indent += formatter.add_indent;
    indent = std::string(formatter.indent, ' ');

    // Requirements
    if (node.requirements.has_value())
        fmt::print(out, "{}{}\n", indent, string(node.requirements.value(), formatter));

    // Types
    if (node.types.has_value())
        fmt::print(out, "{}{}\n", indent, string(node.types.value(), formatter));

    // Constants
    if (node.constants.has_value())
        fmt::print(out, "{}{}\n", indent, string(node.constants.value(), formatter));

    // Predicates
    if (node.predicates.has_value())
        fmt::print(out, "{}{}\n", indent, string(node.predicates.value(), formatter));

    // Functions
    if (node.functions.has_value())
        fmt::print(out, "{}{}\n", indent, string(node.functions.value(), formatter));

    for (const auto& structure : node.structures)
        fmt::print(out, "{}{}\n", indent, string(structure, formatter));

    formatter.indent -= formatter.add_indent;
    indent = std::string(formatter.indent, ' ');

    // End domain
    fmt::print(out, "{})", indent);
}

template void write<DefaultFormatter>(const ast::Domain& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::GroundFunction& node, T formatter, std::ostream& out)
{
    if (node.names.size() > 0)
        fmt::print(out, "({} {})", string(node.function_symbol, formatter), fmt::join(strings(node.names, formatter), " "));
    else
        fmt::print(out, "({})", string(node.function_symbol, formatter));
}

template void write<DefaultFormatter>(const ast::GroundFunction& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AtomicFormulaOfNamesPredicate& node, T formatter, std::ostream& out)
{
    fmt::print(out, "({} {})", string(node.predicate, formatter), fmt::join(strings(node.names, formatter), " "));
}

template void write<DefaultFormatter>(const ast::AtomicFormulaOfNamesPredicate& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AtomicFormulaOfNamesEquality& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(= {} {})", string(node.name_left, formatter), string(node.name_right, formatter));
}

template void write<DefaultFormatter>(const ast::AtomicFormulaOfNamesEquality& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::AtomicFormulaOfNames& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::AtomicFormulaOfNames& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::GroundAtom& node, T formatter, std::ostream& out)
{
    write(node.atomic_formula_of_names, formatter, out);
}

template void write<DefaultFormatter>(const ast::GroundAtom& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::NegatedGroundAtom& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(not {})", string(node.atomic_formula_of_names, formatter));
}

template void write<DefaultFormatter>(const ast::NegatedGroundAtom& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::GroundLiteral& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::GroundLiteral& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::InitialElementLiteral& node, T formatter, std::ostream& out)
{
    write(node.literal, formatter, out);
}

template void write<DefaultFormatter>(const ast::InitialElementLiteral& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::InitialElementTimedLiteral& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(at {} {})", string(node.number, formatter), string(node.literal, formatter));
}

template void write<DefaultFormatter>(const ast::InitialElementTimedLiteral& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::InitialElementFunctionValue& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(= {} {})", string(node.ground_function, formatter), string(node.number, formatter));
}

template void write<DefaultFormatter>(const ast::InitialElementFunctionValue& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::InitialElement& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::InitialElement& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::MetricFunctionExpression& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::MetricFunctionExpression& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::MetricFunctionExpressionNumber& node, T formatter, std::ostream& out)
{
    write(node.number, formatter, out);
}

template void write<DefaultFormatter>(const ast::MetricFunctionExpressionNumber& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::MetricFunctionExpressionBinaryOperator& node, T formatter, std::ostream& out)
{
    fmt::print(out,
               "({} {} {})",
               string(node.binary_operator, formatter),
               string(node.metric_left_function_expression, formatter),
               string(node.metric_right_function_expression, formatter));
}

template void write<DefaultFormatter>(const ast::MetricFunctionExpressionBinaryOperator& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::MetricFunctionExpressionMultiOperator& node, T formatter, std::ostream& out)
{
    fmt::print(out,
               "({} {} {})",
               string(node.multi_operator, formatter),
               string(node.metric_function_expression_first, formatter),
               fmt::join(strings(node.metric_function_expression_remaining, formatter), " "));
}

template void write<DefaultFormatter>(const ast::MetricFunctionExpressionMultiOperator& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::MetricFunctionExpressionMinus& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(- {})", string(node.metric_function_expression, formatter));
}

template void write<DefaultFormatter>(const ast::MetricFunctionExpressionMinus& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::MetricFunctionExpressionBasicFunctionTerm& node, T formatter, std::ostream& out)
{
    write(node.ground_function, formatter, out);
}

template void write<DefaultFormatter>(const ast::MetricFunctionExpressionBasicFunctionTerm& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::MetricFunctionExpressionTotalTime& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "total-time");
}

template void write<DefaultFormatter>(const ast::MetricFunctionExpressionTotalTime& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::MetricFunctionExpressionPreferences& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(is-violated {})", string(node.preference_name, formatter));
}

template void write<DefaultFormatter>(const ast::MetricFunctionExpressionPreferences& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::OptimizationMinimize& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "minimize");
}

template void write<DefaultFormatter>(const ast::OptimizationMinimize&, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::OptimizationMaximize& /*node*/, T /*formatter*/, std::ostream& out)
{
    fmt::print(out, "maximize");
}

template void write<DefaultFormatter>(const ast::OptimizationMaximize&, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Optimization& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::Optimization& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::MetricSpecificationTotalCost& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(:metric {} {})", string(node.optimization_minimize, formatter), string(node.function_symbol_total_cost, formatter));
}

template void write<DefaultFormatter>(const ast::MetricSpecificationTotalCost& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::MetricSpecificationGeneral& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(:metric {} {})", string(node.optimization, formatter), string(node.metric_function_expression, formatter));
}

template void write<DefaultFormatter>(const ast::MetricSpecificationGeneral& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::PreferenceConstraintGoalDescriptor& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::PreferenceConstraintGoalDescriptor& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::PreferenceConstraintGoalDescriptorAnd& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(and {})", fmt::join(string(node.preference_constraint_goal_descriptors, formatter), " "));
}

template void write<DefaultFormatter>(const ast::PreferenceConstraintGoalDescriptorAnd& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::PreferenceConstraintGoalDescriptorForall& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(forall ({}) {})", string(node.typed_list_of_variables, formatter), string(node.preference_constraint_goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::PreferenceConstraintGoalDescriptorForall& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::PreferenceConstraintGoalDescriptorPreference& node, T formatter, std::ostream& out)
{
    fmt::print(out,
               "(preference{} {})",
               node.preference_name.has_value() ? " " + string(node.preference_name.value(), formatter) : "",
               string(node.constraint_goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::PreferenceConstraintGoalDescriptorPreference& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::PreferenceConstraintGoalDescriptorSimple& node, T formatter, std::ostream& out)
{
    write(node.constraint_goal_descriptor, formatter, out);
}

template void write<DefaultFormatter>(const ast::PreferenceConstraintGoalDescriptorSimple& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ProblemName& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(problem {})", string(node.name, formatter));
}

template void write<DefaultFormatter>(const ast::ProblemName& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ProblemDomainName& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(:domain {})", string(node.name, formatter));
}

template void write<DefaultFormatter>(const ast::ProblemDomainName& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Objects& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(:objects {})", string(node.typed_list_of_names, formatter));
}

template void write<DefaultFormatter>(const ast::Objects& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Initial& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(:init {})", fmt::join(strings(node.initial_elements, formatter), " "));
}

template void write<DefaultFormatter>(const ast::Initial& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Goal& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(:goal {})", string(node.precondition_goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::Goal& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::ProblemConstraints& node, T formatter, std::ostream& out)
{
    fmt::print(out, "(:constraints {})", string(node.preference_constraint_goal_descriptor, formatter));
}

template void write<DefaultFormatter>(const ast::ProblemConstraints& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::MetricSpecification& node, T formatter, std::ostream& out)
{
    boost::apply_visitor(NodeVisitorPrinter(formatter, out), node);
}

template void write<DefaultFormatter>(const ast::MetricSpecification& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ast::Problem& node, T formatter, std::ostream& out)
{
    auto indent = std::string(formatter.indent, ' ');

    // Header
    fmt::print(out, "{}(define (problem {})\n", indent, string(node.problem_name, formatter));

    formatter.indent += formatter.add_indent;
    indent = std::string(formatter.indent, ' ');

    // Domain
    fmt::print(out, "{}(:domain {})\n", indent, string(node.domain_name, formatter));

    // Requirements
    if (node.requirements.has_value())
        fmt::print(out, "{}{}\n", indent, string(node.requirements.value(), formatter));

    // Objects
    if (node.objects.has_value())
        fmt::print(out, "{}{}\n", indent, string(node.objects.value(), formatter));

    // Initial
    if (node.initial.has_value())
        fmt::print(out, "{}{}\n", indent, string(node.initial.value(), formatter));

    // Goal
    if (node.goal.has_value())
        fmt::print(out, "{}{}\n", indent, string(node.goal.value(), formatter));

    // Constraints
    if (node.constraints.has_value())
        fmt::print(out, "{}{}\n", indent, string(node.constraints.value(), formatter));

    // Metric
    if (node.metric_specification.has_value())
        fmt::print(out, "{}{}\n", indent, string(node.metric_specification.value(), formatter));

    formatter.indent -= formatter.add_indent;
    indent = std::string(formatter.indent, ' ');

    // End problem
    fmt::print(out, "{})", indent);
}

template void write<DefaultFormatter>(const ast::Problem& node, DefaultFormatter formatter, std::ostream& out);

template<Formatter T>
template<typename Node>
void NodeVisitorPrinter<T>::operator()(const Node& node) const
{
    write(node, formatter, out);
}

template<Formatter F, std::ranges::input_range Range>
void write(const Range& range, F formatter, std::ostream& out)
{
    fmt::print(out, "{}", fmt::join(strings(range, formatter), " "));
}

}
