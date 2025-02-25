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

#include "problem.hpp"

#include "common.hpp"
#include "conditions.hpp"
#include "error_handling.hpp"
#include "goal.hpp"
#include "initial.hpp"
#include "literal.hpp"
#include "loki/details/pddl/domain.hpp"
#include "loki/details/pddl/exceptions.hpp"
#include "loki/details/pddl/problem_parsing_context.hpp"
#include "loki/details/pddl/requirements_enum.hpp"
#include "loki/details/pddl/scope.hpp"
#include "metric.hpp"
#include "objects.hpp"
#include "predicates.hpp"
#include "reference_utils.hpp"
#include "requirements.hpp"
#include "structure.hpp"
#include "unpacking_visitor.hpp"

namespace loki
{
void parse(const ast::Problem& node, ProblemParsingContext& context)
{
    /* Domain name */
    const auto domain_name = parse(node.domain_name.name);
    if (domain_name != context.builder.get_domain()->get_name())
    {
        throw MismatchedDomainError(context.builder.get_domain(), domain_name, context.scopes.top().get_error_handler()(node.domain_name, ""));
    }

    /* Problem name */
    context.builder.get_name() = parse(node.problem_name.name);

    /* Requirements */
    auto requirements_set = RequirementEnumSet { RequirementEnum::STRIPS };
    if (node.requirements.has_value())
    {
        const auto problem_requirements_set = parse(node.requirements.value(), context);
        requirements_set.insert(problem_requirements_set.begin(), problem_requirements_set.end());
    }
    const auto domain_requirements_set = context.builder.get_domain()->get_requirements()->get_requirements();
    requirements_set.insert(domain_requirements_set.begin(), domain_requirements_set.end());
    const auto requirements = context.builder.get_repositories().get_or_create_requirements(requirements_set);
    context.builder.get_requirements() = requirements;
    context.requirements = requirements;

    /* Objects section */
    auto objects = ObjectList {};
    if (node.objects.has_value())
    {
        objects = parse(node.objects.value(), context);
        context.builder.get_objects() = objects;
        track_object_references(objects, context);
    }

    /* Predicates section */
    auto predicates = PredicateList {};
    if (node.derived_predicates.has_value())
    {
        predicates = parse(node.derived_predicates.value(), context);
        context.builder.get_predicates() = predicates;
        track_predicate_references(predicates, context);
    }

    /* Initial section */
    if (node.initial.has_value())
    {
        auto initial_literals = LiteralList();
        auto function_values = FunctionValueList();
        const auto initial_elements = parse(node.initial.value(), context);
        for (const auto& initial_element : initial_elements)
        {
            std::visit(UnpackingVisitor(initial_literals, function_values), initial_element);
        }
        context.builder.get_initial_literals() = std::move(initial_literals);
        context.builder.get_initial_function_values() = std::move(function_values);
    }

    /* Goal section */
    if (node.goal.has_value())
    {
        context.builder.get_goal_condition() = parse(node.goal.value(), context);
    }

    /* Metric section */
    if (node.metric_specification.has_value())
    {
        context.builder.get_optimization_metric() = parse(node.metric_specification.value(), context);
    }

    /* Structure section */
    for (const auto& structure_node : node.structures)
    {
        auto axioms = AxiomList();
        auto actions = ActionList();
        auto variant = parse(structure_node, context);
        std::visit(UnpackingVisitor(actions, axioms), variant);
        context.builder.get_axioms() = std::move(axioms);
    }

    // Check references
    test_object_references(objects, context);
    test_predicate_references(predicates, context);
}
}
