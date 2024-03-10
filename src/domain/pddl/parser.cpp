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

#include "parser.hpp"

#include "loki/domain/pddl/domain.hpp"
#include "loki/domain/pddl/exceptions.hpp"
#include "loki/domain/pddl/object.hpp"
#include "loki/domain/pddl/parameter.hpp"
#include "loki/domain/pddl/parser.hpp"
#include "loki/domain/pddl/predicate.hpp"
#include "loki/domain/pddl/type.hpp"
#include "parser/common.hpp"
#include "parser/constants.hpp"
#include "parser/functions.hpp"
#include "parser/parameters.hpp"
#include "parser/predicates.hpp"
#include "parser/reference_utils.hpp"
#include "parser/requirements.hpp"
#include "parser/structure.hpp"
#include "parser/types.hpp"
#include "unpacking_visitor.hpp"

using namespace loki::domain;
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

    const auto domain = context.factories.domains
                            .get_or_create<pddl::DomainImpl>(domain_name, context.requirements, types, constants, predicates, function_skeletons, action_list);
    context.positions.push_back(domain, domain_node);
    return domain;
}

}
