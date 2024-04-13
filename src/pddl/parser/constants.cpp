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
#include "loki/pddl/exceptions.hpp"
#include "types.hpp"

using namespace std;

namespace loki
{
static void test_multiple_definition(const Object& constant, const ast::Name& node, const Context& context)
{
    const auto constant_name = constant->get_name();
    const auto binding = context.scopes.get<ObjectImpl>(constant_name);
    if (binding.has_value())
    {
        const auto message_1 = context.scopes.get_error_handler()(node, "Defined here:");
        auto message_2 = std::string("");
        const auto [_object, position, error_handler] = binding.value();
        if (position.has_value())
        {
            message_2 = error_handler(position.value(), "First defined here:");
        }
        throw MultiDefinitionConstantError(constant_name, message_1 + message_2);
    }
}

static void insert_context_information(const Object& constant, const ast::Name& node, Context& context)
{
    context.positions.push_back(constant, node);
    context.scopes.insert(constant->get_name(), constant, node);
}

static Object parse_constant_definition(const ast::Name& node, const TypeList& type_list, Context& context)
{
    const auto constant = context.factories.get_or_create_object(parse(node), type_list);
    test_multiple_definition(constant, node, context);
    insert_context_information(constant, node, context);
    return constant;
}

static ObjectList parse_constant_definitions(const std::vector<ast::Name>& nodes, const TypeList& type_list, Context& context)
{
    auto constant_list = ObjectList();
    for (const auto& node : nodes)
    {
        constant_list.push_back(parse_constant_definition(node, type_list, context));
    }
    return constant_list;
}

ObjectList parse(const ast::Constants& constants_node, Context& context)
{
    return boost::apply_visitor(ConstantListVisitor(context), constants_node.typed_list_of_names.get());
}

ConstantListVisitor::ConstantListVisitor(Context& context_) : context(context_) {}

ObjectList ConstantListVisitor::operator()(const std::vector<ast::Name>& name_nodes)
{
    // std::vector<ast::Name> has single base type "object"
    assert(context.scopes.get<TypeImpl>("object").has_value());
    const auto [type, _position, _error_handler] = context.scopes.get<TypeImpl>("object").value();
    return parse_constant_definitions(name_nodes, TypeList { type }, context);
}

ObjectList ConstantListVisitor::operator()(const ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node)
{
    if (!context.requirements->test(RequirementEnum::TYPING))
    {
        throw UndefinedRequirementError(RequirementEnum::TYPING, context.scopes.get_error_handler()(typed_list_of_names_recursively_node, ""));
    }
    context.references.untrack(RequirementEnum::TYPING);
    const auto type_list = boost::apply_visitor(TypeReferenceTypeVisitor(context), typed_list_of_names_recursively_node.type);
    // TypedListOfNamesRecursively has user defined base types
    auto constant_list = parse_constant_definitions(typed_list_of_names_recursively_node.names, type_list, context);
    // Recursively add objects.
    auto additional_objects = boost::apply_visitor(ConstantListVisitor(context), typed_list_of_names_recursively_node.typed_list_of_names.get());
    constant_list.insert(constant_list.end(), additional_objects.begin(), additional_objects.end());
    return constant_list;
}

}
