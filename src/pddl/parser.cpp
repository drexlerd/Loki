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

#include "loki/details/pddl/parser.hpp"

#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/domain.hpp"
#include "loki/details/pddl/exceptions.hpp"
#include "loki/details/pddl/object.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/pddl/predicate.hpp"
#include "loki/details/pddl/type.hpp"
#include "parser/common.hpp"
#include "parser/constants.hpp"
#include "parser/error_handling.hpp"
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

Domain parse(const ast::Domain& domain_node, Context& context)
{
    const auto domain_name = parse(domain_node.domain_name.name);
    /* Requirements section */
    // Make :strips requirement explicit
    auto requirements_set = RequirementEnumSet { RequirementEnum::STRIPS };
    if (domain_node.requirements.has_value())
    {
        const auto domain_requirements_set = parse(domain_node.requirements.value(), context);
        requirements_set.insert(domain_requirements_set.begin(), domain_requirements_set.end());
    }
    const auto requirements = context.factories.get_or_create_requirements(requirements_set);
    context.requirements = requirements;
    if (domain_node.requirements.has_value())
    {
        context.positions.push_back(requirements, domain_node.requirements.value());
    }
    /* Types section */
    auto types = TypeList();
    if (domain_node.types.has_value())
    {
        if (!context.requirements->test(RequirementEnum::TYPING))
        {
            throw UndefinedRequirementError(RequirementEnum::TYPING, context.scopes.top().get_error_handler()(domain_node.types.value(), ""));
        }
        types = parse(domain_node.types.value(), context);
    }
    /* Constants section */
    auto constants = ObjectList();
    if (domain_node.constants.has_value())
    {
        constants = parse(domain_node.constants.value(), context);
    }
    /* Predicates section */
    auto predicates = PredicateList();
    if (domain_node.predicates.has_value())
    {
        predicates = parse(domain_node.predicates.value(), context);
    }
    track_predicate_references(predicates, context);
    /* DerivedPredicates section */
    auto derived_predicates = PredicateList();
    if (domain_node.derived_predicates.has_value())
    {
        derived_predicates = parse(domain_node.derived_predicates.value(), context);
    }
    track_predicate_references(derived_predicates, context);
    /* Functions section */
    auto function_skeletons = FunctionSkeletonList();
    if (domain_node.functions.has_value())
    {
        function_skeletons = parse(domain_node.functions.value(), context);
    }
    track_function_skeleton_references(function_skeletons, context);
    /* Structure section */
    auto axiom_list = AxiomList();
    auto action_list = ActionList();
    for (const auto& structure_node : domain_node.structures)
    {
        auto variant = boost::apply_visitor(StructureVisitor(context), structure_node);
        boost::apply_visitor(UnpackingVisitor(action_list, axiom_list), variant);
    }
    // Check references
    test_predicate_references(predicates, context);
    test_predicate_references(derived_predicates, context);
    test_function_skeleton_references(function_skeletons, context);

    const auto domain =
        context.factories
            .get_or_create_domain(domain_name, requirements, types, constants, predicates, derived_predicates, function_skeletons, action_list, axiom_list);
    context.positions.push_back(domain, domain_node);
    return domain;
}

Problem parse(const ast::Problem& problem_node, Context& context, const Domain& domain)
{
    /* Domain name section */
    const auto domain_name = parse(problem_node.domain_name.name);
    if (domain_name != domain->get_name())
    {
        throw MismatchedDomainError(domain, domain_name, context.scopes.top().get_error_handler()(problem_node.domain_name, ""));
    }

    /* Problem name section */
    const auto problem_name = parse(problem_node.problem_name.name);

    /* Requirements section */
    // Make :strips requirement explicit
    auto requirements_set = RequirementEnumSet { RequirementEnum::STRIPS };
    if (problem_node.requirements.has_value())
    {
        // Keep the problem requirements as is.
        const auto problem_requirements_set = parse(problem_node.requirements.value(), context);
        requirements_set.insert(problem_requirements_set.begin(), problem_requirements_set.end());
    }
    // Create a problem specific requirement
    const auto requirements = context.factories.get_or_create_requirements(requirements_set);
    //  Copy domain requirements over to the parsing context of the problem
    const auto& domain_requirements_set = domain->get_requirements()->get_requirements();
    requirements_set.insert(domain_requirements_set.begin(), domain_requirements_set.end());
    context.requirements = context.factories.get_or_create_requirements(requirements_set);
    if (problem_node.requirements.has_value())
    {
        context.positions.push_back(requirements, problem_node.requirements.value());
    }

    /* Objects section */
    auto objects = ObjectList();
    if (problem_node.objects.has_value())
    {
        objects = parse(problem_node.objects.value(), context);
    }
    track_object_references(objects, context);

    /* DerivedPredicates section */
    auto derived_predicates = PredicateList();
    if (problem_node.derived_predicates.has_value())
    {
        derived_predicates = parse(problem_node.derived_predicates.value(), context);
    }
    track_predicate_references(derived_predicates, context);

    /* Initial section */
    auto initial_literals = LiteralList();
    auto numeric_fluents = NumericFluentList();
    if (problem_node.initial.has_value())
    {
        const auto initial_elements = parse(problem_node.initial.value(), context);
        for (const auto& initial_element : initial_elements)
        {
            std::visit(UnpackingVisitor(initial_literals, numeric_fluents), initial_element);
        }
    }

    /* Goal section */
    auto goal_condition = std::optional<Condition>();
    if (problem_node.goal.has_value())
    {
        goal_condition = parse(problem_node.goal.value(), context);
    }

    /* Metric section */
    auto optimization_metric = std::optional<OptimizationMetric>();
    if (problem_node.metric_specification.has_value())
    {
        optimization_metric = parse(problem_node.metric_specification.value(), context);
    }

    // Check references
    if (context.strict)
    {
        test_object_references(objects, context);
        test_predicate_references(derived_predicates, context);
    }

    /* Structure section */
    auto axioms = AxiomList();
    if (problem_node.axioms.has_value())
    {
        for (const auto& axiom_node : problem_node.axioms.value())
        {
            axioms.push_back(parse(axiom_node, context));
        }
    }

    const auto problem = context.factories.get_or_create_problem(domain,
                                                                 problem_name,
                                                                 requirements,
                                                                 objects,
                                                                 derived_predicates,
                                                                 initial_literals,
                                                                 numeric_fluents,
                                                                 goal_condition,
                                                                 optimization_metric,
                                                                 axioms);
    context.positions.push_back(problem, problem_node);
    return problem;
}

}
