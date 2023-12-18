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

#include "objects.hpp"

#include "../../../domain/pddl/parser/common.hpp"
#include "../../../domain/pddl/parser/types.hpp"
#include "../../../../include/loki/problem/pddl/exceptions.hpp"
#include "../../../../include/loki/domain/pddl/exceptions.hpp"


using namespace loki::problem;
using namespace std;


namespace loki {

ObjectListVisitor::ObjectListVisitor(Context& context_)
    : context(context_) { }


pddl::Object parse_object_reference(const domain::ast::Name& name_node, Context& context) {
    const auto name = parse(name_node);
    const auto binding = context.scopes.get<pddl::ObjectImpl>(name);
    if (!binding.has_value()) {
        throw UndefinedObjectError(name, context.scopes.get_error_handler()(name_node, ""));
    }
    const auto& [object, _position, _error_handler] = binding.value();
    return object;
}


pddl::ObjectList ObjectListVisitor::operator()(const std::vector<domain::ast::Name>& name_nodes) {
    // A visited vector of name has single base type "object"
    auto object_list = pddl::ObjectList();
    assert(context.scopes.get<pddl::TypeImpl>("object").has_value());
    const auto& [type, _position, _error_handler] = context.scopes.get<pddl::TypeImpl>("object").value();
    for (const auto& name_node : name_nodes) {
        const auto name = parse(name_node);
        const auto binding = context.scopes.get<pddl::ObjectImpl>(name);
        if (binding.has_value()) {
            const auto message_1 = context.scopes.get_error_handler()(name_node, "Defined here:");
            auto message_2 = std::string("");
            const auto& [_object, position, error_handler] = binding.value();
            if (position.has_value()) {
                message_2 = error_handler(position.value(), "First defined here:");
            }
            throw MultiDefinitionObjectError(name, message_1 + message_2);
        }
        const auto object = context.factories.objects.get_or_create<pddl::ObjectImpl>(name, pddl::TypeList{type});
        context.scopes.insert<pddl::ObjectImpl>(name, object, name_node);
        object_list.emplace_back(object);
    }
    return object_list;
}

pddl::ObjectList ObjectListVisitor::operator()(const domain::ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node) {
    if (!context.requirements->test(pddl::RequirementEnum::TYPING)) {
        throw UnsupportedRequirementError(pddl::RequirementEnum::TYPING, context.scopes.get_error_handler()(typed_list_of_names_recursively_node, ""));
    }
    context.referenced_enums.untrack(pddl::RequirementEnum::TYPING);
    auto object_list = pddl::ObjectList();
    const auto types = boost::apply_visitor(TypeReferenceTypeVisitor(context),
                                            typed_list_of_names_recursively_node.type);
    // A non-visited vector of names has user defined base types
    for (const auto& name_node : typed_list_of_names_recursively_node.names) {
        const auto name = parse(name_node);
        const auto binding = context.scopes.get<pddl::ObjectImpl>(name);
        if (binding.has_value()) {
            const auto message_1 = context.scopes.get_error_handler()(name_node, "Defined here:");
            auto message_2 = std::string("");
            const auto& [_object, position, error_handler] = binding.value();
            if (position.has_value()) {
                message_2 = error_handler(position.value(), "First defined here:");
            }
            throw MultiDefinitionObjectError(name, message_1 + message_2);
        }
        const auto object = context.factories.objects.get_or_create<pddl::ObjectImpl>(name, types);
        context.scopes.insert<pddl::ObjectImpl>(name, object, name_node);
        object_list.emplace_back(object);
    }
    // Recursively add objects.
    auto additional_objects = boost::apply_visitor(ObjectListVisitor(context), typed_list_of_names_recursively_node.typed_list_of_names.get());
    object_list.insert(object_list.end(), additional_objects.begin(), additional_objects.end());
    return object_list;
}

pddl::ObjectList parse(const ast::Objects& objects_node, Context& context) {
    return boost::apply_visitor(ObjectListVisitor(context), objects_node.typed_list_of_names);
}

}
