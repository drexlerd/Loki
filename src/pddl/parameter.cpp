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

#include "loki/details/pddl/parameter.hpp"

#include "formatter.hpp"
#include "loki/details/pddl/type.hpp"
#include "loki/details/pddl/variable.hpp"
#include "loki/details/utils/collections.hpp"

#include <cassert>

namespace loki
{
ParameterImpl::ParameterImpl(size_t index, Variable variable, TypeList types) : m_index(index), m_variable(std::move(variable)), m_types(std::move(types)) {}

size_t ParameterImpl::get_index() const { return m_index; }

const Variable& ParameterImpl::get_variable() const { return m_variable; }

const TypeList& ParameterImpl::get_bases() const { return m_types; }

bool is_specialized_parameter(const Parameter& specialized_parameter, const Parameter& generalized_parameter)
{
    for (const auto& type : specialized_parameter->get_bases())
    {
        if (is_subtype_or_equal(type, generalized_parameter->get_bases()))
        {
            return true;
        }
    }
    return false;
}

size_t UniquePDDLHasher<const ParameterImpl*>::operator()(const ParameterImpl* e) const
{
    return UniquePDDLHashCombiner()(e->get_variable(), get_sorted_vector(e->get_bases()));
}

bool UniquePDDLEqualTo<const ParameterImpl*>::operator()(const ParameterImpl* l, const ParameterImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_variable() == r->get_variable()) && (get_sorted_vector(l->get_bases()) == get_sorted_vector(r->get_bases()));
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const ParameterImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

}
