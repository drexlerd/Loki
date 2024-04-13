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

#include "loki/pddl/parameter.hpp"

#include "loki/pddl/type.hpp"
#include "loki/pddl/variable.hpp"
#include "loki/utils/collections.hpp"
#include "loki/utils/hash.hpp"

#include <cassert>

namespace loki
{
ParameterImpl::ParameterImpl(int identifier, Variable variable, TypeList types) : Base(identifier), m_variable(std::move(variable)), m_types(std::move(types))
{
}

bool ParameterImpl::is_structurally_equivalent_to_impl(const ParameterImpl& other) const
{
    return (m_variable == other.m_variable) && (get_sorted_vector(m_types) == get_sorted_vector(other.m_types));
}

size_t ParameterImpl::hash_impl() const { return hash_combine(m_variable, hash_container(get_sorted_vector(m_types))); }

void ParameterImpl::str_impl(std::ostream& out, const FormattingOptions& /*options*/) const
{
    out << m_variable->get_name();
    if (!m_types.empty())
    {
        out << " - ";
        if (m_types.size() > 1)
        {
            out << "(either ";
            for (size_t i = 0; i < m_types.size(); ++i)
            {
                if (i != 0)
                    out << " ";
                m_types[i]->get_name();
            }
            out << ")";
        }
        else if (m_types.size() == 1)
        {
            out << m_types.front()->get_name();
        }
    }
}

const Variable& ParameterImpl::get_variable() const { return m_variable; }

const TypeList& ParameterImpl::get_bases() const { return m_types; }

}
