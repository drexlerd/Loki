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

#ifndef LOKI_INCLUDE_LOKI_COMMON_SEGMENTED_VECTOR_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_SEGMENTED_VECTOR_HPP_

#include <array>
#include <memory>
#include <mutex>
#include <vector>


namespace loki {

template<typename T, size_t N>
class SegmentedPersistentVector {
private:
    std::vector<std::array<T, N>> m_data;

    int m_block_index;
    int m_index_in_block;

    size_t m_size;
    size_t m_capacity;

    void increase_capacity() {
        // Add an additional array with capacity N (1 allocation on average)
        m_data.resize(m_data.size() + 1);
        // Move to the next free block
        ++m_block_index;
        // Set index to next free position in block
        m_index_in_block = 0;
        // Increase total capacity
        m_capacity += N;
    }

public:
    explicit SegmentedPersistentVector() : m_block_index(-1), m_index_in_block(0), m_size(0), m_capacity(0) { }

    T* push_back(T value) {
        // Increase capacity if necessary
        if (m_size >= m_capacity) {
            increase_capacity();
        }

        auto& block = m_data[m_block_index];

        // Take ownership of memory
        block[m_index_in_block] = std::move(value);
        // Fetch return value
        T* return_value = &block[m_index_in_block];
        // Move index to next free position in block
        ++m_index_in_block;

        ++m_size;

        return return_value;
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