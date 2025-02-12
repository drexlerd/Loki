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

#include <concepts>
#include <functional>
#include <tuple>
#include <type_traits>
#include <vector>

namespace loki
{

template<typename T, typename = void>
struct is_tuple_of_const_references : std::false_type
{
};

template<typename... Ts>
struct is_tuple_of_const_references<
    std::tuple<Ts...>,
    std::enable_if_t<std::conjunction_v<std::bool_constant<(std::is_lvalue_reference_v<Ts> && std::is_const_v<std::remove_reference_t<Ts>>) ||  // const lvalue
                                                                                                                                                // reference
                                                           (std::is_pointer_v<Ts>&& std::is_const_v<std::remove_pointer_t<Ts>>)  // pointer to const
                                                           >...>>> : std::true_type
{
};

template<typename T>
concept IsTupleOfConstRefs = is_tuple_of_const_references<T>::value;

static_assert(IsTupleOfConstRefs<std::tuple<const int&>>);
static_assert(IsTupleOfConstRefs<std::tuple<const int*>>);
static_assert(!IsTupleOfConstRefs<std::tuple<int*>>);
static_assert(!IsTupleOfConstRefs<std::tuple<const int>>);
static_assert(!IsTupleOfConstRefs<std::tuple<int&>>);
static_assert(!IsTupleOfConstRefs<std::tuple<int>>);
static_assert(!IsTupleOfConstRefs<const std::tuple<int>>);
static_assert(!IsTupleOfConstRefs<std::tuple<int>&>);

/**
 * Concept to check whether a type T has a member function that returns `std::tuple<const T1&,...,const TN&>`.
 *
 * We use it to automatically generate hash and comparison operators based on the tuple of references.
 */

template<typename T>
concept HasIdentifyingMembers = requires(const T a) {
    { a.identifying_members() } -> IsTupleOfConstRefs;
};

}

#endif
