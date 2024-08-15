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

#include <sstream>
#include <vector>

using namespace std;

namespace loki
{
// Printer for std::vector
template<typename T>
inline std::string parse_text(const std::vector<T>& nodes, const DefaultFormatterOptions& options);

// Printer for boost::variant
class NodeVisitorPrinter : public boost::static_visitor<std::string>
{
private:
    const DefaultFormatterOptions* options;

public:
    NodeVisitorPrinter(const DefaultFormatterOptions& options) : options(&options) {}

    template<typename Node>
    std::string operator()(const Node& node) const
    {
        return parse_text(node, *options);
    }
};

string parse_text(const ast::Name& node, const DefaultFormatterOptions&) { return node.characters; }

string parse_text(const ast::Variable& node, const DefaultFormatterOptions&) { return node.characters; }

std::string parse_text(const ast::FunctionSymbol& node, const DefaultFormatterOptions& options) { return parse_text(node.name, options); }

string parse_text(const ast::Term& node, const DefaultFormatterOptions& options) { return boost::apply_visitor(NodeVisitorPrinter(options), node); }

std::string parse_text(const ast::Predicate& node, const DefaultFormatterOptions& options) { return parse_text(node.name, options); }

string parse_text(const ast::Number& node, const DefaultFormatterOptions&)
{
    stringstream ss;
    ss << node.value;
    return ss.str();
}

std::string parse_text(const ast::RequirementStrips&, const DefaultFormatterOptions&) { return ":strips"; }
std::string parse_text(const ast::RequirementTyping&, const DefaultFormatterOptions&) { return ":typing"; }
std::string parse_text(const ast::RequirementNegativePreconditions&, const DefaultFormatterOptions&) { return ":negative-preconditions"; }
std::string parse_text(const ast::RequirementDisjunctivePreconditions&, const DefaultFormatterOptions&) { return ":disjunctive-preconditions"; }
std::string parse_text(const ast::RequirementEquality&, const DefaultFormatterOptions&) { return ":equality"; }
std::string parse_text(const ast::RequirementExistentialPreconditions&, const DefaultFormatterOptions&) { return ":existential-preconditions"; }
std::string parse_text(const ast::RequirementUniversalPreconditions&, const DefaultFormatterOptions&) { return ":universal-preconditions"; }
std::string parse_text(const ast::RequirementQuantifiedPreconditions&, const DefaultFormatterOptions&) { return ":quantified-preconditions"; }
std::string parse_text(const ast::RequirementConditionalEffects&, const DefaultFormatterOptions&) { return ":conditional-effects"; }
std::string parse_text(const ast::RequirementFluents&, const DefaultFormatterOptions&) { return ":fluents"; }
std::string parse_text(const ast::RequirementObjectFluents&, const DefaultFormatterOptions&) { return ":object-fluents"; }
std::string parse_text(const ast::RequirementNumericFluents&, const DefaultFormatterOptions&) { return ":numeric-fluents"; }
std::string parse_text(const ast::RequirementAdl&, const DefaultFormatterOptions&) { return ":adl"; }
std::string parse_text(const ast::RequirementDurativeActions&, const DefaultFormatterOptions&) { return ":durative-actions"; }
std::string parse_text(const ast::RequirementDerivedPredicates&, const DefaultFormatterOptions&) { return ":derived-predicates"; }
std::string parse_text(const ast::RequirementTimedInitialLiterals&, const DefaultFormatterOptions&) { return ":timed-initial-literals"; }
std::string parse_text(const ast::RequirementPreferences&, const DefaultFormatterOptions&) { return ":preferences"; }
std::string parse_text(const ast::RequirementConstraints&, const DefaultFormatterOptions&) { return ":constraints"; }
std::string parse_text(const ast::RequirementActionCosts&, const DefaultFormatterOptions&) { return ":action-costs"; }
std::string parse_text(const ast::RequirementNonDeterministic&, const DefaultFormatterOptions&) { return ":non-deterministic"; }

std::string parse_text(const ast::Requirement& node, const DefaultFormatterOptions& options) { return boost::apply_visitor(NodeVisitorPrinter(options), node); }

string parse_text(const ast::Type& node, const DefaultFormatterOptions& options) { return boost::apply_visitor(NodeVisitorPrinter(options), node); }

string parse_text(const ast::TypeObject& /*node*/, const DefaultFormatterOptions& /*options*/) { return "object"; }

string parse_text(const ast::TypeNumber& /*node*/, const DefaultFormatterOptions& /*options*/) { return "number"; }

string parse_text(const ast::TypeEither& node, const DefaultFormatterOptions& options)
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

string parse_text(const ast::TypedListOfNamesRecursively& node, const DefaultFormatterOptions& options)
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
    auto nested_options = DefaultFormatterOptions { options.indent + options.add_indent, options.add_indent };
    auto nested_text = parse_text(node.typed_list_of_names, nested_options);
    if (nested_text.size() > 0)
    {
        ss << "\n";
        ss << string(nested_options.indent, ' ') << nested_text;
    }
    return ss.str();
}

string parse_text(const ast::TypedListOfNames& node, const DefaultFormatterOptions& options) { return boost::apply_visitor(NodeVisitorPrinter(options), node); }

std::string parse_text(const ast::TypedListOfVariablesRecursively& node, const DefaultFormatterOptions& options)
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
    auto nested_options = DefaultFormatterOptions { options.indent + options.add_indent, options.add_indent };
    auto nested_text = parse_text(node.typed_list_of_variables, options);
    if (nested_text.size() > 0)
    {
        ss << "\n";
        ss << string(nested_options.indent, ' ') << nested_text;
    }
    return ss.str();
}

std::string parse_text(const ast::TypedListOfVariables& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const ast::AtomicFormulaSkeleton& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(" << parse_text(node.predicate, options) << " " << parse_text(node.typed_list_of_variables, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::AtomicFunctionSkeletonTotalCost& node, const DefaultFormatterOptions& /*options*/)
{
    std::stringstream ss;
    ss << "(" << parse_text(node.function_symbol) << ")";
    return ss.str();
}

std::string parse_text(const ast::AtomicFunctionSkeletonGeneral& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(" << parse_text(node.function_symbol, options) << " " << parse_text(node.arguments, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::AtomicFunctionSkeleton& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& node, const DefaultFormatterOptions& options)
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
    auto nested_options = DefaultFormatterOptions { options.indent + options.add_indent, options.add_indent };
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

std::string parse_text(const ast::FunctionTypedListOfAtomicFunctionSkeletons& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const ast::AtomicFormulaOfTermsPredicate& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(" << parse_text(node.predicate, options) << " " << parse_text(node.terms, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::AtomicFormulaOfTermsEquality& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "("
       << "= " << parse_text(node.term_left, options) << " " << parse_text(node.term_right, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::AtomicFormulaOfTerms& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const ast::Atom& node, const DefaultFormatterOptions& options) { return parse_text(node.atomic_formula_of_terms, options); }

std::string parse_text(const ast::NegatedAtom& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(not " << parse_text(node.atomic_formula_of_terms, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::Literal& node, const DefaultFormatterOptions& options) { return boost::apply_visitor(NodeVisitorPrinter(options), node); }

std::string parse_text(const ast::MultiOperatorMul&, const DefaultFormatterOptions&) { return "*"; }
std::string parse_text(const ast::MultiOperatorPlus&, const DefaultFormatterOptions&) { return "+"; }
std::string parse_text(const ast::MultiOperator& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const ast::BinaryOperatorMinus&, const DefaultFormatterOptions&) { return "-"; }
std::string parse_text(const ast::BinaryOperatorDiv&, const DefaultFormatterOptions&) { return "/"; }
std::string parse_text(const ast::BinaryOperator& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const ast::BinaryComparatorGreater&, const DefaultFormatterOptions&) { return ">"; }
std::string parse_text(const ast::BinaryComparatorLess&, const DefaultFormatterOptions&) { return "<"; }
std::string parse_text(const ast::BinaryComparatorEqual&, const DefaultFormatterOptions&) { return "="; }
std::string parse_text(const ast::BinaryComparatorGreaterEqual&, const DefaultFormatterOptions&) { return ">="; }
std::string parse_text(const ast::BinaryComparatorLessEqual&, const DefaultFormatterOptions&) { return "<="; }
std::string parse_text(const ast::BinaryComparator& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const ast::FunctionHead& node, const DefaultFormatterOptions& options)
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

std::string parse_text(const ast::FunctionExpression& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const ast::FunctionExpressionNumber& node, const DefaultFormatterOptions& options) { return parse_text(node.number, options); }

std::string parse_text(const ast::FunctionExpressionBinaryOp& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(" << parse_text(node.binary_operator, options) << " " << parse_text(node.function_expression_left, options) << " "
       << parse_text(node.function_expression_right, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::FunctionExpressionMinus& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(- " << parse_text(node.function_expression, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::FunctionExpressionHead& node, const DefaultFormatterOptions& options) { return parse_text(node.function_head, options); }

std::string parse_text(const ast::GoalDescriptor& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const ast::GoalDescriptorAtom& node, const DefaultFormatterOptions& options) { return parse_text(node.atom, options); }

std::string parse_text(const ast::GoalDescriptorLiteral& node, const DefaultFormatterOptions& options) { return parse_text(node.literal, options); }

std::string parse_text(const ast::GoalDescriptorAnd& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(and " << parse_text(node.goal_descriptors, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::GoalDescriptorOr& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(or " << parse_text(node.goal_descriptors, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::GoalDescriptorNot& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(not " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::GoalDescriptorImply& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(imply " << parse_text(node.goal_descriptor_left, options) << " " << parse_text(node.goal_descriptor_right, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::GoalDescriptorExists& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(exists " << parse_text(node.typed_list_of_variables, options) << " " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::GoalDescriptorForall& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(forall " << parse_text(node.typed_list_of_variables, options) << " " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::GoalDescriptorFunctionComparison& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(" << parse_text(node.binary_comparator, options) << " " << parse_text(node.function_expression_left, options) << " "
       << parse_text(node.function_expression_right, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::ConstraintGoalDescriptor& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const ast::ConstraintGoalDescriptorAnd& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(and " << parse_text(node.constraint_goal_descriptors, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::ConstraintGoalDescriptorForall& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(forall " << parse_text(node.typed_list_of_variables, options) << " " << parse_text(node.constraint_goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::ConstraintGoalDescriptorAtEnd& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(at end " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::ConstraintGoalDescriptorAlways& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(always " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::ConstraintGoalDescriptorSometime& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(sometime " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::ConstraintGoalDescriptorWithin& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(within " << parse_text(node.number, options) << " " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::ConstraintGoalDescriptorAtMostOnce& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(at-most-once " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::ConstraintGoalDescriptorSometimeAfter& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(sometime-after " << parse_text(node.goal_descriptor_left, options) << " " << parse_text(node.goal_descriptor_right, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::ConstraintGoalDescriptorSometimeBefore& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(sometime-before " << parse_text(node.goal_descriptor_left, options) << " " << parse_text(node.goal_descriptor_right, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::ConstraintGoalDescriptorAlwaysWithin& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(always-within " << parse_text(node.number, options) << " " << parse_text(node.goal_descriptor_left, options) << " "
       << parse_text(node.goal_descriptor_right, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::ConstraintGoalDescriptorHoldDuring& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(hold-during " << parse_text(node.number_left, options) << " " << parse_text(node.number_right, options) << " "
       << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::ConstraintGoalDescriptorHoldAfter& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(hold-after " << parse_text(node.number, options) << " " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::PreferenceName& node, const DefaultFormatterOptions& options) { return parse_text(node.name, options); }

std::string parse_text(const ast::PreconditionGoalDescriptor& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const ast::PreconditionGoalDescriptorSimple& node, const DefaultFormatterOptions& options)
{
    return parse_text(node.goal_descriptor, options);
}

std::string parse_text(const ast::PreconditionGoalDescriptorAnd& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(and " << parse_text(node.precondition_goal_descriptors, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::PreconditionGoalDescriptorPreference& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(preference " << parse_text(node.preference_name, options) << " " << parse_text(node.goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::PreconditionGoalDescriptorForall& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(forall " << parse_text(node.typed_list_of_variables, options) << " " << parse_text(node.precondition_goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::AssignOperatorAssign&, const DefaultFormatterOptions&) { return "assign"; }
std::string parse_text(const ast::AssignOperatorScaleUp&, const DefaultFormatterOptions&) { return "scale-up"; }
std::string parse_text(const ast::AssignOperatorScaleDown&, const DefaultFormatterOptions&) { return "scale-down"; }
std::string parse_text(const ast::AssignOperatorIncrease&, const DefaultFormatterOptions&) { return "increase"; }
std::string parse_text(const ast::AssignOperatorDecrease&, const DefaultFormatterOptions&) { return "decrease"; }
std::string parse_text(const ast::AssignOperator& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const ast::Effect& node, const DefaultFormatterOptions& options) { return boost::apply_visitor(NodeVisitorPrinter(options), node); }

std::string parse_text(const ast::EffectProductionLiteral& node, const DefaultFormatterOptions& options) { return parse_text(node.literal, options); }

std::string parse_text(const ast::EffectProductionNumericFluentTotalCost& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(" << parse_text(node.assign_operator_increase, options) << " " << parse_text(node.function_symbol_total_cost, options) << " "
       << parse_text(node.numeric_term, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::EffectProductionNumericFluentGeneral& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(" << parse_text(node.assign_operator, options) << " " << parse_text(node.function_head, options) << " "
       << parse_text(node.function_expression, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::EffectProduction& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const ast::EffectConditionalForall& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(forall " << parse_text(node.typed_list_of_variables, options) << " " << parse_text(node.effect, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::EffectConditionalWhen& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(when " << parse_text(node.goal_descriptor, options) << " " << parse_text(node.effect, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::EffectConditional& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const ast::EffectNumericFluentTotalCostOrEffect& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

std::string parse_text(const ast::EffectRootDeterministic& node, const DefaultFormatterOptions& options) { return boost::apply_visitor(NodeVisitorPrinter(options), node); }

std::string parse_text(const ast::EffectRootNonDeterministic& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(oneof " << parse_text(node.possibilities, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::EffectRoot& node, const DefaultFormatterOptions& options) { return boost::apply_visitor(NodeVisitorPrinter(options), node); }

std::string parse_text(const ast::ActionSymbol& node, const DefaultFormatterOptions& options) { return parse_text(node.name, options); }

std::string parse_text(const ast::ActionBody& node, const DefaultFormatterOptions& options)
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

std::string parse_text(const ast::Action& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << std::string(options.indent, ' ') << "(action " << parse_text(node.action_symbol, options) << "\n";
    DefaultFormatterOptions nested_options { options.indent + options.add_indent, options.add_indent };
    ss << std::string(nested_options.indent, ' ') << ":parameters (" << parse_text(node.typed_list_of_variables, nested_options) << ")\n"
       << parse_text(node.action_body, nested_options) << "\n";
    ss << std::string(options.indent, ' ') << ")";

    return ss.str();
}

std::string parse_text(const ast::Axiom& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << std::string(options.indent, ' ') << "(:derived " << parse_text(node.atomic_formula_skeleton, options) << "\n";
    DefaultFormatterOptions nested_options { options.indent + options.add_indent, options.add_indent };
    ss << std::string(nested_options.indent, ' ') << parse_text(node.goal_descriptor, options) << ")\n";
    return ss.str();
}

std::string parse_text(const ast::DomainName& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(domain " << parse_text(node.name, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::Requirements& node, const DefaultFormatterOptions& options)
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

std::string parse_text(const ast::Types& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(:types " << parse_text(node.typed_list_of_names, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::Constants& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(:constants " << parse_text(node.typed_list_of_names, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::Predicates& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(:predicates " << parse_text(node.atomic_formula_skeletons, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::Functions& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(:functions " << parse_text(node.function_types_list_of_atomic_function_skeletons, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::Constraints& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(:constraints " << parse_text(node.constraint_goal_descriptor, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::Structure& node, const DefaultFormatterOptions& options) { return boost::apply_visitor(NodeVisitorPrinter(options), node); }

std::string parse_text(const ast::Domain& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << string(options.indent, ' ') << "(define " << parse_text(node.domain_name, options) << "\n";
    auto nested_options = DefaultFormatterOptions { options.indent + options.add_indent, options.add_indent };
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

string parse_text(const ast::BasicFunctionTerm& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    if (node.names.size() > 0)
    {
        ss << "(" << parse_text(node.function_symbol, options) << " " << parse_text(node.names, options) << ")";
    }
    else
    {
        ss << parse_text(node.function_symbol, options);
    }
    return ss.str();
}

string parse_text(const ast::AtomicFormulaOfNamesPredicate& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(" << parse_text(node.predicate, options) << " " << parse_text(node.names, options) << ")";
    return ss.str();
}

string parse_text(const ast::AtomicFormulaOfNamesEquality& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "("
       << "= " << parse_text(node.name_left, options) << " " << parse_text(node.name_right, options) << ")";
    return ss.str();
}

string parse_text(const ast::AtomicFormulaOfNames& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

string parse_text(const ast::GroundAtom& node, const DefaultFormatterOptions& options) { return parse_text(node.atomic_formula_of_names, options); }

string parse_text(const ast::NegatedGroundAtom& node, const DefaultFormatterOptions& options)
{
    std::stringstream ss;
    ss << "(not " << parse_text(node.atomic_formula_of_names, options) << ")";
    return ss.str();
}

string parse_text(const ast::GroundLiteral& node, const DefaultFormatterOptions& options) { return boost::apply_visitor(NodeVisitorPrinter(options), node); }

string parse_text(const ast::InitialElementLiteral& node, const DefaultFormatterOptions& options) { return parse_text(node.literal, options); }

string parse_text(const ast::InitialElementTimedLiterals& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(at " << parse_text(node.number, options) << " " << parse_text(node.literal, options) << ")";
    return ss.str();
}

string parse_text(const ast::InitialElementNumericFluentsTotalCost& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(= " << parse_text(node.function_symbol_total_cost, options) << " " << parse_text(node.number, options) << ")";
    return ss.str();
}

string parse_text(const ast::InitialElementNumericFluentsGeneral& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(= " << parse_text(node.basic_function_term, options) << " " << parse_text(node.number, options) << ")";
    return ss.str();
}

string parse_text(const ast::InitialElement& node, const DefaultFormatterOptions& options) { return boost::apply_visitor(NodeVisitorPrinter(options), node); }

string parse_text(const ast::MetricFunctionExpression& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

string parse_text(const ast::MetricFunctionExpressionNumber& node, const DefaultFormatterOptions& options) { return parse_text(node.number, options); }

string parse_text(const ast::MetricFunctionExpressionBinaryOperator& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(" << parse_text(node.binary_operator, options) << " " << parse_text(node.metric_function_expression_left, options) << " "
       << parse_text(node.metric_function_expression_right, options) << ")";
    return ss.str();
}

string parse_text(const ast::MetricFunctionExpressionMultiOperator& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(" << parse_text(node.multi_operator, options) << " " << parse_text(node.metric_function_expression_first, options) << " "
       << parse_text(node.metric_function_expression_remaining, options) << ")";
    return ss.str();
}

string parse_text(const ast::MetricFunctionExpressionMinus& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(-" << parse_text(node.metric_function_expression, options) << ")";
    return ss.str();
}

string parse_text(const ast::MetricFunctionExpressionBasicFunctionTerm& node, const DefaultFormatterOptions& options)
{
    return parse_text(node.basic_function_term, options);
}

string parse_text(const ast::MetricFunctionExpressionTotalTime&, const DefaultFormatterOptions&) { return "total-time"; }

string parse_text(const ast::MetricFunctionExpressionPreferences& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "("
       << "is-violated " << parse_text(node.preference_name, options) << ")";
    return ss.str();
}

string parse_text(const ast::OptimizationMinimize&, const DefaultFormatterOptions&) { return "minimize"; }
string parse_text(const ast::OptimizationMaximize&, const DefaultFormatterOptions&) { return "maximize"; }
string parse_text(const ast::Optimization& node, const DefaultFormatterOptions& options) { return boost::apply_visitor(NodeVisitorPrinter(options), node); }

std::string parse_text(const ast::MetricSpecificationTotalCost& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(:metric " << parse_text(node.optimization_minimize, options) << " " << parse_text(node.function_symbol_total_cost, options) << ")";
    return ss.str();
}

std::string parse_text(const ast::MetricSpecificationGeneral& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(:metric " << parse_text(node.optimization, options) << " " << parse_text(node.metric_function_expression, options) << ")";
    return ss.str();
}

string parse_text(const ast::PreferenceConstraintGoalDescriptor& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

string parse_text(const ast::PreferenceConstraintGoalDescriptorAnd& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(and " << parse_text(node.preference_constraint_goal_descriptors, options) << ")";
    return ss.str();
}

string parse_text(const ast::PreferenceConstraintGoalDescriptorForall& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(forall " << parse_text(node.typed_list_of_variables, options) << " " << parse_text(node.preference_constraint_goal_descriptor, options) << ")";
    return ss.str();
}

string parse_text(const ast::PreferenceConstraintGoalDescriptorPreference& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(preference ";
    if (node.preference_name.has_value())
    {
        ss << parse_text(node.preference_name.value(), options) << " ";
    }
    ss << parse_text(node.constraint_goal_descriptor, options) << ")";
    return ss.str();
}

string parse_text(const ast::PreferenceConstraintGoalDescriptorSimple& node, const DefaultFormatterOptions& options)
{
    return parse_text(node.constraint_goal_descriptor, options);
}

string parse_text(const ast::ProblemName& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(problem " << parse_text(node.name, options) << ")";
    return ss.str();
}

string parse_text(const ast::ProblemDomainName& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(:domain " << parse_text(node.name, options) << ")";
    return ss.str();
}

string parse_text(const ast::Objects& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(:objects " << parse_text(node.typed_list_of_names, options) << ")";
    return ss.str();
}

string parse_text(const ast::Initial& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(:init " << parse_text(node.initial_elements, options) << ")";
    return ss.str();
}

string parse_text(const ast::Goal& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(:goal " << parse_text(node.precondition_goal_descriptor, options) << ")";
    return ss.str();
}

string parse_text(const ast::ProblemConstraints& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << "(:constraints " << parse_text(node.preference_constraint_goal_descriptor, options) << ")";
    return ss.str();
}

string parse_text(const ast::MetricSpecification& node, const DefaultFormatterOptions& options)
{
    return boost::apply_visitor(NodeVisitorPrinter(options), node);
}

string parse_text(const ast::Problem& node, const DefaultFormatterOptions& options)
{
    stringstream ss;
    ss << string(options.indent, ' ') << "(define " << parse_text(node.problem_name, options) << "\n";
    auto nested_options = DefaultFormatterOptions { options.indent + options.add_indent, options.add_indent };
    ss << string(nested_options.indent, ' ') << parse_text(node.domain_name, nested_options) << "\n";
    if (node.requirements.has_value())
    {
        ss << string(nested_options.indent, ' ') << parse_text(node.requirements.value(), nested_options) << "\n";
    }
    if (node.objects.has_value())
    {
        ss << string(nested_options.indent, ' ') << parse_text(node.objects.value(), nested_options) << "\n";
    }
    if (node.initial.has_value())
    {
        ss << string(nested_options.indent, ' ') << parse_text(node.initial.value(), nested_options) << "\n";
    }
    if (node.goal.has_value())
    {
        ss << string(nested_options.indent, ' ') << parse_text(node.goal.value(), nested_options) << "\n";
    }
    if (node.constraints.has_value())
    {
        ss << string(nested_options.indent, ' ') << parse_text(node.constraints.value(), nested_options) << "\n";
    }
    if (node.metric_specification.has_value())
    {
        ss << string(nested_options.indent, ' ') << parse_text(node.metric_specification.value(), nested_options) << "\n";
    }
    ss << string(options.indent, ' ') << ")";
    return ss.str();
}

template<typename T>
inline std::string parse_text(const std::vector<T>& nodes, const DefaultFormatterOptions& options)
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
