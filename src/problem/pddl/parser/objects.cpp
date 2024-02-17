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

#include <loki/problem/pddl/exceptions.hpp>
#include <loki/domain/pddl/exceptions.hpp>


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
    const auto [object, _position, _error_handler] = binding.value();
    context.positions.push_back(object, name_node);
    context.references.untrack(object);
    return object;
}


static void test_multiple_definition_object(const std::string& object_name, const domain::ast::Name& name_node, const Context& context) {
    const auto binding = context.scopes.get<pddl::ObjectImpl>(object_name);
    if (binding.has_value()) {
        const auto message_1 = context.scopes.get_error_handler()(name_node, "Defined here:");
        auto message_2 = std::string("");
        const auto [_object, position, error_handler] = binding.value();
        if (position.has_value()) {
            message_2 = error_handler(position.value(), "First defined here:");
        }
        throw MultiDefinitionObjectError(object_name, message_1 + message_2);
    }
}


static pddl::Object parse_object_definition(const domain::ast::Name& name_node, const pddl::TypeList& type_list, Context& context) {
    const auto name = parse(name_node);
    test_multiple_definition_object(name, name_node, context);
    const auto object = context.factories.objects.get_or_create<pddl::ObjectImpl>(name, type_list);
    context.positions.push_back(object, name_node);
    context.scopes.insert<pddl::ObjectImpl>(name, object, name_node);
    return object;
}


static pddl::ObjectList parse_object_definitions(const std::vector<domain::ast::Name>& name_nodes, const pddl::TypeList& type_list, Context& context) {
    auto object_list = pddl::ObjectList();
    for (const auto& name_node : name_nodes) {
        object_list.emplace_back(parse_object_definition(name_node, type_list, context));
    }
    return object_list;
}


pddl::ObjectList ObjectListVisitor::operator()(const std::vector<domain::ast::Name>& name_nodes) {
    // std::vector<domain::ast::Name> has single base type "object"
    assert(context.scopes.get<pddl::TypeImpl>("object").has_value());
    const auto [type, _position, _error_handler] = context.scopes.get<pddl::TypeImpl>("object").value();
    auto object_list = parse_object_definitions(name_nodes, pddl::TypeList{type}, context);
    return object_list;
}

pddl::ObjectList ObjectListVisitor::operator()(const domain::ast::TypedListOfNamesRecursively& node) {
    if (!context.requirements->test(pddl::RequirementEnum::TYPING)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::TYPING, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::TYPING);
    // TypedListOfNamesRecursively has user defined base types
    const auto type_list = boost::apply_visitor(TypeReferenceTypeVisitor(context), node.type);
    auto object_list = parse_object_definitions(node.names, type_list, context);
    // Recursively add objects.
    auto additional_objects = boost::apply_visitor(ObjectListVisitor(context), node.typed_list_of_names.get());
    object_list.insert(object_list.end(), additional_objects.begin(), additional_objects.end());
    return object_list;
}

pddl::ObjectList parse(const ast::Objects& objects_node, Context& context) {
    return boost::apply_visitor(ObjectListVisitor(context), objects_node.typed_list_of_names);
}

}
