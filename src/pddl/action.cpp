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

#include "loki/details/pddl/action.hpp"

#include "formatter.hpp"
#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/effects.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/utils/collections.hpp"

namespace loki
{
ActionImpl::ActionImpl(size_t index,
                       std::string name,
                       size_t original_arity,
                       ParameterList parameters,
                       std::optional<Condition> condition,
                       std::optional<Effect> effect) :
    m_index(index),
    m_name(std::move(name)),
    m_original_arity(original_arity),
    m_parameters(std::move(parameters)),
    m_condition(std::move(condition)),
    m_effect(std::move(effect))
{
}

size_t ActionImpl::get_index() const { return m_index; }

const std::string& ActionImpl::get_name() const { return m_name; }

size_t ActionImpl::get_original_arity() const { return m_original_arity; }

const ParameterList& ActionImpl::get_parameters() const { return m_parameters; }

const std::optional<Condition>& ActionImpl::get_condition() const { return m_condition; }

const std::optional<Effect>& ActionImpl::get_effect() const { return m_effect; }

size_t UniquePDDLHasher<const ActionImpl*>::operator()(const ActionImpl* e) const
{
    return UniquePDDLHashCombiner()(e->get_name(), get_sorted_vector(e->get_parameters()), e->get_condition(), e->get_effect());
}

bool UniquePDDLEqualTo<const ActionImpl*>::operator()(const ActionImpl* l, const ActionImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_name() == r->get_name()) && (get_sorted_vector(l->get_parameters()) == get_sorted_vector(r->get_parameters()))
               && (l->get_condition() == r->get_condition()) && (l->get_effect() == r->get_effect());
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const ActionImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

}
