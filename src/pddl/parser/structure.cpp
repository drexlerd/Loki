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

#include "structure.hpp"

#include "../formatter.hpp"
#include "common.hpp"
#include "conditions.hpp"
#include "effects.hpp"
#include "error_handling.hpp"
#include "functions.hpp"
#include "literal.hpp"
#include "loki/details/pddl/action.hpp"
#include "loki/details/pddl/axiom.hpp"
#include "loki/details/pddl/domain_parsing_context.hpp"
#include "loki/details/pddl/exceptions.hpp"
#include "loki/details/pddl/problem_parsing_context.hpp"
#include "loki/details/pddl/scope.hpp"
#include "parameters.hpp"
#include "predicates.hpp"
#include "reference_utils.hpp"

namespace loki
{

template<ParsingContext C>
static std::tuple<std::optional<Condition>, std::optional<Effect>> parse(const ast::ActionBody& node, C& context)
{
    std::optional<Condition> condition;
    if (node.precondition_goal_descriptor.has_value())
    {
        condition = parse(node.precondition_goal_descriptor.value(), context);
    }
    std::optional<Effect> effect;
    if (node.effect.has_value())
    {
        effect = parse(node.effect.value(), context);
    }
    return { condition, effect };
}

template<ParsingContext C>
StructureVisitor<C>::StructureVisitor(C& context_) : context(context_)
{
}

template<ParsingContext C>
std::variant<Axiom, Action> StructureVisitor<C>::operator()(const ast::Action& node)
{
    context.scopes.open_scope();
    auto name = parse(node.action_symbol.name);
    auto parameter_list_visitor = ParameterListVisitor(context);
    auto parameter_list = boost::apply_visitor(parameter_list_visitor, node.typed_list_of_variables);
    track_variable_references(parameter_list, context);
    auto [condition, effect] = parse(node.action_body, context);
    test_variable_references(parameter_list, context);
    // Silently removing parameters results in segfaults when using val on the original domain.
    context.scopes.close_scope();

    const auto action = context.builder.get_repositories().get_or_create_action(name, parameter_list.size(), parameter_list, condition, effect);
    context.positions.push_back(action, node);
    return action;
}

template<ParsingContext C>
std::variant<Axiom, Action> StructureVisitor<C>::operator()(const ast::Axiom& node)
{
    test_undefined_requirement(RequirementEnum::DERIVED_PREDICATES, node, context);
    context.references.untrack(RequirementEnum::DERIVED_PREDICATES);

    // Allow free variables in axiom head and body
    context.scopes.open_scope();
    context.ignore_free_variables = true;

    auto predicate_name = parse(node.atomic_formula_skeleton.predicate.name);
    test_undefined_predicate(predicate_name, node.atomic_formula_skeleton.predicate.name, context);

    auto parameter_list_visitor = ParameterListVisitor(context);
    auto parameter_list = boost::apply_visitor(parameter_list_visitor, node.atomic_formula_skeleton.typed_list_of_variables);
    const auto [predicate, position_, error_handler] = context.scopes.top().get_predicate(predicate_name).value();
    // test_arity_compatibility(parameter_list.size(), predicate->get_parameters().size(), node.atomic_formula_skeleton, context);

    // Turn predicate parameters into terms
    auto terms = TermList {};
    for (const auto& parameter : parameter_list)
    {
        terms.push_back(context.builder.get_repositories().get_or_create_term(parameter->get_variable()));
    }

    const auto literal =
        context.builder.get_repositories().get_or_create_literal(true, context.builder.get_repositories().get_or_create_atom(predicate, terms));

    const auto condition = parse(node.goal_descriptor, context);

    context.ignore_free_variables = false;
    context.scopes.close_scope();

    // Free variables and literal variables become explicit parameters
    auto variables = collect_free_variables(*condition);
    // Check whether axiom parameters match derived predicate and
    // subtract axiom parameter variables from free variables
    for (size_t i = 0; i < predicate->get_parameters().size(); ++i)
    {
        const auto axiom_parameter = parameter_list[i];
        const auto predicate_parameter = predicate->get_parameters()[i];

        test_parameter_type_compatibility(axiom_parameter, predicate_parameter, node.atomic_formula_skeleton, context);

        variables.erase(axiom_parameter->get_variable());
    }
    // Turn free variables not mentioned in the parameter list into parameters
    // Those parameters must be appended to the parameter list
    assert(context.scopes.top().get_type("object").has_value());
    const auto [type, _position, _error_handler] = context.scopes.top().get_type("object").value();
    for (const auto variable : variables)
    {
        const auto base_types = TypeList { type };

        parameter_list.push_back(context.builder.get_repositories().get_or_create_parameter(variable, base_types));
    }

    const auto axiom = context.builder.get_repositories().get_or_create_axiom(parameter_list, literal, condition);
    context.positions.push_back(axiom, node);
    return axiom;
}

template<ParsingContext C>
std::variant<Axiom, Action> parse(const ast::Structure& node, C& context)
{
    auto visitor = StructureVisitor(context);
    return boost::apply_visitor(visitor, node);
}

template std::variant<Axiom, Action> parse(const ast::Structure& node, DomainParsingContext& context);
template std::variant<Axiom, Action> parse(const ast::Structure& node, ProblemParsingContext& context);

}
