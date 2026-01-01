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

#include "enums.hpp"

namespace loki
{

/* BinaryComparatorEnum */
BinaryComparatorEnum BinaryComparatorVisitor::operator()(const ast::BinaryComparatorGreater&) const { return BinaryComparatorEnum::GREATER; }

BinaryComparatorEnum BinaryComparatorVisitor::operator()(const ast::BinaryComparatorLess&) const { return BinaryComparatorEnum::LESS; }

BinaryComparatorEnum BinaryComparatorVisitor::operator()(const ast::BinaryComparatorEqual&) const { return BinaryComparatorEnum::EQUAL; }

BinaryComparatorEnum BinaryComparatorVisitor::operator()(const ast::BinaryComparatorUnequal&) const { return BinaryComparatorEnum::UNEQUAL; }

BinaryComparatorEnum BinaryComparatorVisitor::operator()(const ast::BinaryComparatorGreaterEqual&) const { return BinaryComparatorEnum::GREATER_EQUAL; }

BinaryComparatorEnum BinaryComparatorVisitor::operator()(const ast::BinaryComparatorLessEqual&) const { return BinaryComparatorEnum::LESS_EQUAL; }

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

/**
 * AssignOperatorEnum
 */

AssignOperatorEnum AssignOperatorVisitor::operator()(const ast::AssignOperatorAssign&) const { return AssignOperatorEnum::ASSIGN; }

AssignOperatorEnum AssignOperatorVisitor::operator()(const ast::AssignOperatorScaleUp&) const { return AssignOperatorEnum::SCALE_UP; }

AssignOperatorEnum AssignOperatorVisitor::operator()(const ast::AssignOperatorScaleDown&) const { return AssignOperatorEnum::SCALE_DOWN; }

AssignOperatorEnum AssignOperatorVisitor::operator()(const ast::AssignOperatorIncrease&) const { return AssignOperatorEnum::INCREASE; }

AssignOperatorEnum AssignOperatorVisitor::operator()(const ast::AssignOperatorDecrease&) const { return AssignOperatorEnum::DECREASE; }

AssignOperatorEnum parse(const ast::AssignOperator& node) { return boost::apply_visitor(AssignOperatorVisitor(), node); }

/* OptimizationMetricEnum */

OptimizationDeclarationVisitor::OptimizationDeclarationVisitor(ProblemParsingContext& context) : context(context) {}

OptimizationMetricEnum OptimizationDeclarationVisitor::operator()(const ast::OptimizationMinimize& /*node*/) const { return OptimizationMetricEnum::MINIMIZE; }

OptimizationMetricEnum OptimizationDeclarationVisitor::operator()(const ast::OptimizationMaximize& /*node*/) const { return OptimizationMetricEnum::MAXIMIZE; }

OptimizationMetricEnum parse(const ast::Optimization& node, ProblemParsingContext& context)
{
    return boost::apply_visitor(OptimizationDeclarationVisitor(context), node);
}

}