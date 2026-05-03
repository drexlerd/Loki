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

#ifndef LOKI_DETAILS_PDDL_CONCEPTS_HPP_
#define LOKI_DETAILS_PDDL_CONCEPTS_HPP_

#include <concepts>
#include <ranges>
#include <type_traits>

namespace loki
{

template<typename T>
concept ParsingContext = requires(T a) {
    a.scopes;
    a.positions;
    a.references;
    a.builder;
    a.requirements;
};

template<typename T>
concept IsBackInsertibleRange = std::ranges::forward_range<T> &&                       // Must be a forward range
                                std::default_initializable<std::remove_cvref_t<T>> &&  // Must be default-constructible
                                requires(std::remove_cvref_t<T> container, std::ranges::range_value_t<T> value) {
                                    std::back_inserter(container);           // Must support std::back_inserter
                                    *std::back_inserter(container) = value;  // Must support inserting values
                                };

}

#endif