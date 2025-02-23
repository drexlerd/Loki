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

#include "domain.hpp"

#include "common.hpp"
#include "constants.hpp"
#include "error_handling.hpp"
#include "functions.hpp"
#include "loki/details/pddl/domain_parsing_context.hpp"
#include "loki/details/pddl/exceptions.hpp"
#include "loki/details/pddl/requirements_enum.hpp"
#include "loki/details/pddl/scope.hpp"
#include "predicates.hpp"
#include "reference_utils.hpp"
#include "requirements.hpp"
#include "structure.hpp"
#include "types.hpp"
#include "unpacking_visitor.hpp"

namespace loki
{
void parse(const ast::Domain& node, DomainParsingContext& context)
{
    // Create base types.
    const auto base_type_object = context.builder.get_repositories().get_or_create_type("object", TypeList());
    const auto base_type_number = context.builder.get_repositories().get_or_create_type("number", TypeList());
    context.scopes.top().insert_type("object", base_type_object, {});
    context.scopes.top().insert_type("number", base_type_number, {});

    /* Domain name */
    context.builder.get_name() = parse(node.domain_name.name);

    /* Requirements */
    auto requirements_set = RequirementEnumSet { RequirementEnum::STRIPS };
    if (node.requirements.has_value())
    {
        const auto domain_requirements_set = parse(node.requirements.value(), context);
        requirements_set.insert(domain_requirements_set.begin(), domain_requirements_set.end());
    }
    const auto requirements = context.builder.get_repositories().get_or_create_requirements(requirements_set);
    context.builder.get_requirements() = requirements;
    context.requirements = requirements;

    /* Types section */
    auto types = TypeList();
    if (node.types.has_value())
    {
        if (!context.requirements->test(RequirementEnum::TYPING))
        {
            throw UndefinedRequirementError(RequirementEnum::TYPING, context.scopes.top().get_error_handler()(node.types.value(), ""));
        }
        types = parse(node.types.value(), context);
        context.builder.get_types().insert(context.builder.get_types().end(), types.begin(), types.end());
    }
    /* Constants section */
    if (node.constants.has_value())
    {
        const auto constants = parse(node.constants.value(), context);
        context.builder.get_constants().insert(context.builder.get_constants().end(), constants.begin(), constants.end());
        track_object_references(constants, context);
    }

    /* Predicates section */
    auto predicates = PredicateList {};
    if (node.predicates.has_value())
    {
        predicates = parse(node.predicates.value(), context);
        context.builder.get_predicates().insert(context.builder.get_predicates().end(), predicates.begin(), predicates.end());
        track_predicate_references(predicates, context);
    }
    if (context.requirements->test(RequirementEnum::EQUALITY))
    {
        // Create equal predicate with name "=" and two parameters "?left_arg" and "?right_arg"
        const auto binary_parameterlist =
            ParameterList { context.builder.get_repositories().get_or_create_parameter(context.builder.get_repositories().get_or_create_variable("?lhs"),
                                                                                       TypeList { base_type_object }),
                            context.builder.get_repositories().get_or_create_parameter(context.builder.get_repositories().get_or_create_variable("?rhs"),
                                                                                       TypeList { base_type_object })

            };
        const auto equal_predicate = context.builder.get_repositories().get_or_create_predicate("=", binary_parameterlist);
        context.scopes.top().insert_predicate("=", equal_predicate, {});
        context.builder.get_predicates().push_back(equal_predicate);
    }

    /* Functions section */
    auto function_skeletons = FunctionSkeletonList();
    if (node.functions.has_value())
    {
        function_skeletons = parse(node.functions.value(), context);
        context.builder.get_function_skeletons().insert(context.builder.get_function_skeletons().end(), function_skeletons.begin(), function_skeletons.end());
        track_function_skeleton_references(function_skeletons, context);
    }

    /* Structure section */
    for (const auto& structure_node : node.structures)
    {
        auto axioms = AxiomList();
        auto actions = ActionList();
        auto variant = parse(structure_node, context);
        std::visit(UnpackingVisitor(actions, axioms), variant);
        context.builder.get_actions().insert(context.builder.get_actions().end(), actions.begin(), actions.end());
        context.builder.get_axioms().insert(context.builder.get_axioms().end(), axioms.begin(), axioms.end());
    }

    // Check references
    test_predicate_references(predicates, context);
    test_function_skeleton_references(function_skeletons, context);
}
}
