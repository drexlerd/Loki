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
#include "loki/pddl/action.hpp"
#include "loki/pddl/derived_predicate.hpp"
#include "loki/pddl/exceptions.hpp"
#include "parameters.hpp"
#include "reference_utils.hpp"

namespace loki
{

std::tuple<std::optional<pddl::Condition>, std::optional<pddl::Effect>> parse(const ast::ActionBody& node, Context& context)
{
    std::optional<pddl::Condition> condition;
    if (node.precondition_goal_descriptor.has_value())
    {
        condition = parse(node.precondition_goal_descriptor.value(), context);
    }
    std::optional<pddl::Effect> effect;
    if (node.effect.has_value())
    {
        effect = parse(node.effect.value(), context);
    }
    return { condition, effect };
}

pddl::Action parse(const ast::Action& node, Context& context)
{
    context.scopes.open_scope();
    auto name = parse(node.action_symbol.name);
    auto parameter_list = boost::apply_visitor(ParameterListVisitor(context), node.typed_list_of_variables);
    track_variable_references(parameter_list, context);
    auto [condition, effect] = parse(node.action_body, context);
    test_variable_references(parameter_list, context);
    context.scopes.close_scope();
    const auto action = context.factories.actions.get_or_create<pddl::ActionImpl>(name, parameter_list, condition, effect);
    context.positions.push_back(action, node);
    return action;
}

pddl::DerivedPredicate parse(const ast::DerivedPredicate& node, Context& context)
{
    if (!context.requirements->test(pddl::RequirementEnum::DERIVED_PREDICATES))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::DERIVED_PREDICATES, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::DERIVED_PREDICATES);
    context.scopes.open_scope();
    auto parameter_list = boost::apply_visitor(ParameterListVisitor(context), node.typed_list_of_variables);
    track_variable_references(parameter_list, context);
    auto condition = parse(node.goal_descriptor, context);
    const auto derived_predicate = context.factories.derived_predicates.get_or_create<pddl::DerivedPredicateImpl>(parameter_list, condition);
    test_variable_references(parameter_list, context);

    context.scopes.close_scope();
    context.positions.push_back(derived_predicate, node);
    return derived_predicate;
}

StructureVisitor::StructureVisitor(Context& context_) : context(context_) {}

boost::variant<pddl::DerivedPredicate, pddl::Action> parse(const ast::Structure& node, Context& context)
{
    return boost::apply_visitor(StructureVisitor(context), node);
}

}
