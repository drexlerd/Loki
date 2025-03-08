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

/**
 * Concept to check whether a type T has a member function identifying_members.
 *
 * We use it to automatically generate hash and comparison operators based on the tuple of references.
 */

template<typename T>
concept HasIdentifyingMembers = requires(const T a) {
    { a.identifying_members() };
};

}

#endif
