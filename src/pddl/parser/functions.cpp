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
#include "loki/pddl/exceptions.hpp"
#include "objects.hpp"
#include "parameters.hpp"

namespace loki
{

/* MultiOperator */
MultiOperatorEnum MultiOperatorVisitor::operator()(const ast::MultiOperatorMul&) const { return MultiOperatorEnum::MUL; }

MultiOperatorEnum MultiOperatorVisitor::operator()(const ast::MultiOperatorPlus&) const { return MultiOperatorEnum::PLUS; }

/* BinaryOperator */
BinaryOperatorEnum MultiToBinaryOperatorVisitor::operator()(const ast::MultiOperatorMul&) const { return BinaryOperatorEnum::MUL; }

BinaryOperatorEnum MultiToBinaryOperatorVisitor::operator()(const ast::MultiOperatorPlus&) const { return BinaryOperatorEnum::PLUS; }

BinaryOperatorEnum BinaryOperatorVisitor::operator()(const ast::BinaryOperatorDiv&) const { return BinaryOperatorEnum::DIV; }

BinaryOperatorEnum BinaryOperatorVisitor::operator()(const ast::BinaryOperatorMinus&) const { return BinaryOperatorEnum::MINUS; }

BinaryOperatorEnum BinaryOperatorVisitor::operator()(const ast::MultiOperator& node) const
{
    return boost::apply_visitor(MultiToBinaryOperatorVisitor(), node);
}

/* FunctionExpression */
FunctionExpression parse(const ast::FunctionExpression& node, Context& context) { return boost::apply_visitor(FunctionExpressionVisitor(context), node); }

FunctionExpression parse(const ast::FunctionExpressionNumber& node, Context& context)
{
    const auto number = parse(node.number);
    const auto function_expression = context.factories.get_or_create_function_expression_number(number);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

FunctionExpression parse(const ast::FunctionExpressionBinaryOp& node, Context& context)
{
    const auto binary_operator = boost::apply_visitor(BinaryOperatorVisitor(), node.binary_operator);
    const auto left_function_expression = parse(node.function_expression_left, context);
    const auto right_function_expression = parse(node.function_expression_right, context);
    const auto function_expression =
        context.factories.get_or_create_function_expression_binary_operator(binary_operator, left_function_expression, right_function_expression);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

FunctionExpression parse(const ast::FunctionExpressionMinus& node, Context& context)
{
    const auto child_function_expression = parse(node.function_expression, context);
    const auto function_expression = context.factories.get_or_create_function_expression_minus(child_function_expression);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

FunctionExpression parse(const ast::FunctionExpressionHead node, Context& context)
{
    const auto function = parse(node.function_head, context);
    const auto function_expression = context.factories.get_or_create_function_expression_function(function);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

FunctionExpressionVisitor::FunctionExpressionVisitor(Context& context_) : context(context_) {}

/* Function */
Function parse(const ast::FunctionHead& node, Context& context)
{
    const auto function_skeleton = parse_function_skeleton_reference(node.function_symbol, context);
    auto term_list = TermList();
    for (const auto& term_node : node.terms)
    {
        term_list.push_back(boost::apply_visitor(TermReferenceTermVisitor(context), term_node));
    }
    if (function_skeleton->get_parameters().size() != term_list.size())
    {
        throw MismatchedFunctionSkeletonTermListError(function_skeleton, term_list, context.scopes.get_error_handler()(node, ""));
    }
    const auto function = context.factories.get_or_create_function(function_skeleton, term_list);
    context.positions.push_back(function, node);
    context.references.untrack(function->get_function_skeleton());
    return function;
}

/* FunctionSkeleton */
FunctionSkeleton parse_function_skeleton_reference(const ast::FunctionSymbol& node, Context& context)
{
    auto function_name = parse(node.name);
    auto binding = context.scopes.get<FunctionSkeletonImpl>(function_name);
    if (!binding.has_value())
    {
        throw UndefinedFunctionSkeletonError(function_name, context.scopes.get_error_handler()(node, ""));
    }
    const auto [function_skeleton, _position, _error_handler] = binding.value();
    context.references.untrack(function_skeleton);
    return function_skeleton;
}

static void test_multiple_definition(const FunctionSkeleton& function_skeleton, const ast::Name& node, const Context& context)
{
    const auto function_name = function_skeleton->get_name();
    const auto binding = context.scopes.get<FunctionSkeletonImpl>(function_name);
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

static void insert_context_information(const FunctionSkeleton& function_skeleton, const ast::Name& node, Context& context)
{
    context.positions.push_back(function_skeleton, node);
    context.scopes.insert(function_skeleton->get_name(), function_skeleton, node);
}

FunctionSkeleton parse(const ast::AtomicFunctionSkeletonTotalCost& node, Context& context)
{
    if (!context.requirements->test(RequirementEnum::ACTION_COSTS))
    {
        throw UndefinedRequirementError(RequirementEnum::ACTION_COSTS, context.positions.get_error_handler()(node, ""));
    }
    else
    {
        context.references.untrack(RequirementEnum::ACTION_COSTS);
    }
    if ((!context.requirements->test(RequirementEnum::ACTION_COSTS)) && (!context.requirements->test(RequirementEnum::NUMERIC_FLUENTS)))
    {
        throw UndefinedRequirementError(RequirementEnum::NUMERIC_FLUENTS, context.positions.get_error_handler()(node, ""));
    }
    else
    {
        context.references.untrack(RequirementEnum::ACTION_COSTS);
        context.references.untrack(RequirementEnum::NUMERIC_FLUENTS);
    }

    assert(context.scopes.get<TypeImpl>("number").has_value());
    const auto [type, _position, _error_handler] = context.scopes.get<TypeImpl>("number").value();
    auto function_name = parse(node.function_symbol.name);
    auto function_skeleton = context.factories.get_or_create_function_skeleton(function_name, ParameterList {}, type);

    test_multiple_definition(function_skeleton, node.function_symbol.name, context);
    insert_context_information(function_skeleton, node.function_symbol.name, context);

    return function_skeleton;
}

FunctionSkeleton parse(const ast::AtomicFunctionSkeletonGeneral& node, Context& context)
{
    if (!context.requirements->test(RequirementEnum::NUMERIC_FLUENTS))
    {
        throw UndefinedRequirementError(RequirementEnum::NUMERIC_FLUENTS, context.positions.get_error_handler()(node, ""));
    }
    context.references.untrack(RequirementEnum::NUMERIC_FLUENTS);

    context.scopes.open_scope();
    auto function_parameters = boost::apply_visitor(ParameterListVisitor(context), node.arguments);
    context.scopes.close_scope();

    assert(context.scopes.get<TypeImpl>("number").has_value());
    const auto [type, _position, _error_handler] = context.scopes.get<TypeImpl>("number").value();
    auto function_name = parse(node.function_symbol.name);
    auto function_skeleton = context.factories.get_or_create_function_skeleton(function_name, function_parameters, type);

    test_multiple_definition(function_skeleton, node.function_symbol.name, context);
    insert_context_information(function_skeleton, node.function_symbol.name, context);

    return function_skeleton;
}

AtomicFunctionSkeletonVisitor::AtomicFunctionSkeletonVisitor(Context& context_) : context(context_) {}

/* FunctionSkeletonList */
static FunctionSkeletonList parse_function_skeleton_definitions(const std::vector<ast::AtomicFunctionSkeleton>& nodes, Context& context)
{
    auto function_skeleton_list = FunctionSkeletonList();
    for (const auto& node : nodes)
    {
        auto function_skeleton = boost::apply_visitor(AtomicFunctionSkeletonVisitor(context), node);
        function_skeleton_list.push_back(function_skeleton);
    }
    return function_skeleton_list;
}

FunctionSkeletonList parse(const std::vector<ast::AtomicFunctionSkeleton>& formula_skeleton_nodes, Context& context)
{
    auto function_skeleton_list = parse_function_skeleton_definitions(formula_skeleton_nodes, context);
    return function_skeleton_list;
}

FunctionSkeletonList parse(const ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& function_skeleton_list_recursively_node, Context& context)
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

FunctionSkeletonList parse(const ast::Functions& functions_node, Context& context)
{
    return boost::apply_visitor(FunctionSkeletonListVisitor(context), functions_node.function_types_list_of_atomic_function_skeletons);
}

FunctionSkeletonListVisitor::FunctionSkeletonListVisitor(Context& context_) : context(context_) {}

/* FunctionExpression */
FunctionExpression parse(const ast::MetricFunctionExpressionNumber& node, Context& context)
{
    const auto number = parse(node.number);
    const auto function_expression = context.factories.get_or_create_function_expression_number(number);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

FunctionExpression parse(const ast::MetricFunctionExpressionBinaryOperator& node, Context& context)
{
    const auto binary_operator = boost::apply_visitor(BinaryOperatorVisitor(), node.binary_operator);
    const auto left_function_expression = parse(node.metric_function_expression_left, context);
    const auto right_function_expression = parse(node.metric_function_expression_right, context);
    const auto function_expression =
        context.factories.get_or_create_function_expression_binary_operator(binary_operator, left_function_expression, right_function_expression);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

FunctionExpression parse(const ast::MetricFunctionExpressionMultiOperator& node, Context& context)
{
    const auto multi_operator = boost::apply_visitor(MultiOperatorVisitor(), node.multi_operator);
    auto function_expressions = FunctionExpressionList();
    const auto first_function_expression = parse(node.metric_function_expression_first, context);
    function_expressions.push_back(first_function_expression);
    for (const auto& child_node : node.metric_function_expression_remaining)
    {
        const auto next_function_expression = parse(child_node, context);
        function_expressions.push_back(next_function_expression);
    }
    const auto function_expression = context.factories.get_or_create_function_expression_multi_operator(multi_operator, function_expressions);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

FunctionExpression parse(const ast::MetricFunctionExpressionMinus& node, Context& context)
{
    const auto child_function_expression = parse(node.metric_function_expression, context);
    const auto function_expression = context.factories.get_or_create_function_expression_minus(child_function_expression);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

FunctionExpression parse(const ast::MetricFunctionExpressionBasicFunctionTerm& node, Context& context)
{
    const auto function = parse(node.basic_function_term, context);
    const auto function_expression = context.factories.get_or_create_function_expression_function(function);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

FunctionExpression parse(const ast::MetricFunctionExpressionTotalTime& /*node*/, Context& /*context*/)
{
    throw NotImplementedError("parse(const ast::MetricFunctionExpressionTotalTime& node, Context& context)");
}

FunctionExpression parse(const ast::MetricFunctionExpressionPreferences& /*node*/, Context& /*context*/)
{
    throw NotImplementedError("parse(const ast::MetricFunctionExpressionPreferences& node, Context& context)");
}

FunctionExpression parse(const ast::MetricFunctionExpression& node, Context& context)
{
    return boost::apply_visitor(MetricFunctionExpressionDeclarationVisitor(context), node);
}

MetricFunctionExpressionDeclarationVisitor::MetricFunctionExpressionDeclarationVisitor(Context& context_) : context(context_) {}

/* Function */
Function parse(const ast::BasicFunctionTerm& node, Context& context)
{
    const auto function_skeleton = parse_function_skeleton_reference(node.function_symbol, context);
    auto term_list = TermList();
    for (const auto& name_node : node.names)
    {
        term_list.push_back(context.factories.get_or_create_term_object(parse_object_reference(name_node, context)));
    }
    if (function_skeleton->get_parameters().size() != term_list.size())
    {
        throw MismatchedFunctionSkeletonTermListError(function_skeleton, term_list, context.scopes.get_error_handler()(node, ""));
    }
    const auto function = context.factories.get_or_create_function(function_skeleton, term_list);
    context.positions.push_back(function, node);
    return function;
}

}