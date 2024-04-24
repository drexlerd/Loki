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

#include "common.hpp"
#include "error_handling.hpp"
#include "types.hpp"

using namespace std;

namespace loki
{

static Parameter parse_parameter_definition(const ast::Variable& variable_node, const TypeList& type_list, Context& context)
{
    const auto variable = parse(variable_node, context);
    test_multiple_definition_variable(variable, variable_node, context);
    context.scopes.top().insert_variable(variable->get_name(), variable, variable_node);
    const auto parameter = context.factories.get_or_create_parameter(variable, type_list);
    context.positions.push_back(parameter, variable_node);
    return parameter;
}

static ParameterList parse_parameter_definitions(const std::vector<ast::Variable>& variable_nodes, const TypeList& type_list, Context& context)
{
    auto parameter_list = ParameterList();
    for (const auto& variable_node : variable_nodes)
    {
        parameter_list.push_back(parse_parameter_definition(variable_node, type_list, context));
    }
    return parameter_list;
}

ParameterListVisitor::ParameterListVisitor(Context& context_) : context(context_) {}

ParameterList ParameterListVisitor::operator()(const std::vector<ast::Variable>& nodes)
{
    // std::vector<ast::Variable> has single base type "object"
    const auto type = context.factories.get_or_create_type("object", TypeList());
    auto parameter_list = parse_parameter_definitions(nodes, TypeList { type }, context);
    return parameter_list;
}

ParameterList ParameterListVisitor::operator()(const ast::TypedListOfVariablesRecursively& node)
{
    // requires :typing
    test_undefined_requirement(RequirementEnum::TYPING, node, context);
    context.references.untrack(RequirementEnum::TYPING);
    const auto type_list = boost::apply_visitor(TypeReferenceTypeVisitor(context), node.type);
    // TypedListOfVariablesRecursively has user defined types
    auto parameter_list = parse_parameter_definitions(node.variables, type_list, context);
    // Recursively add parameters.
    auto additional_parameters = boost::apply_visitor(ParameterListVisitor(context), node.typed_list_of_variables.get());
    parameter_list.insert(parameter_list.end(), additional_parameters.begin(), additional_parameters.end());
    return parameter_list;
}

}
