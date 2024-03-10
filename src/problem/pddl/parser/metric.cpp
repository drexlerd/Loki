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

#include "../../../domain/pddl/parser/common.hpp"
#include "../../../domain/pddl/parser/functions.hpp"
#include "functions.hpp"
#include "loki/domain/pddl/exceptions.hpp"
#include "loki/problem/pddl/exceptions.hpp"

using namespace loki::problem;
using namespace std;

namespace loki
{

/* OptimizationMetricEnum */
pddl::OptimizationMetricEnum parse(const problem::ast::Optimization& node, Context& context)
{
    return boost::apply_visitor(OptimizationDeclarationVisitor(context), node);
}

pddl::OptimizationMetricEnum parse(const problem::ast::OptimizationMinimize& /*node*/, Context& /*context*/) { return pddl::OptimizationMetricEnum::MINIMIZE; }

pddl::OptimizationMetricEnum parse(const problem::ast::OptimizationMaximize& /*node*/, Context& /*context*/) { return pddl::OptimizationMetricEnum::MAXIMIZE; }

OptimizationDeclarationVisitor::OptimizationDeclarationVisitor(Context& context_) : context(context_) {}

/* OptimizationMetric */
pddl::OptimizationMetric parse(const problem::ast::MetricSpecification& node, Context& context)
{
    return boost::apply_visitor(MetricSpecificationDeclarationVisitor(context), node);
}

pddl::OptimizationMetric parse(const problem::ast::MetricSpecificationTotalCost& node, Context& context)
{
    const auto optimization = pddl::OptimizationMetricEnum::MINIMIZE;
    const auto function_skeleton = parse_function_skeleton_reference(node.function_symbol_total_cost, context);
    const auto function = context.factories.functions.get_or_create<pddl::FunctionImpl>(function_skeleton, pddl::TermList {});
    const auto function_expression = context.factories.function_expressions.get_or_create<pddl::FunctionExpressionFunctionImpl>(function);
    return context.factories.optimization_metrics.get_or_create<pddl::OptimizationMetricImpl>(optimization, function_expression);
}

pddl::OptimizationMetric parse(const problem::ast::MetricSpecificationGeneral& node, Context& context)
{
    const auto optimization = parse(node.optimization, context);
    const auto function_expression = parse(node.metric_function_expression, context);
    return context.factories.optimization_metrics.get_or_create<pddl::OptimizationMetricImpl>(optimization, function_expression);
}

MetricSpecificationDeclarationVisitor::MetricSpecificationDeclarationVisitor(Context& context_) : context(context_) {}

}
