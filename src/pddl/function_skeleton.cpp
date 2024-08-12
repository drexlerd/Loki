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

#include "loki/details/pddl/parameter.hpp"
#include "loki/details/utils/collections.hpp"
#include "loki/details/utils/hash.hpp"

namespace loki
{
FunctionSkeletonImpl::FunctionSkeletonImpl(size_t index, std::string name, ParameterList parameters, Type type) :
    Base(index),
    m_name(std::move(name)),
    m_parameters(parameters),
    m_type(std::move(type))
{
}

void FunctionSkeletonImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(" << m_name;
    for (size_t i = 0; i < m_parameters.size(); ++i)
    {
        out << " ";
        m_parameters[i]->str(out, options);
    }
    out << ")";
}

const std::string& FunctionSkeletonImpl::get_name() const { return m_name; }

const ParameterList& FunctionSkeletonImpl::get_parameters() const { return m_parameters; }

const Type& FunctionSkeletonImpl::get_type() const { return m_type; }

size_t ShallowHash<const FunctionSkeletonImpl*>::operator()(const FunctionSkeletonImpl* e) const
{
    return ShallowHashCombiner()(e->get_name(), e->get_type(), get_sorted_vector(e->get_parameters()));
}

bool ShallowEqualTo<const FunctionSkeletonImpl*>::operator()(const FunctionSkeletonImpl* l, const FunctionSkeletonImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_name() == r->get_name()) && (l->get_type() == r->get_type())
               && (get_sorted_vector(l->get_parameters()) == get_sorted_vector(r->get_parameters()));
    }
    return true;
}

}
