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

#include "loki/details/pddl/object.hpp"
#include "loki/details/pddl/variable.hpp"
#include "loki/details/utils/collections.hpp"
#include "loki/details/utils/equal_to.hpp"
#include "loki/details/utils/hash.hpp"

namespace loki
{

/* TermObjectImpl */
TermObjectImpl::TermObjectImpl(size_t index, Object object) : Base(index), m_object(std::move(object)) {}

void TermObjectImpl::str_impl(std::ostream& out, const FormattingOptions& /*options*/) const { out << m_object->get_name(); }

const Object& TermObjectImpl::get_object() const { return m_object; }

size_t ShallowHash<const TermObjectImpl&>::operator()(const TermObjectImpl& e) const { return ShallowHashCombiner()(e.get_object()); }

bool ShallowEqualTo<const TermObjectImpl&>::operator()(const TermObjectImpl& l, const TermObjectImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_object() == r.get_object());
    }
    return true;
}

/* TermVariableImpl */
TermVariableImpl::TermVariableImpl(size_t index, Variable variable) : Base(index), m_variable(std::move(variable)) {}

void TermVariableImpl::str_impl(std::ostream& out, const FormattingOptions& /*options*/) const { out << m_variable->get_name(); }

const Variable& TermVariableImpl::get_variable() const { return m_variable; }

size_t ShallowHash<const TermVariableImpl&>::operator()(const TermVariableImpl& e) const { return ShallowHashCombiner()(e.get_variable()); }

bool ShallowEqualTo<const TermVariableImpl&>::operator()(const TermVariableImpl& l, const TermVariableImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_variable() == r.get_variable());
    }
    return true;
}

/* TermImpl */

size_t ShallowHash<const TermImpl*>::operator()(const TermImpl* e) const
{
    return std::visit([](const auto& arg) { return ShallowHash<decltype(arg)>()(arg); }, *e);
}

bool ShallowEqualTo<const TermImpl*>::operator()(const TermImpl* l, const TermImpl* r) const { return ShallowEqualTo<TermImpl>()(*l, *r); }

}
