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

#ifndef LOKI_INCLUDE_LOKI_UTILS_HASH_HPP_
#define LOKI_INCLUDE_LOKI_UTILS_HASH_HPP_

#include "loki/details/utils/concepts.hpp"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <ranges>
#include <utility>

namespace loki
{

/// @brief `ShallowHash` is a hasher that should be implemented to work in the context where
/// all objects of a custom type `T` are uniquely created and uncopieable.
/// This ensures that pointers can simply be hashed and do not need to be dereferenced.
/// @tparam T the type of the object to hash.
template<typename T>
struct ShallowHash
{
    size_t operator()(const T& element) const { return std::hash<T>()(element); }
};

struct ShallowHashCombiner
{
public:
    template<typename T>
    void operator()(size_t& seed, const T& value) const
    {
        seed ^= ShallowHash<T>()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    void operator()(size_t& seed, const std::size_t& value) const { seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2); }

    template<typename... Types>
    size_t operator()(const Types&... args) const
    {
        size_t seed = 0;
        ((*this)(seed, args), ...);
        return seed;
    }
};

/// Spezialization for std::ranges::forward_range.
template<typename ForwardRange>
requires std::ranges::forward_range<ForwardRange>
struct ShallowHash<ForwardRange>
{
    size_t operator()(const ForwardRange& range) const
    {
        std::size_t aggregated_hash = 0;
        for (const auto& item : range)
        {
            ShallowHashCombiner()(aggregated_hash, item);
        }
        return aggregated_hash;
    }
};

/// Spezialization for std::variant.
template<typename Variant>
requires IsVariant<Variant>
struct ShallowHash<Variant>
{
    size_t operator()(const Variant& variant) const
    {
        return std::visit([](const auto& arg) { return ShallowHash<decltype(arg)>()(arg); }, variant);
    }
};

}

#endif
