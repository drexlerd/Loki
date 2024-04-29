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

#ifndef LOKI_INCLUDE_LOKI_PDDL_FACTORY_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_FACTORY_HPP_

#include "loki/details/pddl/utils.hpp"
#include "loki/details/utils/segmented_vector.hpp"

#include <memory>
#include <tuple>
#include <type_traits>
#include <unordered_set>
#include <variant>

namespace loki
{

/// @brief The PDDLFactory manages unique objects in a persistent
///        and efficient manner, utilizing a combination of unordered_set for
///        uniqueness checks and SegmentedVector for continuous and
///        cache-efficient storage.
/// @tparam HolderType is the nested type which can be an std::variant.
/// @tparam N is the number of elements per segment
template<typename HolderType, typename Hash = Hash<HolderType*>, typename EqualTo = EqualTo<HolderType*>>
class PDDLFactory
{
private:
    // We use an unordered_set to test for uniqueness.
    // We use pointers to the persistent memory to reduce allocations.
    std::unordered_set<const HolderType*, Hash, EqualTo> m_uniqueness_set;
    // Use pre-allocated memory to store PDDL object persistent and continuously for improved cache locality.
    SegmentedVector<HolderType> m_persistent_vector;

    size_t m_count = 0;

public:
    explicit PDDLFactory(size_t elements_per_segment) : m_persistent_vector(SegmentedVector<HolderType>(elements_per_segment)) {}

    /// @brief Returns a pointer to an existing object
    ///        or creates it before if it does not exist.
    template<typename SubType, typename... Args>
    [[nodiscard]] HolderType const* get_or_create(Args&&... args)
    {
        /* Construct and insert the element in persistent memory. */

        // Ensure that element with identifier i is stored at position i.
        size_t identifier = m_count;
        assert(identifier == m_persistent_vector.size());

        // Explicitly call the constructor of T to give exclusive access to the factory.
        auto element = HolderType(std::move(SubType(identifier, std::forward<Args>(args)...)));
        const auto* element_ptr = &(m_persistent_vector.push_back(std::move(element)));
        // The pointer to the location in persistent memory.
        assert(element_ptr);

        /* Test for uniqueness */
        auto it = m_uniqueness_set.find(element_ptr);
        if (it == m_uniqueness_set.end())
        {
            /* Element is unique! */

            m_uniqueness_set.emplace(element_ptr);
            // Validate the element by increasing the identifier to the next free position
            ++m_count;
        }
        else
        {
            /* Element is not unique! */

            element_ptr = *it;
            // Remove duplicate from vector
            m_persistent_vector.pop_back();
        }
        // Ensure that indexing matches size of uniqueness set.
        assert(m_uniqueness_set.size() == m_count);

        return element_ptr;
    }

    /**
     * Accessors
     */

    /// @brief Returns a pointer to an existing object with the given identifier.
    [[nodiscard]] HolderType const* get(size_t identifier) const { return &(m_persistent_vector.at(identifier)); }

    /**
     * Iterators
     */
    class const_iterator
    {
    private:
        typename SegmentedVector<HolderType>::const_iterator m_iter;

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = HolderType;
        using pointer = HolderType*;
        using reference = HolderType&;
        using iterator_category = std::forward_iterator_tag;

        const_iterator(const SegmentedVector<HolderType>& persistent_vector, bool begin) : m_iter(begin ? persistent_vector.begin() : persistent_vector.end())
        {
        }

        [[nodiscard]] decltype(auto) operator*() const { return *m_iter; }

        const_iterator& operator++()
        {
            ++m_iter;
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        [[nodiscard]] bool operator==(const const_iterator& other) const { return m_iter == other.m_iter; }

        [[nodiscard]] bool operator!=(const const_iterator& other) const { return !(*this == other); }
    };

    [[nodiscard]] const_iterator begin() const { return const_iterator(m_persistent_vector, true); }

    [[nodiscard]] const_iterator end() const { return const_iterator(m_persistent_vector, false); }

    /**
     * Capacity
     */

    size_t size() const { return m_count; }
};

}

#endif
