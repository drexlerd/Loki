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

#include "function_skeleton.hpp"
#include "function_operators.hpp"
#include "parameters.hpp"
#include "common.hpp"

#include <loki/domain/pddl/exceptions.hpp>


namespace loki {


/* FunctionExpression */
pddl::FunctionExpression parse(const domain::ast::FunctionExpression& node, Context& context) {
    return boost::apply_visitor(FunctionExpressionVisitor(context), node);
}

pddl::FunctionExpression parse(const domain::ast::FunctionExpressionNumber& node, Context& context) {
    const auto number = parse(node.number);
    const auto function_expression = context.factories.function_expressions.get_or_create<pddl::FunctionExpressionNumberImpl>(number);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

pddl::FunctionExpression parse(const domain::ast::FunctionExpressionBinaryOp& node, Context& context) {
    const auto binary_operator = boost::apply_visitor(BinaryOperatorVisitor(), node.binary_operator);
    const auto left_function_expression = parse(node.function_expression_left, context);
    const auto right_function_expression = parse(node.function_expression_right, context);
    const auto function_expression = context.factories.function_expressions.get_or_create<pddl::FunctionExpressionBinaryOperatorImpl>(binary_operator, left_function_expression, right_function_expression);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

pddl::FunctionExpression parse(const domain::ast::FunctionExpressionMinus& node, Context& context) {
    const auto child_function_expression = parse(node.function_expression, context);
    const auto function_expression = context.factories.function_expressions.get_or_create<pddl::FunctionExpressionMinusImpl>(child_function_expression);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

pddl::FunctionExpression parse(const domain::ast::FunctionExpressionHead node, Context& context) {
    const auto function = parse(node.function_head, context);
    const auto function_expression = context.factories.function_expressions.get_or_create<pddl::FunctionExpressionFunctionImpl>(function);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

FunctionExpressionVisitor::FunctionExpressionVisitor(Context& context_)
    : context(context_) { }


/* Function */
pddl::Function parse(const domain::ast::FunctionHead& node, Context& context) {
    const auto function_skeleton = parse_function_skeleton_reference(node.function_symbol, context);
    auto term_list = pddl::TermList();
    for (const auto& term_node : node.terms) {
        term_list.push_back(boost::apply_visitor(TermReferenceTermVisitor(context), term_node));
    }
    if (function_skeleton->get_parameters().size() != term_list.size()) {
        throw MismatchedFunctionSkeletonTermListError(function_skeleton, term_list, context.scopes.get_error_handler()(node, ""));
    }
    const auto function = context.factories.functions.get_or_create<pddl::FunctionImpl>(function_skeleton, term_list);
    context.positions.push_back(function, node);
    context.references.untrack(function->get_function_skeleton());
    return function;
}

}