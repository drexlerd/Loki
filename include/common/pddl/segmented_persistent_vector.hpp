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

#include <array>
#include <cassert>
#include <memory>
#include <mutex>
#include <vector>
#include <iostream>


namespace loki {

using BytesPerSegment = size_t;

template<typename T, BytesPerSegment N=100000>
class SegmentedPersistentVector {
private:
    std::vector<std::vector<T>> m_data;

    size_t m_elements_per_block;

    size_t m_size;
    size_t m_capacity;

    void increase_capacity() {
        // Add an additional vector with capacity N (1 allocation on average)
        m_data.resize(m_data.size() + 1);
        m_data.back().reserve(m_elements_per_block);
        // Increase total capacity
        m_capacity += m_elements_per_block;
    }

    size_t segment_index(int identifier) const {
        return identifier / m_elements_per_block;
    }

    size_t element_index(int identifier) const {
        return identifier % m_elements_per_block;
    }

public:
    explicit SegmentedPersistentVector() : m_elements_per_block(N / sizeof(T)), m_size(0), m_capacity(0) { 
        assert(m_elements_per_block > 0);
        // std::cout << "SegmentedPersistentVector(" << "bytes_per_segment: " << N << ", " << "sizeof(T): " << sizeof(T) << ", " << "elements_per_segment: " << N / sizeof(T) << std::endl;
    }

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