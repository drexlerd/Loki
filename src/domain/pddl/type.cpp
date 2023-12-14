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

#include "../../../include/loki/domain/pddl/type.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"


namespace loki::pddl {
TypeImpl::TypeImpl(int identifier, const std::string& name, const TypeList& bases)
    : Base(identifier)
    , m_name(name)
    , m_bases(bases)
{
}

bool TypeImpl::are_equal_impl(const TypeImpl& other) const {
    return (m_name == other.m_name) && (get_sorted_vector(m_bases) == get_sorted_vector(other.m_bases));
}

size_t TypeImpl::hash_impl() const {
    return hash_combine(m_name, hash_vector(get_sorted_vector(m_bases)));
}

void TypeImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << m_name;
}

const std::string& TypeImpl::get_name() const {
    return m_name;
}

const TypeList& TypeImpl::get_bases() const {
    return m_bases;
}

}

namespace std {
    bool less<loki::pddl::Type>::operator()(
        const loki::pddl::Type& left_type,
        const loki::pddl::Type& right_type) const {
        return *left_type < *right_type;
    }

    std::size_t hash<loki::pddl::TypeImpl>::operator()(const loki::pddl::TypeImpl& type) const {
        return type.hash_impl();
    }
}
