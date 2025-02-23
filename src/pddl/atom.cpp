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

#include "loki/details/pddl/atom.hpp"

#include "formatter.hpp"
#include "loki/details/pddl/predicate.hpp"
#include "loki/details/pddl/term.hpp"

namespace loki
{
AtomImpl::AtomImpl(size_t index, Predicate predicate, TermList terms) : m_index(index), m_predicate(std::move(predicate)), m_terms(std::move(terms)) {}

size_t AtomImpl::get_index() const { return m_index; }

const Predicate& AtomImpl::get_predicate() const { return m_predicate; }

const TermList& AtomImpl::get_terms() const { return m_terms; }

std::ostream& operator<<(std::ostream& out, const AtomImpl& element)
{
    write<StringTag>(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, Atom element)
{
    write<AddressTag>(*element, out);
    return out;
}

}
