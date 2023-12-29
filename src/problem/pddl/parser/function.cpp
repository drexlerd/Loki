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

#include "function.hpp"

#include "objects.hpp"

#include "../../../domain/pddl/parser/common.hpp"
#include "../../../domain/pddl/parser/functions.hpp"
#include "../../../../include/domain/pddl/exceptions.hpp"


namespace loki {

/* FunctionExpression */
pddl::FunctionExpression parse(const problem::ast::MetricFunctionExpressionNumber& node, Context& context) {
    const auto number = parse(node.number);
    const auto function_expression = context.factories.function_expressions.get_or_create<pddl::FunctionExpressionNumberImpl>(number);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

pddl::FunctionExpression parse(const problem::ast::MetricFunctionExpressionBinaryOperator& node, Context& context) {
    const auto binary_operator = boost::apply_visitor(BinaryOperatorVisitor(), node.binary_operator);
    const auto left_function_expression = parse(node.metric_function_expression_left, context);
    const auto right_function_expression = parse(node.metric_function_expression_right, context);
    const auto function_expression = context.factories.function_expressions.get_or_create<pddl::FunctionExpressionBinaryOperatorImpl>(binary_operator, left_function_expression, right_function_expression);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

pddl::FunctionExpression parse(const problem::ast::MetricFunctionExpressionMultiOperator& node, Context& context) {
    const auto multi_operator = boost::apply_visitor(MultiOperatorVisitor(), node.multi_operator);
    auto function_expressions = pddl::FunctionExpressionList();
    const auto first_function_expression = parse(node.metric_function_expression_first, context);
    function_expressions.push_back(first_function_expression);
    for (const auto& child_node : node.metric_function_expression_remaining) {
        const auto next_function_expression = parse(child_node, context);
        function_expressions.push_back(next_function_expression);
    }
    const auto function_expression = context.factories.function_expressions.get_or_create<pddl::FunctionExpressionMultiOperatorImpl>(multi_operator, function_expressions);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

pddl::FunctionExpression parse(const problem::ast::MetricFunctionExpressionMinus& node, Context& context) {
    const auto child_function_expression = parse(node.metric_function_expression, context);
    const auto function_expression = context.factories.function_expressions.get_or_create<pddl::FunctionExpressionMinusImpl>(child_function_expression);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

pddl::FunctionExpression parse(const problem::ast::MetricFunctionExpressionBasicFunctionTerm& node, Context& context) {
    const auto function = parse(node.basic_function_term, context);
    const auto function_expression = context.factories.function_expressions.get_or_create<pddl::FunctionExpressionFunctionImpl>(function);
    context.positions.push_back(function_expression, node);
    return function_expression;
}

pddl::FunctionExpression parse(const problem::ast::MetricFunctionExpressionTotalTime& /*node*/, Context& /*context*/) {
    throw NotImplementedError("parse(const problem::ast::MetricFunctionExpressionTotalTime& node, Context& context)");
}

pddl::FunctionExpression parse(const problem::ast::MetricFunctionExpressionPreferences& /*node*/, Context& /*context*/) {
    throw NotImplementedError("parse(const problem::ast::MetricFunctionExpressionPreferences& node, Context& context)");
}

pddl::FunctionExpression parse(const problem::ast::MetricFunctionExpression& node, Context& context) {
    return boost::apply_visitor(MetricFunctionExpressionDeclarationVisitor(context), node);
}

MetricFunctionExpressionDeclarationVisitor::MetricFunctionExpressionDeclarationVisitor(Context& context_) : context(context_) { }


/* Function */
pddl::Function parse(const problem::ast::BasicFunctionTerm& node, Context& context) {
    const auto function_skeleton = parse_function_skeleton_reference(node.function_symbol, context);
    pddl::TermList term_list;
    for (const auto& name_node : node.names) {
        term_list.push_back(context.factories.terms.get_or_create<pddl::TermObjectImpl>(parse_object_reference(name_node, context)));
    }
    if (function_skeleton->get_parameters().size() != term_list.size()) {
        throw MismatchedFunctionSkeletonTermListError(function_skeleton, term_list, context.scopes.get_error_handler()(node, ""));
    }
    const auto function = context.factories.functions.get_or_create<pddl::FunctionImpl>(function_skeleton, term_list);
    context.positions.push_back(function, node);
    return function;
}

}
