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
#include "parameters.hpp"

#include "../../../../include/loki/domain/pddl/exceptions.hpp"

using namespace loki::domain;
using namespace std;


namespace loki {
pddl::PredicateList parse(const ast::Predicates& predicates_node, const error_handler_type& error_handler, Context& context) {
    pddl::PredicateList predicate_list;
    for (const auto& atomic_formula_skeleton : predicates_node.atomic_formula_skeletons) {
        const auto name = parse(atomic_formula_skeleton.predicate.name);
        if (context.scopes.back()->get<pddl::PredicateImpl>(name)) {
            error_handler(atomic_formula_skeleton.predicate, "");
            throw MultiDefinitionPredicateError(name, context.error_stream->str());
        }
        context.open_scope();
        const auto parameters = boost::apply_visitor(ParameterListVisitor(error_handler, context),
                                                     atomic_formula_skeleton.typed_list_of_variables);
        context.close_scope();
        const auto predicate = context.cache.get_or_create<pddl::PredicateImpl>(name, parameters);
        context.scopes.back()->insert<pddl::PredicateImpl>(name, predicate);
        predicate_list.emplace_back(predicate);
    }
    return predicate_list;
}

}
