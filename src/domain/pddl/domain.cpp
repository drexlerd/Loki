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

#include "../../../include/loki/domain/pddl/domain.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"


namespace loki::pddl {
DomainImpl::DomainImpl(int identifier,
    const std::string& name,
    const Requirements& requirements,
    const TypeList& types,
    const ObjectList& constants,
    const PredicateList& predicates)
    : m_identifier(identifier)
    , m_name(name)
    , m_requirements(requirements)
    , m_types(types)
    , m_constants(constants)
    , m_predicates(predicates)
{
}

bool DomainImpl::operator==(const DomainImpl& other) const {
    return (m_name == other.m_name)
        && (m_requirements == other.m_requirements)
        && (sorted(m_types) == sorted(other.m_types))
        && (sorted(m_constants) == sorted(other.m_constants))
        && (sorted(m_predicates) == sorted(other.m_predicates));
}


bool DomainImpl::operator!=(const DomainImpl& other) const {
    return !(*this == other);
}

bool DomainImpl::operator<(const DomainImpl& other) const {
    return m_identifier < other.m_identifier;
}

bool DomainImpl::operator>(const DomainImpl& other) const {
    return m_identifier > other.m_identifier;
}

size_t DomainImpl::hash() const {
    return hash_combine(
        m_name,
        m_requirements,
        hash_vector(sorted(m_types)),
        hash_vector(sorted(m_constants)),
        hash_vector(sorted(m_predicates)));
}

const std::string& DomainImpl::get_name() const {
    return m_name;
}

const Requirements& DomainImpl::get_requirements() const {
    return m_requirements;
}

const TypeList& DomainImpl::get_types() const {
    return m_types;
}

const ObjectList& DomainImpl::get_constants() const {
    return m_constants;
}

const PredicateList& DomainImpl::get_predicates() const {
    return m_predicates;
}

}

namespace std {
    bool less<loki::pddl::Domain>::operator()(
        const loki::pddl::Domain& left_domain,
        const loki::pddl::Domain& right_domain) const {
        return *left_domain < *right_domain;
    }

    std::size_t hash<loki::pddl::DomainImpl>::operator()(const loki::pddl::DomainImpl& domain) const {
        return domain.hash();
    }
}
