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

#include "../../../../include/loki/domain/pddl/exceptions.hpp"

using namespace loki::domain;
using namespace std;


namespace loki {

/* TypeDeclarationTypeVisitor */

TypeDeclarationTypeVisitor::TypeDeclarationTypeVisitor(Context& context_)
    : context(context_) { }

pddl::TypeList TypeDeclarationTypeVisitor::operator()(const ast::TypeObject& node) {
    const auto type = context.factories.types.get_or_create<pddl::TypeImpl>("object");
    context.positions.push_back(type, node);
    return { type };
}

pddl::TypeList TypeDeclarationTypeVisitor::operator()(const ast::TypeNumber& node) {
    const auto type = context.factories.types.get_or_create<pddl::TypeImpl>("number");
    context.positions.push_back(type, node);
    return { type };
}

pddl::TypeList TypeDeclarationTypeVisitor::operator()(const domain::ast::Name& node) {
    auto name = parse(node);
    const auto type = context.factories.types.get_or_create<pddl::TypeImpl>(name);
    context.positions.push_back(type, node);
    return { type };
}

pddl::TypeList TypeDeclarationTypeVisitor::operator()(const ast::TypeEither& node) {
    // we flatten nested either types
    pddl::TypeList type_list;
    for (auto& child_node : node.types) {
        auto types = boost::apply_visitor(*this, child_node);
        type_list.insert(type_list.end(), types.begin(), types.end());
    }
    return type_list;
}

/* TypeReferenceTypeVisitor */

TypeReferenceTypeVisitor::TypeReferenceTypeVisitor(const Context& context_)
    : context(context_) { }

pddl::TypeList TypeReferenceTypeVisitor::operator()(const ast::TypeObject&) {
    const auto binding = context.scopes.get<pddl::TypeImpl>("object");
    assert(binding.has_value());
    const auto& [type, _position, _error_handler] = binding.value();
    return {type};
}

pddl::TypeList TypeReferenceTypeVisitor::operator()(const ast::TypeNumber&) {
    const auto binding = context.scopes.get<pddl::TypeImpl>("number");
    assert(binding.has_value());
    const auto& [type, _position, _error_handler] = binding.value();
    return {type};
}

pddl::TypeList TypeReferenceTypeVisitor::operator()(const domain::ast::Name& node) {
    auto name = parse(node);
    auto binding = context.scopes.get<pddl::TypeImpl>(name);
    if (!binding.has_value()) {
        throw UndefinedTypeError(name, context.scopes.get_error_handler()(node, ""));
    }
    const auto& [type, _position, _error_handler] = binding.value();
    context.positions.push_back(type, node);
    return { type };
}

pddl::TypeList TypeReferenceTypeVisitor::operator()(const ast::TypeEither& node) {
    // we flatten nested either types
    auto type_list = pddl::TypeList();
    for (auto& child_node : node.types) {
        auto types = boost::apply_visitor(*this, child_node);
        type_list.insert(type_list.end(), types.begin(), types.end());
    }
    return type_list;
}

/* TypeDeclarationTypedListOfNamesVisitor */
static void test_multiple_definition(const pddl::Type& type, const domain::ast::Name& node, const Context& context) {
    const auto type_name = type->get_name();
    const auto binding = context.scopes.get<pddl::TypeImpl>(type_name);
    if (binding.has_value()) {
        const auto message_1 = context.scopes.get_error_handler()(node, "Defined here:");
        auto message_2 = std::string("");
        const auto& [_type, position, error_handler] = binding.value();
        if (position.has_value()) {
            message_2 = error_handler(position.value(), "First defined here:");
        }
        throw MultiDefinitionTypeError(type_name, message_1 + message_2);
    }
}


static void test_reserved_type(const pddl::Type& type, const domain::ast::Name& node, const Context& context) {
    if (type->get_name() == "object") {
        throw ReservedTypeError("object", context.scopes.get_error_handler()(node, ""));
    }
    // We also reserve type name number although PDDL specification allows it.
    // However, this allows using regular types as function types for simplicity.
    if (type->get_name() == "number") {
        throw ReservedTypeError("number", context.scopes.get_error_handler()(node, ""));
    }
}


static void insert_context_information(const pddl::Type& type, const domain::ast::Name& node, Context& context) {
    context.positions.push_back(type, node);
    context.scopes.insert(type->get_name(), type, node);
}


static pddl::Type parse_type_definition(const domain::ast::Name& node, const pddl::TypeList& type_list, Context& context) {
    const auto name = parse(node);
    const auto type = context.factories.types.get_or_create<pddl::TypeImpl>(name, type_list);
    test_reserved_type(type, node, context);
    test_multiple_definition(type, node, context);
    insert_context_information(type, node, context);
    return type;
}


static pddl::TypeList parse_type_definitions(const std::vector<domain::ast::Name>& nodes, const pddl::TypeList& parent_type_list, Context& context) {
    auto type_list = pddl::TypeList();
    for (const auto& node : nodes) {
        type_list.push_back(parse_type_definition(node, parent_type_list, context));
    }
    return type_list;
}


TypeDeclarationTypedListOfNamesVisitor::TypeDeclarationTypedListOfNamesVisitor(Context& context_)
    : context(context_) { }

pddl::TypeList TypeDeclarationTypedListOfNamesVisitor::operator()(const std::vector<domain::ast::Name>& name_nodes) {
    // std::vector<domain::ast::Name> has single base type "object"
    assert(context.scopes.get<pddl::TypeImpl>("object").has_value());
    const auto& [type_object, _position, _error_handler] = context.scopes.get<pddl::TypeImpl>("object").value();
    const auto type_list = parse_type_definitions(name_nodes, pddl::TypeList{type_object}, context);
    return type_list;
}

pddl::TypeList TypeDeclarationTypedListOfNamesVisitor::operator()(const ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node) {
    // requires :typing
    if (!context.requirements->test(pddl::RequirementEnum::TYPING)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::TYPING, context.scopes.get_error_handler()(typed_list_of_names_recursively_node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::TYPING);
    // TypedListOfNamesRecursively has user defined base types.
    const auto parent_type_list = boost::apply_visitor(TypeDeclarationTypeVisitor(context),
                                            typed_list_of_names_recursively_node.type);
    auto type_list = parse_type_definitions(typed_list_of_names_recursively_node.names, parent_type_list, context);
    // Recursively add types.
    const auto additional_types = boost::apply_visitor(*this, typed_list_of_names_recursively_node.typed_list_of_names.get());
    type_list.insert(type_list.end(), additional_types.begin(), additional_types.end());
    return type_list;
}

/* Other functions */

pddl::TypeList parse(const ast::Types& types_node, Context& context) {
    return boost::apply_visitor(TypeDeclarationTypedListOfNamesVisitor(context), types_node.typed_list_of_names);
}

}
