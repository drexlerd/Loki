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

#include "../../../include/loki/domain/pddl/object.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"


namespace loki::pddl {
ObjectImpl::ObjectImpl(int identifier, const std::string& name, const TypeList& types)
    : m_identifier(identifier)
    , m_name(name)
    , m_types(types)
{
}

bool ObjectImpl::operator==(const ObjectImpl& other) const {
    return (m_name == other.m_name) && (sorted(m_types) == sorted(other.m_types));
}

bool ObjectImpl::operator!=(const ObjectImpl& other) const {
    return !(*this == other);
}

bool ObjectImpl::operator<(const ObjectImpl& other) const {
    return m_identifier < other.m_identifier;
}

bool ObjectImpl::operator>(const ObjectImpl& other) const {
    return m_identifier > other.m_identifier;
}

size_t ObjectImpl::hash() const {
    return hash_combine(m_name, hash_vector(sorted(m_types)));
}

const std::string& ObjectImpl::get_name() const {
    return m_name;
}

const TypeList& ObjectImpl::get_bases() const {
    return m_types;
}

}

namespace std {
    std::size_t hash<loki::pddl::ObjectImpl>::operator()(const loki::pddl::ObjectImpl& object) const {
        return object.hash();
    }
}
