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

#ifndef LOKI_INCLUDE_LOKI_COMMON_PDDL_FACTORY_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_PDDL_FACTORY_HPP_

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <mutex>
#include <tuple>


namespace loki {

/* Not used anymore but still a useful piece of code. */

/// @brief A thread-safe reference-counted object cache.
///        An idea by Herb Sutter.
///        Custom deleter idea: https://stackoverflow.com/questions/49782011/herb-sutters-10-liner-with-cleanup
///        TODO: improve the quality of the answer on stackoverflow
template<typename... Ts>
class ReferenceCountedObjectFactory {
private:

    /// @brief Encapsulates the data of a single type.
    template<typename T>
    struct PerTypeCache {
        std::unordered_set<T> uniqueness;
        std::unordered_map<int, std::weak_ptr<const T>> identifier_to_object;
    };

    /// @brief Encapsulates the data of all types.
    struct Cache {
        std::tuple<PerTypeCache<Ts>...> data;
        // Identifiers are shared across types since types can be polymorphic
        int count = 0;
        // Mutex is shared for thread-safe changes to count that is shared across types
        std::mutex mutex;
    };

    std::shared_ptr<Cache> m_cache;

public:
    ReferenceCountedObjectFactory()
        : m_cache(std::make_shared<Cache>()) { }

    /// @brief Gets a shared reference to the object of type T with the given arguments.
    ///        If such an object does not exists then it creates one.
    /// @tparam ...Args The arguments that are passed to the constructor of T.
    /// @param ...args
    /// @return
    template<typename T, typename... Args>
    [[nodiscard]] std::shared_ptr<const T> get_or_create(Args... args) {
        /* we must declare sp before locking the mutex
           s.t. the deleter is called after the mutex was released in case of stack unwinding. */
        std::shared_ptr<T> sp;
        std::lock_guard<std::mutex> hold(m_cache->mutex);

        auto& t_cache = std::get<PerTypeCache<T>>(m_cache->data);
        int identifier = m_cache->count;
        auto key = T(identifier, args...);
        const auto& [it, inserted] = t_cache.uniqueness.insert(key);
        if (!inserted) {
            assert(t_cache.identifier_to_object.count(it->get_identifier()));
            return t_cache.identifier_to_object.at(it->get_identifier()).lock();
        }
        ++m_cache->count;
        /* Must explicitly call the constructor of T to give exclusive access to the factory. */
        // Extensions: To ensure that the memory for T and the control block is allocated once,
        // we could use std::allocated_shared and provide a custom allocator.
        sp = std::shared_ptr<T>(
            new T(identifier, args...),
            [cache=m_cache, identifier](T* x)
            {
                {
                    std::lock_guard<std::mutex> hold(cache->mutex);
                    auto& t_cache = std::get<PerTypeCache<T>>(cache->data);
                    t_cache.uniqueness.erase(*x);
                    t_cache.identifier_to_object.erase(identifier);
                }
                /* After cache removal, we can call the objects destructor
                    and recursively call the deleter of children if their ref count goes to 0 */
                delete x;
            }
        );
        t_cache.identifier_to_object.emplace(identifier, sp);
        return sp;
    }
};

}

#endif