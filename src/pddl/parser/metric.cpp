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

#include "metric.hpp"

#include "common.hpp"
#include "functions.hpp"
#include "loki/details/pddl/exceptions.hpp"
#include "loki/details/pddl/problem_parsing_context.hpp"
#include "loki/details/pddl/scope.hpp"

using namespace std;

namespace loki
{

/* OptimizationMetricEnum */

OptimizationDeclarationVisitor::OptimizationDeclarationVisitor(ProblemParsingContext& context) : context(context) {}

OptimizationMetricEnum OptimizationDeclarationVisitor::operator()(const ast::OptimizationMinimize& /*node*/) const { return OptimizationMetricEnum::MINIMIZE; }

OptimizationMetricEnum OptimizationDeclarationVisitor::operator()(const ast::OptimizationMaximize& /*node*/) const { return OptimizationMetricEnum::MAXIMIZE; }

OptimizationMetricEnum parse(const ast::Optimization& node, ProblemParsingContext& context)
{
    return boost::apply_visitor(OptimizationDeclarationVisitor(context), node);
}

/* OptimizationMetric */
MetricSpecificationDeclarationVisitor::MetricSpecificationDeclarationVisitor(ProblemParsingContext& context_) : context(context_) {}

OptimizationMetric MetricSpecificationDeclarationVisitor::operator()(const ast::MetricSpecificationTotalCost& node)
{
    const auto optimization = OptimizationMetricEnum::MINIMIZE;
    const auto function_skeleton = parse_function_skeleton_reference(node.function_symbol_total_cost, context);
    const auto function = context.builder.get_or_create_function(function_skeleton, TermList {});
    const auto function_expression = context.builder.get_or_create_function_expression(context.builder.get_or_create_function_expression_function(function));
    return context.builder.get_or_create_optimization_metric(optimization, function_expression);
}

OptimizationMetric MetricSpecificationDeclarationVisitor::operator()(const ast::MetricSpecificationGeneral& node)
{
    const auto optimization = parse(node.optimization, context);
    auto metric_fexpr_visitor = MetricFunctionExpressionDeclarationVisitor(context);
    const auto function_expression = boost::apply_visitor(metric_fexpr_visitor, node.metric_function_expression);
    return context.builder.get_or_create_optimization_metric(optimization, function_expression);
}

OptimizationMetric parse(const ast::MetricSpecification& node, ProblemParsingContext& context)
{
    auto visitor = MetricSpecificationDeclarationVisitor(context);
    return boost::apply_visitor(visitor, node);
}

}
