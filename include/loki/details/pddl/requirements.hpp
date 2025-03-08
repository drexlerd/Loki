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

#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/requirements_enum.hpp"

namespace loki
{

class RequirementsImpl
{
private:
    size_t m_index;
    RequirementEnumSet m_requirements;

    RequirementsImpl(size_t index, RequirementEnumSet requirements);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    RequirementsImpl(const RequirementsImpl& other) = delete;
    RequirementsImpl& operator=(const RequirementsImpl& other) = delete;
    RequirementsImpl(RequirementsImpl&& other) = default;
    RequirementsImpl& operator=(RequirementsImpl&& other) = default;

    bool test(RequirementEnum requirement) const;

    size_t get_index() const;
    const RequirementEnumSet& get_requirements() const;

    auto identifying_members() const { return std::tuple(std::cref(get_requirements())); }
};

extern std::ostream& operator<<(std::ostream& out, const RequirementsImpl& element);

extern std::ostream& operator<<(std::ostream& out, Requirements element);

}

#endif
