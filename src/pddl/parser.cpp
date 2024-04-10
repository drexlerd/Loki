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

#include "loki/pddl/parser.hpp"

#include "loki/pddl/declarations.hpp"
#include "loki/pddl/domain.hpp"
#include "loki/pddl/exceptions.hpp"
#include "loki/pddl/object.hpp"
#include "loki/pddl/parameter.hpp"
#include "loki/pddl/predicate.hpp"
#include "loki/pddl/type.hpp"
#include "parser/common.hpp"
#include "parser/constants.hpp"
#include "parser/functions.hpp"
#include "parser/goal.hpp"
#include "parser/initial.hpp"
#include "parser/metric.hpp"
#include "parser/objects.hpp"
#include "parser/parameters.hpp"
#include "parser/predicates.hpp"
#include "parser/reference_utils.hpp"
#include "parser/requirements.hpp"
#include "parser/structure.hpp"
#include "parser/types.hpp"
#include "unpacking_visitor.hpp"

using namespace std;

namespace loki
{

pddl::Domain parse(const ast::Domain& domain_node, Context& context)
{
    const auto domain_name = parse(domain_node.domain_name.name);
    /* Requirements section */
    if (domain_node.requirements.has_value())
    {
        context.requirements = context.factories.requirements.get_or_create<pddl::RequirementsImpl>(parse(domain_node.requirements.value(), context));
        context.positions.push_back(context.requirements, domain_node.requirements.value());
    }
    else
    {
        // Default requirements
        context.requirements = context.factories.requirements.get_or_create<pddl::RequirementsImpl>(pddl::RequirementEnumSet { pddl::RequirementEnum::STRIPS });
    }
    /* Types section */
    auto types = pddl::TypeList();
    if (domain_node.types.has_value())
    {
        if (!context.requirements->test(pddl::RequirementEnum::TYPING))
        {
            throw UndefinedRequirementError(pddl::RequirementEnum::TYPING, context.scopes.get_error_handler()(domain_node.types.value(), ""));
        }
        types = parse(domain_node.types.value(), context);
    }
    /* Constants section */
    auto constants = pddl::ObjectList();
    if (domain_node.constants.has_value())
    {
        constants = parse(domain_node.constants.value(), context);
    }
    /* Predicates section */
    auto predicates = pddl::PredicateList();
    if (domain_node.predicates.has_value())
    {
        predicates = parse(domain_node.predicates.value(), context);
    }
    track_predicate_references(predicates, context);
    /* Functions section */
    auto function_skeletons = pddl::FunctionSkeletonList();
    if (domain_node.functions.has_value())
    {
        function_skeletons = parse(domain_node.functions.value(), context);
    }
    track_function_skeleton_references(function_skeletons, context);
    /* Action Schema section */
    auto derived_predicate_list = pddl::DerivedPredicateList();
    auto action_list = pddl::ActionList();
    for (const auto& structure_node : domain_node.structures)
    {
        auto variant = boost::apply_visitor(StructureVisitor(context), structure_node);
        boost::apply_visitor(UnpackingVisitor(action_list, derived_predicate_list), variant);
    }
    // Check references
    test_predicate_references(predicates, context);
    test_function_skeleton_references(function_skeletons, context);

    const auto domain = context.factories.domains.get_or_create<pddl::DomainImpl>(domain_name,
                                                                                  context.requirements,
                                                                                  types,
                                                                                  constants,
                                                                                  predicates,
                                                                                  function_skeletons,
                                                                                  action_list,
                                                                                  derived_predicate_list);
    context.positions.push_back(domain, domain_node);
    return domain;
}

pddl::Problem parse(const ast::Problem& problem_node, Context& context, const pddl::Domain& domain)
{
    /* Domain name section */
    const auto domain_name = parse(problem_node.domain_name.name);
    if (domain_name != domain->get_name())
    {
        throw MismatchedDomainError(domain, domain_name, context.scopes.get_error_handler()(problem_node.domain_name, ""));
    }
    /* Problem name section */
    const auto problem_name = parse(problem_node.problem_name.name);
    /* Requirements section */
    if (problem_node.requirements.has_value())
    {
        context.requirements = context.factories.requirements.get_or_create<pddl::RequirementsImpl>(parse(problem_node.requirements.value(), context));
        context.positions.push_back(context.requirements, problem_node.requirements.value());
    }
    else
    {
        // Default requirements
        context.requirements = context.factories.requirements.get_or_create<pddl::RequirementsImpl>(pddl::RequirementEnumSet { pddl::RequirementEnum::STRIPS });
    }
    /* Objects section */
    auto objects = pddl::ObjectList();
    if (problem_node.objects.has_value())
    {
        objects = parse(problem_node.objects.value(), context);
    }
    track_object_references(objects, context);
    /* Initial section */
    auto initial_literals = pddl::GroundLiteralList();
    auto numeric_fluents = pddl::NumericFluentList();
    const auto initial_elements = parse(problem_node.initial, context);
    for (const auto& initial_element : initial_elements)
    {
        std::visit(UnpackingVisitor(initial_literals, numeric_fluents), initial_element);
    }
    /* Goal section */
    const auto goal_condition = parse(problem_node.goal, context);
    /* Metric section */
    auto optimization_metric = std::optional<pddl::OptimizationMetric>();
    if (problem_node.metric_specification.has_value())
    {
        optimization_metric = parse(problem_node.metric_specification.value(), context);
    }

    // Check references
    test_object_references(objects, context);

    // Derived_predicates cannot be de defined for problems but may be added during translation
    auto derived_predicates = pddl::DerivedPredicateList {};

    const auto problem = context.factories.problems.get_or_create<pddl::ProblemImpl>(domain,
                                                                                     problem_name,
                                                                                     context.requirements,
                                                                                     objects,
                                                                                     initial_literals,
                                                                                     numeric_fluents,
                                                                                     goal_condition,
                                                                                     optimization_metric,
                                                                                     derived_predicates);
    context.positions.push_back(problem, problem_node);
    return problem;
}

}
