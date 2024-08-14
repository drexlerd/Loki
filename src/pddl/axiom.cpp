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
#include "loki/details/utils/collections.hpp"

namespace loki
{
AxiomImpl::AxiomImpl(size_t index, std::string derived_predicate_name, ParameterList parameters, Condition condition, size_t num_parameters_to_ground_head) :
    m_index(index),
    m_derived_predicate_name(std::move(derived_predicate_name)),
    m_parameters(std::move(parameters)),
    m_condition(std::move(condition)),
    m_num_parameters_to_ground_head(num_parameters_to_ground_head)
{
}

size_t AxiomImpl::get_index() const { return m_index; }

const std::string& AxiomImpl::get_derived_predicate_name() const { return m_derived_predicate_name; }

const Condition& AxiomImpl::get_condition() const { return m_condition; }

const ParameterList& AxiomImpl::get_parameters() const { return m_parameters; }

size_t AxiomImpl::get_num_parameters_to_ground_head() const { return m_num_parameters_to_ground_head; }

size_t UniquePDDLHasher<const AxiomImpl*>::operator()(const AxiomImpl* e) const
{
    return UniquePDDLHashCombiner()(e->get_derived_predicate_name(), get_sorted_vector(e->get_parameters()), e->get_condition());
}

bool UniquePDDLEqualTo<const AxiomImpl*>::operator()(const AxiomImpl* l, const AxiomImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_derived_predicate_name() == r->get_derived_predicate_name())
               && (get_sorted_vector(l->get_parameters()) == get_sorted_vector(r->get_parameters())) && (l->get_condition() == r->get_condition());
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const AxiomImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

}
