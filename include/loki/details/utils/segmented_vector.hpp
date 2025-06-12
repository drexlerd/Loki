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
#include <bit>
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
    std::vector<std::vector<T>> m_segments;
    size_t m_offset;
    size_t m_size;
    size_t m_capacity;

    static size_t get_index(size_t pos) { return std::countr_zero(std::bit_floor(pos + 1)); }

    static size_t get_offset(size_t pos) { return pos - (std::bit_floor(pos + 1) - 1); }

    /// @brief Resizes the vector to fit an inserted element.
    /// Note: get_index and get_offset is tailored specifically for the doubling strategy starting at 1.
    void resize_to_fit()
    {
        const auto remaining_entries = m_segments.back().capacity() - m_offset;

        if (remaining_entries == 0)
        {
            const auto new_segment_size = m_segments.back().size() * 2;
            m_segments.resize(m_segments.size() + 1);
            m_segments.back().reserve(new_segment_size);
            m_offset = 0;
            m_capacity += new_segment_size;
        }
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
    SegmentedVector() : m_segments(), m_offset(0), m_size(0), m_capacity(0)
    {
        m_segments.resize(1);
        m_segments.back().reserve(1);
        m_capacity = 1;
    }

    /**
     * Modifiers
     */
    void push_back(T value)
    {
        resize_to_fit();

        m_segments.back().push_back(std::move(value));
        ++m_offset;
        ++m_size;
    }

    template<typename... Args>
    T& emplace_back(Args&&... args)
    {
        resize_to_fit();

        auto& element = m_segments.back().emplace_back(std::forward<Args>(args)...);
        ++m_offset;
        ++m_size;

        return element;
    }

    /**
     * Accessors
     */

    T& operator[](size_t pos)
    {
        assert(pos < size());
        const auto index = get_index(pos);
        const auto offset = get_offset(pos);
        return m_segments[index][offset];
    }

    const T& operator[](size_t pos) const
    {
        assert(pos < size());
        const auto index = get_index(pos);
        const auto offset = get_offset(pos);
        return m_segments[index][offset];
    }

    T& at(size_t pos)
    {
        range_check(pos);
        const auto index = get_index(pos);
        const auto offset = get_offset(pos);
        return m_segments[index][offset];
    }

    const T& at(size_t pos) const
    {
        range_check(pos);
        const auto index = get_index(pos);
        const auto offset = get_offset(pos);
        return m_segments[index][offset];
    }

    T& back()
    {
        range_check(size() - 1);
        return m_segments.back().back();
    }

    const T& back() const
    {
        range_check(size() - 1);
        return m_segments.back().back();
    }

    class const_iterator
    {
    private:
        const SegmentedVector* m_vec;
        size_t m_pos;
        size_t m_index;
        size_t m_offset;

        void advance()
        {
            if (++m_pos >= m_vec->size())
                return;

            if (++m_offset >= m_vec->m_segments[m_index].size())
            {
                ++m_index;
                m_offset = 0;
            }
        }

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = const value_type*;
        using reference = const value_type&;
        using iterator_category = std::input_iterator_tag;
        using iterator_concept = std::input_iterator_tag;

        const_iterator() : m_vec(nullptr), m_pos(0), m_index(0), m_offset(0) {}
        const_iterator(const SegmentedVector& vec, bool begin) : m_vec(&vec), m_pos(0), m_index(0), m_offset(0)
        {
            m_pos = begin ? 0 : vec.size();

            if (m_pos < vec.size())
            {
                m_index = SegmentedVector::get_index(m_pos);
                m_offset = SegmentedVector::get_offset(m_pos);
            }
            else
            {
                // Point past the last element
                m_index = vec.m_segments.size();
                m_offset = 0;
            }
        }
        pointer operator*() const { return &m_vec->m_segments[m_index][m_offset]; }
        const_iterator& operator++()
        {
            advance();
            return *this;
        }
        const_iterator operator++(int)
        {
            auto tmp = const_iterator(*this);
            ++(*this);
            return tmp;
        }
        bool operator==(const const_iterator& other) const { return m_pos == other.m_pos; }
        bool operator!=(const const_iterator& other) const { return !(*this == other); }
    };

    const_iterator begin() const { return const_iterator(*this, true); }

    const_iterator end() const { return const_iterator(*this, false); }

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