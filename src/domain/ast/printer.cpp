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

#include "loki/domain/ast/printer.hpp"

#include "loki/problem/ast/printer.hpp"

#include <sstream>
#include <vector>

using namespace std;

namespace loki
{
// Printer for std::vector
template<typename T>
inline std::string parse_text(const std::vector<T>& nodes, const FormattingOptions& options);

// Printer for boost::variant
class NodeVisitorPrinter : public boost::static_visitor<std::string>
{
private:
    const FormattingOptions* options;

public:
    NodeVisitorPrinter(const FormattingOptions& options) : options(&options) {}

    template<typename Node>
    std::string operator()(const Node& node) const
    {
        return parse_text(node, *options);
    }
};

string parse_text(const domain::ast::Name& node, const FormattingOptions&) { return node.characters; }

string parse_text(const domain::ast::Variable& node, const FormattingOptions&) { return node.characters; }

std::string parse_text(const domain::ast::FunctionSymbol& node, const FormattingOptions& options) { return parse_text(node.name, options); }

string parse_text(const domain::ast::Term& node, const FormattingOptions& options) { return boost::apply_visitor(NodeVisitorPrinter(options), node); }

std::string parse_text(const domain::ast::Predicate& node, const FormattingOptions& options) { return parse_text(node.name, options); }

string parse_text(const domain::ast::Number& node, const FormattingOptions&)
{
    stringstream ss;
    ss << node.value;
    return ss.str();
}

std::string parse_text(const domain::ast::RequirementStrips&, const FormattingOptions&) { return ":strips"; }
std::string parse_text(const domain::ast::RequirementTyping&, const FormattingOptions&) { return ":typing"; }
std::string parse_text(const domain::ast::RequirementNegativePreconditions&, const FormattingOptions&) { return ":negative-preconditions"; }
std::string parse_text(const domain::ast::RequirementDisjunctivePreconditions&, const FormattingOptions&) { return ":disjunctive-preconditions"; }
std::string parse_text(const domain::ast::RequirementEquality&, const FormattingOptions&) { return ":equality"; }
std::string parse_text(const domain::ast::RequirementExistentialPreconditions&, const FormattingOptions&) { return ":existential-preconditions"; }
std::string parse_text(const domain::ast::RequirementUniversalPreconditions&, const FormattingOptions&) { return ":universal-preconditions"; }
std::string parse_text(const domain::ast::RequirementQuantifiedPreconditions&, const FormattingOptions&) { return ":quantified-preconditions"; }
std::string parse_text(const domain::ast::RequirementConditionalEffects&, const FormattingOptions&) { return ":conditional-effects"; }
std::string parse_text(const domain::ast::RequirementFluents&, const FormattingOptions&) { return ":fluents"; }
std::string parse_text(const domain::ast::RequirementObjectFluents&, const FormattingOptions&) { return ":object-fluents"; }
std::string parse_text(const domain::ast::RequirementNumericFluents&, const FormattingOptions&) { return ":numeric-fluents"; }
std::string parse_text(const domain::ast::RequirementAdl&, const FormattingOptions&) { return ":adl"; }
std::string parse_text(const domain::ast::RequirementDurativeActions&, const FormattingOptions&) { return ":durative-actions"; }
std::string parse_text(const domain::ast::RequirementDerivedPredicates&, const FormattingOptions&) { return ":derived-predicates"; }
std::string parse_text(const domain::ast::RequirementTimedInitialLiterals&, const FormattingOptions&) { return ":timed-initial-literals"; }
std::string parse_text(const domain::ast::RequirementPreferences&, const FormattingOptions&) { return ":preferences"; }
std::string parse_text(const domain::ast::RequirementConstraints&, const FormattingOptions&) { return ":constraints"; }
std::string parse_text(const domain::ast::RequirementActionCosts&, const FormattingOptions&) { return ":action-costs"; }

std::string parse_text(const domain::ast::Requirement& node, const FormattingOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

string parse_text(const domain::ast::Type& node, const FormattingOptions& options) { return boost::apply_visitor(NodeVisitorPrinter(options), node); }

string parse_text(const domain::ast::TypeObject& /*node*/, const FormattingOptions& /*options*/) { return "object"; }

string parse_text(const domain::ast::TypeNumber& /*node*/, const FormattingOptions& /*options*/) { return "number"; }

string parse_text(const domain::ast::TypeEither& node, const FormattingOptions& options)
{
    stringstream ss;
    ss << "(either ";
    for (size_t i = 0; i < node.types.size(); ++i)
    {
        if (i != 0)
            ss << " ";
        ss << parse_text(node.types[i], options);
    }
    ss << ")";
    return ss.str();
}

string parse_text(const domain::ast::TypedListOfNamesRecursively& node, const FormattingOptions& options)
{
    stringstream ss;
    for (size_t i = 0; i < node.names.size(); ++i)
    {
        if (i != 0)
            ss << " ";
        ss << parse_text(node.names[i], options);
    }
    ss << " - " << parse_text(node.type, options);

    // lookahead
    auto nested_options = FormattingOptions { options.indent + options.add_indent, options.add_indent };
    auto nested_text = parse_text(node.typed_list_of_names, nested_options);
    if (nested_text.size() > 0)
    {
        ss << "\n";
        ss << string(nested_options.indent, ' ') << nested_text;
    }
    return ss.str();
}

string parse_text(const domain::ast::TypedListOfNames& node, const FormattingOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const domain::ast::TypedListOfVariablesRecursively& node, const FormattingOptions& options)
{
    stringstream ss;
    for (size_t i = 0; i < node.variables.size(); ++i)
    {
        if (i != 0)
            ss << " ";
        ss << parse_text(node.variables[i]);
    }
    ss << " - " << parse_text(node.type, options);

    // lookahead
    auto nested_options = FormattingOptions { options.indent + options.add_indent, options.add_indent };
    auto nested_text = parse_text(node.typed_list_of_variables, options);
    if (nested_text.size() > 0)
    {
        ss << "\n";
        ss << string(nested_options.indent, ' ') << nested_text;
    }
    return ss.str();
}

std::string parse_text(const domain::ast::TypedListOfVariables& node, const FormattingOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const domain::ast::AtomicFormulaSkeleton& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(" << parse_text(node.predicate, options) << " " << parse_text(node.typed_list_of_variables, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::AtomicFunctionSkeletonTotalCost& node, const FormattingOptions& /*options*/)
{
    std::stringstream ss;
    ss << "(" << parse_text(node.function_symbol) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::AtomicFunctionSkeletonGeneral& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(" << parse_text(node.function_symbol, options) << " " << parse_text(node.arguments, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::AtomicFunctionSkeleton& node, const FormattingOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const domain::ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& node, const FormattingOptions& options)
{
    stringstream ss;
    for (size_t i = 0; i < node.atomic_function_skeletons.size(); ++i)
    {
        if (i != 0)
            ss << " ";
        ss << parse_text(node.atomic_function_skeletons[i], options);
    }
    ss << " - " << parse_text(node.function_type, options);

    // lookahead
    auto nested_options = FormattingOptions { options.indent + options.add_indent, options.add_indent };
    if (node.function_typed_list_of_atomic_function_skeletons.has_value())
    {
        auto nested_text = parse_text(node.function_typed_list_of_atomic_function_skeletons.value(), options);
        if (nested_text.size() > 0)
        {
            ss << "\n";
            ss << string(nested_options.indent, ' ') << nested_text;
        }
    }
    return ss.str();
}

std::string parse_text(const domain::ast::FunctionTypedListOfAtomicFunctionSkeletons& node, const FormattingOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const domain::ast::AtomicFormulaOfTermsPredicate& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(" << parse_text(node.predicate, options) << " " << parse_text(node.terms, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::AtomicFormulaOfTermsEquality& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "("
       << "= " << parse_text(node.term_left, options) << " " << parse_text(node.term_right, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::AtomicFormulaOfTerms& node, const FormattingOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const domain::ast::Atom& node, const FormattingOptions& options) { return parse_text(node.atomic_formula_of_terms, options); }

std::string parse_text(const domain::ast::NegatedAtom& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(not " << parse_text(node.atomic_formula_of_terms, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::Literal& node, const FormattingOptions& options) { return boost::apply_visitor(NodeVisitorPrinter(options), node); }

std::string parse_text(const domain::ast::MultiOperatorMul&, const FormattingOptions&) { return "*"; }
std::string parse_text(const domain::ast::MultiOperatorPlus&, const FormattingOptions&) { return "+"; }
std::string parse_text(const domain::ast::MultiOperator& node, const FormattingOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const domain::ast::BinaryOperatorMinus&, const FormattingOptions&) { return "-"; }
std::string parse_text(const domain::ast::BinaryOperatorDiv&, const FormattingOptions&) { return "/"; }
std::string parse_text(const domain::ast::BinaryOperator& node, const FormattingOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const domain::ast::BinaryComparatorGreater&, const FormattingOptions&) { return ">"; }
std::string parse_text(const domain::ast::BinaryComparatorLess&, const FormattingOptions&) { return "<"; }
std::string parse_text(const domain::ast::BinaryComparatorEqual&, const FormattingOptions&) { return "="; }
std::string parse_text(const domain::ast::BinaryComparatorGreaterEqual&, const FormattingOptions&) { return ">="; }
std::string parse_text(const domain::ast::BinaryComparatorLessEqual&, const FormattingOptions&) { return "<="; }
std::string parse_text(const domain::ast::BinaryComparator& node, const FormattingOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const domain::ast::FunctionHead& node, const FormattingOptions& options)
{
    std::stringstream ss;
    if (node.terms.size() > 0)
    {
        ss << "(" << parse_text(node.function_symbol, options) << " " << parse_text(node.terms, options) << ")";
    }
    else
    {
        ss << parse_text(node.function_symbol, options);
    }
    return ss.str();
}

std::string parse_text(const domain::ast::FunctionExpression& node, const FormattingOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const domain::ast::FunctionExpressionNumber& node, const FormattingOptions& options) { return parse_text(node.number, options); }

std::string parse_text(const domain::ast::FunctionExpressionBinaryOp& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(" << parse_text(node.binary_operator, options) << " " << parse_text(node.function_expression_left, options) << " "
       << parse_text(node.function_expression_right, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::FunctionExpressionMinus& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(- " << parse_text(node.function_expression, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::FunctionExpressionHead& node, const FormattingOptions& options) { return parse_text(node.function_head, options); }

std::string parse_text(const domain::ast::GoalDescriptor& node, const FormattingOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const domain::ast::GoalDescriptorAtom& node, const FormattingOptions& options) { return parse_text(node.atom, options); }

std::string parse_text(const domain::ast::GoalDescriptorLiteral& node, const FormattingOptions& options) { return parse_text(node.literal, options); }

std::string parse_text(const domain::ast::GoalDescriptorAnd& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(and " << parse_text(node.goal_descriptors, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::GoalDescriptorOr& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(or " << parse_text(node.goal_descriptors, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::GoalDescriptorNot& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(not " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::GoalDescriptorImply& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(imply " << parse_text(node.goal_descriptor_left, options) << " " << parse_text(node.goal_descriptor_right, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::GoalDescriptorExists& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(exists " << parse_text(node.typed_list_of_variables, options) << " " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::GoalDescriptorForall& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(forall " << parse_text(node.typed_list_of_variables, options) << " " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::GoalDescriptorFunctionComparison& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(" << parse_text(node.binary_comparator, options) << " " << parse_text(node.function_expression_left, options) << " "
       << parse_text(node.function_expression_right, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::ConstraintGoalDescriptor& node, const FormattingOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const domain::ast::ConstraintGoalDescriptorAnd& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(and " << parse_text(node.constraint_goal_descriptors, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::ConstraintGoalDescriptorForall& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(forall " << parse_text(node.typed_list_of_variables, options) << " " << parse_text(node.constraint_goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::ConstraintGoalDescriptorAtEnd& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(at end " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::ConstraintGoalDescriptorAlways& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(always " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::ConstraintGoalDescriptorSometime& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(sometime " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::ConstraintGoalDescriptorWithin& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(within " << parse_text(node.number, options) << " " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::ConstraintGoalDescriptorAtMostOnce& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(at-most-once " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::ConstraintGoalDescriptorSometimeAfter& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(sometime-after " << parse_text(node.goal_descriptor_left, options) << " " << parse_text(node.goal_descriptor_right, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::ConstraintGoalDescriptorSometimeBefore& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(sometime-before " << parse_text(node.goal_descriptor_left, options) << " " << parse_text(node.goal_descriptor_right, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::ConstraintGoalDescriptorAlwaysWithin& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(always-within " << parse_text(node.number, options) << " " << parse_text(node.goal_descriptor_left, options) << " "
       << parse_text(node.goal_descriptor_right, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::ConstraintGoalDescriptorHoldDuring& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(hold-during " << parse_text(node.number_left, options) << " " << parse_text(node.number_right, options) << " "
       << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::ConstraintGoalDescriptorHoldAfter& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(hold-after " << parse_text(node.number, options) << " " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::PreferenceName& node, const FormattingOptions& options) { return parse_text(node.name, options); }

std::string parse_text(const domain::ast::PreconditionGoalDescriptor& node, const FormattingOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const domain::ast::PreconditionGoalDescriptorSimple& node, const FormattingOptions& options)
{
    return parse_text(node.goal_descriptor, options);
}

std::string parse_text(const domain::ast::PreconditionGoalDescriptorAnd& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(and " << parse_text(node.precondition_goal_descriptors, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::PreconditionGoalDescriptorPreference& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(preference " << parse_text(node.preference_name, options) << " " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::PreconditionGoalDescriptorForall& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(forall " << parse_text(node.typed_list_of_variables, options) << " " << parse_text(node.precondition_goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::AssignOperatorAssign&, const FormattingOptions&) { return "assign"; }
std::string parse_text(const domain::ast::AssignOperatorScaleUp&, const FormattingOptions&) { return "scale-up"; }
std::string parse_text(const domain::ast::AssignOperatorScaleDown&, const FormattingOptions&) { return "scale-down"; }
std::string parse_text(const domain::ast::AssignOperatorIncrease&, const FormattingOptions&) { return "increase"; }
std::string parse_text(const domain::ast::AssignOperatorDecrease&, const FormattingOptions&) { return "decrease"; }
std::string parse_text(const domain::ast::AssignOperator& node, const FormattingOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const domain::ast::Effect& node, const FormattingOptions& options) { return boost::apply_visitor(NodeVisitorPrinter(options), node); }

std::string parse_text(const domain::ast::EffectProductionLiteral& node, const FormattingOptions& options) { return parse_text(node.literal, options); }

std::string parse_text(const domain::ast::EffectProductionNumericFluentTotalCost& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(" << parse_text(node.assign_operator_increase, options) << " " << parse_text(node.function_symbol_total_cost, options) << " "
       << parse_text(node.numeric_term, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::EffectProductionNumericFluentGeneral& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(" << parse_text(node.assign_operator, options) << " " << parse_text(node.function_head, options) << " "
       << parse_text(node.function_expression, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::EffectProduction& node, const FormattingOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const domain::ast::EffectConditionalForall& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(forall " << parse_text(node.typed_list_of_variables, options) << " " << parse_text(node.effect, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::EffectConditionalWhen& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(when " << parse_text(node.goal_descriptor, options) << " " << parse_text(node.effect, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::EffectConditional& node, const FormattingOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const domain::ast::ActionSymbol& node, const FormattingOptions& options) { return parse_text(node.name, options); }

std::string parse_text(const domain::ast::ActionBody& node, const FormattingOptions& options)
{
    std::stringstream ss;
    if (node.precondition_goal_descriptor.has_value())
    {
        ss << std::string(options.indent, ' ') << ":precondition " << parse_text(node.precondition_goal_descriptor.value(), options) << "\n";
    }
    if (node.effect.has_value())
    {
        ss << std::string(options.indent, ' ') << ":effect " << parse_text(node.effect.value(), options);
    }
    return ss.str();
}

std::string parse_text(const domain::ast::Action& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << std::string(options.indent, ' ') << "(action " << parse_text(node.action_symbol, options) << "\n";
    FormattingOptions nested_options { options.indent + options.add_indent, options.add_indent };
    ss << std::string(nested_options.indent, ' ') << ":parameters (" << parse_text(node.typed_list_of_variables, nested_options) << ")\n"
       << parse_text(node.action_body, nested_options) << "\n";
    ss << std::string(options.indent, ' ') << ")";

    return ss.str();
}

std::string parse_text(const domain::ast::DerivedPredicate& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << std::string(options.indent, ' ') << "(:derived " << parse_text(node.typed_list_of_variables, options) << " "
       << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::DomainName& node, const FormattingOptions& options)
{
    stringstream ss;
    ss << "(domain " << parse_text(node.name, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::Requirements& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(:requirements ";
    for (size_t i = 0; i < node.requirements.size(); ++i)
    {
        if (i != 0)
            ss << " ";
        ss << parse_text(node.requirements[i], options);
    }
    ss << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::Types& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(:types " << parse_text(node.typed_list_of_names, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::Constants& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(:constants " << parse_text(node.typed_list_of_names, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::Predicates& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(:predicates " << parse_text(node.atomic_formula_skeletons, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::Functions& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(:functions " << parse_text(node.function_types_list_of_atomic_function_skeletons, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::Constraints& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << "(:constraints " << parse_text(node.constraint_goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const domain::ast::Structure& node, const FormattingOptions& options) { return boost::apply_visitor(NodeVisitorPrinter(options), node); }

std::string parse_text(const domain::ast::Domain& node, const FormattingOptions& options)
{
    std::stringstream ss;
    ss << string(options.indent, ' ') << "(define " << parse_text(node.domain_name, options) << "\n";
    auto nested_options = FormattingOptions { options.indent + options.add_indent, options.add_indent };
    if (node.requirements.has_value())
    {
        ss << string(nested_options.indent, ' ') << parse_text(node.requirements.value(), nested_options) << "\n";
    }
    if (node.types.has_value())
    {
        ss << string(nested_options.indent, ' ') << parse_text(node.types.value(), nested_options) << "\n";
    }
    if (node.constants.has_value())
    {
        ss << string(nested_options.indent, ' ') << parse_text(node.constants.value(), nested_options) << "\n";
    }
    if (node.predicates.has_value())
    {
        ss << string(nested_options.indent, ' ') << parse_text(node.predicates.value(), nested_options) << "\n";
    }
    if (node.functions.has_value())
    {
        ss << string(nested_options.indent, ' ') << parse_text(node.functions.value(), nested_options) << "\n";
    }
    if (node.constraints.has_value())
    {
        ss << string(nested_options.indent, ' ') << parse_text(node.constraints.value(), nested_options) << "\n";
    }
    for (size_t i = 0; i < node.structures.size(); ++i)
    {
        ss << string(nested_options.indent, ' ') << parse_text(node.structures[i], nested_options) << "\n";
    }
    ss << std::string(options.indent, ' ') << ")";
    return ss.str();
}

template<typename T>
inline std::string parse_text(const std::vector<T>& nodes, const FormattingOptions& options)
{
    std::stringstream ss;
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        if (i != 0)
            ss << " ";
        ss << parse_text(nodes[i], options);
    }
    return ss.str();
}
}
