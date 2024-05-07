/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
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

#include "loki/details/pddl/type.hpp"

#include "loki/details/utils/collections.hpp"
#include "loki/details/utils/hash.hpp"

namespace loki
{
TypeImpl::TypeImpl(size_t identifier, std::string name, TypeList bases) : Base(identifier), m_name(std::move(name)), m_bases(std::move(bases)) {}

bool TypeImpl::is_structurally_equivalent_to_impl(const TypeImpl& other) const
{
    return (m_name == other.m_name) && (get_sorted_vector(m_bases) == get_sorted_vector(other.m_bases));
}

size_t TypeImpl::hash_impl() const { return hash_combine(m_name, hash_container(get_sorted_vector(m_bases))); }

void TypeImpl::str_impl(std::ostream& out, const FormattingOptions& /*options*/) const
{
    out << m_name;
    if (!m_bases.empty())
    {
        out << " - ";
        if (m_bases.size() > 1)
        {
            out << "(either ";
            for (size_t i = 0; i < m_bases.size(); ++i)
            {
                if (i != 0)
                    out << " ";
                out << m_bases[i]->get_name();
            }
            out << ")";
        }
        else if (m_bases.size() == 1)
        {
            out << m_bases.front()->get_name();
        }
    }
}

const std::string& TypeImpl::get_name() const { return m_name; }

const TypeList& TypeImpl::get_bases() const { return m_bases; }

static void collect_types_from_hierarchy_recursively(const TypeList& types, TypeSet& ref_result)
{
    for (const auto& type : types)
    {
        ref_result.insert(type);
        collect_types_from_hierarchy_recursively(type->get_bases(), ref_result);
    }
}

TypeSet collect_types_from_hierarchy(const TypeList& types)
{
    auto result = TypeSet {};
    collect_types_from_hierarchy_recursively(types, result);
    return result;
}
}
