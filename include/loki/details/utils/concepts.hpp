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

template<typename T>
struct is_valid_type : std::false_type
{
};

// Allow `const T&`
template<typename T>
struct is_valid_type<const T&> : std::true_type
{
};

// Allow `const T&&` (rvalue reference)
template<typename T>
struct is_valid_type<const T&&> : std::true_type
{
};

// Allow `const T*&`
template<typename T>
struct is_valid_type<const T*&> : std::true_type
{
};

// Allow `const T*`
template<typename T>
struct is_valid_type<const T*> : std::true_type
{
};

// Allow `const T* const`
template<typename T>
struct is_valid_type<const T* const> : std::true_type
{
};

// Allow `const T* &&` (rvalue reference to pointer)
template<typename T>
struct is_valid_type<const T*&&> : std::true_type
{
};

template<typename T, typename = void>
struct is_tuple_is_identifying_members_tuple : std::false_type
{
};

template<typename... Ts>
struct is_tuple_is_identifying_members_tuple<std::tuple<Ts...>> : std::conjunction<is_valid_type<Ts>...>
{
};

template<typename T>
concept IsIdentifyingMembersTuple = is_tuple_is_identifying_members_tuple<T>::value;

/**
 * Concept to check whether a type T has a member function that returns `IsIdentifyingMembersTuple`.
 *
 * We use it to automatically generate hash and comparison operators based on the tuple of references.
 */

template<typename T>
concept HasIdentifyingMembers = requires(const T a) {
    { a.identifying_members() } -> IsIdentifyingMembersTuple;
};

}

#endif
