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
#include "unpacking_visitor.hpp"

#include "../../../include/loki/domain/pddl/exceptions.hpp"
#include "../../../include/loki/domain/pddl/parser.hpp"
#include "../../../include/loki/domain/pddl/object.hpp"
#include "../../../include/loki/domain/pddl/parameter.hpp"
#include "../../../include/loki/domain/pddl/predicate.hpp"
#include "../../../include/loki/domain/pddl/type.hpp"
#include "../../../include/loki/domain/pddl/domain.hpp"
#include "../../../include/loki/domain/pddl/object.hpp"

#include "parser/common.hpp"
#include "parser/constants.hpp"
#include "parser/functions.hpp"
#include "parser/parameters.hpp"
#include "parser/predicates.hpp"
#include "parser/requirements.hpp"
#include "parser/types.hpp"
#include "parser/structure.hpp"

using namespace loki::domain;
using namespace std;


namespace loki {

pddl::Domain parse(const ast::Domain& domain_node, Context& context) {
    const auto domain_name = parse(domain_node.domain_name.name);
    /* Requirements section */
    if (domain_node.requirements.has_value()) {
        context.requirements = context.factories.requirements.get_or_create<pddl::RequirementsImpl>(
            parse(domain_node.requirements.value()));
    } else {
        // Default requirements
        context.requirements = context.factories.requirements.get_or_create<pddl::RequirementsImpl>(
            pddl::RequirementEnumSet{pddl::RequirementEnum::STRIPS});
    }
    /* Types section */
    pddl::TypeList types;
    if (domain_node.types.has_value()) {
        if (!context.requirements->test(pddl::RequirementEnum::TYPING)) {
            throw UndefinedRequirementError(pddl::RequirementEnum::TYPING, context.scopes.get_error_handler()(domain_node.types.value(), ""));
        }
        types = parse(domain_node.types.value(), context);
    }
    /* Constants section */
    pddl::ObjectList constants;
    if (domain_node.constants.has_value()) {
        constants = parse(domain_node.constants.value(), context);
    }
    /* Predicates section */
    pddl::PredicateList predicates;
    if (domain_node.predicates.has_value()) {
        predicates = parse(domain_node.predicates.value(), context);
    }
    /* Functions section */
    pddl::FunctionSkeletonList function_skeletons;
    if (domain_node.functions.has_value()) {
        function_skeletons = parse(domain_node.functions.value(), context);
    }
    /* Action Schema section */
    pddl::DerivedPredicateList derived_predicate_list;
    pddl::ActionList action_list;
    for (const auto& structure_node : domain_node.structures) {
        auto variant = boost::apply_visitor(StructureVisitor(context), structure_node);
        boost::apply_visitor(UnpackingVisitor(action_list, derived_predicate_list), variant);
    }
    // Check references
    // cannot do that with constants
    /*
    for (const auto& constant : constants) {
        if (!context.references.exists(constant)) {
            const auto& [_constant, position, error_handler] = context.scopes.get<pddl::ObjectImpl>(constant->get_name()).value();
            throw UnusedConstantError(constant->get_name(), error_handler(position.value(), ""));
        }
    }
    */
    return context.factories.domains.get_or_create<pddl::DomainImpl>(domain_name, context.requirements, types, constants, predicates, function_skeletons, action_list);
}

}
