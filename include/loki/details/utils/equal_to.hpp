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

#include "loki/details/utils/concepts.hpp"
#include "loki/details/utils/observer_ptr.hpp"

#include <functional>
#include <span>
#include <utility>

namespace loki
{

/// @brief `EqualTo` is our custom equality comparator, like std::equal_to.
///
/// Forwards to std::equal_to by default.
/// Specializations can be injected into the namespace.
template<typename T>
struct EqualTo
{
    bool operator()(const T& lhs, const T& rhs) const { return std::equal_to<T>()(lhs, rhs); }
};

/// @brief EqualTo specialization for a std::span.
///
/// Compares the hashes of the the pointer and size.
template<typename T>
struct EqualTo<std::span<T>>
{
    bool operator()(const std::span<T>& lhs, const std::span<T>& rhs) const { return (lhs.size() == rhs.size()) && (lhs.data() == rhs.data()); }
};

/// @brief EqualTo specialization for types T that satisfy `HasIdentifiableMembers`.
/// Dereferences the underlying pointer before forwarding the call to the std::equal_to
/// specialization of `IdentifiableMemberProxy` of T to pairwise compare all members.
/// @tparam T is the type.
template<typename T>
struct EqualTo<ObserverPtr<T>>
{
    bool operator()(loki::ObserverPtr<T> lhs, loki::ObserverPtr<T> rhs) const { return EqualTo<T>()(*lhs, *rhs); }
};

/// @brief EqualTo specialization for an `IdentifiableMembersProxy`
/// that pairwise compares all members.
/// @tparam ...Ts are the types of all members.
template<HasIdentifiableMembers T>
struct EqualTo<T>
{
    using MembersTupleType = decltype(std::declval<T>().identifiable_members());

    bool operator()(const T& lhs, const T& rhs) const { return EqualTo<MembersTupleType>()(lhs.identifiable_members(), rhs.identifiable_members()); }
};

}

#endif
