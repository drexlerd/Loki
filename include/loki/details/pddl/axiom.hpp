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

#ifndef LOKI_INCLUDE_LOKI_PDDL_DERIVED_PREDICATE_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_DERIVED_PREDICATE_HPP_

#include "loki/details/pddl/declarations.hpp"

#include <string>

namespace loki
{
class AxiomImpl
{
private:
    size_t m_index;
    std::string m_derived_predicate_name;
    ParameterList m_parameters;
    Condition m_condition;
    size_t m_num_parameters_to_ground_head;

    AxiomImpl(size_t index, std::string derived_predicate_name, ParameterList parameters, Condition condition, size_t num_parameters_to_ground_head);

    // Give access to the constructor.
    template<HasIdentifiableMembers T>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    AxiomImpl(const AxiomImpl& other) = delete;
    AxiomImpl& operator=(const AxiomImpl& other) = delete;
    AxiomImpl(AxiomImpl&& other) = default;
    AxiomImpl& operator=(AxiomImpl&& other) = default;

    size_t get_index() const;
    const std::string& get_derived_predicate_name() const;
    const ParameterList& get_parameters() const;
    const Condition& get_condition() const;
    size_t get_num_parameters_to_ground_head() const;

    auto identifiable_members() const
    {
        return std::forward_as_tuple(std::as_const(m_derived_predicate_name),
                                     std::as_const(m_parameters),
                                     std::as_const(m_condition),
                                     std::as_const(m_num_parameters_to_ground_head));
    }
};

extern std::ostream& operator<<(std::ostream& out, const AxiomImpl& element);

}

#endif
