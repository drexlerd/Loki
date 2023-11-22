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

#include "../../../include/loki/domain/pddl/requirements.hpp"
#include "../../../include/loki/common/hash.hpp"


namespace loki::pddl {
RequirementsImpl::RequirementsImpl(int identifier, const RequirementEnumSet& requirements)
    : Base(identifier)
    , m_requirements(requirements)
{
}

bool RequirementsImpl::are_equal_impl(const RequirementsImpl& other) const {
    return (m_requirements == other.m_requirements);
}

size_t RequirementsImpl::hash_impl() const {
    return hash_set(m_requirements);
}

bool RequirementsImpl::test(RequirementEnum requirement) const {
    return m_requirements.count(requirement);
}

}

namespace std {
    bool less<loki::pddl::Requirements>::operator()(
        const loki::pddl::Requirements& left_requirements,
        const loki::pddl::Requirements& right_requirements) const {
        return *left_requirements < *right_requirements;
    }

    std::size_t hash<loki::pddl::RequirementsImpl>::operator()(const loki::pddl::RequirementsImpl& requirements) const {
        return requirements.hash_impl();
    }
}
