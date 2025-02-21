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
    size_t m_num_element_per_segment;
    size_t m_maximum_num_elements_per_segment;

    std::vector<std::vector<T>> m_segments;

    std::vector<T*> m_accessor;

    size_t m_size;
    size_t m_capacity;

    void increase_capacity()
    {
        assert(m_num_element_per_segment > 0);

        // Use doubling strategy to make future insertions cheaper.
        m_num_element_per_segment = std::min(2 * m_num_element_per_segment, m_maximum_num_elements_per_segment);

        // Add an additional vector with capacity N
        m_segments.resize(m_segments.size() + 1);
        // This reserve is important to avoid reallocations
        m_segments.back().reserve(m_num_element_per_segment);
        // Increase total capacity
        m_capacity += m_num_element_per_segment;
    }

    void range_check(size_t pos) const
    {
        if (pos >= size())
        {
            throw std::out_of_range("SegmentedVector::range_check: pos (which is " + std::to_string(pos) + ") >= this->size() (which is "
                                    + std::to_string(size()) + ")");
        }
    }

public:
    SegmentedVector(size_t initial_num_element_per_segment = 16, size_t maximum_num_elements_per_segment = 16 * 1024) :
        m_num_element_per_segment(initial_num_element_per_segment),
        m_maximum_num_elements_per_segment(maximum_num_elements_per_segment),
        m_segments(),
        m_size(0),
        m_capacity(0)
    {
    }

    /**
     * Modifiers
     */
    void push_back(T value)
    {
        // Increase capacity if necessary
        if (m_size >= m_capacity)
        {
            increase_capacity();
        }

        // Take ownership of memory, store address in accessor.
        auto& segment = m_segments.back();
        auto& element = segment.emplace_back(std::move(value));
        m_accessor.push_back(&element);
        ++m_size;
    }

    template<typename... Args>
    T& emplace_back(Args&&... args)
    {
        // Increase capacity if necessary
        if (m_size >= m_capacity)
        {
            increase_capacity();
        }

        // Emplace the new element directly in the segment
        auto& segment = m_segments.back();
        auto& element = segment.emplace_back(std::forward<Args>(args)...);
        m_accessor.push_back(&element);
        ++m_size;

        return element;
    }

    /**
     * Accessors
     */

    T& operator[](size_t pos)
    {
        assert(pos < size());
        return *m_accessor[pos];
    }

    const T& operator[](size_t pos) const
    {
        assert(pos < size());
        return *m_accessor[pos];
    }

    T& at(size_t pos)
    {
        range_check(pos);
        return *m_accessor[pos];
    }

    const T& at(size_t pos) const
    {
        range_check(pos);
        return *m_accessor[pos];
    }

    T& back()
    {
        range_check(size() - 1);
        return *m_accessor.back();
    }

    const T& back() const
    {
        range_check(size() - 1);
        return *m_accessor.back();
    }

    auto begin() const { return m_accessor.begin(); }

    auto end() const { return m_accessor.end(); }

    size_t num_segments() const { return m_segments.size(); }

    /**
     * Capacity
     */

    size_t size() const { return m_size; }

    size_t capacity() const { return m_capacity; }

    bool empty() const { return m_size == 0; }
};

}

#endif