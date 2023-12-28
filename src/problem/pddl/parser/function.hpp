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

#ifndef LOKI_SRC_PROBLEM_PDDL_PARSER_FUNCTION_HPP_
#define LOKI_SRC_PROBLEM_PDDL_PARSER_FUNCTION_HPP_

#include "../../../../include/problem/ast/ast.hpp"
#include "../../../../include/problem/pddl/parser.hpp"



namespace loki {

class MetricFunctionExpressionDeclarationVisitor : boost::static_visitor<pddl::FunctionExpression> {
private:
    Context& context; 

public:
    MetricFunctionExpressionDeclarationVisitor(Context& context_);

    pddl::FunctionExpression operator()(const problem::ast::MetricFunctionExpressionNumber& node);

    pddl::FunctionExpression operator()(const problem::ast::MetricFunctionExpressionBinaryOperator& node);

    pddl::FunctionExpression operator()(const problem::ast::MetricFunctionExpressionMultiOperator& node);

    pddl::FunctionExpression operator()(const problem::ast::MetricFunctionExpressionMinus& node);

    pddl::FunctionExpression operator()(const problem::ast::MetricFunctionExpressionBasicFunctionTerm& node);

    pddl::FunctionExpression operator()(const problem::ast::MetricFunctionExpressionTotalTime& node);

    pddl::FunctionExpression operator()(const problem::ast::MetricFunctionExpressionPreferences& node);
};

extern pddl::FunctionExpression parse(const problem::ast::MetricFunctionExpression& node, Context& context);

extern pddl::Function parse(const problem::ast::BasicFunctionTerm& node, Context& context);

}

#endif
