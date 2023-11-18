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

#include "help_functions.hpp"

#include "../../../include/loki/domain/pddl/predicate.hpp"

#include <memory>

namespace loki::pddl {
PredicateImpl::PredicateImpl(int identifier, const std::string& name, const ParameterList& parameters)
    : m_identifier(identifier)
    , m_name(name)
    , m_parameters(parameters)
{
}

bool PredicateImpl::operator==(const PredicateImpl& other) const {
    return (m_name == other.m_name) && (m_parameters == other.m_parameters);
}

bool PredicateImpl::operator!=(const PredicateImpl& other) const {
    return !(*this == other);
}

bool PredicateImpl::operator<(const PredicateImpl& other) const {
    return m_identifier < other.m_identifier;
}

bool PredicateImpl::operator>(const PredicateImpl& other) const {
    return m_identifier > other.m_identifier;
}

size_t PredicateImpl::hash() const {
    return hash_combine(m_name, hash_vector(m_parameters));
}

const std::string& PredicateImpl::get_name() const {
    return m_name;
}

const ParameterList& PredicateImpl::get_parameters() const {
    return m_parameters;
}

}

namespace std {
    std::size_t hash<loki::pddl::PredicateImpl>::operator()(const loki::pddl::PredicateImpl& predicate) const {
        return predicate.hash();
    }
}
