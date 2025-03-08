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

#include "loki/details/pddl/literal.hpp"

#include "formatter.hpp"
#include "loki/details/pddl/atom.hpp"

namespace loki
{
LiteralImpl::LiteralImpl(size_t index, bool is_negated, Atom atom) : m_index(index), m_is_negated(is_negated), m_atom(std::move(atom)) {}

size_t LiteralImpl::get_index() const { return m_index; }

bool LiteralImpl::is_negated() const { return m_is_negated; }

Atom LiteralImpl::get_atom() const { return m_atom; }

std::ostream& operator<<(std::ostream& out, const LiteralImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, Literal element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

}
