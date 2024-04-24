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

#include "common.hpp"
#include "error_handling.hpp"
#include "loki/details/pddl/exceptions.hpp"
#include "types.hpp"

using namespace std;

namespace loki
{

ObjectListVisitor::ObjectListVisitor(Context& context_) : context(context_) {}

Object parse_object_reference(const ast::Name& name_node, Context& context)
{
    const auto name = parse(name_node);
    test_undefined_object(name, name_node, context);
    const auto binding = context.scopes.top().get_object(name);
    const auto [object, _position, _error_handler] = binding.value();
    context.positions.push_back(object, name_node);
    context.references.untrack(object);
    return object;
}

static void insert_context_information(const Object& object, const ast::Name& node, Context& context)
{
    context.positions.push_back(object, node);
    context.scopes.top().insert_object(object->get_name(), object, node);
}

static Object parse_object_definition(const ast::Name& name_node, const TypeList& type_list, Context& context)
{
    const auto object = context.factories.get_or_create_object(parse(name_node), type_list);
    test_multiple_definition_object(object, name_node, context);
    insert_context_information(object, name_node, context);
    return object;
}

static ObjectList parse_object_definitions(const std::vector<ast::Name>& name_nodes, const TypeList& type_list, Context& context)
{
    auto object_list = ObjectList();
    for (const auto& name_node : name_nodes)
    {
        object_list.emplace_back(parse_object_definition(name_node, type_list, context));
    }
    return object_list;
}

ObjectList ObjectListVisitor::operator()(const std::vector<ast::Name>& name_nodes)
{
    // std::vector<ast::Name> has single base type "object"
    assert(context.scopes.top().get_type("object").has_value());
    const auto [type, _position, _error_handler] = context.scopes.top().get_type("object").value();
    auto object_list = parse_object_definitions(name_nodes, TypeList { type }, context);
    return object_list;
}

ObjectList ObjectListVisitor::operator()(const ast::TypedListOfNamesRecursively& node)
{
    test_undefined_requirement(RequirementEnum::TYPING, node, context);
    context.references.untrack(RequirementEnum::TYPING);
    // TypedListOfNamesRecursively has user defined base types
    const auto type_list = boost::apply_visitor(TypeReferenceTypeVisitor(context), node.type);
    auto object_list = parse_object_definitions(node.names, type_list, context);
    // Recursively add objects.
    auto additional_objects = boost::apply_visitor(*this, node.typed_list_of_names.get());
    object_list.insert(object_list.end(), additional_objects.begin(), additional_objects.end());
    return object_list;
}

ObjectList parse(const ast::Objects& objects_node, Context& context)
{
    return boost::apply_visitor(ObjectListVisitor(context), objects_node.typed_list_of_names);
}

}
