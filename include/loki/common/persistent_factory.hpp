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

#ifndef LOKI_INCLUDE_LOKI_COMMON_PERSISTENT_FACTORY_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_PERSISTENT_FACTORY_HPP_

#include <unordered_set>
#include <memory>
#include <mutex>
#include <tuple>


namespace loki {

template<typename... Ts>
class PersistentFactory {
private:
    template<typename T>
    struct DerferencedHash {
        std::size_t operator()(const std::unique_ptr<const T>& ptr) const {
            return std::hash<T>()(*ptr);
        }
    };

    /// @brief Equality comparison of the objects underlying the pointers.
    template<typename T>
    struct DereferencedEquality {
        bool operator()(const std::unique_ptr<const T>& left, const std::unique_ptr<const T>& right) const {
            return *left == *right;
        }
    };

    /// @brief Encapsulates the data of a single type.
    template<typename T>
    struct PerTypeCache {
        std::unordered_set<std::unique_ptr<const T>, DerferencedHash<T>, DereferencedEquality<T>> data;
    };

    std::tuple<PerTypeCache<Ts>...> m_data;
    // Identifiers are shared across types since types can be polymorphic
    int m_count = 0;
    // Mutex is shared for thread-safe changes to count that is shared across types
    std::mutex m_mutex;

public:
    /// @brief Gets a shared reference to the object of type T with the given arguments.
    ///        If such an object does not exists then it creates one.
    /// @tparam ...Args The arguments that are passed to the constructor of T.
    /// @param ...args
    /// @return
    template<typename T, typename... Args>
    [[nodiscard]] T const* get_or_create(Args&&... args) {
        /* we must declare sp before locking the mutex
           s.t. the deleter is called after the mutex was released in case of stack unwinding. */
        std::lock_guard<std::mutex> hold(m_mutex);
        auto& t_cache = std::get<PerTypeCache<T>>(m_data);
        int identifier = m_count;
        /* Must explicitly call the constructor of T to give exclusive access to the factory. */
        const auto [it, inserted] = t_cache.data.emplace(std::make_unique<T>(std::move(T(identifier, std::move(args)...))));
        if (inserted) {
            ++m_count;
        }
        return it->get();
    }
};

}


#endif