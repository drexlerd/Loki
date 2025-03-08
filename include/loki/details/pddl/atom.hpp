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

#ifndef LOKI_INCLUDE_LOKI_PDDL_ATOM_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_ATOM_HPP_

#include "loki/details/pddl/declarations.hpp"

#include <string>

namespace loki
{
class AtomImpl
{
private:
    size_t m_index;
    Predicate m_predicate;
    TermList m_terms;

    AtomImpl(size_t index, Predicate predicate, TermList terms);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    AtomImpl(const AtomImpl& other) = delete;
    AtomImpl& operator=(const AtomImpl& other) = delete;
    AtomImpl(AtomImpl&& other) = default;
    AtomImpl& operator=(AtomImpl&& other) = default;

    size_t get_index() const;
    Predicate get_predicate() const;
    const TermList& get_terms() const;

    auto identifying_members() const { return std::tuple(get_predicate(), std::cref(get_terms())); }
};

extern std::ostream& operator<<(std::ostream& out, const AtomImpl& element);

extern std::ostream& operator<<(std::ostream& out, Atom element);

}

#endif
