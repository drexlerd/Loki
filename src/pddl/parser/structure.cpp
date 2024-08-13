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

#include "common.hpp"
#include "conditions.hpp"
#include "effects.hpp"
#include "error_handling.hpp"
#include "functions.hpp"
#include "literal.hpp"
#include "loki/details/pddl/action.hpp"
#include "loki/details/pddl/axiom.hpp"
#include "loki/details/pddl/formatter.hpp"
#include "parameters.hpp"
#include "predicates.hpp"
#include "reference_utils.hpp"

namespace loki
{

std::tuple<std::optional<Condition>, std::optional<Effect>> parse(const ast::ActionBody& node, Context& context)
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

Action parse(const ast::Action& node, Context& context)
{
    context.scopes.open_scope();
    auto name = parse(node.action_symbol.name);
    auto parameter_list = boost::apply_visitor(ParameterListVisitor(context), node.typed_list_of_variables);
    track_variable_references(parameter_list, context);
    auto [condition, effect] = parse(node.action_body, context);
    test_variable_references(parameter_list, context);
    // Remove unused action parameters
    auto used_parameters = ParameterList {};
    for (const auto& parameter : parameter_list)
    {
        if (!context.references.exists(parameter->get_variable()))
        {
            used_parameters.push_back(parameter);
        }
        else
        {
            if (!context.quiet)
            {
                std::cout << "Removed unused parameter " << StreamWriter(*parameter) << " from action " << name << std::endl;
            }
        }
    }
    context.scopes.close_scope();

    const auto action = context.factories.get_or_create_action(name, used_parameters.size(), used_parameters, condition, effect);
    context.positions.push_back(action, node);
    return action;
}

Axiom parse(const ast::Axiom& node, Context& context)
{
    test_undefined_requirement(RequirementEnum::DERIVED_PREDICATES, node, context);
    context.references.untrack(RequirementEnum::DERIVED_PREDICATES);

    // Allow free variables in axiom head and body
    context.scopes.open_scope();
    context.allow_free_variables = true;

    auto predicate_name = parse(node.atomic_formula_skeleton.predicate.name);
    test_undefined_predicate(predicate_name, node.atomic_formula_skeleton.predicate.name, context);

    auto parameters = boost::apply_visitor(ParameterListVisitor(context), node.atomic_formula_skeleton.typed_list_of_variables);
    const auto [predicate, position_, error_handler] = context.scopes.top().get_predicate(predicate_name).value();
    test_arity_compatibility(parameters.size(), predicate->get_parameters().size(), node.atomic_formula_skeleton, context);

    const auto condition = parse(node.goal_descriptor, context);

    context.allow_free_variables = false;
    context.scopes.close_scope();

    // Free variables and literal variables become explicit parameters
    auto variables = collect_free_variables(*condition);
    // Check whether axiom parameters match derived predicate and
    // subtract axiom parameter variables from free variables
    for (size_t i = 0; i < parameters.size(); ++i)
    {
        const auto axiom_parameter = parameters[i];
        const auto predicate_parameter = predicate->get_parameters()[i];

        test_parameter_type_compatibility(axiom_parameter, predicate_parameter, node.atomic_formula_skeleton, context);

        variables.erase(axiom_parameter->get_variable());
    }
    // Turn free variables not mentioned in the parameter list into parameters
    // Those parameters must be appended to the parameter list
    for (const auto variable : variables)
    {
        const auto base_types = TypeList { context.factories.get_or_create_type("object", TypeList {}) };

        parameters.push_back(context.factories.get_or_create_parameter(variable, base_types));
    }
    const auto axiom = context.factories.get_or_create_axiom(predicate_name, parameters, condition, parameters.size());
    context.positions.push_back(axiom, node);
    return axiom;
}

StructureVisitor::StructureVisitor(Context& context_) : context(context_) {}

boost::variant<Axiom, Action> parse(const ast::Structure& node, Context& context) { return boost::apply_visitor(StructureVisitor(context), node); }

}
