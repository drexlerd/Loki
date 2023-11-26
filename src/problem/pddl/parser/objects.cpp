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

#include "objects.hpp"

#include "../../../domain/pddl/parser/common.hpp"
#include "../../../domain/pddl/parser/types.hpp"

using namespace loki::problem;
using namespace std;


namespace loki {

ObjectListVisitor::ObjectListVisitor(const error_handler_type& error_handler_, Context& context_)
    : error_handler(error_handler_), context(context_) { }


pddl::ObjectList ObjectListVisitor::operator()(const std::vector<domain::ast::Name>& name_nodes) {
    // A visited vector of name has single base type "object"
    pddl::ObjectList object_list;
    assert(!context.domain_context->cache.get_or_create<pddl::TypeImpl>("object").created);
    const auto type = context.domain_context->cache.get_or_create<pddl::TypeImpl>("object").object;
    for (const auto& name_node : name_nodes) {
        const auto name = parse(name_node);
        const auto object = context.cache.get_or_create<pddl::ObjectImpl>(name, pddl::TypeList{type}).object;
        context.objects_by_name.emplace(name, object);
        object_list.emplace_back(object);
    }
    return object_list;
}

pddl::ObjectList ObjectListVisitor::operator()(const domain::ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node) {
    pddl::ObjectList object_list;
    const auto types = boost::apply_visitor(TypeReferenceVisitor(error_handler, *context.domain_context),
                                            typed_list_of_names_recursively_node.type);
    // A non-visited vector of names has user defined base types
    for (const auto& name_node : typed_list_of_names_recursively_node.names) {
        const auto name = parse(name_node);
        const auto object = context.cache.get_or_create<pddl::ObjectImpl>(name, types).object;
        context.objects_by_name.emplace(name, object);
        object_list.emplace_back(object);
    }
    // Recursively add objects.
    auto additional_objects = this->operator()(typed_list_of_names_recursively_node.typed_list_of_names);
    object_list.insert(object_list.end(), additional_objects.begin(), additional_objects.end());
    return object_list;
}

pddl::ObjectList ObjectListVisitor::operator()(const domain::ast::TypedListOfNames& node) {
    return boost::apply_visitor(*this, node);
}


pddl::ObjectList parse(const ast::Objects& objects_node, const error_handler_type& error_handler, Context& context) {
    // Dominik: Should we copy domain constants to objects? Duplicates?
    return boost::apply_visitor(ObjectListVisitor(error_handler, context), objects_node.typed_list_of_names);
}

}
