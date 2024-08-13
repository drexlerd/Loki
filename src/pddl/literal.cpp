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

#include "loki/details/pddl/atom.hpp"

namespace loki
{
LiteralImpl::LiteralImpl(size_t index, bool is_negated, Atom atom) : Base(index), m_is_negated(is_negated), m_atom(std::move(atom)) {}

void LiteralImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    if (m_is_negated)
    {
        out << "(not ";
        m_atom->str(out, options);
        out << ")";
    }
    else
    {
        m_atom->str(out, options);
    }
}

bool LiteralImpl::is_negated() const { return m_is_negated; }

const Atom& LiteralImpl::get_atom() const { return m_atom; }

}
