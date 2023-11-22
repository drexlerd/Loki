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

#include "../../../include/loki/domain/pddl/conditions.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"


namespace loki::pddl {
/* BaseCondition */
ConditionImpl::ConditionImpl(int identifier)
    : Base(identifier) { }

ConditionImpl::~ConditionImpl() = default;


/* Literal */
ConditionLiteralImpl::ConditionLiteralImpl(int identifier, const Literal& literal)
    : ConditionImpl(identifier)
    , m_literal(literal) { }

ConditionLiteralImpl::~ConditionLiteralImpl() = default;


bool ConditionLiteralImpl::are_equal_impl(const ConditionImpl& other) const {
    // https://stackoverflow.com/questions/11332075/comparing-polymorphic-base-types-in-c-without-rtti
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const ConditionLiteralImpl&>(other);
        return m_literal == other_derived.m_literal;
    }
    return false;
}

size_t ConditionLiteralImpl::hash_impl() const {
    return std::hash<Literal>()(m_literal);
}

void ConditionLiteralImpl::accept(ConditionVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}

const Literal& ConditionLiteralImpl::get_literal() const {
    return m_literal;
}


/* And */
ConditionAndImpl::ConditionAndImpl(int identifier, const ConditionList& conditions)
    : ConditionImpl(identifier)
    , m_conditions(conditions) { }

ConditionAndImpl::~ConditionAndImpl() = default;


bool ConditionAndImpl::are_equal_impl(const ConditionImpl& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const ConditionAndImpl&>(other);
        return sorted(m_conditions) == sorted(other_derived.m_conditions);
    }
    return false;
}


size_t ConditionAndImpl::hash_impl() const {
    return hash_vector(sorted(m_conditions));
}

void ConditionAndImpl::accept(ConditionVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}

const ConditionList& ConditionAndImpl::get_conditions() const {
    return m_conditions;
}

}

namespace std {
    bool less<loki::pddl::Condition>::operator()(
        const loki::pddl::Condition& left_condition,
        const loki::pddl::Condition& right_condition) const {
        return *left_condition < *right_condition;
    }

    std::size_t hash<loki::pddl::ConditionLiteralImpl>::operator()(const loki::pddl::ConditionLiteralImpl& condition) const {
        return condition.hash_impl();
    }

    std::size_t hash<loki::pddl::ConditionAndImpl>::operator()(const loki::pddl::ConditionAndImpl& condition) const {
        return condition.hash_impl();
    }
}
