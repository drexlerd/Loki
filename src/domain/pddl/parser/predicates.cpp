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
#include "parameters.hpp"
#include "common.hpp"

#include "../../../../include/domain/pddl/exceptions.hpp"


namespace loki {

static void test_multiple_definition(const pddl::Predicate& predicate, const domain::ast::Predicate& node, const Context& context) {
    const auto predicate_name = predicate->get_name();
    const auto binding = context.scopes.get<pddl::PredicateImpl>(predicate_name);
    if (binding.has_value()) {
        const auto message_1 = context.scopes.get_error_handler()(node, "Defined here:");
        auto message_2 = std::string("");
        const auto& [_predicate, position, error_handler] = binding.value();
        if (position.has_value()) {
            message_2 = error_handler(position.value(), "First defined here:");
        } else {
            // Reserved type error?
        }
        throw MultiDefinitionPredicateError(predicate_name, message_1 + message_2);
    }
}


static void insert_context_information(const pddl::Predicate& predicate, const domain::ast::Predicate& node, Context& context) {
    context.positions.push_back(predicate, node);
    context.scopes.insert<pddl::PredicateImpl>(predicate->get_name(), predicate, node);
    context.referenced_pointers.track(predicate);
}


pddl::PredicateList parse(const domain::ast::Predicates& predicates_node, Context& context) {
    auto predicate_list = pddl::PredicateList();
    for (const auto& atomic_formula_skeleton : predicates_node.atomic_formula_skeletons) {
        context.scopes.open_scope();
        const auto parameters = boost::apply_visitor(ParameterListVisitor(context),
                                                     atomic_formula_skeleton.typed_list_of_variables);
        context.scopes.close_scope();
        const auto name = parse(atomic_formula_skeleton.predicate.name);
        const auto predicate = context.factories.predicates.get_or_create<pddl::PredicateImpl>(name, parameters);
        test_multiple_definition(predicate, atomic_formula_skeleton.predicate, context);
        insert_context_information(predicate, atomic_formula_skeleton.predicate, context);
        predicate_list.emplace_back(predicate);
    }
    return predicate_list;
}

}
