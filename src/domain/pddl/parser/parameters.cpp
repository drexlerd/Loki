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

#include "parameters.hpp"

#include "types.hpp"
#include "common.hpp"

#include <loki/domain/pddl/exceptions.hpp>

using namespace loki::domain;
using namespace std;


namespace loki {

static void test_multiple_definition(const pddl::Variable& variable, const ast::Variable& node, const Context& context) {
    const auto binding = context.scopes.get<pddl::VariableImpl>(variable->get_name());
    if (binding.has_value()) {
        const auto message_1 = context.scopes.get_error_handler()(node, "Defined here:");
        auto message_2 = std::string("");
        const auto& [_variable, position, error_handler] = binding.value();
        if (position.has_value()) {
            message_2 = error_handler(position.value(), "First defined here:");
        }
        throw MultiDefinitionVariableError(variable->get_name(), message_1 + message_2);
    }
}


static void insert_context_information(const pddl::Variable& variable, const ast::Variable& node, Context& context) {
    context.scopes.insert(variable->get_name(), variable, node);
}


static pddl::Parameter parse_parameter_definition(const domain::ast::Variable& variable_node, const pddl::TypeList& type_list, Context& context) {
    const auto variable = parse(variable_node, context);
    test_multiple_definition(variable, variable_node, context);
    insert_context_information(variable, variable_node, context);

    const auto parameter = context.factories.parameters.get_or_create<pddl::ParameterImpl>(variable, type_list);
    context.positions.push_back(parameter, variable_node);
    return parameter;
}


static pddl::ParameterList parse_parameter_definitions(const std::vector<domain::ast::Variable>& variable_nodes, const pddl::TypeList& type_list, Context& context) {
    auto parameter_list = pddl::ParameterList();
    for (const auto& variable_node : variable_nodes) {
        parameter_list.emplace_back(parse_parameter_definition(variable_node, type_list, context));
    }
    return parameter_list;
}


ParameterListVisitor::ParameterListVisitor(Context& context_)
    : context(context_) { }

pddl::ParameterList ParameterListVisitor::operator()(const std::vector<ast::Variable>& nodes) {
    // std::vector<ast::Variable> has single base type "object"
    const auto type = context.factories.types.get_or_create<pddl::TypeImpl>("object");
    auto parameter_list = parse_parameter_definitions(nodes, pddl::TypeList{type}, context);
    return parameter_list;
}

pddl::ParameterList ParameterListVisitor::operator()(const ast::TypedListOfVariablesRecursively& node) {
    // requires :typing
    if (!context.requirements->test(pddl::RequirementEnum::TYPING)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::TYPING, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::TYPING);
    const auto type_list = boost::apply_visitor(TypeReferenceTypeVisitor(context), node.type);
    // TypedListOfVariablesRecursively has user defined types
    auto parameter_list = parse_parameter_definitions(node.variables, type_list, context);
    // Recursively add parameters.
    auto additional_parameters = boost::apply_visitor(*this, node.typed_list_of_variables.get());
    parameter_list.insert(parameter_list.end(), additional_parameters.begin(), additional_parameters.end());
    return parameter_list;
}


}
