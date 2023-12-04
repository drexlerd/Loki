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

#ifndef LOKI_INCLUDE_LOKI_COMMON_CACHE_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_CACHE_HPP_

#include <cassert>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <iostream>
#include <tuple>


namespace loki {
template<typename KeyType, typename... Ts>
class Cache {
    private:
        template<typename T>
        using ElementType = std::shared_ptr<const T>;

        template<typename T>
        using MapType = std::unordered_map<KeyType, ElementType<T>>;

        std::tuple<MapType<Ts>...> bindings;

    public:
        /// @brief Gets a binding of type T. Returns nullptr if it does not exist.
        template<typename T> 
        ElementType<T> get(const KeyType& key) const {
            const auto& t_bindings = std::get<MapType<T>>(bindings);
            auto it = t_bindings.find(key);
            if (it != t_bindings.end()) {
                return it->second;
            }
            return nullptr;
        }

        template<typename T>
        const MapType<T>& get() const {
            return std::get<MapType<T>>(bindings);
        }

        /// @brief Inserts a binding of type T
        template<typename T>
        void insert(const KeyType& key, const ElementType<T>& binding) {
            auto& t_bindings = std::get<MapType<T>>(bindings);
            assert(!t_bindings.count(key));
            t_bindings.emplace(key, binding);
        }
};

}


#endif