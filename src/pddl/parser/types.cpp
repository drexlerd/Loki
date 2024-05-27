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

#include <algorithm>

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

CollectParentTypesHierarchyVisitor::CollectParentTypesHierarchyVisitor(Context& context_, std::unordered_map<std::string, Position>& type_last_occurrence_) :
    context(context_),
    type_last_occurrence(type_last_occurrence_)
{
}

std::unordered_set<std::string> CollectParentTypesHierarchyVisitor::operator()(const ast::TypeObject&) { return std::unordered_set<std::string> { "object" }; }

std::unordered_set<std::string> CollectParentTypesHierarchyVisitor::operator()(const ast::TypeNumber&)
{  //
    return std::unordered_set<std::string> { "number" };
}

std::unordered_set<std::string> CollectParentTypesHierarchyVisitor::operator()(const ast::Name& node)
{
    // Do not allow reserved types as user defined types!
    const auto type_name = parse(node);

    test_reserved_type(type_name, node, context);

    type_last_occurrence[type_name] = node;

    return std::unordered_set<std::string> { parse(node) };
}

std::unordered_set<std::string> CollectParentTypesHierarchyVisitor::operator()(const ast::TypeEither& node)
{
    auto type_names = std::unordered_set<std::string> {};
    for (const auto& type_node : node.types)
    {
        const auto nested_type_names = boost::apply_visitor(CollectParentTypesHierarchyVisitor(context, type_last_occurrence), type_node);

        type_names.insert(nested_type_names.begin(), nested_type_names.end());
    }

    return type_names;
}

CollectTypesHierarchyVisitor::CollectTypesHierarchyVisitor(Context& context_,
                                                           std::unordered_map<std::string, std::unordered_set<std::string>>& parent_types_,
                                                           std::unordered_map<std::string, Position>& type_last_occurrence_) :
    context(context_),
    child_types(parent_types_),
    type_last_occurrence(type_last_occurrence_)
{
}

void CollectTypesHierarchyVisitor::operator()(const std::vector<ast::Name>& nodes)
{
    for (const auto& name_node : nodes)
    {
        const auto child_type = parse(name_node);

        child_types["object"].insert(child_type);

        test_reserved_type(child_type, name_node, context);

        type_last_occurrence[child_type] = name_node;
    }
}

void CollectTypesHierarchyVisitor::operator()(const ast::TypedListOfNamesRecursively& node)
{
    // requires :typing
    test_undefined_requirement(RequirementEnum::TYPING, node, context);
    context.references.untrack(RequirementEnum::TYPING);

    const auto parent_types = boost::apply_visitor(CollectParentTypesHierarchyVisitor(context, type_last_occurrence), node.type);

    for (const auto& parent_type : parent_types)
    {
        for (const auto& name_node : node.names)
        {
            const auto child_type = parse(name_node);

            child_types[parent_type].insert(child_type);

            test_reserved_type(child_type, name_node, context);

            type_last_occurrence[child_type] = name_node;
        }
    }

    boost::apply_visitor(CollectTypesHierarchyVisitor(context, child_types, type_last_occurrence), node.typed_list_of_names.get());
}

/* Other functions */

template<typename T>
bool is_subseteq(const std::unordered_set<T>& set1, const std::unordered_set<T>& set2)
{
    for (const auto& element1 : set1)
    {
        if (!set2.count(element1))
        {
            return false;
        }
    }
    return true;
}

TypeList parse(const ast::Types& types_node, Context& context)
{
    auto child_types = std::unordered_map<std::string, std::unordered_set<std::string>> {};
    auto type_last_occurrence = std::unordered_map<std::string, Position> {};

    boost::apply_visitor(CollectTypesHierarchyVisitor(context, child_types, type_last_occurrence), types_node.typed_list_of_names);

    auto all_types = std::unordered_set<std::string> {};
    for (const auto& [parent, childs] : child_types)
    {
        for (const auto& child : childs)
        {
            all_types.insert(child);
        }
        all_types.insert(parent);
    }

    auto parent_types = std::unordered_map<std::string, std::unordered_set<std::string>> {};
    for (const auto& type : all_types)
    {
        parent_types.emplace(type, std::unordered_set<std::string> {});
    }

    for (const auto& [parent, childs] : child_types)
    {
        for (const auto& child : childs)
        {
            parent_types[child].insert(parent);
        }
    }

    const auto original_parent_types = parent_types;

    auto instantiated_types = std::unordered_map<std::string, Type> {};

    auto remaining = all_types;

    while (!remaining.empty())
    {
        // Find types for which all base types are instantiated.
        auto selected = std::vector<std::string> {};

        bool found = false;

        for (const auto& type : remaining)
        {
            if (parent_types.at(type).empty())
            {
                selected.push_back(type);

                found = true;
            }
        }

        if (!found)
        {
            throw std::runtime_error("Types hierarchy contains a circular dependency!");
        }

        // Instantiate the type
        for (const auto& type_name : selected)
        {
            remaining.erase(type_name);
            for (const auto& child_type : all_types)
            {
                parent_types[child_type].erase(type_name);
            }

            auto base_types = TypeList {};
            for (const auto& base_type_name : original_parent_types.at(type_name))
            {
                base_types.push_back(instantiated_types.at(base_type_name));
            }
            auto instantiated_type = context.factories.get_or_create_type(type_name, base_types);

            instantiated_types.emplace(type_name, instantiated_type);
        }
    }

    auto result = TypeList {};
    for (const auto& [type_name, type] : instantiated_types)
    {
        result.push_back(type);

        // Base types were already added to the context.
        if (type_name != "object" && type_name != "number")
        {
            context.scopes.top().insert_type(type_name, type, type_last_occurrence.at(type_name));
        }
    }

    return result;
}

}
