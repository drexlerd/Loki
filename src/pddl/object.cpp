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

#include "loki/details/pddl/object.hpp"

#include "formatter.hpp"
#include "loki/details/pddl/type.hpp"
#include "loki/details/utils/collections.hpp"

namespace loki
{
ObjectImpl::ObjectImpl(size_t index, std::string name, TypeList types) : m_index(index), m_name(std::move(name)), m_types(std::move(types)) {}

size_t ObjectImpl::get_index() const { return m_index; }

const std::string& ObjectImpl::get_name() const { return m_name; }

const TypeList& ObjectImpl::get_bases() const { return m_types; }

std::ostream& operator<<(std::ostream& out, const ObjectImpl& element)
{
    write_typed(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, Object element)
{
    write_typed(*element, AddressFormatter(), out);
    return out;
}

}
