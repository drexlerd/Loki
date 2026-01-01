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

#ifndef LOKI_SRC_PDDL_PARSER_ENUMS_HPP_
#define LOKI_SRC_PDDL_PARSER_ENUMS_HPP_

#include "loki/details/ast/ast.hpp"
#include "loki/details/pddl/concepts.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/effects.hpp"
#include "loki/details/pddl/function_expressions.hpp"
#include "loki/details/pddl/metric.hpp"

#include <variant>

namespace loki
{
/* BinaryComparator */
struct BinaryComparatorVisitor : boost::static_visitor<BinaryComparatorEnum>
{
    BinaryComparatorEnum operator()(const ast::BinaryComparatorGreater& node) const;
    BinaryComparatorEnum operator()(const ast::BinaryComparatorLess& node) const;
    BinaryComparatorEnum operator()(const ast::BinaryComparatorEqual& node) const;
    BinaryComparatorEnum operator()(const ast::BinaryComparatorUnequal& node) const;
    BinaryComparatorEnum operator()(const ast::BinaryComparatorGreaterEqual& node) const;
    BinaryComparatorEnum operator()(const ast::BinaryComparatorLessEqual& node) const;
};

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

/* AssignOperator */

struct AssignOperatorVisitor : boost::static_visitor<AssignOperatorEnum>
{
    AssignOperatorEnum operator()(const ast::AssignOperatorAssign& node) const;
    AssignOperatorEnum operator()(const ast::AssignOperatorScaleUp& node) const;
    AssignOperatorEnum operator()(const ast::AssignOperatorScaleDown& node) const;
    AssignOperatorEnum operator()(const ast::AssignOperatorIncrease& node) const;
    AssignOperatorEnum operator()(const ast::AssignOperatorDecrease& node) const;
};

extern AssignOperatorEnum parse(const ast::AssignOperator& node);

/* OptimizationMetricEnum */

struct OptimizationDeclarationVisitor : boost::static_visitor<OptimizationMetricEnum>
{
    ProblemParsingContext& context;

    OptimizationDeclarationVisitor(ProblemParsingContext& context);

    OptimizationMetricEnum operator()(const ast::OptimizationMinimize& node) const;
    OptimizationMetricEnum operator()(const ast::OptimizationMaximize& node) const;
};

extern OptimizationMetricEnum parse(const ast::Optimization& node, ProblemParsingContext& context);

}

#endif
