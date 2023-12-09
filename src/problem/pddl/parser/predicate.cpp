/*
 * Copyright (C) 2023 Dominik Drexler
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

#include "predicate.hpp"

#include "../../../domain/pddl/parser/common.hpp"
#include "../../../domain/pddl/parser/types.hpp"
#include "../../../../include/loki/problem/pddl/exceptions.hpp"
#include "../../../../include/loki/domain/pddl/exceptions.hpp"

using namespace loki::problem;
using namespace std;


namespace loki {

pddl::Predicate parse(const domain::ast::Predicate& predicate, problem::Context& context) {
    const auto name = parse(predicate.name);
    const auto binding = context.domain_context->get_current_scope().get<pddl::PredicateImpl>(name);
    if (!binding.has_value()) {
        context.error_handler(predicate, "");
        throw UndefinedPredicateError(name, context.error_stream->str());
    }
    return binding.value().object;
}

}
