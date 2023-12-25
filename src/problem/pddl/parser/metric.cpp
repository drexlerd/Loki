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

#include "function.hpp"

#include "../../../domain/pddl/parser/common.hpp"
#include "../../../domain/pddl/parser/functions.hpp"
#include "../../../../include/loki/problem/pddl/exceptions.hpp"
#include "../../../../include/loki/domain/pddl/exceptions.hpp"


using namespace loki::problem;
using namespace std;


namespace loki {
OptimizationDeclarationVisitor::OptimizationDeclarationVisitor(Context& context_) : context(context_) { }

pddl::OptimizationMetricEnum OptimizationDeclarationVisitor::operator()(const problem::ast::OptimizationMinimize& node) {
    return pddl::OptimizationMetricEnum::MINIMIZE;
}

pddl::OptimizationMetricEnum OptimizationDeclarationVisitor::operator()(const problem::ast::OptimizationMaximize& node) {
    return pddl::OptimizationMetricEnum::MAXIMIZE;
}

pddl::OptimizationMetricEnum parse(const problem::ast::Optimization& node, Context& context) {
    return boost::apply_visitor(OptimizationDeclarationVisitor(context), node);
}


MetricSpecificationDeclarationVisitor::MetricSpecificationDeclarationVisitor(Context& context_) : context(context_) { }

pddl::OptimizationMetric MetricSpecificationDeclarationVisitor::operator()(const problem::ast::MetricSpecificationTotalCost& node) {
    const auto optimization = pddl::OptimizationMetricEnum::MINIMIZE;
    const auto function_skeleton = parse_function_skeleton_reference(node.function_symbol_total_cost, context);
    const auto function = context.factories.functions.get_or_create<pddl::FunctionImpl>(function_skeleton, pddl::TermList{});
    const auto function_expression = context.factories.function_expressions.get_or_create<pddl::FunctionExpressionFunctionImpl>(function);
    return context.factories.optimization_metrics.get_or_create<pddl::OptimizationMetricImpl>(optimization, function_expression);
}

pddl::OptimizationMetric MetricSpecificationDeclarationVisitor::operator()(const problem::ast::MetricSpecificationGeneral& node) {
    const auto optimization = parse(node.optimization, context);
    const auto function_expression = parse(node.metric_function_expression, context);
    return context.factories.optimization_metrics.get_or_create<pddl::OptimizationMetricImpl>(optimization, function_expression);
}


pddl::OptimizationMetric parse(const problem::ast::MetricSpecification& node, Context& context) {
    return boost::apply_visitor(MetricSpecificationDeclarationVisitor(context), node);
}

}
