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

#include "../../../include/loki/domain/pddl/derived_predicate.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"


namespace loki::pddl {
DerivedPredicateImpl::DerivedPredicateImpl(int identifier, const ParameterList& parameters, const Condition& condition)
    : Base(identifier)
    , m_parameters(parameters)
    , m_condition(condition)
{
}

bool DerivedPredicateImpl::operator==(const DerivedPredicateImpl& other) const {
    return (sorted(m_parameters) == sorted(other.m_parameters))
        && (m_condition == other.m_condition);
}

bool DerivedPredicateImpl::operator!=(const DerivedPredicateImpl& other) const {
    return !(*this == other);
}

size_t DerivedPredicateImpl::hash() const {
    return hash_combine(hash_vector(sorted(m_parameters)), m_condition);
}

const ParameterList& DerivedPredicateImpl::get_parameters() const {
    return m_parameters;
}

const Condition& DerivedPredicateImpl::get_condition() const {
    return m_condition;
}

}


namespace std {
    bool less<loki::pddl::DerivedPredicate>::operator()(
        const loki::pddl::DerivedPredicate& left_predicate,
        const loki::pddl::DerivedPredicate& right_predicate) const {
        return *left_predicate < *right_predicate;
    }

    std::size_t hash<loki::pddl::DerivedPredicateImpl>::operator()(const loki::pddl::DerivedPredicateImpl& action) const {
        return action.hash();
    }
}
