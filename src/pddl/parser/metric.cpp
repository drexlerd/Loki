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

using namespace std;

namespace loki
{

/* OptimizationMetricEnum */
OptimizationMetricEnum parse(const ast::Optimization& node, Context& context) { return boost::apply_visitor(OptimizationDeclarationVisitor(context), node); }

OptimizationMetricEnum parse(const ast::OptimizationMinimize& /*node*/, Context& /*context*/) { return OptimizationMetricEnum::MINIMIZE; }

OptimizationMetricEnum parse(const ast::OptimizationMaximize& /*node*/, Context& /*context*/) { return OptimizationMetricEnum::MAXIMIZE; }

OptimizationDeclarationVisitor::OptimizationDeclarationVisitor(Context& context_) : context(context_) {}

/* OptimizationMetric */
OptimizationMetric parse(const ast::MetricSpecification& node, Context& context)
{
    return boost::apply_visitor(MetricSpecificationDeclarationVisitor(context), node);
}

OptimizationMetric parse(const ast::MetricSpecificationTotalCost& node, Context& context)
{
    const auto optimization = OptimizationMetricEnum::MINIMIZE;
    const auto function_skeleton = parse_function_skeleton_reference(node.function_symbol_total_cost, context);
    const auto function = context.factories.get_or_create_function(function_skeleton, TermList {});
    const auto function_expression = context.factories.get_or_create_function_expression_function(function);
    return context.factories.get_or_create_optimization_metric(optimization, function_expression);
}

OptimizationMetric parse(const ast::MetricSpecificationGeneral& node, Context& context)
{
    const auto optimization = parse(node.optimization, context);
    const auto function_expression = parse(node.metric_function_expression, context);
    return context.factories.get_or_create_optimization_metric(optimization, function_expression);
}

MetricSpecificationDeclarationVisitor::MetricSpecificationDeclarationVisitor(Context& context_) : context(context_) {}

}
