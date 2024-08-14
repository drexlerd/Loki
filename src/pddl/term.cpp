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

#include "loki/details/pddl/term.hpp"

#include "formatter.hpp"
#include "loki/details/pddl/object.hpp"
#include "loki/details/pddl/variable.hpp"

namespace loki
{

/* TermObjectImpl */
TermObjectImpl::TermObjectImpl(size_t index, Object object) : m_index(index), m_object(std::move(object)) {}

size_t TermObjectImpl::get_index() const { return m_index; }

const Object& TermObjectImpl::get_object() const { return m_object; }

/* TermVariableImpl */
TermVariableImpl::TermVariableImpl(size_t index, Variable variable) : m_index(index), m_variable(std::move(variable)) {}

size_t TermVariableImpl::get_index() const { return m_index; }

const Variable& TermVariableImpl::get_variable() const { return m_variable; }

size_t UniquePDDLHasher<const TermObjectImpl&>::operator()(const TermObjectImpl& e) const { return UniquePDDLHashCombiner()(e.get_object()); }

size_t UniquePDDLHasher<const TermVariableImpl&>::operator()(const TermVariableImpl& e) const { return UniquePDDLHashCombiner()(e.get_variable()); }

size_t UniquePDDLHasher<const TermImpl*>::operator()(const TermImpl* e) const
{
    return std::visit([](const auto& arg) { return UniquePDDLHasher<decltype(arg)>()(arg); }, *e);
}

bool UniquePDDLEqualTo<const TermObjectImpl&>::operator()(const TermObjectImpl& l, const TermObjectImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_object() == r.get_object());
    }
    return true;
}

bool UniquePDDLEqualTo<const TermVariableImpl&>::operator()(const TermVariableImpl& l, const TermVariableImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_variable() == r.get_variable());
    }
    return true;
}

bool UniquePDDLEqualTo<const TermImpl*>::operator()(const TermImpl* l, const TermImpl* r) const { return UniquePDDLEqualTo<TermImpl>()(*l, *r); }

std::ostream& operator<<(std::ostream& out, const TermObjectImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const TermVariableImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const TermImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

}
