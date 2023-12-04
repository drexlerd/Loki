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

/// @brief A reference-counted object cache.
/// Original idea by Herb Sutter.
/// Custom deleter idea: https://stackoverflow.com/questions/49782011/herb-sutters-10-liner-with-cleanup
template<typename... Ts>
class ReferenceCountedObjectFactory {
private:
    template<typename T>
    struct PerTypeCache {
        // Weak_ptr cannot be key, so we simply use the object itself.
        std::unordered_map<T, std::weak_ptr<T>> data;
    };

    std::tuple<std::shared_ptr<PerTypeCache<Ts>>...> m_cache;

    // Identifiers are shared since types can be polymorphic
    int m_count = 0;

public:
    ReferenceCountedObjectFactory()
        : m_cache((std::make_shared<PerTypeCache<Ts>>())...) { }

    /// @brief Gets a shared reference to the object of type T with the given arguments.
    ///        If such an object does not exists then it creates one.
    /// @tparam ...Args The arguments that are passed to the constructor of T.
    /// @param ...args
    /// @return
    template<typename T, typename... Args>
    std::shared_ptr<const T> get_or_create(Args&&... args) {
        auto& t_cache = std::get<std::shared_ptr<PerTypeCache<T>>>(m_cache);
        /* Must explicitly call the constructor of T to give exclusive access to the factory. */
        auto element = std::make_unique<T>(T(m_count, args...));
        /* we must declare sp before locking the mutex
           s.t. the deleter is called after the mutex was released in case of stack unwinding. */
        std::shared_ptr<T> sp;
        auto& cached = t_cache->data[*element];
        sp = cached.lock();
        // std::lock_guard<std::mutex> hold(t_cache->mutex);
        if (!sp) {
            ++m_count;
            cached = sp = std::shared_ptr<T>(
                element.get(),
                [parent=t_cache, original_deleter=element.get_deleter()](T* x)
                {
                    {
                        // std::lock_guard<std::mutex> hold(parent->mutex);
                        parent->data.erase(*x);
                    }
                    /* After cache removal, we can call the objects destructor
                       and recursively call the deleter of children if their ref count goes to 0 */
                    original_deleter(x);
                }
            );
            element.release();
        }

        return sp;
    }
};

}


#endif