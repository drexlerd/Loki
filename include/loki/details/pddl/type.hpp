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
#include "loki/details/pddl/factory.hpp"

#include <string>

namespace loki
{
class TypeImpl : public Base<TypeImpl>
{
private:
    std::string m_name;
    TypeList m_bases;

    TypeImpl(size_t identifier, std::string name, TypeList bases = {});

    // Give access to the constructor.
    friend class PDDLFactory<TypeImpl, Hash<TypeImpl*>, EqualTo<TypeImpl*>>;

    /// @brief Test for semantic equivalence
    bool is_structurally_equivalent_to_impl(const TypeImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<TypeImpl>;

public:
    const std::string& get_name() const;
    const TypeList& get_bases() const;
};

/// @brief Collects all types from a hierarchy.
extern TypeSet collect_types_from_hierarchy(const TypeList& types);

}

#endif
