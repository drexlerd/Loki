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
#include "types.hpp"

using namespace loki::domain;
using namespace std;

namespace loki {

ParameterListVisitor::ParameterListVisitor(const error_handler_type& error_handler_, Context& context_)
    : error_handler(error_handler_), context(context_) { }

pddl::ParameterList ParameterListVisitor::operator()(const std::vector<ast::Variable>& variable_nodes) {
    // A visited vector of variable has single base type "object"
    pddl::ParameterList parameter_list;
    assert(!context.cache.get_or_create<pddl::TypeImpl>("object").created);
    const auto type = context.cache.get_or_create<pddl::TypeImpl>("object").object;
    for (const auto& variable_node : variable_nodes) {
        const auto name = parse(variable_node, error_handler, context);
        const auto parameter = context.cache.get_or_create<pddl::ParameterImpl>(name, pddl::TypeList{type}).object;
        parameter_list.emplace_back(parameter);
    }
    return parameter_list;
}

pddl::ParameterList ParameterListVisitor::operator()(const ast::TypedListOfVariablesRecursively& typed_variables_node) {
    pddl::ParameterList parameter_list;
    const auto types = boost::apply_visitor(TypeReferenceVisitor(error_handler, context),
                                            typed_variables_node.type);
    // A non-visited vector of variables has user defined types
    for (const auto& variable_node : typed_variables_node.variables) {
        const auto name = parse(variable_node, error_handler, context);
        const auto parameter = context.cache.get_or_create<pddl::ParameterImpl>(name, types).object;
        parameter_list.emplace_back(parameter);
    }
    // Recursively add parameters.
    auto additional_parameters = boost::apply_visitor(ParameterListVisitor(error_handler, context), typed_variables_node.typed_list_of_variables.get());
    parameter_list.insert(parameter_list.end(), additional_parameters.begin(), additional_parameters.end());
    return parameter_list;
}


}
