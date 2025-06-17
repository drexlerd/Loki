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
    explicit SegmentedRepository(const SegmentedRepository* parent = nullptr) :
        m_uniqueness_set(),
        m_persistent_vector(),
        m_parent(parent),
        m_num_parent_elements(0)
    {
        if (parent)
        {
            if (parent->m_parent)
                throw std::runtime_error("SegmentedRepository::SegmentedRepository: Multi chaining of SegmentedRepository is not supported.");

            m_num_parent_elements = m_parent->size();
        }
    }

    SegmentedRepository(const SegmentedRepository& other) = delete;
    SegmentedRepository& operator=(const SegmentedRepository& other) = delete;
    SegmentedRepository(SegmentedRepository&& other) = default;
    SegmentedRepository& operator=(SegmentedRepository&& other) = default;

    T const* find(const T& element) const
    {
        auto element_ptr = ObserverPtr<const T>(&element);

        if (auto result = find(element_ptr))
        {
            return result;
        }

        if (auto result = (m_parent ? m_parent->find(element_ptr) : nullptr))
        {
            return result;
        }

        return nullptr;
    }

    T const* insert(const T& element)
    {
        // Ensure that element does not exist
        assert(!find(element));

        // Copy element to persistent memory
        m_persistent_vector.push_back(element);

        // Fetch the pointer to persistent element;
        const auto persistent_addr = &m_persistent_vector.back();

        // Mark the element as not unique.
        m_uniqueness_set.insert(persistent_addr);

        // Return pointer to persistent element.
        return persistent_addr;
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

        /* Test for uniqueness. */
        if (auto result = find(element))
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

    class const_iterator
    {
    private:
        const SegmentedRepository* m_repo;
        bool m_in_parent;

        SegmentedVector<T>::const_iterator m_it;
        SegmentedVector<T>::const_iterator m_end;

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = const value_type*;
        using reference = const value_type&;
        using iterator_category = std::input_iterator_tag;
        using iterator_concept = std::input_iterator_tag;

        const_iterator() : m_repo(nullptr), m_in_parent(false), m_it(), m_end() {}
        const_iterator(const SegmentedRepository& repo, bool begin) : m_repo(&repo)
        {
            if (begin)
            {
                if (repo.m_parent && !repo.m_parent->m_persistent_vector.empty())
                {
                    m_in_parent = true;
                    m_it = repo.m_parent->m_persistent_vector.begin();
                    m_end = repo.m_parent->m_persistent_vector.end();
                }
                else
                {
                    m_in_parent = false;
                    m_it = repo.m_persistent_vector.begin();
                    m_end = repo.m_persistent_vector.end();
                }
            }
            else
            {
                m_in_parent = false;
                m_it = repo.m_persistent_vector.end();
                m_end = repo.m_persistent_vector.end();
            }
        }
        pointer operator*() const { return *m_it; }
        const_iterator& operator++()
        {
            ++m_it;
            if (m_in_parent && m_it == m_end)
            {
                m_it = m_repo->m_persistent_vector.begin();
                m_end = m_repo->m_persistent_vector.end();
                m_in_parent = false;
            }
            return *this;
        }
        const_iterator operator++(int)
        {
            auto tmp = const_iterator(*this);
            ++(*this);
            return tmp;
        }
        const_iterator& operator+=(difference_type n)
        {
            while (n-- > 0)
                ++(*this);
            return *this;
        }

        const_iterator operator+(difference_type n) const
        {
            auto copy = *this;
            copy += n;
            return copy;
        }
        bool operator==(const const_iterator& other) const { return m_in_parent == other.m_in_parent && m_it == other.m_it; }
        bool operator!=(const const_iterator& other) const { return !(*this == other); }
    };

    const_iterator begin() const { return const_iterator(*this, true); }

    const_iterator end() const { return const_iterator(*this, false); }

    const SegmentedVector<T>& get_storage() const { return m_persistent_vector; }

    /**
     * Capacity
     */

    size_t size() const { return m_persistent_vector.size() + m_num_parent_elements; }
};

}

#endif
