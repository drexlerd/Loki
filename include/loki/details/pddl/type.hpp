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

#ifndef LOKI_INCLUDE_LOKI_PDDL_TYPE_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_TYPE_HPP_

#include "loki/details/pddl/base.hpp"
#include "loki/details/pddl/declarations.hpp"

#include <string>

namespace loki
{
class TypeImpl : public Base<TypeImpl>
{
private:
    std::string m_name;
    TypeList m_bases;

    TypeImpl(size_t index, std::string name, TypeList bases = {});

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<TypeImpl>;

public:
    const std::string& get_name() const;
    const TypeList& get_bases() const;
};

template<>
struct ShallowHash<const TypeImpl*>
{
    size_t operator()(const TypeImpl* e) const;
};

template<>
struct ShallowEqualTo<const TypeImpl*>
{
    bool operator()(const TypeImpl* l, const TypeImpl* r) const;
};

/// @brief Collects all types from a hierarchy.
extern TypeSet collect_types_from_hierarchy(const TypeList& types);

/// @brief Return true iff type is a subtype of or equal to one of the types in parent_types
extern bool is_subtype_or_equal(const Type& type, const TypeList& parent_types);

}

#endif
