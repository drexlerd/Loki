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

#include "types.hpp"

#include "common.hpp"
#include "error_handling.hpp"

using namespace std;

namespace loki
{

/* TypeDeclarationTypeVisitor */

TypeDeclarationTypeVisitor::TypeDeclarationTypeVisitor(Context& context_) : context(context_) {}

TypeList TypeDeclarationTypeVisitor::operator()(const ast::TypeObject& node)
{
    const auto type = context.factories.get_or_create_type("object", TypeList());
    context.positions.push_back(type, node);
    return { type };
}

TypeList TypeDeclarationTypeVisitor::operator()(const ast::TypeNumber& node)
{
    const auto type = context.factories.get_or_create_type("number", TypeList());
    context.positions.push_back(type, node);
    return { type };
}

TypeList TypeDeclarationTypeVisitor::operator()(const ast::Name& node)
{
    auto name = parse(node);
    const auto type = context.factories.get_or_create_type(name, TypeList());
    context.positions.push_back(type, node);
    return { type };
}

TypeList TypeDeclarationTypeVisitor::operator()(const ast::TypeEither& node)
{
    // we flatten nested either types
    TypeList type_list;
    for (auto& child_node : node.types)
    {
        auto types = boost::apply_visitor(*this, child_node);
        type_list.insert(type_list.end(), types.begin(), types.end());
    }
    return type_list;
}

/* TypeReferenceTypeVisitor */

TypeReferenceTypeVisitor::TypeReferenceTypeVisitor(const Context& context_) : context(context_) {}

TypeList TypeReferenceTypeVisitor::operator()(const ast::TypeObject&)
{
    const auto binding = context.scopes.top().get_type("object");
    assert(binding.has_value());
    const auto [type, _position, _error_handler] = binding.value();
    return { type };
}

TypeList TypeReferenceTypeVisitor::operator()(const ast::TypeNumber&)
{
    const auto binding = context.scopes.top().get_type("number");
    assert(binding.has_value());
    const auto [type, _position, _error_handler] = binding.value();
    return { type };
}

TypeList TypeReferenceTypeVisitor::operator()(const ast::Name& node)
{
    auto name = parse(node);
    test_undefined_type(name, node, context);
    auto binding = context.scopes.top().get_type(name);
    const auto [type, _position, _error_handler] = binding.value();
    context.positions.push_back(type, node);
    return { type };
}

TypeList TypeReferenceTypeVisitor::operator()(const ast::TypeEither& node)
{
    // we flatten nested either types
    auto type_list = TypeList();
    for (auto& child_node : node.types)
    {
        auto types = boost::apply_visitor(*this, child_node);
        type_list.insert(type_list.end(), types.begin(), types.end());
    }
    return type_list;
}

/* TypeDeclarationTypedListOfNamesVisitor */

static void insert_context_information(const Type& type, const ast::Name& node, Context& context)
{
    context.positions.push_back(type, node);
    context.scopes.top().insert_type(type->get_name(), type, node);
}

static Type parse_type_definition(const ast::Name& node, const TypeList& type_list, Context& context)
{
    const auto name = parse(node);
    const auto type = context.factories.get_or_create_type(name, type_list);
    test_reserved_type(type, node, context);
    test_multiple_definition_type(type, node, context);
    insert_context_information(type, node, context);
    return type;
}

static TypeList parse_type_definitions(const std::vector<ast::Name>& nodes, const TypeList& parent_type_list, Context& context)
{
    auto type_list = TypeList();
    for (const auto& node : nodes)
    {
        type_list.push_back(parse_type_definition(node, parent_type_list, context));
    }
    return type_list;
}

TypeDeclarationTypedListOfNamesVisitor::TypeDeclarationTypedListOfNamesVisitor(Context& context_) : context(context_) {}

TypeList TypeDeclarationTypedListOfNamesVisitor::operator()(const std::vector<ast::Name>& name_nodes)
{
    // std::vector<ast::Name> has single base type "object"
    assert(context.scopes.top().get_type("object").has_value());
    const auto [type_object, _position, _error_handler] = context.scopes.top().get_type("object").value();
    const auto type_list = parse_type_definitions(name_nodes, TypeList { type_object }, context);
    return type_list;
}

TypeList TypeDeclarationTypedListOfNamesVisitor::operator()(const ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node)
{
    // requires :typing
    test_undefined_requirement(RequirementEnum::TYPING, typed_list_of_names_recursively_node, context);
    context.references.untrack(RequirementEnum::TYPING);
    // TypedListOfNamesRecursively has user defined base types.
    // Note: we use reference visitor here because parent types must already be declared
    const auto parent_type_list = boost::apply_visitor(TypeReferenceTypeVisitor(context), typed_list_of_names_recursively_node.type);
    auto type_list = parse_type_definitions(typed_list_of_names_recursively_node.names, parent_type_list, context);
    // Recursively add types.
    const auto additional_types = boost::apply_visitor(*this, typed_list_of_names_recursively_node.typed_list_of_names.get());
    type_list.insert(type_list.end(), additional_types.begin(), additional_types.end());
    return type_list;
}

/* Other functions */

TypeList parse(const ast::Types& types_node, Context& context)
{
    return boost::apply_visitor(TypeDeclarationTypedListOfNamesVisitor(context), types_node.typed_list_of_names);
}

}
