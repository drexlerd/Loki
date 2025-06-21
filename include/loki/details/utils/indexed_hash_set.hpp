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

#ifndef LOKI_INCLUDE_LOKI_UTILS_INDEXED_HASH_SET_HPP_
#define LOKI_INCLUDE_LOKI_UTILS_INDEXED_HASH_SET_HPP_

#include "loki/details/utils/equal_to.hpp"
#include "loki/details/utils/hash.hpp"

#include <absl/container/node_hash_set.h>
#include <memory>
#include <tuple>
#include <unordered_set>
#include <variant>

namespace loki
{

template<typename T, typename Hash = Hash<T>, typename EqualTo = EqualTo<T>>
class IndexedHashSet
{
private:
    /* Internal elements.*/
    absl::node_hash_set<T, Hash, EqualTo> m_set;
    std::vector<const T*> m_vector;

    /* External elements. */
    const IndexedHashSet* m_parent;

    void range_check(size_t pos) const
    {
        if (pos >= size())
        {
            throw std::out_of_range("IndexedHashSet::range_check: pos (which is " + std::to_string(pos) + ") >= this->size() (which is "
                                    + std::to_string(size()) + ")");
        }
    }

public:
    explicit IndexedHashSet(const IndexedHashSet* parent = nullptr) : m_set(), m_vector(), m_parent(parent)
    {
        if (parent)
        {
            if (parent->m_parent)
                throw std::runtime_error("IndexedHashSet::IndexedHashSet: Multi chaining of IndexedHashSet is not supported.");
        }
    }

    IndexedHashSet(const IndexedHashSet& other) = delete;
    IndexedHashSet& operator=(const IndexedHashSet& other) = delete;
    IndexedHashSet(IndexedHashSet&& other) = default;
    IndexedHashSet& operator=(IndexedHashSet&& other) = default;

    T const* find(const T& element) const
    {
        {
            if (auto it = m_set.find(element); it != m_set.end())
            {
                return &*it;
            }
        }

        {
            if (m_parent)
            {
                if (auto it = m_parent->m_set.find(element); it != m_parent->m_set.end())
                {
                    return &*it;
                }
            }
        }

        return nullptr;
    }

    T const* insert(const T& element)
    {
        if (m_parent)
        {
            if (auto it = m_parent->m_set.find(element); it != m_parent->m_set.end())
            {
                return &*it;
            }
        }

        auto result = m_set.insert(element);

        if (result.second)
        {
            m_vector.push_back(&*result.first);
        }

        return &*result.first;
    }

    /// @brief Returns a pointer to an existing object or creates it before if it does not exist.
    template<typename... Args>
    T const* get_or_create(Args&&... args)
    {
        assert(m_set.size() == m_vector.size());

        /* Construct and insert the element in persistent memory. */

        size_t index = size();

        auto element = T(index, std::forward<Args>(args)...);

        if (m_parent)
        {
            if (auto it = m_parent->m_set.find(element); it != m_parent->m_set.end())
            {
                return &*it;
            }
        }

        auto result = m_set.insert(std::move(element));

        if (result.second)
        {
            m_vector.push_back(&*result.first);
        }

        return &*result.first;
    }

    /**
     * Accessors
     */

    /// @brief Returns a pointer to an existing object with the given pos.
    T const* operator[](size_t pos) const
    {
        assert(pos < size());
        return (pos < parent_size()) ? m_parent->m_vector[pos] : (m_vector[pos - parent_size()]);
    }

    /// @brief Returns a pointer to an existing object with the given pos.
    /// @param pos
    /// @return
    T const* at(size_t pos) const
    {
        range_check(pos);
        return (pos < parent_size()) ? m_parent->m_vector.at(pos) : (m_vector.at(pos - parent_size()));
    }

    class const_iterator
    {
    private:
        const IndexedHashSet* m_repo;
        bool m_in_parent;

        std::vector<const T*>::const_iterator m_it;
        std::vector<const T*>::const_iterator m_end;

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = const value_type*;
        using reference = const value_type&;
        using iterator_category = std::input_iterator_tag;
        using iterator_concept = std::input_iterator_tag;

        const_iterator() : m_repo(nullptr), m_in_parent(false), m_it(), m_end() {}
        const_iterator(const IndexedHashSet& repo, bool begin) : m_repo(&repo)
        {
            if (begin)
            {
                if (repo.m_parent && !repo.m_parent->m_vector.empty())
                {
                    m_in_parent = true;
                    m_it = repo.m_parent->m_vector.begin();
                    m_end = repo.m_parent->m_vector.end();
                }
                else
                {
                    m_in_parent = false;
                    m_it = repo.m_vector.begin();
                    m_end = repo.m_vector.end();
                }
            }
            else
            {
                m_in_parent = false;
                m_it = repo.m_vector.end();
                m_end = repo.m_vector.end();
            }
        }
        reference operator*() const { return **m_it; }
        const_iterator& operator++()
        {
            ++m_it;
            if (m_in_parent && m_it == m_end)
            {
                m_it = m_repo->m_vector.begin();
                m_end = m_repo->m_vector.end();
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

    const absl::node_hash_set<T, Hash, EqualTo>& get_set() const { return m_set; }

    const std::vector<const T*>& get_vector() const { return m_vector; }

    /**
     * Capacity
     */

    size_t parent_size() const { return m_parent ? m_parent->size() : 0; }
    size_t size() const { return m_set.size() + parent_size(); }
};

}

#endif
