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

#include "loki/details/utils/murmurhash3.h"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <utility>
#include <variant>
#include <vector>

namespace loki
{

/**
 * Forward declarations
 */

template<typename T>
inline void hash_combine(size_t& seed, const T& value);

template<typename T, typename... Rest>
inline void hash_combine(size_t& seed, const Rest&... rest);

template<typename... Ts>
inline size_t hash_combine(const Ts&... rest);

/// @brief `Hash` is our custom hasher, like std::hash.
///
/// Forwards to std::hash by default.
/// Specializations can be injected into the namespace.
template<typename T>
struct Hash
{
    size_t operator()(const T& element) const { return std::hash<T>()(element); }
};

/// @brief Hash specialization for std::set.
///
/// Combines the hashes of all elements in the set.
/// @tparam T
template<typename T>
struct Hash<std::set<T>>
{
    size_t operator()(const std::set<T>& set) const
    {
        std::size_t aggregated_hash = 0;
        for (const auto& item : set)
        {
            loki::hash_combine(aggregated_hash, item);
        }
        return aggregated_hash;
    }
};

/// @brief Hash specialization for std::map.
///
/// Combines the hashes of all elements in the map.
/// @tparam K is the key type.
/// @tparam V is the value type
template<typename K, typename V>
struct Hash<std::map<K, V>>
{
    size_t operator()(const std::map<K, V>& map) const
    {
        std::size_t aggregated_hash = 0;
        for (const auto& item : map)
        {
            loki::hash_combine(aggregated_hash, item);
        }
        return aggregated_hash;
    }
};

/// @brief Hash specialization for std::vector.
///
/// Combines the hashes of all elements in the vector.
/// @tparam T
template<typename T>
struct Hash<std::vector<T>>
{
    size_t operator()(const std::vector<T>& vec) const
    {
        size_t seed = vec.size();
        size_t hash[2] = { 0, 0 };

        loki::MurmurHash3_x64_128(vec.data(), vec.size() * sizeof(T), seed, hash);

        loki::hash_combine(seed, hash[0]);
        loki::hash_combine(seed, hash[1]);

        return seed;
    }
};

/// @brief Hash specialization for a std::pair.
///
/// Combines the hashes for first and second.
/// @tparam T1
/// @tparam T2
template<typename T1, typename T2>
struct Hash<std::pair<T1, T2>>
{
    size_t operator()(const std::pair<T1, T2>& pair) const { return loki::hash_combine(pair.first, pair.second); }
};

/// @brief Hash specialization for a std::tuple.
///
/// Combines the hashes of all elements in the tuple.
/// @tparam ...Ts
template<typename... Ts>
struct Hash<std::tuple<Ts...>>
{
    size_t operator()(const std::tuple<Ts...>& tuple) const
    {
        size_t aggregated_hash = sizeof...(Ts);
        std::apply([&aggregated_hash](const Ts&... args) { (loki::hash_combine(aggregated_hash, args), ...); }, tuple);
        return aggregated_hash;
    }
};

/// @brief Hash specialization for a std::variant.
///
/// Hashes the underlying object.
/// @tparam ...Ts
template<typename... Ts>
struct Hash<std::variant<Ts...>>
{
    size_t operator()(const std::variant<Ts...>& variant) const
    {
        return std::visit(
            [](const auto& arg)
            {
                using DecayedType = std::decay_t<decltype(arg)>;
                return Hash<DecayedType>()(arg);
            },
            variant);
    }
};

/// @brief Hash specialization for an std::optional.
///
/// Hashes the underlying object if it exists, otherwise, returns 0.
/// @tparam T
template<typename T>
struct Hash<std::optional<T>>
{
    size_t operator()(const std::optional<T>& optional) const { return optional.has_value() ? Hash<T>()(optional.value()) : 0; }
};

/**
 * Definitions
 */

/// @brief Computes that hash for an object and combines it into the seed using Boost's hash combine function.
/// @tparam T is the type of the object.
/// @param seed is the seed.
/// @param value is the object.
template<typename T>
inline void hash_combine(size_t& seed, const T& value)
{
    seed ^= Hash<T>()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<typename T, typename... Rest>
inline void hash_combine(size_t& seed, const Rest&... rest)
{
    (loki::hash_combine(seed, rest), ...);
}

template<typename... Ts>
inline size_t hash_combine(const Ts&... rest)
{
    size_t seed = 0;
    (loki::hash_combine(seed, rest), ...);
    return seed;
}

}

#endif
