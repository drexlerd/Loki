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

    size_t size() const { return m_size; }

    size_t capacity() const { return m_capacity; }
};

}

#endif