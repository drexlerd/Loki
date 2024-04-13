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

#ifndef LOKI_SRC_PDDL_PARSER_FUNCTIONS_HPP_
#define LOKI_SRC_PDDL_PARSER_FUNCTIONS_HPP_

#include "loki/details/ast/ast.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/function_expressions.hpp"
#include "loki/details/pddl/function_skeleton.hpp"
#include "loki/details/pddl/parser.hpp"

#include <variant>

namespace loki
{
/* MultiOperator */
struct MultiOperatorVisitor : boost::static_visitor<MultiOperatorEnum>
{
    MultiOperatorEnum operator()(const ast::MultiOperatorMul& node) const;
    MultiOperatorEnum operator()(const ast::MultiOperatorPlus& node) const;
};

/* BinaryOperator */
struct MultiToBinaryOperatorVisitor : boost::static_visitor<BinaryOperatorEnum>
{
    BinaryOperatorEnum operator()(const ast::MultiOperatorMul& node) const;
    BinaryOperatorEnum operator()(const ast::MultiOperatorPlus& node) const;
};

struct BinaryOperatorVisitor : boost::static_visitor<BinaryOperatorEnum>
{
    BinaryOperatorEnum operator()(const ast::BinaryOperatorDiv& node) const;
    BinaryOperatorEnum operator()(const ast::BinaryOperatorMinus& node) const;
    BinaryOperatorEnum operator()(const ast::MultiOperator& node) const;
};

/* FunctionExpression */
extern FunctionExpression parse(const ast::FunctionExpressionNumber& node, Context& context);
extern FunctionExpression parse(const ast::FunctionExpressionBinaryOp& node, Context& context);
extern FunctionExpression parse(const ast::FunctionExpressionMinus& node, Context& context);
extern FunctionExpression parse(const ast::FunctionExpressionHead node, Context& context);
extern FunctionExpression parse(const ast::FunctionExpression& node, Context& context);

class FunctionExpressionVisitor : boost::static_visitor<FunctionExpression>
{
private:
    Context& context;

public:
    FunctionExpressionVisitor(Context& context_);

    template<typename Node>
    FunctionExpression operator()(const Node& node) const
    {
        return parse(node, context);
    }
};

/* Function */
extern Function parse(const ast::FunctionHead& node, Context& context);

/* FunctionSkeleton */
extern FunctionSkeleton parse_function_skeleton_reference(const ast::FunctionSymbol& node, Context& context);
extern FunctionSkeleton parse(const ast::AtomicFunctionSkeletonTotalCost& node);
extern FunctionSkeleton parse(const ast::AtomicFunctionSkeletonGeneral& node);

class AtomicFunctionSkeletonVisitor : boost::static_visitor<FunctionSkeleton>
{
private:
    Context& context;

public:
    AtomicFunctionSkeletonVisitor(Context& context_);

    template<typename Node>
    FunctionSkeleton operator()(const Node& node)
    {
        return parse(node, context);
    }
};

/* FunctionSkeletonList */
extern FunctionSkeletonList parse(const std::vector<ast::AtomicFunctionSkeleton>& formula_skeleton_nodes);
extern FunctionSkeletonList parse(const ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& function_skeleton_list_recursively_node);
extern FunctionSkeletonList parse(const ast::Functions& functions_node, Context& context);

class FunctionSkeletonListVisitor : boost::static_visitor<FunctionSkeletonList>
{
private:
    Context& context;

public:
    FunctionSkeletonListVisitor(Context& context_);

    template<typename Node>
    FunctionSkeletonList operator()(const Node& node)
    {
        return parse(node, context);
    }
};

/* FunctionExpression */
extern FunctionExpression parse(const ast::MetricFunctionExpression& node, Context& context);
extern FunctionExpression parse(const ast::MetricFunctionExpressionNumber& node, Context& context);
extern FunctionExpression parse(const ast::MetricFunctionExpressionBinaryOperator& node, Context& context);
extern FunctionExpression parse(const ast::MetricFunctionExpressionMultiOperator& node, Context& context);
extern FunctionExpression parse(const ast::MetricFunctionExpressionMinus& node, Context& context);
extern FunctionExpression parse(const ast::MetricFunctionExpressionBasicFunctionTerm& node, Context& context);
extern FunctionExpression parse(const ast::MetricFunctionExpressionTotalTime& node, Context& context);
extern FunctionExpression parse(const ast::MetricFunctionExpressionPreferences& node, Context& context);

class MetricFunctionExpressionDeclarationVisitor : boost::static_visitor<FunctionExpression>
{
private:
    Context& context;

public:
    MetricFunctionExpressionDeclarationVisitor(Context& context_);

    template<typename Node>
    FunctionExpression operator()(const Node& node) const
    {
        return parse(node, context);
    }
};

/* Function */
extern Function parse(const ast::BasicFunctionTerm& node, Context& context);

}

#endif  // LOKI_SRC_DOMAIN_PDDL_PARSER_FUNCTIONS_HPP_
