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

#ifndef LOKI_INCLUDE_LOKI_PDDL_UTILS_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_UTILS_HPP_

#include <type_traits>
#include <variant>

namespace loki
{

/**
 * Hash
 */

template<typename T, typename Enable = void>
struct Hash;

// Specialization for pointers
template<typename T>
struct Hash<T*, void>
{
    std::size_t operator()(const T* ptr) const { return ptr->hash(); }
};

// Specialization for non-pointers
template<typename T>
struct Hash<T, std::enable_if_t<!std::is_pointer_v<T>>>  // Enable only if T is not a pointer
{
    std::size_t operator()(const T& obj) const { return obj.hash(); }
};

// Specialization for std::variant
template<typename... Ts>
struct Hash<std::variant<Ts...>>
{
    std::size_t operator()(const std::variant<Ts...>& ptr) const
    {
        return std::visit([](const auto& val) -> std::size_t { return val.hash(); }, ptr);
    }
};

template<typename... Ts>
struct Hash<std::variant<Ts...>*>
{
    std::size_t operator()(const std::variant<Ts...>* ptr) const
    {
        return std::visit([](const auto& val) -> std::size_t { return val.hash(); }, *ptr);
    }
};

/**
 * EqualTo
 */

template<typename T, typename Enable = void>
struct EqualTo;

// Specialization for pointers
template<typename T>
struct EqualTo<T*, void>
{
    bool operator()(const T* left, const T* right) const { return *left == *right; }
};

// Specialization for non-pointers
template<typename T>
struct EqualTo<T, std::enable_if_t<!std::is_pointer_v<T>>>  // Enable only if T is not a pointer
{
    bool operator()(const T& left, const T& right) const { return left == right; }
};

}

#endif
