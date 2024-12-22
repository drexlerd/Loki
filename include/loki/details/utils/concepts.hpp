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
 *<
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LOKI_INCLUDE_LOKI_UTILS_CONCEPTS_HPP_
#define LOKI_INCLUDE_LOKI_UTILS_CONCEPTS_HPP_

#include "loki/details/utils/hash.hpp"

#include <concepts>
#include <functional>
#include <tuple>
#include <type_traits>
#include <vector>

namespace loki
{

// Helper metafunction to check if all elements in a parameter pack are const references
template<typename... Ts>
struct are_const_references : std::bool_constant<((std::is_reference_v<Ts> && std::is_const_v<std::remove_reference_t<Ts>>) && ...)>
{
};

// Specialization for non-tuple types (this will fail SFINAE when the type is not a std::tuple)
template<typename T, typename = void>
struct is_tuple_of_const_references : std::false_type
{
};

// Specialization for std::tuple
template<typename... Ts>
struct is_tuple_of_const_references<std::tuple<Ts...>, std::enable_if_t<are_const_references<Ts...>::value>> : std::true_type
{
};

// Concept based on the metafunction
template<typename T>
concept IsTupleOfConstReferences = is_tuple_of_const_references<T>::value;

static_assert(IsTupleOfConstReferences<std::tuple<const int&>>);
static_assert(!IsTupleOfConstReferences<std::tuple<const int>>);
static_assert(!IsTupleOfConstReferences<std::tuple<int>>);
static_assert(!IsTupleOfConstReferences<std::tuple<int&>>);
static_assert(!IsTupleOfConstReferences<std::tuple<int*>>);
static_assert(!IsTupleOfConstReferences<std::tuple<const int*>>);

template<typename T>
concept HasIdentifiableMembers = requires(T a) {
    // Check if `a.members()` exists and returns a std::tuple of const references
    { a.identifiable_members() } -> IsTupleOfConstReferences;
};

}

#endif
