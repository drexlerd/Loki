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

#include <loki/problem/ast/ast.hpp>
#include <loki/problem/pddl/parser.hpp>



namespace loki {

/* FunctionExpression */
extern pddl::FunctionExpression parse(const problem::ast::MetricFunctionExpression& node, Context& context);
extern pddl::FunctionExpression parse(const problem::ast::MetricFunctionExpressionNumber& node, Context& context);
extern pddl::FunctionExpression parse(const problem::ast::MetricFunctionExpressionBinaryOperator& node, Context& context);
extern pddl::FunctionExpression parse(const problem::ast::MetricFunctionExpressionMultiOperator& node, Context& context);
extern pddl::FunctionExpression parse(const problem::ast::MetricFunctionExpressionMinus& node, Context& context);
extern pddl::FunctionExpression parse(const problem::ast::MetricFunctionExpressionBasicFunctionTerm& node, Context& context);
extern pddl::FunctionExpression parse(const problem::ast::MetricFunctionExpressionTotalTime& node, Context& context);
extern pddl::FunctionExpression parse(const problem::ast::MetricFunctionExpressionPreferences& node, Context& context);

class MetricFunctionExpressionDeclarationVisitor : boost::static_visitor<pddl::FunctionExpression> {
private:
    Context& context; 

public:
    MetricFunctionExpressionDeclarationVisitor(Context& context_);

    template<typename Node>
    pddl::FunctionExpression operator()(const Node& node) const {
        return parse(node, context);
    }
};


/* Function */
extern pddl::Function parse(const problem::ast::BasicFunctionTerm& node, Context& context);

}

#endif
