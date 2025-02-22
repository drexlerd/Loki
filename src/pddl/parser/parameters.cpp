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
#include "loki/details/pddl/domain_parsing_context.hpp"
#include "loki/details/pddl/problem_parsing_context.hpp"
#include "loki/details/pddl/scope.hpp"
#include "types.hpp"

using namespace std;

namespace loki
{

template<ParsingContext C>
static Parameter parse_parameter_definition(const ast::Variable& variable_node, const TypeList& type_list, C& context)
{
    const auto variable = parse(variable_node, context);
    test_multiple_definition_variable(variable, variable_node, context);
    context.scopes.top().insert_variable(variable->get_name(), variable, variable_node);
    const auto parameter = context.builder.get_repositories().get_or_create_parameter(variable, type_list);
    context.positions.push_back(parameter, variable_node);
    return parameter;
}

template<ParsingContext C>
static ParameterList parse_parameter_definitions(const std::vector<ast::Variable>& variable_nodes, const TypeList& type_list, C& context)
{
    auto parameter_list = ParameterList();
    for (const auto& variable_node : variable_nodes)
    {
        parameter_list.push_back(parse_parameter_definition(variable_node, type_list, context));
    }
    return parameter_list;
}

template<ParsingContext C>
ParameterListVisitor<C>::ParameterListVisitor(C& context_) : context(context_)
{
}

template<ParsingContext C>
ParameterList ParameterListVisitor<C>::operator()(const std::vector<ast::Variable>& nodes)
{
    // std::vector<ast::Variable> has single base type "object"
    const auto binding = context.scopes.top().get_type("object");
    assert(binding.has_value());
    const auto [type, _position, _error_handler] = binding.value();
    auto parameter_list = parse_parameter_definitions(nodes, TypeList { type }, context);
    return parameter_list;
}

template<ParsingContext C>
ParameterList ParameterListVisitor<C>::operator()(const ast::TypedListOfVariablesRecursively& node)
{
    // requires :typing
    test_undefined_requirement(RequirementEnum::TYPING, node, context);
    context.references.untrack(RequirementEnum::TYPING);
    auto type_ref_visitor = TypeReferenceTypeVisitor(context);
    const auto type_list = boost::apply_visitor(type_ref_visitor, node.type);
    // TypedListOfVariablesRecursively has user defined types
    auto parameter_list = parse_parameter_definitions(node.variables, type_list, context);
    // Recursively add parameters.
    auto parameter_list_visitor = ParameterListVisitor(context);
    auto additional_parameters = boost::apply_visitor(parameter_list_visitor, node.typed_list_of_variables.get());
    parameter_list.insert(parameter_list.end(), additional_parameters.begin(), additional_parameters.end());
    return parameter_list;
}

template struct ParameterListVisitor<DomainParsingContext>;
template struct ParameterListVisitor<ProblemParsingContext>;

}
