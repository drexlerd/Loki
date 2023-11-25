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

#include <cassert>


namespace loki::pddl {

std::unordered_map<RequirementEnum, std::string> requirement_enum_to_string = {
    { RequirementEnum::STRIPS, ":strips" },
    { RequirementEnum::TYPING, ":typing" },
    { RequirementEnum::NEGATIVE_PRECONDITIONS, ":negative-preconditions" },
    { RequirementEnum::DISJUNCTIVE_PRECONDITIONS, ":disjunctive-preconditions" },
    { RequirementEnum::EQUALITY, ":equality" },
    { RequirementEnum::EXISTENTIAL_PRECONDITIONS, ":existential-preconditions" },
    { RequirementEnum::UNIVERSAL_PRECONDITIONS, ":universal-preconditions" },
    { RequirementEnum::QUANTIFIED_PRECONDITIONS, ":quantified-preconditions" },
    { RequirementEnum::CONDITIONAL_EFFECTS, ":conditional-effects" },
    { RequirementEnum::FLUENTS, ":fluents" },
    { RequirementEnum::OBJECT_FLUENTS, ":object-fluents" },
    { RequirementEnum::NUMERIC_FLUENTS, ":numeric-fluents" },
    { RequirementEnum::ADL, ":adl" },
    { RequirementEnum::DURATIVE_ACTIONS, ":durative-actions" },
    { RequirementEnum::DERIVED_PREDICATES, ":derived-predicates" },
    { RequirementEnum::TIMED_INITIAL_LITERALS, ":timed-initial-literals" },
    { RequirementEnum::PREFERENCES, ":preferences" },
    { RequirementEnum::CONSTRAINTS, ":constraints" },
    { RequirementEnum::ACTION_COSTS, ":action-costs" }
};

const std::string& to_string(pddl::RequirementEnum requirement) {
    assert(requirement_enum_to_string.count(requirement));
    return requirement_enum_to_string.at(requirement);
}


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

void RequirementsImpl::str_impl(std::stringstream& out, const FormattingOptions& options) const {
    out << "TODO";
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
