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
    bool operator()(const T& lhs, const T& rhs) const { return std::equal_to<T> {}(lhs, rhs); }
};

template<typename T, size_t N>
struct EqualTo<std::array<T, N>>
{
    bool operator()(const std::array<T, N>& lhs, const std::array<T, N>& rhs) const
    {
        if constexpr (N == 0)
            return true;

        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), loki::EqualTo<std::decay_t<T>> {});
    }
};

template<typename T>
struct EqualTo<std::reference_wrapper<T>>
{
    bool operator()(const std::reference_wrapper<T>& lhs, const std::reference_wrapper<T>& rhs) const
    {
        return loki::EqualTo<std::decay_t<T>> {}(lhs.get(), rhs.get());
    }
};

template<typename Key, typename Compare, typename Allocator>
struct EqualTo<std::set<Key, Compare, Allocator>>
{
    bool operator()(const std::set<Key, Compare, Allocator>& lhs, const std::set<Key, Compare, Allocator>& rhs) const
    {
        // Check size first
        if (lhs.size() != rhs.size())
        {
            return false;
        }

        // Compare each element using loki::EqualTo
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), loki::EqualTo<std::decay_t<Key>> {});
    }
};

template<typename Key, typename T, typename Compare, typename Allocator>
struct EqualTo<std::map<Key, T, Compare, Allocator>>
{
    bool operator()(const std::map<Key, T, Compare, Allocator>& lhs, const std::map<Key, T, Compare, Allocator>& rhs) const
    {
        // Check if sizes are different
        if (lhs.size() != rhs.size())
        {
            return false;
        }

        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), loki::EqualTo<std::pair<std::decay_t<Key>, std::decay_t<T>>> {});
    }
};

template<typename T, typename Allocator>
struct EqualTo<std::vector<T, Allocator>>
{
    bool operator()(const std::vector<T, Allocator>& lhs, const std::vector<T, Allocator>& rhs) const
    {
        // Check size first
        if (lhs.size() != rhs.size())
        {
            return false;
        }

        // Compare each element using loki::EqualTo
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), loki::EqualTo<std::decay_t<T>> {});
    }
};

template<typename T1, typename T2>
struct EqualTo<std::pair<T1, T2>>
{
    bool operator()(const std::pair<T1, T2>& lhs, const std::pair<T1, T2>& rhs) const
    {
        return loki::EqualTo<std::decay_t<T1>>()(lhs.first, rhs.first) && loki::EqualTo<std::decay_t<T2>> {}(lhs.second, rhs.second);
    }
};

template<typename... Ts>
struct EqualTo<std::tuple<Ts...>>
{
    bool operator()(const std::tuple<Ts...>& lhs, const std::tuple<Ts...>& rhs) const
    {
        return std::apply(
            [&rhs](const Ts&... lhs_args)
            { return std::apply([&lhs_args...](const Ts&... rhs_args) { return (loki::EqualTo<std::decay_t<Ts>> {}(lhs_args, rhs_args) && ...); }, rhs); },
            lhs);
    }
};

template<typename... Ts>
struct EqualTo<std::variant<Ts...>>
{
    bool operator()(const std::variant<Ts...>& lhs, const std::variant<Ts...>& rhs) const
    {
        return std::visit(
            [](const auto& l, const auto& r)
            {
                // Check if types match
                if constexpr (std::is_same_v<std::decay_t<decltype(l)>, std::decay_t<decltype(r)>>)
                {
                    // Recursively apply loki::EqualTo for matching types
                    return loki::EqualTo<std::decay_t<decltype(l)>> {}(l, r);
                }
                // Different types are always unequal
                return false;
            },
            lhs,
            rhs);
    }
};

template<typename T>
struct EqualTo<std::optional<T>>
{
    bool operator()(const std::optional<T>& lhs, const std::optional<T>& rhs) const
    {
        // Check for presence of values
        if (lhs.has_value() != rhs.has_value())
        {
            return false;
        }

        // If both are empty, they're equal
        if (!lhs.has_value() && !rhs.has_value())
        {
            return true;
        }

        // Compare the contained values using loki::EqualTo
        return loki::EqualTo<std::decay_t<T>> {}(lhs.value(), rhs.value());
    }
};

template<typename T, std::size_t Extent>
struct EqualTo<std::span<T, Extent>>
{
    bool operator()(const std::span<T, Extent>& lhs, const std::span<T, Extent>& rhs) const
    {
        // Check size first
        if (lhs.size() != rhs.size())
        {
            return false;
        }

        // Compare each element using loki::EqualTo
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), loki::EqualTo<std::decay_t<T>> {});
    }
};

/// @brief EqualTo specialization for types T that satisfy `HasIdentifyingMembers`.
/// Dereferences the underlying pointer before forwarding the call to the std::equal_to
/// specialization of `IdentifiableMemberProxy` of T to pairwise compare all members.
/// @tparam T is the type.
template<typename T>
struct EqualTo<ObserverPtr<T>>
{
    bool operator()(loki::ObserverPtr<T> lhs, loki::ObserverPtr<T> rhs) const { return EqualTo<std::decay_t<T>> {}(*lhs, *rhs); }
};

/// @brief EqualTo specialization for an `IdentifiableMembersProxy`
/// that pairwise compares all members.
/// @tparam ...Ts are the types of all members.
template<HasIdentifyingMembers T>
struct EqualTo<T>
{
    using MembersTupleType = decltype(std::declval<T>().identifying_members());

    bool operator()(const T& lhs, const T& rhs) const
    {
        return EqualTo<std::decay_t<MembersTupleType>> {}(lhs.identifying_members(), rhs.identifying_members());
    }
};

}

#endif
