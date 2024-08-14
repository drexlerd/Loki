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

#include "loki/details/pddl/function_skeleton.hpp"

#include "formatter.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/utils/collections.hpp"

namespace loki
{
FunctionSkeletonImpl::FunctionSkeletonImpl(size_t index, std::string name, ParameterList parameters, Type type) :
    m_index(index),
    m_name(std::move(name)),
    m_parameters(parameters),
    m_type(std::move(type))
{
}

size_t FunctionSkeletonImpl::get_index() const { return m_index; }

const std::string& FunctionSkeletonImpl::get_name() const { return m_name; }

const ParameterList& FunctionSkeletonImpl::get_parameters() const { return m_parameters; }

const Type& FunctionSkeletonImpl::get_type() const { return m_type; }

size_t UniquePDDLHasher<const FunctionSkeletonImpl*>::operator()(const FunctionSkeletonImpl* e) const
{
    return UniquePDDLHashCombiner()(e->get_name(), e->get_type(), get_sorted_vector(e->get_parameters()));
}

bool UniquePDDLEqualTo<const FunctionSkeletonImpl*>::operator()(const FunctionSkeletonImpl* l, const FunctionSkeletonImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_name() == r->get_name()) && (l->get_type() == r->get_type())
               && (get_sorted_vector(l->get_parameters()) == get_sorted_vector(r->get_parameters()));
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const FunctionSkeletonImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

}
