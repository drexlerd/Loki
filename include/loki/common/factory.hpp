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

#ifndef LOKI_INCLUDE_LOKI_COMMON_FACTORY_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_FACTORY_HPP_

#include <unordered_map>
#include <memory>
#include <mutex>
#include <iostream>
#include <tuple>


namespace loki {

/// @brief A thread-safe reference-counted object cache.
/// Original idea by Herb Sutter.
/// Custom deleter idea: https://stackoverflow.com/questions/49782011/herb-sutters-10-liner-with-cleanup
template<typename... Ts>
class ReferenceCountedObjectFactory {
private:
    template<typename T>
    struct ValueHash {
        std::size_t operator()(const std::shared_ptr<const T>& ptr) const {
            return std::hash<T>()(*ptr);
        }
    };

    /// @brief Equality comparison of the objects underlying the pointers.
    template<typename T>
    struct ValueEqual {
        bool operator()(const std::shared_ptr<const T>& left, const std::shared_ptr<const T>& right) const {
            return *left == *right;
        }
    };

    /// @brief Encapsulates the data of a single type.
    template<typename T>
    struct PerTypeCache {
        // cannot use weak_ptr<T> in set, so we use it as value in map.
        // shared_ptr<T> is key because
        //   1) polymorphic types do not have copy/move
        //   2) mapping from identifier to key for deletion
        // We could use raw pointer as key since we do not need reference counting.
        std::unordered_map<std::shared_ptr<const T>, std::weak_ptr<T>, ValueHash<T>, ValueEqual<T>> data;
        // For removal, we use an additional mapping from the identifier to the key of the data map.
        std::unordered_map<int, std::shared_ptr<const T>> identifier_to_key;
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
    std::shared_ptr<const T> get_or_create(Args&&... args) {
        /* we must declare sp before locking the mutex
           s.t. the deleter is called after the mutex was released in case of stack unwinding. */
        std::shared_ptr<T> sp;
        std::lock_guard<std::mutex> hold(m_cache->mutex);

        auto& t_cache = std::get<PerTypeCache<T>>(m_cache->data);
        int identifier = m_cache->count;
        /* Must explicitly call the constructor of T to give exclusive access to the factory. */
        auto key = std::make_shared<T>(T(identifier, args...));
        auto& cached = t_cache.data[key];
        sp = cached.lock();
        if (!sp) {
            ++m_cache->count;
            t_cache.identifier_to_key.emplace(identifier, key);
            /* Must explicitly call the constructor of T to give exclusive access to the factory. */
            cached = sp = std::shared_ptr<T>(
                new T(identifier, args...),
                [cache=m_cache, identifier](T* x)
                {
                    {
                        std::lock_guard<std::mutex> hold(cache->mutex);
                        auto& t_cache = std::get<PerTypeCache<T>>(cache->data);
                        const auto& key = t_cache.identifier_to_key.at(identifier);
                        t_cache.data.erase(key);
                        t_cache.identifier_to_key.erase(identifier);
                    }
                    /* After cache removal, we can call the objects destructor
                       and recursively call the deleter of children if their ref count goes to 0 */
                    delete x;
                }
            );
        }

        return sp;
    }
};

}


#endif