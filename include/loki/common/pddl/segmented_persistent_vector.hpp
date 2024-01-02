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

#ifndef LOKI_INCLUDE_LOKI_COMMON_PDDL_SEGMENTED_VECTOR_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_PDDL_SEGMENTED_VECTOR_HPP_

#include "declarations.hpp"

#include <array>
#include <cassert>
#include <memory>
#include <mutex>
#include <vector>
#include <iostream>


namespace loki {
/// @brief The SegmentedPersistentVector persistently stores elements of type T 
///        in segments of size N, ensuring that references to elements do not 
///        become invalidated upon reallocation.
/// @tparam T is the nested type
/// @tparam N is the number of elements per segment
template<typename T, ElementsPerSegment N>
class SegmentedPersistentVector {
private:
    std::vector<std::vector<T>> m_data;

    size_t m_size;
    size_t m_capacity;

    void increase_capacity() {
        // Add an additional vector with capacity N
        m_data.resize(m_data.size() + 1);
        m_data.back().reserve(N);
        // Increase total capacity
        m_capacity += N;
    }

    size_t segment_index(int identifier) const {
        return identifier / N;
    }

    size_t element_index(int identifier) const {
        return identifier % N;
    }

public:
    explicit SegmentedPersistentVector() : m_size(0), m_capacity(0) { }

    const T& push_back(T value) {
        // Increase capacity if necessary
        if (m_size >= m_capacity) {
            increase_capacity();
        }
        auto& segment = m_data[segment_index(size())];

        // Take ownership of memory
        segment.push_back(std::move(value));
        // Fetch return value
        const T& return_value = segment[element_index(size())];
        
        ++m_size;
        return return_value;
    }

    T& operator[](int index) {
        assert(index < static_cast<int>(size()));
        return m_data[segment_index(index)][element_index(index)];
    }

    void pop_back() {
        assert(size() > 0);
        auto& segment = m_data[segment_index(size() - 1)];
        segment.pop_back();
        --m_size;
    }

    const T& operator[](int identifier) const {
        assert(identifier >= 0 && identifier <= static_cast<int>(size()));
        return m_data[segment_index(identifier)][element_index(identifier)];
    }

    size_t size() const {
        return m_size;
    }

    size_t capacity() const {
        return m_capacity;
    }
};

}

#endif