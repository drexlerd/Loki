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

#include "conditions.hpp"
#include "effects.hpp"
#include "common.hpp"
#include "parameters.hpp"

#include "../../../../include/loki/domain/pddl/exceptions.hpp"
#include "../../../../include/loki/domain/pddl/action.hpp"
#include "../../../../include/loki/domain/pddl/derived_predicate.hpp"


namespace loki {

std::tuple<std::optional<pddl::Condition>, std::optional<pddl::Effect>> parse(const domain::ast::ActionBody& node, Context& context) {
    std::optional<pddl::Condition> condition;
    if (node.precondition_goal_descriptor.has_value()) {
        condition = parse(node.precondition_goal_descriptor.value(), context);
    }
    std::optional<pddl::Effect> effect;
    if (node.effect.has_value()) {
        effect = parse(node.effect.value(), context);
    }
    return {condition, effect};
}

pddl::Action parse(const domain::ast::Action& node, Context& context) {
    context.scopes.open_scope();
    auto name = parse(node.action_symbol.name);
    auto parameters = boost::apply_visitor(ParameterListVisitor(context), node.typed_list_of_variables);
    auto [condition, effect] = parse(node.action_body, context);
    context.scopes.close_scope();
    return context.factories.actions.get_or_create<pddl::ActionImpl>(name, parameters, condition, effect);
}

pddl::DerivedPredicate parse(const domain::ast::DerivedPredicate& node, Context& context) {
    if (!context.requirements->test(pddl::RequirementEnum::DERIVED_PREDICATES)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::DERIVED_PREDICATES, context.scopes.get_error_handler()(node, ""));
    }
    auto parameters = boost::apply_visitor(ParameterListVisitor(context),
        node.typed_list_of_variables);
    auto condition = parse(node.goal_descriptor, context);
    return context.factories.derived_predicates.get_or_create<pddl::DerivedPredicateImpl>(parameters, condition);
}


StructureVisitor::StructureVisitor(Context& context_)
    : context(context_) { }


boost::variant<pddl::DerivedPredicate, pddl::Action> parse(
    const domain::ast::Structure& node, Context& context) {
    return boost::apply_visitor(StructureVisitor(context), node);
}


}

