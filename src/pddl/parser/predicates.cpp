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

#include "predicates.hpp"

#include "common.hpp"
#include "error_handling.hpp"
#include "loki/details/pddl/domain_parsing_context.hpp"
#include "loki/details/pddl/problem_parsing_context.hpp"
#include "loki/details/pddl/scope.hpp"
#include "parameters.hpp"

namespace loki
{

template<ParsingContext C>
static void insert_context_information(const Predicate& predicate, const ast::Predicate& node, C& context)
{
    context.positions.push_back(predicate, node);
    context.scopes.top().insert_predicate(predicate->get_name(), predicate, node);
}

template<ParsingContext C>
static Predicate parse_predicate_definition(const ast::AtomicFormulaSkeleton& node, C& context)
{
    context.scopes.open_scope();
    auto parameter_list_visitor = ParameterListVisitor(context);
    const auto parameters = boost::apply_visitor(parameter_list_visitor, node.typed_list_of_variables);
    context.scopes.close_scope();
    const auto predicate_name = parse(node.predicate.name);
    const auto predicate = context.builder.get_or_create_predicate(predicate_name, parameters);
    test_multiple_definition_predicate(predicate, node.predicate, context);
    insert_context_information(predicate, node.predicate, context);
    return predicate;
}

template<ParsingContext C>
static PredicateList parse_predicate_definitions(const std::vector<ast::AtomicFormulaSkeleton>& nodes, C& context)
{
    auto predicate_list = PredicateList();
    for (const auto& node : nodes)
    {
        predicate_list.push_back(parse_predicate_definition(node, context));
    }
    return predicate_list;
}

template<ParsingContext C>
PredicateList parse(const ast::Predicates& predicates_node, C& context)
{
    const auto predicate_list = parse_predicate_definitions(predicates_node.atomic_formula_skeletons, context);
    return predicate_list;
}

template PredicateList parse(const ast::Predicates& predicates_node, DomainParsingContext& context);
template PredicateList parse(const ast::Predicates& predicates_node, ProblemParsingContext& context);

}
