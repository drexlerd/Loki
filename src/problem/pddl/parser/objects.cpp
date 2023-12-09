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

pddl::Object parse_object_reference(const domain::ast::Name& name_node, problem::Context& context) {
    const auto name = parse(name_node);
    const auto binding = context.get_current_scope().get<pddl::ObjectImpl>(name);
    if (!binding.has_value()) {
        context.error_handler(name_node, "");
        throw UndefinedObjectError(name, context.error_stream->str());
    }
    return binding.value().object;
}

ObjectListVisitor::ObjectListVisitor(Context& context_)
    : context(context_) { }


pddl::ObjectList ObjectListVisitor::operator()(const std::vector<domain::ast::Name>& name_nodes) {
    // A visited vector of name has single base type "object"
    pddl::ObjectList object_list;
    const auto type = context.domain_context->cache.get_or_create<pddl::TypeImpl>("object");
    for (const auto& name_node : name_nodes) {
        const auto name = parse(name_node);
        if (context.domain_context->get_current_scope().get<pddl::ObjectImpl>(name)) {
            context.error_handler(name_node, "");
            throw ObjectIsConstantError(name, context.error_stream->str());
        }
        else if (context.get_current_scope().get<pddl::ObjectImpl>(name)) {
            context.error_handler(name_node, "");
            throw MultiDefinitionObjectError(name, context.error_stream->str());
        }
        const auto object = context.domain_context->cache.get_or_create<pddl::ObjectImpl>(name, pddl::TypeList{type});
        context.get_current_scope().insert<pddl::ObjectImpl>(name, object, name_node);
        object_list.emplace_back(object);
    }
    return object_list;
}

pddl::ObjectList ObjectListVisitor::operator()(const domain::ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node) {
    pddl::ObjectList object_list;
    const auto types = boost::apply_visitor(TypeReferenceVisitor(*context.domain_context),
                                            typed_list_of_names_recursively_node.type);
    // A non-visited vector of names has user defined base types
    for (const auto& name_node : typed_list_of_names_recursively_node.names) {
        const auto name = parse(name_node);
        auto domain_binding = context.domain_context->get_current_scope().get<pddl::ObjectImpl>(name);
        if (domain_binding.has_value()) {
            context.error_handler(name_node, "Defined here:");
            if (domain_binding.value().position.has_value()) {
                context.domain_context->error_handler(domain_binding.value().position.value(), "First defined here:");
            }
            throw ObjectIsConstantError(name, context.error_stream->str() + context.domain_context->error_stream->str());
        }
        auto problem_binding = context.get_current_scope().get<pddl::ObjectImpl>(name);
        if (problem_binding.has_value()) {
            context.error_handler(name_node, "Defined here:");
            if (problem_binding.value().position.has_value()) {
                context.error_handler(problem_binding.value().position.value(), "First defined here:");
            }
            throw MultiDefinitionObjectError(name, context.error_stream->str());
        }
        const auto object = context.domain_context->cache.get_or_create<pddl::ObjectImpl>(name, types);
        context.get_current_scope().insert<pddl::ObjectImpl>(name, object, name_node);
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
