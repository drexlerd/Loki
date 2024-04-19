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

#ifndef LOKI_INCLUDE_LOKI_UTILS_SEGMENTED_VECTOR_HPP_
#define LOKI_INCLUDE_LOKI_UTILS_SEGMENTED_VECTOR_HPP_

#include <array>
#include <cassert>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

namespace loki
{
/// @brief The SegmentedVector persistently stores elements of type T
///        in segments of size N, ensuring that references to elements do not
///        become invalidated upon reallocation.
/// @tparam T is the nested type
template<typename T>
class SegmentedVector
{
private:
    size_t m_elements_per_segment;

    std::vector<std::vector<T>> m_data;

    size_t m_size;
    size_t m_capacity;

    void increase_capacity()
    {
        // Add an additional vector with capacity N
        m_data.resize(m_data.size() + 1);
        m_data.back().reserve(m_elements_per_segment);
        // Increase total capacity
        m_capacity += m_elements_per_segment;
    }

    size_t segment_index(size_t pos) const { return pos / m_elements_per_segment; }

    size_t element_index(size_t pos) const { return pos % m_elements_per_segment; }

public:
    explicit SegmentedVector(size_t elements_per_segment) : m_elements_per_segment(elements_per_segment), m_size(0), m_capacity(0) {}

    /**
     * Modifiers
     */
    const T& push_back(T value)
    {
        // Increase capacity if necessary
        if (m_size >= m_capacity)
        {
            increase_capacity();
        }
        auto& segment = m_data[segment_index(size())];

        // Take ownership of memory
        segment.push_back(std::move(value));
        ++m_size;

        // Fetch return value
        return segment.back();
    }

    /**
     * Accessors
     */

    T& operator[](size_t pos)
    {
        assert(pos < size());
        return m_data[segment_index(pos)][element_index(pos)];
    }

    const T& operator[](size_t pos) const
    {
        assert(pos < size());
        return m_data[segment_index(pos)][element_index(pos)];
    }

    /**
     * Iterators
     */
    class const_iterator
    {
    private:
        typename std::vector<std::vector<T>>::const_iterator m_outer_iter;
        typename std::vector<std::vector<T>>::const_iterator m_outer_end;
        typename std::vector<T>::const_iterator m_inner_iter;

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::forward_iterator_tag;

        const_iterator(const std::vector<std::vector<T>>& data, bool begin) : m_outer_iter(begin ? data.begin() : data.end()), m_outer_end(data.end())
        {
            // Dereferencing end() is undefined behavior, so we must check before initializing the inner iterator.
            if (begin && m_outer_iter != m_outer_end)
            {
                m_inner_iter = m_outer_iter->begin();
            }
        }

        [[nodiscard]] decltype(auto) operator*() const { return *m_inner_iter; }

        const_iterator& operator++()
        {
            if (++m_inner_iter == m_outer_iter->end())
            {
                if (++m_outer_iter != m_outer_end)
                {
                    m_inner_iter = m_outer_iter->begin();
                }
            }
            return *this;
        }

        const_iterator operator++(int)
        {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        [[nodiscard]] bool operator==(const const_iterator& other) const
        {
            return m_outer_iter == other.m_outer_iter && (m_outer_iter == m_outer_end || m_inner_iter == other.m_inner_iter);
        }

        [[nodiscard]] bool operator!=(const const_iterator& other) const { return !(*this == other); }
    };

    [[nodiscard]] const_iterator begin() const { return const_iterator(m_data, true); }

    [[nodiscard]] const_iterator end() const { return const_iterator(m_data, false); }

    /**
     * Capacity
     */

    size_t size() const { return m_size; }

    size_t capacity() const { return m_capacity; }
};

}

#endif