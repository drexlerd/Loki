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

#ifndef LOKI_INCLUDE_LOKI_PDDL_PREDICATE_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_PREDICATE_HPP_

#include "loki/details/pddl/declarations.hpp"

#include <string>

namespace loki
{

// An alternative name is AtomSkeleton
class PredicateImpl
{
private:
    size_t m_index;
    std::string m_name;
    ParameterList m_parameters;

    PredicateImpl(size_t index, std::string name, ParameterList parameters);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    PredicateImpl(const PredicateImpl& other) = delete;
    PredicateImpl& operator=(const PredicateImpl& other) = delete;
    PredicateImpl(PredicateImpl&& other) = default;
    PredicateImpl& operator=(PredicateImpl&& other) = default;

    size_t get_index() const;
    const std::string& get_name() const;
    const ParameterList& get_parameters() const;

    auto identifying_members() const { return std::tuple(std::cref(get_name()), std::cref(get_parameters())); }
};

extern std::ostream& operator<<(std::ostream& out, const PredicateImpl& element);

extern std::ostream& operator<<(std::ostream& out, Predicate element);

}

#endif
