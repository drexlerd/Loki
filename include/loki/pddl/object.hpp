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

#include "loki/pddl/base.hpp"
#include "loki/pddl/declarations.hpp"
#include "loki/pddl/factory.hpp"

#include <string>

namespace loki
{
template<typename HolderType, typename Hash, typename EqualTo>
class PDDLFactory;
}

namespace loki::pddl
{
class ObjectImpl : public Base<ObjectImpl>
{
private:
    std::string m_name;
    TypeList m_types;

    ObjectImpl(int identifier, std::string name, TypeList types = {});

    // Give access to the constructor.
    friend class loki::PDDLFactory<ObjectImpl, loki::Hash<ObjectImpl*>, loki::EqualTo<ObjectImpl*>>;

    /// @brief Test for semantic equivalence
    bool is_structurally_equivalent_to_impl(const ObjectImpl& other) const;
    size_t hash_impl() const;

    // Give access to the private interface implementations.
    friend class Base<ObjectImpl>;

public:
    void str(std::ostream& out, const FormattingOptions& options, bool typing_enabled) const;

    const std::string& get_name() const;
    const TypeList& get_bases() const;
};

}

#endif
