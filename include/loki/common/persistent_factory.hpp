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

#ifndef LOKI_INCLUDE_LOKI_COMMON_PERSISTENT_FACTORY_2_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_PERSISTENT_FACTORY_2_HPP_

#include <unordered_set>
#include <memory>
#include <mutex>
#include <tuple>


namespace loki {

template<typename HolderType>
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


    std::unordered_set<std::unique_ptr<const HolderType>, DerferencedHash<HolderType>, DereferencedEquality<HolderType>> m_data;
    // Identifiers are shared across types since types can be polymorphic
    int m_count = 0;
    // Mutex is shared for thread-safe changes to count that is shared across types
    std::mutex m_mutex;

public:
    template<typename SubType, typename... Args>
    [[nodiscard]] HolderType const* get_or_create(Args... args) {
        std::lock_guard<std::mutex> hold(m_mutex);
        int identifier = m_count;
        /* Must explicitly call the constructor of T to give exclusive access to the factory. */
        const auto [it, inserted] = m_data.emplace(std::make_unique<HolderType>(std::move(SubType(identifier, std::move(args)...))));
        if (inserted) {
            ++m_count;
        }
        return it->get();
    }
};

}


#endif