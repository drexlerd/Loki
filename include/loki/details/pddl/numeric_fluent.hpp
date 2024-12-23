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

#ifndef LOKI_INCLUDE_LOKI_PDDL_NUMERIC_FLUENT_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_NUMERIC_FLUENT_HPP_

#include "loki/details/pddl/declarations.hpp"

#include <string>

namespace loki
{
class NumericFluentImpl
{
private:
    size_t m_index;
    Function m_function;
    double m_number;

    // Give access to the constructor.
    template<HasIdentifiableMembers T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

    NumericFluentImpl(size_t index, Function function, double number);

public:
    // moveable but not copyable
    NumericFluentImpl(const NumericFluentImpl& other) = delete;
    NumericFluentImpl& operator=(const NumericFluentImpl& other) = delete;
    NumericFluentImpl(NumericFluentImpl&& other) = default;
    NumericFluentImpl& operator=(NumericFluentImpl&& other) = default;

    size_t get_index() const;
    const Function& get_function() const;
    double get_number() const;

    auto identifiable_members() const { return std::forward_as_tuple(std::as_const(m_function), std::as_const(m_number)); }
};

extern std::ostream& operator<<(std::ostream& out, const NumericFluentImpl& element);

}

#endif
