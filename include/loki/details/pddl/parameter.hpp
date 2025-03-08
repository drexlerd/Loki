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

#ifndef LOKI_INCLUDE_LOKI_PDDL_PARAMETER_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_PARAMETER_HPP_

#include "loki/details/pddl/declarations.hpp"

#include <string>

namespace loki
{

class ParameterImpl
{
private:
    size_t m_index;
    Variable m_variable;
    TypeList m_types;

    ParameterImpl(size_t index, Variable variable, TypeList types);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    ParameterImpl(const ParameterImpl& other) = delete;
    ParameterImpl& operator=(const ParameterImpl& other) = delete;
    ParameterImpl(ParameterImpl&& other) = default;
    ParameterImpl& operator=(ParameterImpl&& other) = default;

    size_t get_index() const;
    Variable get_variable() const;
    const TypeList& get_bases() const;

    auto identifying_members() const { return std::tuple(get_variable(), std::cref(get_bases())); }
};

/// @brief Return true iff specialized_parameter is a specialized version of generalized_parameter
/// by checking whether the types are compatible
extern bool is_specialized_parameter(const Parameter& specialized_parameter, const Parameter& generalized_parameter);

extern std::ostream& operator<<(std::ostream& out, const ParameterImpl& element);

extern std::ostream& operator<<(std::ostream& out, Parameter element);
}

#endif
