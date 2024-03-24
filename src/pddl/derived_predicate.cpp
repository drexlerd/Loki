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

#include "loki/pddl/derived_predicate.hpp"

#include "loki/pddl/conditions.hpp"
#include "loki/pddl/effects.hpp"
#include "loki/utils/collections.hpp"
#include "loki/utils/hash.hpp"

namespace loki::pddl
{
DerivedPredicateImpl::DerivedPredicateImpl(int identifier, ParameterList parameters, Condition condition) :
    Base(identifier),
    m_parameters(std::move(parameters)),
    m_condition(std::move(condition))
{
}

bool DerivedPredicateImpl::is_structurally_equivalent_to_impl(const DerivedPredicateImpl& other) const
{
    return (get_sorted_vector(m_parameters) == get_sorted_vector(other.m_parameters)) && (m_condition == other.m_condition);
}

size_t DerivedPredicateImpl::hash_impl() const { return hash_combine(hash_container(get_sorted_vector(m_parameters)), m_condition); }

void DerivedPredicateImpl::str_impl(std::ostream& out, const FormattingOptions& /*options*/) const { out << "TODO"; }

const ParameterList& DerivedPredicateImpl::get_parameters() const { return m_parameters; }

const Condition& DerivedPredicateImpl::get_condition() const { return m_condition; }

}
