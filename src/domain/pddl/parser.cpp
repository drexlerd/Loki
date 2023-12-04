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

std::string parse(const domain::ast::DomainName& domain_name_node, const error_handler_type& error_handler, domain::Context& context) {
    return parse(domain_name_node.name);
}

pddl::Domain parse(const ast::Domain& domain_node, const error_handler_type& error_handler, Context& context) {
    const auto domain_name = parse(domain_node.domain_name, error_handler, context);
    /* Requirements section */
    if (domain_node.requirements.has_value()) {
        context.requirements = parse(domain_node.requirements.value(), error_handler, context);
    } else {
        // Default requirements
        context.requirements = context.cache.get_or_create<pddl::RequirementsImpl>(
            pddl::RequirementEnumSet{pddl::RequirementEnum::STRIPS});
    }
    /* Types section */
    pddl::TypeList types;
    if (domain_node.types.has_value()) {
        if (!context.requirements->test(pddl::RequirementEnum::TYPING)) {
            error_handler(domain_node.types.value(), "");
            throw UndefinedRequirementError(pddl::RequirementEnum::TYPING, context.error_stream->str());
        }
        types = parse(domain_node.types.value(), error_handler, context);
    }
    /* Constants section */
    pddl::ObjectList constants;
    if (domain_node.constants.has_value()) {
        constants = parse(domain_node.constants.value(), error_handler, context);
    }
    /* Predicates section */
    pddl::PredicateList predicates;
    if (domain_node.predicates.has_value()) {
        predicates = parse(domain_node.predicates.value(), error_handler, context);
    }
    /* Functions section */
    pddl::FunctionSkeletonList function_skeletons;
    if (domain_node.functions.has_value()) {
        function_skeletons = parse(domain_node.functions.value(), error_handler, context);
    }
    /* Action Schema section */
    pddl::DerivedPredicateList derived_predicate_list;
    pddl::ActionList action_list;
    for (const auto& structure_node : domain_node.structures) {
        auto variant = boost::apply_visitor(StructureVisitor(error_handler, context), structure_node);
        boost::apply_visitor(UnpackingVisitor(error_handler, context, action_list, derived_predicate_list), variant);
    }
    return context.cache.get_or_create<pddl::DomainImpl>(domain_name, context.requirements, types, constants, predicates);
}

}
