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
#include "loki/details/pddl/concepts.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/function_expressions.hpp"

#include <variant>

namespace loki
{

/* FunctionExpression */

template<ParsingContext C>
struct FunctionExpressionVisitor : boost::static_visitor<FunctionExpression>
{
    C& context;

    FunctionExpressionVisitor(C& context_);

    FunctionExpression operator()(const ast::FunctionExpressionNumber& node);
    FunctionExpression operator()(const ast::FunctionExpressionBinaryOp& node);
    FunctionExpression operator()(const ast::FunctionExpressionMinus& node);
    FunctionExpression operator()(const ast::FunctionExpressionHead node);
    FunctionExpression operator()(const ast::FunctionExpression& node);
};

/* Function */
template<ParsingContext C>
extern Function parse(const ast::FunctionHead& node, C& context);

/* FunctionSkeleton */
struct AtomicFunctionSkeletonVisitor : boost::static_visitor<FunctionSkeleton>
{
    DomainParsingContext& context;

    AtomicFunctionSkeletonVisitor(DomainParsingContext& context_);

    FunctionSkeleton operator()(const ast::AtomicFunctionSkeletonTotalCost& node);
    FunctionSkeleton operator()(const ast::AtomicFunctionSkeletonGeneral& node);
};

template<ParsingContext C>
extern FunctionSkeleton parse_function_skeleton_reference(const ast::FunctionSymbol& node, C& context);

/* FunctionSkeletonList */

class FunctionSkeletonListVisitor : boost::static_visitor<FunctionSkeletonList>
{
private:
    DomainParsingContext& context;

public:
    FunctionSkeletonListVisitor(DomainParsingContext& context_);

    FunctionSkeletonList operator()(const std::vector<ast::AtomicFunctionSkeleton>& node);
    FunctionSkeletonList operator()(const ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& node);
    FunctionSkeletonList operator()(const ast::FunctionTypedListOfAtomicFunctionSkeletons& node);
};

extern FunctionSkeletonList parse(const ast::Functions& node, DomainParsingContext& context);

/* FunctionExpression */

class MetricFunctionExpressionDeclarationVisitor : boost::static_visitor<FunctionExpression>
{
private:
    ProblemParsingContext& context;

public:
    MetricFunctionExpressionDeclarationVisitor(ProblemParsingContext& context_);

    FunctionExpression operator()(const ast::MetricFunctionExpression& node);
    FunctionExpression operator()(const ast::MetricFunctionExpressionNumber& node);
    FunctionExpression operator()(const ast::MetricFunctionExpressionBinaryOperator& node);
    FunctionExpression operator()(const ast::MetricFunctionExpressionMultiOperator& node);
    FunctionExpression operator()(const ast::MetricFunctionExpressionMinus& node);
    FunctionExpression operator()(const ast::MetricFunctionExpressionBasicFunctionTerm& node);
    FunctionExpression operator()(const ast::MetricFunctionExpressionTotalTime& node);
    FunctionExpression operator()(const ast::MetricFunctionExpressionPreferences& node);
};

/* Function */
extern Function parse(const ast::GroundFunction& node, ProblemParsingContext& context);

}

#endif
