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

#include "loki/pddl/base.hpp"
#include "loki/pddl/declarations.hpp"

#include <string>

namespace loki
{
template<typename HolderType>
class PersistentFactory;
}

namespace loki::pddl
{
class TypeImpl : public Base<TypeImpl>
{
private:
    std::string m_name;
    TypeList m_bases;

    TypeImpl(int identifier, std::string name, TypeList bases = {});

    // Give access to the constructor.
    template<typename>
    friend class loki::PersistentFactory;

    /// @brief Test for semantic equivalence
    bool is_structurally_equivalent_to_impl(const TypeImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<TypeImpl>;

public:
    const std::string& get_name() const;
    const TypeList& get_bases() const;
};
}

namespace std
{
// Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
template<>
struct less<loki::pddl::Type>
{
    bool operator()(const loki::pddl::Type& left_type, const loki::pddl::Type& right_type) const;
};

template<>
struct hash<loki::pddl::TypeImpl>
{
    std::size_t operator()(const loki::pddl::TypeImpl& type) const;
};
}

#endif