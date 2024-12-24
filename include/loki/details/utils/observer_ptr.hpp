/*
 * Copyright (C) 2023 Dominik Drexler
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

#ifndef LOKI_INCLUDE_LOKI_UTILS_OBSERVER_PTR_HPP_
#define LOKI_INCLUDE_LOKI_UTILS_OBSERVER_PTR_HPP_

#include "loki/details/utils/members_proxy.hpp"

namespace loki
{

/// @brief `ObserverPtr` wraps T* a non-owning raw pointer.
///
/// Implicit raw pointer conversion is deleted to avoid accidental usage of std overloads, e.g., std::hash<T*>.
/// Comparison operators are deleted to avoid accidental pointer comparisons.
/// @tparam T is the underlying type.
template<typename T>
class ObserverPtr
{
public:
    ObserverPtr() noexcept : ptr(nullptr) {}
    ObserverPtr(T* ptr) noexcept : ptr(ptr) {}  ///< allow implicit conversion from raw pointer

    /**
     * Accessors
     */

    T* get() noexcept { return ptr; }
    const T* get() const noexcept { return ptr; }

    T& operator*() { return *ptr; }
    T* operator->() { return ptr; }
    const T& operator*() const { return *ptr; }
    const T* operator->() const { return ptr; }

    /**
     * Comparisons
     */

    // Explicitly delete comparison to require user defined comparisons.
    bool operator==(const ObserverPtr& other) const noexcept = delete;
    bool operator!=(const ObserverPtr& other) const noexcept = delete;
    bool operator<(const ObserverPtr& other) const noexcept = delete;
    bool operator<=(const ObserverPtr& other) const noexcept = delete;
    bool operator>(const ObserverPtr& other) const noexcept = delete;
    bool operator>=(const ObserverPtr& other) const noexcept = delete;

    /**
     * Conversions
     */

    // Explicitly delete raw pointer conversions.
    operator T*() const = delete;

    // Conversion to boolean
    explicit operator bool() const noexcept { return ptr != nullptr; }

private:
    T* ptr;
};

}

/// @brief std::hash specialization for types T that satisfy `HasIdentifiableMembers`.
/// Dereferences the underlying pointer before forwarding the call to the std::hash
/// specialization of `IdentifiableMembersProxy` of T to compute a hash based on all members.
/// @tparam T is the type.
template<loki::HasIdentifiableMembers T>
struct std::hash<loki::ObserverPtr<T>>
{
    size_t operator()(loki::ObserverPtr<T> ptr) const { return std::hash<loki::IdentifiableMembersProxy<T>>()(loki::IdentifiableMembersProxy<T>(*ptr)); }
};

/// @brief std::equal_to specialization for types T that satisfy `HasIdentifiableMembers`.
/// Dereferences the underlying pointer before forwarding the call to the std::equal_to
/// specialization of `IdentifiableMemberProxy` of T to pairwise compare all members.
/// @tparam T is the type.
template<loki::HasIdentifiableMembers T>
struct std::equal_to<loki::ObserverPtr<T>>
{
    bool operator()(loki::ObserverPtr<T> lhs, loki::ObserverPtr<T> rhs) const
    {
        return std::equal_to<loki::IdentifiableMembersProxy<T>>()(loki::IdentifiableMembersProxy<T>(*lhs), loki::IdentifiableMembersProxy<T>(*rhs));
    }
};

#endif