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

#include "loki/details/pddl/requirements.hpp"

#include "loki/details/utils/hash.hpp"

#include <cassert>

namespace loki
{

std::unordered_map<RequirementEnum, std::string> requirement_enum_to_string = { { RequirementEnum::STRIPS, ":strips" },
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
                                                                                { RequirementEnum::ACTION_COSTS, ":action-costs" } };

const std::string& to_string(RequirementEnum requirement)
{
    assert(requirement_enum_to_string.count(requirement));
    return requirement_enum_to_string.at(requirement);
}

RequirementsImpl::RequirementsImpl(size_t index, RequirementEnumSet requirements) : Base(index), m_requirements(std::move(requirements)) {}

bool RequirementsImpl::is_structurally_equivalent_to_impl(const RequirementsImpl& other) const
{
    if (this != &other)
    {
        return (m_requirements == other.m_requirements);
    }
    return true;
}

size_t RequirementsImpl::hash_impl() const { return HashCombiner()(m_requirements); }

void RequirementsImpl::str_impl(std::ostream& out, const FormattingOptions& /*options*/) const
{
    out << "(:requirements ";
    int i = 0;
    for (const auto& requirement : m_requirements)
    {
        if (i != 0)
            out << " ";
        out << to_string(requirement);
        ++i;
    }
    out << ")";
}

bool RequirementsImpl::test(RequirementEnum requirement) const { return m_requirements.count(requirement); }

const RequirementEnumSet& RequirementsImpl::get_requirements() const { return m_requirements; }

}
