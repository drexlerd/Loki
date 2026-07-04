/*
 * Copyright (C) 2024-2026 Dominik Drexler
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

#include "utils.hpp"

#include <string>
#include <vector>

namespace loki::benchmarks
{

AtomList create_atoms(size_t num_objects, size_t num_predicates, pddl::Repository& repository, pddl::Builder& builder)
{
    auto objects = std::vector<pddl::ObjectView> {};
    objects.reserve(num_objects);
    for (size_t i = 1; i <= num_objects; ++i)
        objects.push_back(builder.object(repository, cista::offset::string("object_" + std::to_string(i)), {}));

    auto parameters = ygg::IndexList<pddl::Parameter> {};
    parameters.push_back(builder.parameter(repository, builder.variable(repository, cista::offset::string("variable_left")).get_index(), {}).get_index());
    parameters.push_back(builder.parameter(repository, builder.variable(repository, cista::offset::string("variable_right")).get_index(), {}).get_index());

    auto predicates = std::vector<pddl::PredicateView> {};
    predicates.reserve(num_predicates);
    for (size_t i = 1; i <= num_predicates; ++i)
        predicates.push_back(builder.predicate(repository, cista::offset::string("predicate_" + std::to_string(i)), parameters));

    auto atoms = AtomList {};
    atoms.reserve(num_objects * num_objects * num_predicates);
    for (const auto& predicate : predicates)
    {
        for (const auto& object_left : objects)
        {
            const auto left_term = builder.term(repository, ygg::Data<pddl::Term>::Variant(object_left.get_index())).get_index();
            for (const auto& object_right : objects)
            {
                const auto right_term = builder.term(repository, ygg::Data<pddl::Term>::Variant(object_right.get_index())).get_index();
                atoms.push_back(builder.atom(repository, predicate.get_index(), ygg::IndexList<pddl::Term> { left_term, right_term }));
            }
        }
    }
    return atoms;
}

}
