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
#include "loki/details/utils/collections.hpp"
#include "loki/details/utils/hash.hpp"

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

size_t ShallowHash<LiteralImpl>::operator()(const LiteralImpl& e) const { return ShallowHashCombiner()(e.is_negated(), e.get_atom()); }

bool ShallowEqualTo<LiteralImpl>::operator()(const LiteralImpl& l, const LiteralImpl& r) const
{
    if (&l != &r)
    {
        return (l.is_negated() == r.is_negated()) && (l.get_atom() == r.get_atom());
    }
    return true;
}

}
