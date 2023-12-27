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

#include "segmented_persistent_vector.hpp"

#include <unordered_set>
#include <memory>
#include <mutex>
#include <tuple>
#include <type_traits>
#include <variant>


namespace loki {

template<typename T> struct is_variant : std::false_type {};

template<typename ...Args>
struct is_variant<std::variant<Args...>> : std::true_type {};

template<typename T>
inline constexpr bool is_variant_v=is_variant<T>::value;

template<typename T>
static int getIdentifier(const T& holder) {
    if constexpr (is_variant_v<T>) {
        // Use std::visit for std::variant
        return std::visit([](const auto& obj) { return obj.get_identifier(); }, holder);
    } else {
        // Direct call for non-variant types
        return holder.get_identifier();
    }
}

template<typename HolderType>
class PersistentFactory {
private:
    std::unordered_set<HolderType> m_uniqueness_set;
    SegmentedPersistentVector<HolderType> m_persistent_vector;
    
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
        const auto [it, inserted] = m_uniqueness_set.emplace(SubType(identifier, args...));
        if (inserted) {
            ++m_count;
            /* Ensure that element with identifier i will be stored at location i*/
            assert(identifier == static_cast<int>(m_persistent_vector.size()));
            return &m_persistent_vector.push_back(SubType(identifier, std::move(args)...));
        }
        return &m_persistent_vector[getIdentifier(*it)];
    }
};

}


#endif