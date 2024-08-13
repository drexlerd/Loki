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

namespace loki
{

/* TermObjectImpl */
TermObjectImpl::TermObjectImpl(size_t index, Object object) : Base(index), m_object(std::move(object)) {}

const Object& TermObjectImpl::get_object() const { return m_object; }

/* TermVariableImpl */
TermVariableImpl::TermVariableImpl(size_t index, Variable variable) : Base(index), m_variable(std::move(variable)) {}

const Variable& TermVariableImpl::get_variable() const { return m_variable; }

}
