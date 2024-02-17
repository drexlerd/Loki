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

#include "reference_utils.hpp"

#include <loki/problem/pddl/exceptions.hpp>


namespace loki {

void track_object_references(const pddl::ObjectList& object_list, Context& context) {
    for (const auto& object : object_list) {
        context.references.track(object);
    }
}

void test_object_references(const pddl::ObjectList& object_list, const Context& context) {
    for (const auto& object : object_list) {
        if (context.references.exists(object)) {
            const auto [_object, position, error_handler] = context.scopes.get<pddl::ObjectImpl>(object->get_name()).value();
            throw UnusedObjectError(object->get_name(), error_handler(position.value(), ""));
        }
    }
}


}