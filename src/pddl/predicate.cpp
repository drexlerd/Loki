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

#include "loki/pddl/predicate.hpp"

#include "loki/pddl/parameter.hpp"
#include "loki/pddl/type.hpp"
#include "loki/pddl/variable.hpp"
#include "loki/utils/hash.hpp"

#include <memory>

namespace loki::pddl
{
PredicateImpl::PredicateImpl(int identifier, std::string name, ParameterList parameters) :
    Base(identifier),
    m_name(std::move(name)),
    m_parameters(std::move(parameters))
{
}

bool PredicateImpl::is_structurally_equivalent_to_impl(const PredicateImpl& other) const
{
    return (m_name == other.m_name) && (m_parameters == other.m_parameters);
}

size_t PredicateImpl::hash_impl() const { return hash_combine(m_name, hash_container(m_parameters)); }

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

}
