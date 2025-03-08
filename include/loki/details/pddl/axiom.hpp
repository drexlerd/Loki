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
    ParameterList m_parameters;  // the updated parameters which might contain subtypes.
    Literal m_literal;
    Condition m_condition;

    AxiomImpl(size_t index, ParameterList parameters, Literal subtyped_literal, Condition condition);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    AxiomImpl(const AxiomImpl& other) = delete;
    AxiomImpl& operator=(const AxiomImpl& other) = delete;
    AxiomImpl(AxiomImpl&& other) = default;
    AxiomImpl& operator=(AxiomImpl&& other) = default;

    size_t get_index() const;
    const ParameterList& get_parameters() const;
    Literal get_literal() const;
    Condition get_condition() const;

    auto identifying_members() const { return std::tuple(std::cref(get_parameters()), get_literal(), get_condition()); }
};

extern std::ostream& operator<<(std::ostream& out, const AxiomImpl& element);

extern std::ostream& operator<<(std::ostream& out, Axiom element);

}

#endif
