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

#ifndef LOKI_INCLUDE_LOKI_UTILS_EQUAL_TO_HPP_
#define LOKI_INCLUDE_LOKI_UTILS_EQUAL_TO_HPP_

#include <functional>
#include <span>

namespace loki
{

/// @brief `EqualTo` is our custom equality comparator, like std::equal_to.
///
/// Forwards to std::equal_to by default.
/// Specializations can be injected into the namespace.
template<typename T>
struct EqualTo
{
    bool operator()(const T& lhs, const T& rhs) { return std::equal_to<T>()(lhs, rhs); }
};

/// @brief EqualTo specialization for a std::span.
///
/// Compares the hashes of the the pointer and size.
template<typename T>
struct EqualTo<std::span<T>>
{
    bool operator()(const std::span<T>& lhs, const std::span<T>& rhs) const { return (lhs.size() == rhs.size()) && (lhs.data() == rhs.data()); }
};

}

#endif
