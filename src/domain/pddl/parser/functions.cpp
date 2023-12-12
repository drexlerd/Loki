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

#include "functions.hpp"

#include "common.hpp"
#include "parameters.hpp"

#include "../../../../include/loki/domain/pddl/exceptions.hpp"


namespace loki {

FunctionSkeletonListVisitor::FunctionSkeletonListVisitor(Context& context_)
    : context(context_) { }

pddl::FunctionSkeletonList FunctionSkeletonListVisitor::operator()(const std::vector<domain::ast::AtomicFunctionSkeleton>& formula_skeleton_nodes) {
    pddl::FunctionSkeletonList function_skeleton_list;
    assert(context.scopes.get<pddl::TypeImpl>("number").has_value());
    auto function_type = context.scopes.get<pddl::TypeImpl>("number")->value.object;
    for (const auto& atomic_function_skeleton : formula_skeleton_nodes) {
        auto function_name = parse(atomic_function_skeleton.function_symbol.name);
        const auto binding = context.scopes.get<pddl::FunctionSkeletonImpl>(function_name);
        if (binding.has_value()) {
            const auto message_1 = context.scopes.get_error_handler()(atomic_function_skeleton.function_symbol.name, "Defined here:");
            auto message_2 = std::string("");
            if (binding.value().value.position.has_value()) {
                message_2 = binding.value().error_handler(binding.value().value.position.value(), "First defined here:");
            }
            throw MultiDefinitionFunctionSkeletonError(function_name, message_1 + message_2);
        }
        auto function_parameters = boost::apply_visitor(ParameterListVisitor(context), atomic_function_skeleton.arguments);
        auto function_skeleton = context.cache.get_or_create<pddl::FunctionSkeletonImpl>(function_name, function_parameters, function_type);
        context.scopes.insert<pddl::FunctionSkeletonImpl>(function_name, function_skeleton, atomic_function_skeleton.function_symbol.name);
        function_skeleton_list.push_back(function_skeleton);
    }
    return function_skeleton_list;
}

pddl::FunctionSkeletonList FunctionSkeletonListVisitor::operator()(const domain::ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& function_skeleton_list_recursively_node) {
    pddl::FunctionSkeletonList function_skeleton_list;
    // We do not parse function_skeleton_list_recursively_node.function_type because we do not support :object-fluents
    assert(context.scopes.get<pddl::TypeImpl>("number").has_value());
    auto function_type = context.scopes.get<pddl::TypeImpl>("number")->value.object;
    for (const auto& atomic_function_skeleton : function_skeleton_list_recursively_node.atomic_function_skeletons) {
        auto function_name = parse(atomic_function_skeleton.function_symbol.name);
        const auto binding = context.scopes.get<pddl::FunctionSkeletonImpl>(function_name);
        if (binding.has_value()) {
            const auto message_1 = context.scopes.get_error_handler()(atomic_function_skeleton.function_symbol.name, "Defined here:");
            auto message_2 = std::string("");
            if (binding.value().value.position.has_value()) {
                message_2 = binding.value().error_handler(binding.value().value.position.value(), "First defined here:");
            }
            throw MultiDefinitionFunctionSkeletonError(function_name, message_1 + message_2);
        }
        context.scopes.open_scope();
        auto function_parameters = boost::apply_visitor(ParameterListVisitor(context), atomic_function_skeleton.arguments);
        context.scopes.close_scope();
        auto function_skeleton = context.cache.get_or_create<pddl::FunctionSkeletonImpl>(function_name, function_parameters, function_type);
        context.scopes.insert<pddl::FunctionSkeletonImpl>(function_name, function_skeleton, atomic_function_skeleton.function_symbol.name);
        function_skeleton_list.push_back(function_skeleton);
    }
    if (function_skeleton_list_recursively_node.function_typed_list_of_atomic_function_skeletons.has_value()) {
        auto add_function_skeleton_list = this->operator()(function_skeleton_list_recursively_node.function_typed_list_of_atomic_function_skeletons.value());
    }
    return function_skeleton_list;
}

pddl::FunctionSkeletonList FunctionSkeletonListVisitor::operator()(const domain::ast::FunctionTypedListOfAtomicFunctionSkeletons& function_skeleton_list_node) {
    return this->operator()(function_skeleton_list_node);
}

pddl::FunctionSkeletonList parse(const domain::ast::Functions& functions_node, Context& context) {
    return boost::apply_visitor(FunctionSkeletonListVisitor(context), functions_node.function_types_list_of_atomic_function_skeletons);
}

}