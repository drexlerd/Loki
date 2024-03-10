/*
 * Copyright (C) 2023 Dominik Drexler
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

#ifndef LOKI_INCLUDE_LOKI_PDDL_REQUIREMENTS_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_REQUIREMENTS_HPP_

#include "loki/pddl/base.hpp"
#include "loki/pddl/declarations.hpp"

#include <set>
#include <string>
#include <unordered_map>

namespace loki
{
template<typename HolderType>
class PersistentFactory;
}

namespace loki::pddl
{
enum class RequirementEnum
{
    STRIPS,
    TYPING,
    NEGATIVE_PRECONDITIONS,
    DISJUNCTIVE_PRECONDITIONS,
    EQUALITY,
    EXISTENTIAL_PRECONDITIONS,
    UNIVERSAL_PRECONDITIONS,
    QUANTIFIED_PRECONDITIONS,
    CONDITIONAL_EFFECTS,
    FLUENTS,
    OBJECT_FLUENTS,
    NUMERIC_FLUENTS,
    ADL,
    DURATIVE_ACTIONS,
    DERIVED_PREDICATES,
    TIMED_INITIAL_LITERALS,
    PREFERENCES,
    CONSTRAINTS,
    ACTION_COSTS,
};

using RequirementEnumSet = std::set<RequirementEnum>;
using RequirementEnumList = std::vector<RequirementEnum>;

extern std::unordered_map<RequirementEnum, std::string> requirement_enum_to_string;
extern const std::string& to_string(pddl::RequirementEnum requirement);

class RequirementsImpl : public Base<RequirementsImpl>
{
private:
    RequirementEnumSet m_requirements;

    RequirementsImpl(int identifier, RequirementEnumSet requirements);

    // Give access to the constructor.
    template<typename>
    friend class loki::PersistentFactory;

    /// @brief Test for semantic equivalence
    bool is_structurally_equivalent_to_impl(const RequirementsImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<RequirementsImpl>;

public:
    bool test(RequirementEnum requirement) const;

    const RequirementEnumSet& get_requirements() const;
};

}

namespace std
{
// Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
template<>
struct less<loki::pddl::Requirements>
{
    bool operator()(const loki::pddl::Requirements& left_requirements, const loki::pddl::Requirements& right_requirements) const;
};

template<>
struct hash<loki::pddl::RequirementsImpl>
{
    std::size_t operator()(const loki::pddl::RequirementsImpl& requirements) const;
};
}

#endif
