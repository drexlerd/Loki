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

#ifndef LOKI_SRC_PROBLEM_PDDL_PARSER_METRIC_HPP_
#define LOKI_SRC_PROBLEM_PDDL_PARSER_METRIC_HPP_

#include <loki/problem/ast/ast.hpp>
#include <loki/problem/pddl/parser.hpp>



namespace loki {

/* OptimizationMetricEnum */
extern pddl::OptimizationMetricEnum parse(const problem::ast::Optimization& node, Context& context);
extern pddl::OptimizationMetricEnum parse(const problem::ast::OptimizationMinimize& node, Context& context);   
extern pddl::OptimizationMetricEnum parse(const problem::ast::OptimizationMaximize& node, Context& context);

class OptimizationDeclarationVisitor : boost::static_visitor<pddl::OptimizationMetric> {
private:
    Context& context;

public:
    OptimizationDeclarationVisitor(Context& context_);

    template<typename Node> 
    pddl::OptimizationMetricEnum operator()(const Node& node) const {
        return parse(node, context);
    }
};


/* OptimizationMetric */
extern pddl::OptimizationMetric parse(const problem::ast::MetricSpecification& node, Context& context);
extern pddl::OptimizationMetric parse(const problem::ast::MetricSpecificationTotalCost& node, Context& context);
extern pddl::OptimizationMetric parse(const problem::ast::MetricSpecificationGeneral& node, Context& context);

class MetricSpecificationDeclarationVisitor : boost::static_visitor<pddl::OptimizationMetric> {
private:
    Context& context;

public:
    MetricSpecificationDeclarationVisitor(Context& context_);

    template<typename Node> 
    pddl::OptimizationMetric operator()(const Node& node) const {
        return parse(node, context);
    }
};

}

#endif
