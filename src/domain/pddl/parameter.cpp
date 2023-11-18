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

#include "../../../include/loki/domain/pddl/parameter.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"


namespace loki::pddl {
ParameterImpl::ParameterImpl(int identifier, const std::string& name, const TypeList& types)
    : m_identifier(identifier)
    , m_name(name)
    , m_types(types)
{
}

bool ParameterImpl::operator==(const ParameterImpl& other) const {
    return (m_name == other.m_name) && (sorted(m_types) == sorted(other.m_types));
}

bool ParameterImpl::operator!=(const ParameterImpl& other) const {
    return !(*this == other);
}

bool ParameterImpl::operator<(const ParameterImpl& other) const {
    return m_identifier < other.m_identifier;
}

bool ParameterImpl::operator>(const ParameterImpl& other) const {
    return m_identifier > other.m_identifier;
}

size_t ParameterImpl::hash() const {
    return hash_combine(m_name, hash_vector(sorted(m_types)));
}

const std::string& ParameterImpl::get_name() const {
    return m_name;
}

const TypeList& ParameterImpl::get_bases() const {
    return m_types;
}

}

namespace std {
    bool less<loki::pddl::Parameter>::operator()(
        const loki::pddl::Parameter& left_parameter,
        const loki::pddl::Parameter& right_parameter) const {
        return *left_parameter < *right_parameter;
    }

    std::size_t hash<loki::pddl::ParameterImpl>::operator()(const loki::pddl::ParameterImpl& object) const {
        return object.hash();
    }
}
