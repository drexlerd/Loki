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

#ifndef LOKI_INCLUDE_LOKI_COMMON_PDDL_PERSISTENT_FACTORY_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_PDDL_PERSISTENT_FACTORY_HPP_

#include "declarations.hpp"
#include "segmented_vector.hpp"

#include <unordered_set>
#include <memory>
#include <mutex>
#include <tuple>
#include <type_traits>
#include <variant>


namespace loki {
/// @brief The PersistentFactory class manages unique objects in a persistent 
///        and efficient manner, utilizing a combination of unordered_set for 
///        uniqueness checks and SegmentedVector for continuous and 
///        cache-efficient storage.
/// @tparam HolderType is the nested type which can be an std::variant.
/// @tparam N is the number of elements per segment
template<typename HolderType, ElementsPerSegment N>
class PersistentFactory {
private:
    template<typename T>
    struct DerferencedHash {
        std::size_t operator()(const T* ptr) const {
            return std::hash<T>()(*ptr);
        }
    };

    /// @brief Equality comparison of the objects underlying the pointers.
    template<typename T>
    struct DereferencedEquality {
        bool operator()(const T* left, const T* right) const {
            return *left == *right;
        }
    };

    // We use an unordered_set to test for uniqueness.
    // We use pointers to the persistent memory to reduce allocations.
    std::unordered_set<const HolderType*, DerferencedHash<HolderType>, DereferencedEquality<HolderType>> m_uniqueness_set;
    // Use pre-allocated memory to store PDDL object persistent and continuously for improved cache locality.
    SegmentedVector<HolderType, N> m_persistent_vector;
    
    int m_count = 0;

    std::mutex m_mutex;

public:
    /// @brief Returns a pointer to an existing object
    ///        or creates it before if it does not exist.v
    template<typename SubType, typename... Args>
    [[nodiscard]] HolderType const* get_or_create(Args... args) {
        std::lock_guard<std::mutex> hold(m_mutex);
        /* Construct and insert the element in persistent memory. */
        int identifier = m_count;
        // Ensure that element with identifier i is stored at position i.
        assert((identifier == (static_cast<int>(m_persistent_vector.size())-1))
            || (identifier == static_cast<int>(m_persistent_vector.size())));
        // The pointer to the location in persistent memory.
        const auto* element_ptr = static_cast<HolderType*>(nullptr);
        // Explicitly call the constructor of T to give exclusive access to the factory.
        auto element = HolderType(std::move(SubType(identifier, std::move(args)...)));
        bool overwrite_last_element = (identifier == static_cast<int>(m_persistent_vector.size()) - 1);
        if (overwrite_last_element) {
            element_ptr = &(m_persistent_vector[identifier] = std::move(element));
        } else {
            element_ptr = &(m_persistent_vector.push_back(std::move(element)));
        }
        /* Test for uniqueness */
        auto it = m_uniqueness_set.find(element_ptr);
        if (it == m_uniqueness_set.end()) {
            // Element is unique! 
            m_uniqueness_set.emplace(element_ptr);
            // Validate the element by increasing the identifier to the next free position
            ++m_count;
        } else {
            // Element is not unique!
            // Return the existing one.
            element_ptr = *it;
        }
        return element_ptr;
    }

    size_t size() const {
        return m_count;
    }
};

}


#endif