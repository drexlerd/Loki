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

#ifndef LOKI_INCLUDE_LOKI_PDDL_OBJECT_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_OBJECT_HPP_

#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/equal_to.hpp"
#include "loki/details/pddl/hash.hpp"

#include <string>

namespace loki
{
class ObjectImpl
{
private:
    size_t m_index;
    std::string m_name;
    TypeList m_types;

    ObjectImpl(size_t index, std::string name, TypeList types = {});

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

public:
    // moveable but not copyable
    ObjectImpl(const ObjectImpl& other) = delete;
    ObjectImpl& operator=(const ObjectImpl& other) = delete;
    ObjectImpl(ObjectImpl&& other) = default;
    ObjectImpl& operator=(ObjectImpl&& other) = default;

    size_t get_index() const;
    const std::string& get_name() const;
    const TypeList& get_bases() const;
};

template<>
struct UniquePDDLHasher<const ObjectImpl*>
{
    size_t operator()(const ObjectImpl* e) const;
};

template<>
struct UniquePDDLEqualTo<const ObjectImpl*>
{
    bool operator()(const ObjectImpl* l, const ObjectImpl* r) const;
};

extern std::ostream& operator<<(std::ostream& out, const ObjectImpl& element);

}

#endif
