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
#include "enums.hpp"
#include "error_handling.hpp"
#include "loki/details/pddl/domain_parsing_context.hpp"
#include "loki/details/pddl/exceptions.hpp"
#include "loki/details/pddl/function_skeleton.hpp"
#include "loki/details/pddl/object.hpp"
#include "loki/details/pddl/problem_parsing_context.hpp"
#include "loki/details/pddl/scope.hpp"
#include "loki/details/pddl/variable.hpp"
#include "objects.hpp"
#include "parameters.hpp"

namespace loki
{

/* FunctionExpression */
template<ParsingContext C>
FunctionExpressionVisitor<C>::FunctionExpressionVisitor(C& context_) : context(context_)
{
}

template<ParsingContext C>
FunctionExpression FunctionExpressionVisitor<C>::operator()(const ast::FunctionExpression& node)
{
    auto visitor = FunctionExpressionVisitor(context);
    return boost::apply_visitor(visitor, node);
}

template<ParsingContext C>
FunctionExpression FunctionExpressionVisitor<C>::operator()(const ast::FunctionExpressionNumber& node)
{
    const auto number = parse(node.number);
    const auto function_expression = context.builder.get_repositories().get_or_create_function_expression(
        context.builder.get_repositories().get_or_create_function_expression_number(number));
    context.positions.push_back(function_expression, node);
    return function_expression;
}

template<ParsingContext C>
FunctionExpression FunctionExpressionVisitor<C>::operator()(const ast::FunctionExpressionBinaryOp& node)
{
    const auto binary_operator = boost::apply_visitor(BinaryOperatorVisitor(), node.binary_operator);
    const auto left_function_expression = this->operator()(node.left_function_expression);
    const auto right_function_expression = this->operator()(node.right_function_expression);
    const auto function_expression = context.builder.get_repositories().get_or_create_function_expression(
        context.builder.get_repositories().get_or_create_function_expression_binary_operator(binary_operator,
                                                                                             left_function_expression,
                                                                                             right_function_expression));
    context.positions.push_back(function_expression, node);
    return function_expression;
}

template<ParsingContext C>
FunctionExpression FunctionExpressionVisitor<C>::operator()(const ast::FunctionExpressionMultiOp& node)
{
    const auto multi_operator = boost::apply_visitor(MultiOperatorVisitor(), node.multi_operator);
    auto function_expressions = FunctionExpressionList {};
    for (const auto& child_node : node.function_expressions)
        function_expressions.push_back(this->operator()(child_node));
    const auto function_expression = context.builder.get_repositories().get_or_create_function_expression(
        context.builder.get_repositories().get_or_create_function_expression_multi_operator(multi_operator, function_expressions));
    context.positions.push_back(function_expression, node);
    return function_expression;
}

template<ParsingContext C>
FunctionExpression FunctionExpressionVisitor<C>::operator()(const ast::FunctionExpressionMinus& node)
{
    const auto child_function_expression = this->operator()(node.function_expression);
    const auto function_expression = context.builder.get_repositories().get_or_create_function_expression(
        context.builder.get_repositories().get_or_create_function_expression_minus(child_function_expression));
    context.positions.push_back(function_expression, node);
    return function_expression;
}

template<ParsingContext C>
FunctionExpression FunctionExpressionVisitor<C>::operator()(const ast::FunctionExpressionHead node)
{
    const auto function = parse(node.function_head, context);
    const auto function_expression = context.builder.get_repositories().get_or_create_function_expression(
        context.builder.get_repositories().get_or_create_function_expression_function(function));
    context.positions.push_back(function_expression, node);
    return function_expression;
}

template struct FunctionExpressionVisitor<DomainParsingContext>;
template struct FunctionExpressionVisitor<ProblemParsingContext>;

/* Function */

template<ParsingContext C>
Function parse(const ast::FunctionHead& node, C& context)
{
    const auto function_skeleton = parse_function_skeleton_reference(node.function_symbol, context);
    auto term_list = TermList();
    for (const auto& term_node : node.terms)
    {
        auto visitor = TermReferenceTermVisitor(context);
        term_list.push_back(boost::apply_visitor(visitor, term_node));
    }
    test_arity_compatibility(function_skeleton->get_parameters().size(), term_list.size(), node, context);
    const auto function = context.builder.get_repositories().get_or_create_function(function_skeleton, term_list);
    context.positions.push_back(function, node);
    context.references.untrack(function->get_function_skeleton());
    return function;
}

template Function parse(const ast::FunctionHead& node, DomainParsingContext& context);
template Function parse(const ast::FunctionHead& node, ProblemParsingContext& context);

/* FunctionSkeleton */
template<ParsingContext C>
FunctionSkeleton parse_function_skeleton_reference(const ast::FunctionSymbol& node, C& context)
{
    auto function_name = parse(node.name);
    test_undefined_function_skeleton(function_name, node, context);
    auto binding = context.scopes.top().get_function_skeleton(function_name);
    const auto [function_skeleton, _position, _error_handler] = binding.value();
    context.references.untrack(function_skeleton);
    return function_skeleton;
}

template FunctionSkeleton parse_function_skeleton_reference(const ast::FunctionSymbol& node, DomainParsingContext& context);
template FunctionSkeleton parse_function_skeleton_reference(const ast::FunctionSymbol& node, ProblemParsingContext& context);

static void insert_context_information(const FunctionSkeleton& function_skeleton, const ast::Name& node, DomainParsingContext& context)
{
    context.positions.push_back(function_skeleton, node);
    context.scopes.top().insert_function_skeleton(function_skeleton->get_name(), function_skeleton, node);
}

AtomicFunctionSkeletonVisitor::AtomicFunctionSkeletonVisitor(DomainParsingContext& context_) : context(context_) {}

FunctionSkeleton AtomicFunctionSkeletonVisitor::operator()(const ast::AtomicFunctionSkeletonTotalCost& node)
{
    test_undefined_requirements(RequirementEnumList { RequirementEnum::ACTION_COSTS, RequirementEnum::NUMERIC_FLUENTS }, node, context);
    context.references.untrack(RequirementEnum::ACTION_COSTS);
    context.references.untrack(RequirementEnum::NUMERIC_FLUENTS);

    assert(context.scopes.top().get_type("number").has_value());
    const auto [type, _position, _error_handler] = context.scopes.top().get_type("number").value();
    auto function_name = parse(node.function_symbol.name);
    auto function_skeleton = context.builder.get_repositories().get_or_create_function_skeleton(function_name, ParameterList {}, type);

    test_multiple_definition_function_skeleton(function_skeleton, node.function_symbol.name, context);
    insert_context_information(function_skeleton, node.function_symbol.name, context);

    return function_skeleton;
}

FunctionSkeleton AtomicFunctionSkeletonVisitor::operator()(const ast::AtomicFunctionSkeletonGeneral& node)
{
    test_undefined_requirements(RequirementEnumList { RequirementEnum::ACTION_COSTS, RequirementEnum::NUMERIC_FLUENTS }, node, context);
    context.references.untrack(RequirementEnum::ACTION_COSTS);
    context.references.untrack(RequirementEnum::NUMERIC_FLUENTS);

    context.scopes.open_scope();
    auto visitor = ParameterListVisitor(context);
    auto function_parameters = boost::apply_visitor(visitor, node.arguments);
    context.scopes.close_scope();

    assert(context.scopes.top().get_type("number").has_value());
    const auto [type, _position, _error_handler] = context.scopes.top().get_type("number").value();
    auto function_name = parse(node.function_symbol.name);
    auto function_skeleton = context.builder.get_repositories().get_or_create_function_skeleton(function_name, function_parameters, type);

    test_multiple_definition_function_skeleton(function_skeleton, node.function_symbol.name, context);
    insert_context_information(function_skeleton, node.function_symbol.name, context);

    return function_skeleton;
}

/* FunctionSkeletonList */
static FunctionSkeletonList parse_function_skeleton_definitions(const std::vector<ast::AtomicFunctionSkeleton>& nodes, DomainParsingContext& context)
{
    auto function_skeleton_list = FunctionSkeletonList();
    auto visitor = AtomicFunctionSkeletonVisitor(context);
    for (const auto& node : nodes)
    {
        auto function_skeleton = boost::apply_visitor(visitor, node);
        function_skeleton_list.push_back(function_skeleton);
    }
    return function_skeleton_list;
}

FunctionSkeletonListVisitor::FunctionSkeletonListVisitor(DomainParsingContext& context_) : context(context_) {}

FunctionSkeletonList FunctionSkeletonListVisitor::operator()(const std::vector<ast::AtomicFunctionSkeleton>& formula_skeleton_nodes)
{
    auto function_skeleton_list = parse_function_skeleton_definitions(formula_skeleton_nodes, context);
    return function_skeleton_list;
}

FunctionSkeletonList
FunctionSkeletonListVisitor::operator()(const ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& function_skeleton_list_recursively_node)
{
    auto function_skeleton_list = parse_function_skeleton_definitions(function_skeleton_list_recursively_node.atomic_function_skeletons, context);

    if (function_skeleton_list_recursively_node.function_typed_list_of_atomic_function_skeletons.has_value())
    {
        auto add_function_skeleton_list =
            this->operator()(function_skeleton_list_recursively_node.function_typed_list_of_atomic_function_skeletons.value().get());
        function_skeleton_list.insert(function_skeleton_list.end(), add_function_skeleton_list.begin(), add_function_skeleton_list.end());
    }
    return function_skeleton_list;
}

FunctionSkeletonList FunctionSkeletonListVisitor::operator()(const ast::FunctionTypedListOfAtomicFunctionSkeletons& node)
{
    return boost::apply_visitor(*this, node);
}

FunctionSkeletonList parse(const ast::Functions& functions_node, DomainParsingContext& context)
{
    auto visitor = FunctionSkeletonListVisitor(context);
    return boost::apply_visitor(visitor, functions_node.function_types_list_of_atomic_function_skeletons);
}

/* MetricFunctionExpression */

MetricFunctionExpressionDeclarationVisitor::MetricFunctionExpressionDeclarationVisitor(ProblemParsingContext& context_) : context(context_) {}

FunctionExpression MetricFunctionExpressionDeclarationVisitor::operator()(const ast::MetricFunctionExpressionNumber& node)
{
    const auto number = parse(node.number);
    const auto function_expression = context.builder.get_repositories().get_or_create_function_expression(
        context.builder.get_repositories().get_or_create_function_expression_number(number));
    context.positions.push_back(function_expression, node);
    return function_expression;
}

FunctionExpression MetricFunctionExpressionDeclarationVisitor::operator()(const ast::MetricFunctionExpressionBinaryOperator& node)
{
    const auto binary_operator = boost::apply_visitor(BinaryOperatorVisitor(), node.binary_operator);
    const auto left_function_expression = this->operator()(node.metric_left_function_expression);
    const auto right_function_expression = this->operator()(node.metric_right_function_expression);
    const auto function_expression = context.builder.get_repositories().get_or_create_function_expression(
        context.builder.get_repositories().get_or_create_function_expression_binary_operator(binary_operator,
                                                                                             left_function_expression,
                                                                                             right_function_expression));
    context.positions.push_back(function_expression, node);
    return function_expression;
}

FunctionExpression MetricFunctionExpressionDeclarationVisitor::operator()(const ast::MetricFunctionExpressionMultiOperator& node)
{
    const auto multi_operator = boost::apply_visitor(MultiOperatorVisitor(), node.multi_operator);
    auto function_expressions = FunctionExpressionList();
    const auto first_function_expression = this->operator()(node.metric_function_expression_first);
    function_expressions.push_back(first_function_expression);
    for (const auto& child_node : node.metric_function_expression_remaining)
    {
        const auto next_function_expression = this->operator()(child_node);
        function_expressions.push_back(next_function_expression);
    }
    const auto function_expression = context.builder.get_repositories().get_or_create_function_expression(
        context.builder.get_repositories().get_or_create_function_expression_multi_operator(multi_operator, function_expressions));
    context.positions.push_back(function_expression, node);
    return function_expression;
}

FunctionExpression MetricFunctionExpressionDeclarationVisitor::operator()(const ast::MetricFunctionExpressionMinus& node)
{
    const auto child_function_expression = this->operator()(node.metric_function_expression);
    const auto function_expression = context.builder.get_repositories().get_or_create_function_expression(
        context.builder.get_repositories().get_or_create_function_expression_minus(child_function_expression));
    context.positions.push_back(function_expression, node);
    return function_expression;
}

FunctionExpression MetricFunctionExpressionDeclarationVisitor::operator()(const ast::MetricFunctionExpressionBasicFunctionTerm& node)
{
    const auto function = parse(node.ground_function, context);
    const auto function_expression = context.builder.get_repositories().get_or_create_function_expression(
        context.builder.get_repositories().get_or_create_function_expression_function(function));
    context.positions.push_back(function_expression, node);
    return function_expression;
}

FunctionExpression MetricFunctionExpressionDeclarationVisitor::operator()(const ast::MetricFunctionExpressionTotalTime& /*node*/)
{
    throw NotImplementedError("parse(const ast::MetricFunctionExpressionTotalTime& node, Context& context)");
}

FunctionExpression MetricFunctionExpressionDeclarationVisitor::operator()(const ast::MetricFunctionExpressionPreferences& /*node*/)
{
    throw NotImplementedError("parse(const ast::MetricFunctionExpressionPreferences& node, Context& context)");
}

FunctionExpression MetricFunctionExpressionDeclarationVisitor::operator()(const ast::MetricFunctionExpression& node)
{
    return boost::apply_visitor(MetricFunctionExpressionDeclarationVisitor(context), node);
}

/* Function */
Function parse(const ast::GroundFunction& node, ProblemParsingContext& context)
{
    const auto function_skeleton = parse_function_skeleton_reference(node.function_symbol, context);
    auto term_list = TermList();
    for (const auto& name_node : node.names)
    {
        term_list.push_back(context.builder.get_repositories().get_or_create_term(parse_object_reference(name_node, context)));
    }
    test_arity_compatibility(function_skeleton->get_parameters().size(), term_list.size(), node, context);
    const auto function = context.builder.get_repositories().get_or_create_function(function_skeleton, term_list);
    context.positions.push_back(function, node);
    return function;
}

}