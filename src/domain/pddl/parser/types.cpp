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

pddl::TypeList TypeDeclarationTypeVisitor::operator()(const ast::Type& type_node) {
    return boost::apply_visitor(*this, type_node);
}

pddl::TypeList TypeDeclarationTypeVisitor::operator()(const domain::ast::Name& name_node) {
    auto name = parse(name_node);
    return { context.factories.types.get_or_create<pddl::TypeImpl>(name) };
}

pddl::TypeList TypeDeclarationTypeVisitor::operator()(const ast::TypeEither& either_type_node) {
    // we flatten nested either types
    pddl::TypeList type_list;
    for (auto& type_node : either_type_node.types) {
        auto types = this->operator()(type_node);
        type_list.insert(type_list.end(), types.begin(), types.end());
    }
    return type_list;
}

/* TypeReferenceTypeVisitor */

TypeReferenceTypeVisitor::TypeReferenceTypeVisitor(const Context& context_)
    : context(context_) { }

pddl::TypeList TypeReferenceTypeVisitor::operator()(const ast::Type& type_node) {
    return boost::apply_visitor(*this, type_node);
}

pddl::TypeList TypeReferenceTypeVisitor::operator()(const domain::ast::Name& name_node) {
    auto name = parse(name_node);
    auto binding = context.scopes.get<pddl::TypeImpl>(name);
    if (!binding.has_value()) {
        throw UndefinedTypeError(name, context.scopes.get_error_handler()(name_node, ""));
    }
    const auto& [type, _position, _error_handler] = binding.value();
    return { type };
}

pddl::TypeList TypeReferenceTypeVisitor::operator()(const ast::TypeEither& either_type_node) {
    // we flatten nested either types
    pddl::TypeList type_list;
    for (auto& type_node : either_type_node.types) {
        auto types = this->operator()(type_node);
        type_list.insert(type_list.end(), types.begin(), types.end());
    }
    return type_list;
}

/* TypeDeclarationTypedListOfNamesVisitor */

TypeDeclarationTypedListOfNamesVisitor::TypeDeclarationTypedListOfNamesVisitor(Context& context_)
    : context(context_) { }

pddl::TypeList TypeDeclarationTypedListOfNamesVisitor::operator()(const std::vector<domain::ast::Name>& name_nodes) {
    // A visited vector of name has single base type "object"
    pddl::TypeList type_list;
    assert(context.scopes.get<pddl::TypeImpl>("object").has_value());
    const auto& [type_object, _position, _error_handler] = context.scopes.get<pddl::TypeImpl>("object").value();
    for (const auto& name_node : name_nodes) {
        const auto name = parse(name_node);
        const auto binding = context.scopes.get<pddl::TypeImpl>(name);
        if (binding.has_value()) {
            const auto message_1 = context.scopes.get_error_handler()(name_node, "Defined here:");
            auto message_2 = std::string("");
            const auto& [_type, position, error_handler] = binding.value();
            if (position.has_value()) {
                message_2 = error_handler(position.value(), "First defined here:");
            } else {
                // Reserved type?
            }
            throw MultiDefinitionTypeError(name, message_1 + message_2);
        }
        const auto type = context.factories.types.get_or_create<pddl::TypeImpl>(name, pddl::TypeList{type_object});
        type_list.push_back(type);
        context.scopes.insert<pddl::TypeImpl>(name, type, name_node);
    }
    return type_list;
}

pddl::TypeList TypeDeclarationTypedListOfNamesVisitor::operator()(const ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node) {
    // requires :typing
    if (!context.requirements->test(pddl::RequirementEnum::TYPING)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::TYPING, context.scopes.get_error_handler()(typed_list_of_names_recursively_node, ""));
    }
    context.referenced_enums.untrack(pddl::RequirementEnum::TYPING);
    pddl::TypeList type_list;
    const auto types = boost::apply_visitor(TypeDeclarationTypeVisitor(context),
                                            typed_list_of_names_recursively_node.type);
    // A non-visited vector of names has user defined base types.
    for (const auto& name_node : typed_list_of_names_recursively_node.names) {
        const auto name = parse(name_node);
        if (name == "object") {
            throw SemanticParserError("Unexpected type name \"object\". It is a reserved type name.", context.scopes.get_error_handler()(name_node, ""));
        }
        // We also reserve type name number although PDDL specification allows it.
        // However, this allows using regular types as function types for simplicity.
        if (name == "number") {
            throw SemanticParserError("Unexpected type name \"number\". It is a reserved type name.", context.scopes.get_error_handler()(name_node, ""));
        }
        const auto binding = context.scopes.get<pddl::TypeImpl>(name);
        if (binding.has_value()) {
            const auto message_1 = context.scopes.get_error_handler()(name_node, "Defined here:");
            auto message_2 = std::string("");
            const auto& [_type, position, error_handler] = binding.value();
            if (position.has_value()) {
                message_2 = error_handler(position.value(), "First defined here:");
            } else {
                // Reserved type?
            }
            throw MultiDefinitionTypeError(name, message_1 + message_2);
        }
        const auto type = context.factories.types.get_or_create<pddl::TypeImpl>(name, types);
        type_list.push_back(type);
        context.scopes.insert<pddl::TypeImpl>(name, type, name_node);
    }
    // Recursively add types.
    auto additional_types = boost::apply_visitor(TypeDeclarationTypedListOfNamesVisitor(context), typed_list_of_names_recursively_node.typed_list_of_names.get());
    type_list.insert(type_list.end(), additional_types.begin(), additional_types.end());
    return type_list;
}

/* Other functions */

pddl::TypeList parse(const ast::Types& types_node, Context& context) {
    return boost::apply_visitor(TypeDeclarationTypedListOfNamesVisitor(context), types_node.typed_list_of_names);
}

}
