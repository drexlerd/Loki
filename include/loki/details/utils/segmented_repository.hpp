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

#ifndef LOKI_INCLUDE_LOKI_UTILS_SEGMENTED_REPOSITORY_HPP_
#define LOKI_INCLUDE_LOKI_UTILS_SEGMENTED_REPOSITORY_HPP_

#include "loki/details/utils/equal_to.hpp"
#include "loki/details/utils/hash.hpp"
#include "loki/details/utils/observer_ptr.hpp"
#include "loki/details/utils/segmented_vector.hpp"

#include <absl/container/flat_hash_set.h>
#include <memory>
#include <tuple>
#include <unordered_set>
#include <variant>

namespace loki
{

/// @brief `SegmentedRepository` is a container for managing the unique creation of immutable objects
/// in a persistent and efficient manner, utilizing a combination of unordered_set for
/// uniqueness checks and SegmentedVector for continuous and cache-efficient storage of value types.
///
/// The unordered_set's key type is an `ObserverPtr<const T>` that point to the persistent memory.
/// This has the advantage that the uniqueness map stays small because for each potentially large
/// object of type T, the unordered_map only has to store a pointer (8 byte).
/// Alternatively, one could have also used raw pointers directly.
/// However, using raw pointers is error prone because the STL provides specializations
/// for raw pointers that are meaningless in our context.
/// The `ObserverPtr` has explicitly deleted conversions and comparison,
/// hence requiring users to explicitly specialize std::hash and std::equal_to.
/// @tparam T is the type of the elements to be stored.
/// @tparam Hash is the hash function that defaults to the std::hash specialization of `ObserverPtr<const T>`
/// A user has to explicitly define it. A type that satisfies the concept `HasIdentifyingMembers`
/// in its intended way will obtain automatically generated std::hash specialization.
/// @tparam KeyEqual is the comparison function that defaults to the std::equal_to specialization of
/// `ObserverPtr<const T>`. A user has to explicitly define it. Similarly as for the Hash,
/// a type that satisfies the concept `HasIdentifyingMembers` in its intended way will obtain
/// automatically generated std::equal_to specialization.
template<typename T, typename Hash = Hash<ObserverPtr<const T>>, typename EqualTo = EqualTo<ObserverPtr<const T>>>
class SegmentedRepository
{
private:
    /* Internal elements.*/
    // We use an unordered_set to test for uniqueness.
    absl::flat_hash_set<ObserverPtr<const T>, Hash, EqualTo> m_uniqueness_set;
    // We use pre-allocated memory to store objects persistent.
    SegmentedVector<T> m_persistent_vector;

    /* External elements. */
    const SegmentedRepository* m_parent;
    size_t m_num_parent_elements;

    void range_check(size_t pos) const
    {
        if (pos >= size())
        {
            throw std::out_of_range("SegmentedRepository::range_check: pos (which is " + std::to_string(pos) + ") >= this->size() (which is "
                                    + std::to_string(size()) + ")");
        }
    }

    T const* find(ObserverPtr<const T> element_ptr) const
    {
        if (auto it = m_uniqueness_set.find(element_ptr); it != m_uniqueness_set.end())
        {
            return it->get();
        }
        return nullptr;
    }

public:
    SegmentedRepository(size_t initial_num_element_per_segment = 16, size_t maximum_num_elements_per_segment = 16 * 1024) :
        m_uniqueness_set(),
        m_persistent_vector(SegmentedVector<T>(initial_num_element_per_segment, maximum_num_elements_per_segment)),
        m_parent(nullptr),
        m_num_parent_elements(0)
    {
    }
    SegmentedRepository(const SegmentedRepository& other) = delete;
    SegmentedRepository& operator=(const SegmentedRepository& other) = delete;
    SegmentedRepository(SegmentedRepository&& other) = default;
    SegmentedRepository& operator=(SegmentedRepository&& other) = default;

    void set_parent(const SegmentedRepository& parent)
    {
        assert(m_uniqueness_set.empty() && m_persistent_vector.empty());
        m_parent = &parent;
        m_num_parent_elements = m_parent->size();
    }

    /// @brief Returns a pointer to an existing object or creates it before if it does not exist.
    template<typename... Args>
    T const* get_or_create(Args&&... args)
    {
        assert(m_uniqueness_set.size() == m_persistent_vector.size());

        /* Construct and insert the element in persistent memory. */

        // Ensure that element with identifier i is stored at position i.
        size_t index = m_uniqueness_set.size() + m_num_parent_elements;

        assert((m_parent ? m_parent->size() : 0) == m_num_parent_elements);

        // Create element of type T
        auto element = T(index, std::forward<Args>(args)...);
        auto element_ptr = ObserverPtr<const T>(&element);

        /* Test for uniqueness. */
        if (auto result = find(element_ptr))
        {
            return result;
        }

        if (auto result = (m_parent ? m_parent->find(element_ptr) : nullptr))
        {
            return result;
        }

        /* Element is unique! */

        // Copy element to persistent memory
        m_persistent_vector.push_back(std::move(element));

        // Fetch the pointer to persistent element;
        const auto persistent_addr = &m_persistent_vector.back();

        // Mark the element as not unique.
        m_uniqueness_set.insert(persistent_addr);

        // Return pointer to persistent element.
        return persistent_addr;
    }

    /**
     * Accessors
     */

    /// @brief Returns a pointer to an existing object with the given pos.
    T const* operator[](size_t pos) const
    {
        assert(pos < size());
        return (pos < m_num_parent_elements) ? m_parent[pos] : &(m_persistent_vector[pos - m_num_parent_elements]);
    }

    /// @brief Returns a pointer to an existing object with the given pos.
    /// @param pos
    /// @return
    T const* at(size_t pos) const
    {
        range_check(pos);
        return (pos < m_num_parent_elements) ? m_parent->at(pos) : &(m_persistent_vector.at(pos - m_num_parent_elements));
    }

    auto begin() const { return m_persistent_vector.begin(); }

    auto end() const { return m_persistent_vector.end(); }

    const SegmentedVector<T>& get_storage() const { return m_persistent_vector; }

    /**
     * Capacity
     */

    size_t size() const { return m_persistent_vector.size() + m_num_parent_elements; }
};

}

#endif
