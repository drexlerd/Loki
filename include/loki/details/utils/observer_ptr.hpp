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

namespace loki
{

/// @brief `ObserverPtr` wraps T* a non-owning raw pointer.
/// Conversion to raw pointer is explicitly deleted to avoid accidental usage of std overloads, e.g., std::hash<T*>.
/// @tparam T is the underlying type.
template<typename T>
class ObserverPtr
{
public:
    ObserverPtr() noexcept : ptr(nullptr) {}
    ObserverPtr(T* ptr) noexcept : ptr(ptr) {}  ///< allow implicit conversion from raw pointer

    T* get() noexcept { return ptr; }
    const T* get() const noexcept { return ptr; }

    // Dereference convenience
    T& operator*() { return *ptr; }
    T* operator->() { return ptr; }
    const T& operator*() const { return *ptr; }
    const T* operator->() const { return ptr; }

    // Comparison operators
    bool operator==(const ObserverPtr& other) const noexcept { return ptr == other.ptr; }
    bool operator!=(const ObserverPtr& other) const noexcept { return ptr != other.ptr; }
    bool operator<(const ObserverPtr& other) const noexcept { return ptr < other.ptr; }
    bool operator<=(const ObserverPtr& other) const noexcept { return ptr <= other.ptr; }
    bool operator>(const ObserverPtr& other) const noexcept { return ptr > other.ptr; }
    bool operator>=(const ObserverPtr& other) const noexcept { return ptr >= other.ptr; }

    // nullptr comparisons
    bool operator==(std::nullptr_t) const noexcept { return ptr == nullptr; }
    bool operator!=(std::nullptr_t) const noexcept { return ptr != nullptr; }

    // Conversion to boolean
    explicit operator bool() const noexcept { return ptr != nullptr; }

private:
    T* ptr;
};

}

#endif
