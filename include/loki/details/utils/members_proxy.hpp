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

#ifndef LOKI_INCLUDE_LOKI_UTILS_MEMBERS_PROXY_HPP_
#define LOKI_INCLUDE_LOKI_UTILS_MEMBERS_PROXY_HPP_

#include "loki/details/utils/concepts.hpp"
#include "loki/details/utils/equal_to.hpp"
#include "loki/details/utils/hash.hpp"

#include <concepts>
#include <functional>
#include <tuple>
#include <type_traits>
#include <vector>

namespace loki
{

/// @brief `IdentifiableMembersProxy` encapsulates the identifiable members of a type T.
/// Its primary purpose is to allow specialization of std::hash and std::equal_to
/// to automatically generating hash and comparison operators based on the identifiable members.
/// @tparam T
template<HasIdentifiableMembers T>
class IdentifiableMembersProxy
{
public:
    using MembersTupleType = decltype(std::declval<T>().identifiable_members());

    explicit IdentifiableMembersProxy(const T& value) : m_members(value.identifiable_members()) {}

    const auto& members() const { return m_members; }

private:
    MembersTupleType m_members;
};

}

/// @brief std::hash specialization for an `IdentifiableMembersProxy`
/// that computes a hash based on all members.
/// @tparam ...Ts are the types of all members.
template<loki::HasIdentifiableMembers T>
struct std::hash<loki::IdentifiableMembersProxy<T>>
{
    size_t operator()(const loki::IdentifiableMembersProxy<T>& proxy) const { return loki::hash_combine(proxy.members()); }
};

/// @brief std::equal_to specialization for an `IdentifiableMembersProxy`
/// that pairwise compares all members.
/// @tparam ...Ts are the types of all members.
template<loki::HasIdentifiableMembers T>
struct std::equal_to<loki::IdentifiableMembersProxy<T>>
{
    using MembersTupleType = decltype(std::declval<loki::IdentifiableMembersProxy<T>>().members());

    bool operator()(const loki::IdentifiableMembersProxy<T>& lhs, const loki::IdentifiableMembersProxy<T>& rhs) const
    {
        return loki::EqualTo<MembersTupleType>()(lhs.members(), rhs.members());
    }
};

#endif
