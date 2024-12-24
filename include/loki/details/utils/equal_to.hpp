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

#include <array>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

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

/// @brief EqualTo specialization for std::array.
template<typename T, size_t N>
struct EqualTo<std::array<T, N>>
{
    bool operator()(const std::array<T, N>& lhs, const std::array<T, N>& rhs) const
    {
        for (size_t i = 0; i < N; ++i)
        {
            if (!EqualTo<std::remove_cvref_t<T>>()(lhs[i], rhs[i]))
            {
                return false;
            }
        }
        return true;
    }
};

/// @brief EqualTo specialization for std::set.
template<typename Key, typename Compare, typename Allocator>
struct EqualTo<std::set<Key, Compare, Allocator>>
{
    bool operator()(const std::set<Key, Compare, Allocator>& lhs, const std::set<Key, Compare, Allocator>& rhs) const
    {
        if (lhs.size() != rhs.size())
            return false;

        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), EqualTo<std::remove_cvref_t<Key>>());
    }
};

/// @brief EqualTo specialization for std::map.
template<typename Key, typename T, typename Compare, typename Allocator>
struct EqualTo<std::map<Key, T, Compare, Allocator>>
{
    bool operator()(const std::map<Key, T, Compare, Allocator>& lhs, const std::map<Key, T, Compare, Allocator>& rhs) const
    {
        if (lhs.size() != rhs.size())
            return false;

        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), EqualTo<std::remove_cvref_t<Key>>());
    }
};

/// @brief EqualTo specialization for std::vector.
template<typename T, typename Allocator>
struct EqualTo<std::vector<T, Allocator>>
{
    bool operator()(const std::vector<T, Allocator>& lhs, const std::vector<T, Allocator>& rhs) const
    {
        if (lhs.size() != rhs.size())
            return false;

        for (size_t i = 0; i < lhs.size(); ++i)
        {
            if (!EqualTo<std::remove_cvref_t<T>>()(lhs[i], rhs[i]))
            {
                return false;
            }
        }
        return true;
    }
};

/// @brief EqualTo specialization for a std::pair.
template<typename T1, typename T2>
struct EqualTo<std::pair<T1, T2>>
{
    bool operator()(const std::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs) const
    {
        return EqualTo<std::remove_cvref_t<T1>>()(lhs.first, rhs.first) && EqualTo<std::remove_cvref_t<T2>>()(lhs.second, rhs.second);
    }
};

/// @brief EqualTo specialization for a std::tuple.
template<typename... Ts>
struct EqualTo<std::tuple<Ts...>>
{
    bool operator()(const std::tuple<Ts...>& lhs, const std::tuple<Ts...>& rhs) const { return tuple_equals(lhs, rhs, std::index_sequence_for<Ts...> {}); }

private:
    template<std::size_t... Is>
    bool tuple_equals(const std::tuple<Ts...>& lhs, const std::tuple<Ts...>& rhs, std::index_sequence<Is...>) const
    {
        return (... && (EqualTo<std::remove_cvref_t<std::tuple_element_t<Is, std::tuple<Ts...>>>>()(std::get<Is>(lhs), std::get<Is>(rhs))));
    }
};

/// @brief EqualTo specialization for a std::variant.
template<typename... Ts>
struct EqualTo<std::variant<Ts...>>
{
    struct Visitor
    {
        template<typename T>
        bool operator()(const T& a, const T& b) const
        {
            return EqualTo<std::remove_cvref_t<T>>()(a, b);
        }

        template<typename T, typename U>
        bool operator()(const T&, const U&) const
        {
            return false;
        }
    };

    bool operator()(const std::variant<Ts...>& lhs, const std::variant<Ts...>& rhs) const
    {
        if (lhs.index() != rhs.index())
        {
            return false;
        }

        return std::visit(Visitor {}, lhs, rhs);
    }
};

/// @brief EqualTo specialization for a std::optional.
template<typename T>
struct EqualTo<std::optional<T>>
{
    bool operator()(const std::optional<T>& lhs, const std::optional<T>& rhs) const
    {
        if (!lhs && !rhs)
        {
            return true;
        }

        if (lhs && rhs)
        {
            return EqualTo<std::remove_cvref_t<T>>()(*lhs, *rhs);
        }

        return false;
    }
};

/// @brief EqualTo specialization for a std::span.
///
/// Compares the hashes of the the pointer and size.
template<typename T, std::size_t Extent>
struct EqualTo<std::span<T, Extent>>
{
    bool operator()(const std::span<T, Extent>& lhs, const std::span<T, Extent>& rhs) const { return (lhs.size() == rhs.size()) && (lhs.data() == rhs.data()); }
};

/// @brief EqualTo specialization for types T that satisfy `HasIdentifiableMembers`.
/// Dereferences the underlying pointer before forwarding the call to the std::equal_to
/// specialization of `IdentifiableMemberProxy` of T to pairwise compare all members.
/// @tparam T is the type.
template<typename T>
struct EqualTo<ObserverPtr<T>>
{
    bool operator()(loki::ObserverPtr<T> lhs, loki::ObserverPtr<T> rhs) const { return EqualTo<std::remove_cvref_t<T>>()(*lhs, *rhs); }
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
