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

using namespace loki::problem;
using namespace std;


namespace loki {

pddl::Object parse_object_reference(const domain::ast::Name& name_node, Context& context) {
    const auto name = parse(name_node);
    const auto& scope = context.scopes.get_current_scope();
    const auto binding = scope.get<pddl::ObjectImpl>(name);
    if (!binding.has_value()) {
        throw UndefinedObjectError(name, binding.value().scope->get_error_message(name_node, ""));
    }
    return binding.value().value.object;
}

ObjectListVisitor::ObjectListVisitor(Context& context_)
    : context(context_) { }


pddl::ObjectList ObjectListVisitor::operator()(const std::vector<domain::ast::Name>& name_nodes) {
    // A visited vector of name has single base type "object"
    pddl::ObjectList object_list;
    const auto& current_scope = context.scopes.get_current_scope();
    const auto type_binding = current_scope.get<pddl::TypeImpl>("object");
    assert(type_binding.has_value());
    const auto type = type_binding.value().value.object;
    for (const auto& name_node : name_nodes) {
        const auto name = parse(name_node);
        auto domain_binding = current_scope.get<pddl::ObjectImpl>(name);
        if (domain_binding.has_value()) {
            const auto current_definition_error_message = current_scope.get_error_message(name_node, "Defined here:");
            auto first_definition_error_message = std::string("");
            const auto& first_definition_scope = domain_binding.value().scope;
            const auto position = domain_binding.value().value.position;
            if (position.has_value()) {
                first_definition_error_message = first_definition_scope->get_error_message(position.value(), "First defined here:");
            }
            throw ObjectIsConstantError(name, current_definition_error_message + first_definition_error_message);
        }
        auto problem_binding = context.scopes.get_current_scope().get<pddl::ObjectImpl>(name);
        if (problem_binding.has_value()) {
            context.error_handler(name_node, "Defined here:");
            if (problem_binding.value().position.has_value()) {
                context.error_handler(problem_binding.value().position.value(), "First defined here:");
            }
            throw MultiDefinitionObjectError(name, context.error_stream->str());
        }
        const auto object = context.cache.get_or_create<pddl::ObjectImpl>(name, pddl::TypeList{type});
        context.scopes.get_current_scope().insert<pddl::ObjectImpl>(name, object, name_node);
        object_list.emplace_back(object);
    }
    return object_list;
}

pddl::ObjectList ObjectListVisitor::operator()(const domain::ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node) {
    pddl::ObjectList object_list;
    const auto types = boost::apply_visitor(TypeReferenceTypeVisitor(context.domain_context),
                                            typed_list_of_names_recursively_node.type);
    // A non-visited vector of names has user defined base types
    for (const auto& name_node : typed_list_of_names_recursively_node.names) {
        const auto name = parse(name_node);
        auto domain_binding = context.domain_context.scopes.get_current_scope().get<pddl::ObjectImpl>(name);
        if (domain_binding.has_value()) {
            context.error_handler(name_node, "Defined here:");
            if (domain_binding.value().position.has_value()) {
                context.error_handler(domain_binding.value().position.value(), "First defined here:");
            }
            throw ObjectIsConstantError(name, context.error_stream->str() + context.domain_context.error_stream->str());
        }
        auto problem_binding = context.scopes.get_current_scope().get<pddl::ObjectImpl>(name);
        if (problem_binding.has_value()) {
            context.error_handler(name_node, "Defined here:");
            if (problem_binding.value().position.has_value()) {
                context.error_handler(problem_binding.value().position.value(), "First defined here:");
            }
            throw MultiDefinitionObjectError(name, context.error_stream->str());
        }
        const auto object = context.cache.get_or_create<pddl::ObjectImpl>(name, types);
        context.scopes.get_current_scope().insert<pddl::ObjectImpl>(name, object, name_node);
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
