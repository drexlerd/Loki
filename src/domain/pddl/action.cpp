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

#include "../../../include/loki/domain/pddl/action.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"


namespace loki::pddl {
ActionImpl::ActionImpl(int identifier, const std::string& name, const Condition& condition, const EffectList& effects)
    : m_identifier(identifier)
    , m_name(name)
    , m_condition(condition)
    , m_effects(effects)
{
}

bool ActionImpl::operator==(const ActionImpl& other) const {
    return (m_name == other.m_name)
        && (m_condition == other.m_condition)
        && (sorted(m_effects) == sorted(other.m_effects));
}

bool ActionImpl::operator!=(const ActionImpl& other) const {
    return !(*this == other);
}

bool ActionImpl::operator<(const ActionImpl& other) const {
    return m_identifier < other.m_identifier;
}

bool ActionImpl::operator>(const ActionImpl& other) const {
    return m_identifier > other.m_identifier;
}

size_t ActionImpl::hash() const {
    return hash_combine(
        m_name,
        m_condition,
        hash_vector(sorted(m_effects)));
}

const std::string& ActionImpl::get_name() const {
    return m_name;
}

const Condition& ActionImpl::get_condition() const {
    return m_condition;
}

const EffectList& ActionImpl::get_effects() const {
    return m_effects;
}

}

namespace std {
    bool less<loki::pddl::Action>::operator()(
        const loki::pddl::Action& left_action,
        const loki::pddl::Action& right_action) const {
        return *left_action < *right_action;
    }

    std::size_t hash<loki::pddl::ActionImpl>::operator()(const loki::pddl::ActionImpl& action) const {
        return action.hash();
    }
}
