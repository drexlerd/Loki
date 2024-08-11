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

#ifndef LOKI_INCLUDE_LOKI_UTILS_VALUE_TYPE_FACTORY_HPP_
#define LOKI_INCLUDE_LOKI_UTILS_VALUE_TYPE_FACTORY_HPP_

#include "loki/details/utils/hash.hpp"
#include "loki/details/utils/equal_to.hpp"
#include "loki/details/utils/segmented_vector.hpp"

#include <memory>
#include <tuple>
#include <type_traits>
#include <unordered_set>
#include <variant>

namespace loki
{

/// @brief `UniqueValueTypeFactory` manages unique creation of objects
/// in a persistent and efficient manner, utilizing a combination of unordered_set for
/// uniqueness checks and SegmentedVector for continuous and cache-efficient storage of value types.
/// @tparam HolderType is the holder value type which can be an std::variant.
/// Note that using a base class value type will result in object slicing.
/// @tparam Hash the hash function, default takes the hash value of the dereferenced object.
/// @tparam EqualTo the comparison function, default compares the dereferenced objects.
template<typename HolderType, typename Hash = Hash<const HolderType*, true>, typename EqualTo = EqualTo<const HolderType*, true>>
class UniqueValueTypeFactory
{
private:
    // We use an unordered_set to test for uniqueness.
    // We use pointers to the persistent memory.
    std::unordered_set<const HolderType*, Hash, EqualTo> m_uniqueness_set;

    // Use pre-allocated memory to store PDDL object persistent.
    SegmentedVector<HolderType> m_persistent_vector;

    void range_check(size_t pos) const
    {
        if (pos >= size())
        {
            throw std::out_of_range("SegmentedVector::range_check: pos (which is " + std::to_string(pos) + ") >= this->size() (which is "
                                    + std::to_string(size()) + ")");
        }
    }

public:
    UniqueValueTypeFactory(size_t initial_num_element_per_segment = 16, size_t maximum_num_elements_per_segment = 16 * 1024) :
        m_persistent_vector(SegmentedVector<HolderType>(initial_num_element_per_segment, maximum_num_elements_per_segment))
    {
    }
    UniqueValueTypeFactory(const UniqueValueTypeFactory& other) = delete;
    UniqueValueTypeFactory& operator=(const UniqueValueTypeFactory& other) = delete;
    UniqueValueTypeFactory(UniqueValueTypeFactory&& other) = default;
    UniqueValueTypeFactory& operator=(UniqueValueTypeFactory&& other) = default;

    /// @brief Returns a pointer to an existing object
    ///        or creates it before if it does not exist.
    template<typename SubType, typename... Args>
    HolderType const* get_or_create(Args&&... args)
    {
        /* Construct and insert the element in persistent memory. */

        // Ensure that element with identifier i is stored at position i.
        size_t index = m_uniqueness_set.size();
        assert(index == m_persistent_vector.size());

        // Explicitly call the constructor of T to give exclusive access to the factory.
        const auto* element_ptr = &m_persistent_vector.emplace_back(SubType(index, std::forward<Args>(args)...));
        // The pointer to the location in persistent memory.
        assert(element_ptr);

        /* Test for uniqueness */
        auto it = m_uniqueness_set.find(element_ptr);
        if (it == m_uniqueness_set.end())
        {
            /* Element is unique! */

            m_uniqueness_set.emplace(element_ptr);
        }
        else
        {
            /* Element is not unique! */

            element_ptr = *it;
            // Remove duplicate from vector
            m_persistent_vector.pop_back();
        }

        return element_ptr;
    }

    /**
     * Accessors
     */

    /// @brief Returns a pointer to an existing object with the given pos.
    HolderType const* operator[](size_t pos) const
    {
        assert(pos < size());
        return &(m_persistent_vector.at(pos));
    }

    HolderType const* at(size_t pos) const
    {
        range_check(pos);
        return &(m_persistent_vector.at(pos));
    }

    auto begin() const { return m_persistent_vector.begin(); }

    auto end() const { return m_persistent_vector.end(); }

    const SegmentedVector<HolderType>& get_storage() const { return m_persistent_vector; }

    /**
     * Capacity
     */

    size_t size() const { return m_persistent_vector.size(); }
};

template<typename... Ts>
class VariadicUniqueValueTypeFactory
{
private:
    std::tuple<UniqueValueTypeFactory<Ts>...> m_factories;

public:
    VariadicUniqueValueTypeFactory(size_t initial_num_element_per_segment = 16, size_t maximum_num_elements_per_segment = 16 * 1024) :
        m_factories(std::make_tuple(UniqueValueTypeFactory<Ts>(initial_num_element_per_segment, maximum_num_elements_per_segment)...))
    {
    }

    template<typename T>
    UniqueValueTypeFactory<T>& get()
    {
        return std::get<UniqueValueTypeFactory<T>>(m_factories);
    }

    template<typename T>
    const UniqueValueTypeFactory<T>& get() const
    {
        return std::get<UniqueValueTypeFactory<T>>(m_factories);
    }
};

}

#endif
