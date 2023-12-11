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

#include "constants.hpp"
#include "common.hpp"
#include "types.hpp"

#include "../../../../include/loki/domain/pddl/exceptions.hpp"

using namespace loki::domain;
using namespace std;


namespace loki {
ConstantListVisitor::ConstantListVisitor(Context& context_)
    : context(context_) { }


pddl::ObjectList ConstantListVisitor::operator()(const std::vector<ast::Name>& name_nodes) {
    // A visited vector of name has single base type "object"
    pddl::ObjectList object_list;
    const auto type = context.cache.get_or_create<pddl::TypeImpl>("object");
    for (const auto& name_node : name_nodes) {
        const auto name = parse(name_node);
        const auto binding = context.scopes->get<pddl::ObjectImpl>(name);
        if (binding.has_value()) {
            const auto message_1 = context.scopes->get_error_handler()(name_node, "Defined here:");
            auto message_2 = std::string("");
            if (binding.value().value.position.has_value()) {
                message_2 = binding.value().error_handler(binding.value().value.position.value(), "First defined here:");
            } else {
                // Reserved type?
            }
            throw MultiDefinitionConstantError(name, message_1 + message_2);
        }
        const auto object = context.cache.get_or_create<pddl::ObjectImpl>(name, pddl::TypeList{type});
        context.scopes->insert<pddl::ObjectImpl>(name, object, name_node);
        object_list.emplace_back(object);
    }
    return object_list;
}

pddl::ObjectList ConstantListVisitor::operator()(const ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node) {
    pddl::ObjectList object_list;
    const auto types = boost::apply_visitor(TypeReferenceTypeVisitor(context),
                                            typed_list_of_names_recursively_node.type);
    // A non-visited vector of names has user defined base types
    for (const auto& name_node : typed_list_of_names_recursively_node.names) {
        const auto name = parse(name_node);
        const auto binding = context.scopes->get<pddl::ObjectImpl>(name);
        if (binding.has_value()) {
            const auto message_1 = context.scopes->get_error_handler()(name_node, "Defined here:");
            auto message_2 = std::string("");
            if (binding.value().value.position.has_value()) {
                message_2 = binding.value().error_handler(binding.value().value.position.value(), "First defined here:");
            } else {
                // Reserved type?
            }
            throw MultiDefinitionConstantError(name, message_1 + message_2);
        }
        const auto object = context.cache.get_or_create<pddl::ObjectImpl>(name, types);
        context.scopes->insert<pddl::ObjectImpl>(name, object, name_node);
        object_list.emplace_back(object);
    }
    // Recursively add objects.
    auto additional_objects = boost::apply_visitor(ConstantListVisitor(context), typed_list_of_names_recursively_node.typed_list_of_names.get());
    object_list.insert(object_list.end(), additional_objects.begin(), additional_objects.end());
    return object_list;
}

pddl::ObjectList parse(const ast::Constants& constants_node, Context& context) {
    return boost::apply_visitor(ConstantListVisitor(context), constants_node.typed_list_of_names);
}

}
