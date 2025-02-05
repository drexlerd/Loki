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

#include "loki/details/pddl/axiom.hpp"

#include "formatter.hpp"
#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/effects.hpp"
#include "loki/details/pddl/literal.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/pddl/predicate.hpp"

namespace loki
{
AxiomImpl::AxiomImpl(size_t index, ParameterList parameters, Predicate derived_predicate, Literal subtyped_literal, Condition condition) :
    m_index(index),
    m_parameters(std::move(parameters)),
    m_derived_predicate(std::move(derived_predicate)),
    m_subtyped_literal(std::move(subtyped_literal)),
    m_condition(std::move(condition))
{
}

size_t AxiomImpl::get_index() const { return m_index; }

const ParameterList& AxiomImpl::get_parameters() const { return m_parameters; }

const Predicate& AxiomImpl::get_derived_predicate() const { return m_derived_predicate; }

const Literal& AxiomImpl::get_subtyped_literal() const { return m_subtyped_literal; }

const Condition& AxiomImpl::get_condition() const { return m_condition; }

std::ostream& operator<<(std::ostream& out, const AxiomImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

}
