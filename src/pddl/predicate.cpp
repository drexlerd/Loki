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

#include "formatter.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/pddl/type.hpp"
#include "loki/details/pddl/variable.hpp"

#include <memory>

namespace loki
{
PredicateImpl::PredicateImpl(size_t index, std::string name, ParameterList parameters) :
    m_index(index),
    m_name(std::move(name)),
    m_parameters(std::move(parameters))
{
}

size_t PredicateImpl::get_index() const { return m_index; }

const std::string& PredicateImpl::get_name() const { return m_name; }

const ParameterList& PredicateImpl::get_parameters() const { return m_parameters; }

std::ostream& operator<<(std::ostream& out, const PredicateImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

}
