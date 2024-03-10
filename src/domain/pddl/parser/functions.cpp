/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
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

#include "functions.hpp"

#include "common.hpp"
#include "loki/domain/pddl/exceptions.hpp"
#include "parameters.hpp"

namespace loki
{

/* MultiOperator */
pddl::MultiOperatorEnum MultiOperatorVisitor::operator()(const domain::ast::MultiOperatorMul&) const { return pddl::MultiOperatorEnum::MUL; }

pddl::MultiOperatorEnum MultiOperatorVisitor::operator()(const domain::ast::MultiOperatorPlus&) const { return pddl::MultiOperatorEnum::PLUS; }

/* BinaryOperator */
pddl::BinaryOperatorEnum MultiToBinaryOperatorVisitor::operator()(const domain::ast::MultiOperatorMul&) const { return pddl::BinaryOperatorEnum::MUL; }

pddl::BinaryOperatorEnum MultiToBinaryOperatorVisitor::operator()(const domain::ast::MultiOperatorPlus&) const { return pddl::BinaryOperatorEnum::PLUS; }

pddl::BinaryOperatorEnum BinaryOperatorVisitor::operator()(const domain::ast::BinaryOperatorDiv&) const { return pddl::BinaryOperatorEnum::DIV; }

pddl::BinaryOperatorEnum BinaryOperatorVisitor::operator()(const domain::ast::BinaryOperatorMinus&) const { return pddl::BinaryOperatorEnum::MINUS; }

pddl::BinaryOperatorEnum BinaryOperatorVisitor::operator()(const domain::ast::MultiOperator& node) const
{
    return boost::apply_visitor(MultiToBinaryOperatorVisitor(), node);
}

/* FunctionExpression */
pddl::FunctionExpression parse(const domain::ast::FunctionExpression& node, Context& context)
{
    return boost::apply_visitor(FunctionExpressionVisitor(context), node);
}

pddl::FunctionExpression parse(const domain::ast::FunctionExpressionNumber& node, Context& context)
{
    const auto number = parse(node.number);
    const auto function_expression = context.factories.function_expressions.get_or_create<pddl::FunctionExpressionNumberImpl>(number);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

pddl::FunctionExpression parse(const domain::ast::FunctionExpressionBinaryOp& node, Context& context)
{
    const auto binary_operator = boost::apply_visitor(BinaryOperatorVisitor(), node.binary_operator);
    const auto left_function_expression = parse(node.function_expression_left, context);
    const auto right_function_expression = parse(node.function_expression_right, context);
    const auto function_expression =
        context.factories.function_expressions.get_or_create<pddl::FunctionExpressionBinaryOperatorImpl>(binary_operator,
                                                                                                         left_function_expression,
                                                                                                         right_function_expression);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

pddl::FunctionExpression parse(const domain::ast::FunctionExpressionMinus& node, Context& context)
{
    const auto child_function_expression = parse(node.function_expression, context);
    const auto function_expression = context.factories.function_expressions.get_or_create<pddl::FunctionExpressionMinusImpl>(child_function_expression);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

pddl::FunctionExpression parse(const domain::ast::FunctionExpressionHead node, Context& context)
{
    const auto function = parse(node.function_head, context);
    const auto function_expression = context.factories.function_expressions.get_or_create<pddl::FunctionExpressionFunctionImpl>(function);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

FunctionExpressionVisitor::FunctionExpressionVisitor(Context& context_) : context(context_) {}

/* Function */
pddl::Function parse(const domain::ast::FunctionHead& node, Context& context)
{
    const auto function_skeleton = parse_function_skeleton_reference(node.function_symbol, context);
    auto term_list = pddl::TermList();
    for (const auto& term_node : node.terms)
    {
        term_list.push_back(boost::apply_visitor(TermReferenceTermVisitor(context), term_node));
    }
    if (function_skeleton->get_parameters().size() != term_list.size())
    {
        throw MismatchedFunctionSkeletonTermListError(function_skeleton, term_list, context.scopes.get_error_handler()(node, ""));
    }
    const auto function = context.factories.functions.get_or_create<pddl::FunctionImpl>(function_skeleton, term_list);
    context.positions.push_back(function, node);
    context.references.untrack(function->get_function_skeleton());
    return function;
}

/* FunctionSkeleton */
pddl::FunctionSkeleton parse_function_skeleton_reference(const domain::ast::FunctionSymbol& node, Context& context)
{
    auto function_name = parse(node.name);
    auto binding = context.scopes.get<pddl::FunctionSkeletonImpl>(function_name);
    if (!binding.has_value())
    {
        throw UndefinedFunctionSkeletonError(function_name, context.scopes.get_error_handler()(node, ""));
    }
    const auto [function_skeleton, _position, _error_handler] = binding.value();
    context.references.untrack(function_skeleton);
    return function_skeleton;
}

static void test_multiple_definition(const pddl::FunctionSkeleton& function_skeleton, const domain::ast::Name& node, const Context& context)
{
    const auto function_name = function_skeleton->get_name();
    const auto binding = context.scopes.get<pddl::FunctionSkeletonImpl>(function_name);
    if (binding.has_value())
    {
        const auto message_1 = context.scopes.get_error_handler()(node, "Defined here:");
        auto message_2 = std::string("");
        const auto [_function_skeleton, position, error_handler] = binding.value();
        if (position.has_value())
        {
            message_2 = error_handler(position.value(), "First defined here:");
        }
        throw MultiDefinitionFunctionSkeletonError(function_name, message_1 + message_2);
    }
}

static void insert_context_information(const pddl::FunctionSkeleton& function_skeleton, const domain::ast::Name& node, Context& context)
{
    context.positions.push_back(function_skeleton, node);
    context.scopes.insert<pddl::FunctionSkeletonImpl>(function_skeleton->get_name(), function_skeleton, node);
}

pddl::FunctionSkeleton parse(const domain::ast::AtomicFunctionSkeletonTotalCost& node, Context& context)
{
    if (!context.requirements->test(pddl::RequirementEnum::ACTION_COSTS))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::ACTION_COSTS, context.positions.get_error_handler()(node, ""));
    }
    else
    {
        context.references.untrack(pddl::RequirementEnum::ACTION_COSTS);
    }
    if ((!context.requirements->test(pddl::RequirementEnum::ACTION_COSTS)) && (!context.requirements->test(pddl::RequirementEnum::NUMERIC_FLUENTS)))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::NUMERIC_FLUENTS, context.positions.get_error_handler()(node, ""));
    }
    else
    {
        context.references.untrack(pddl::RequirementEnum::ACTION_COSTS);
        context.references.untrack(pddl::RequirementEnum::NUMERIC_FLUENTS);
    }

    assert(context.scopes.get<pddl::TypeImpl>("number").has_value());
    const auto [type, _position, _error_handler] = context.scopes.get<pddl::TypeImpl>("number").value();
    auto function_name = parse(node.function_symbol.name);
    auto function_skeleton = context.factories.function_skeletons.get_or_create<pddl::FunctionSkeletonImpl>(function_name, pddl::ParameterList {}, type);

    test_multiple_definition(function_skeleton, node.function_symbol.name, context);
    insert_context_information(function_skeleton, node.function_symbol.name, context);

    return function_skeleton;
}

pddl::FunctionSkeleton parse(const domain::ast::AtomicFunctionSkeletonGeneral& node, Context& context)
{
    if (!context.requirements->test(pddl::RequirementEnum::NUMERIC_FLUENTS))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::NUMERIC_FLUENTS, context.positions.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::NUMERIC_FLUENTS);

    context.scopes.open_scope();
    auto function_parameters = boost::apply_visitor(ParameterListVisitor(context), node.arguments);
    context.scopes.close_scope();

    assert(context.scopes.get<pddl::TypeImpl>("number").has_value());
    const auto [type, _position, _error_handler] = context.scopes.get<pddl::TypeImpl>("number").value();
    auto function_name = parse(node.function_symbol.name);
    auto function_skeleton = context.factories.function_skeletons.get_or_create<pddl::FunctionSkeletonImpl>(function_name, function_parameters, type);

    test_multiple_definition(function_skeleton, node.function_symbol.name, context);
    insert_context_information(function_skeleton, node.function_symbol.name, context);

    return function_skeleton;
}

AtomicFunctionSkeletonVisitor::AtomicFunctionSkeletonVisitor(Context& context_) : context(context_) {}

/* FunctionSkeletonList */
static pddl::FunctionSkeletonList parse_function_skeleton_definitions(const std::vector<domain::ast::AtomicFunctionSkeleton>& nodes, Context& context)
{
    auto function_skeleton_list = pddl::FunctionSkeletonList();
    for (const auto& node : nodes)
    {
        auto function_skeleton = boost::apply_visitor(AtomicFunctionSkeletonVisitor(context), node);
        function_skeleton_list.push_back(function_skeleton);
    }
    return function_skeleton_list;
}

pddl::FunctionSkeletonList parse(const std::vector<domain::ast::AtomicFunctionSkeleton>& formula_skeleton_nodes, Context& context)
{
    auto function_skeleton_list = parse_function_skeleton_definitions(formula_skeleton_nodes, context);
    return function_skeleton_list;
}

pddl::FunctionSkeletonList parse(const domain::ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& function_skeleton_list_recursively_node,
                                 Context& context)
{
    auto function_skeleton_list = parse_function_skeleton_definitions(function_skeleton_list_recursively_node.atomic_function_skeletons, context);

    if (function_skeleton_list_recursively_node.function_typed_list_of_atomic_function_skeletons.has_value())
    {
        auto add_function_skeleton_list =
            boost::apply_visitor(FunctionSkeletonListVisitor(context),
                                 function_skeleton_list_recursively_node.function_typed_list_of_atomic_function_skeletons.value().get());
        function_skeleton_list.insert(function_skeleton_list.end(), add_function_skeleton_list.begin(), add_function_skeleton_list.end());
    }
    return function_skeleton_list;
}

pddl::FunctionSkeletonList parse(const domain::ast::Functions& functions_node, Context& context)
{
    return boost::apply_visitor(FunctionSkeletonListVisitor(context), functions_node.function_types_list_of_atomic_function_skeletons);
}

FunctionSkeletonListVisitor::FunctionSkeletonListVisitor(Context& context_) : context(context_) {}

}