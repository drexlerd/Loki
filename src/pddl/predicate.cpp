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

#include "loki/details/pddl/predicate.hpp"

#include "loki/details/pddl/parameter.hpp"
#include "loki/details/pddl/type.hpp"
#include "loki/details/pddl/variable.hpp"
#include "loki/details/utils/collections.hpp"
#include "loki/details/utils/hash.hpp"

#include <memory>

namespace loki
{
PredicateImpl::PredicateImpl(size_t index, std::string name, ParameterList parameters) :
    Base(index),
    m_name(std::move(name)),
    m_parameters(std::move(parameters))
{
}

void PredicateImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(" << m_name;
    for (size_t i = 0; i < m_parameters.size(); ++i)
    {
        out << " ";
        m_parameters[i]->str(out, options);
    }
    out << ")";
}

const std::string& PredicateImpl::get_name() const { return m_name; }

const ParameterList& PredicateImpl::get_parameters() const { return m_parameters; }

size_t ShallowHash<const PredicateImpl*>::operator()(const PredicateImpl* e) const
{
    return ShallowHashCombiner()(e->get_name(), get_sorted_vector(e->get_parameters()));
}

bool ShallowEqualTo<const PredicateImpl*>::operator()(const PredicateImpl* l, const PredicateImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_name() == r->get_name()) && (get_sorted_vector(l->get_parameters()) == get_sorted_vector(r->get_parameters()));
    }
    return true;
}

}
