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

#include "../../../include/loki/problem/pddl/parser.hpp"

#include "parser/objects.hpp"
#include "parser/initial.hpp"
#include "parser/goal.hpp"
#include "parser/metric.hpp"
#include "parser/reference_utils.hpp"

#include "../../domain/pddl/parser/common.hpp"
#include "../../domain/pddl/parser/requirements.hpp"
#include "../../domain/pddl/unpacking_visitor.hpp"

#include "../../../include/loki/problem/pddl/exceptions.hpp"
#include "../../../include/loki/domain/pddl/domain.hpp"
#include "../../../include/loki/domain/pddl/requirements.hpp"


namespace loki {

pddl::Problem parse(const problem::ast::Problem& problem_node, Context& context, const pddl::Domain& domain) {
    /* Domain name section */
    const auto domain_name = parse(problem_node.domain_name.name);
    if (domain_name != domain->get_name()) {
        throw MismatchedDomainError(domain, domain_name, context.scopes.get_error_handler()(problem_node.domain_name, ""));
    }
    /* Problem name section */
    const auto problem_name = parse(problem_node.problem_name.name);
    /* Requirements section */
    if (problem_node.requirements.has_value()) {
        context.requirements = context.factories.requirements.get_or_create<pddl::RequirementsImpl>(
            parse(problem_node.requirements.value(), context));
        context.positions.push_back(context.requirements, problem_node.requirements.value());
    } else {
        // Default requirements
        context.requirements = context.factories.requirements.get_or_create<pddl::RequirementsImpl>(
            pddl::RequirementEnumSet{pddl::RequirementEnum::STRIPS});
    }
    /* Objects section */
    auto objects = pddl::ObjectList();
    if (problem_node.objects.has_value()) {
        objects = parse(problem_node.objects.value(), context);
    }
    track_object_references(objects, context);
    /* Initial section */
    auto initial_literals = pddl::LiteralList();
    auto numeric_fluents = pddl::NumericFluentList();
    const auto initial_elements = parse(problem_node.initial, context);
    for (const auto& initial_element : initial_elements) {
        std::visit(UnpackingVisitor(initial_literals, numeric_fluents), initial_element);
    }
    /* Goal section */
    const auto goal_condition = parse(problem_node.goal, context);
    /* Metric section */
    auto optimization_metric = std::optional<pddl::OptimizationMetric>();
    if (problem_node.metric_specification.has_value()) {
        optimization_metric = parse(problem_node.metric_specification.value(), context);
    }

    // Check references
    test_object_references(objects, context);

    const auto problem = context.factories.problems.get_or_create<pddl::ProblemImpl>(domain, problem_name, context.requirements, objects, initial_literals, numeric_fluents, goal_condition, optimization_metric);
    context.positions.push_back(problem, problem_node);
    return problem;
}

}