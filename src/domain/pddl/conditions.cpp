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

#include "../../../include/loki/domain/pddl/literal.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"


namespace loki::pddl {
/* BaseCondition */
ConditionImpl::ConditionImpl(int identifier)
    : Base<ConditionImpl>(identifier) { }

ConditionImpl::~ConditionImpl() = default;


/* Literal */
ConditionLiteralImpl::ConditionLiteralImpl(int identifier, const Literal& literal)
    : ConditionImpl(identifier)
    , m_literal(literal) { }

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

void ConditionLiteralImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << *m_literal;
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

bool ConditionAndImpl::are_equal_impl(const ConditionImpl& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const ConditionAndImpl&>(other);
        return get_sorted_vector(m_conditions) == get_sorted_vector(other_derived.m_conditions);
    }
    return false;
}

size_t ConditionAndImpl::hash_impl() const {
    return hash_vector(get_sorted_vector(m_conditions));
}

void ConditionAndImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << "(and ";
    for (size_t i = 0; i < m_conditions.size(); ++i) {
        if (i != 0) out << " ";
        out << *m_conditions[i];
    }
    out << ")";
}

void ConditionAndImpl::accept(ConditionVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}

const ConditionList& ConditionAndImpl::get_conditions() const {
    return m_conditions;
}


/* Or */
ConditionOrImpl::ConditionOrImpl(int identifier, const ConditionList& conditions)
    : ConditionImpl(identifier)
    , m_conditions(conditions) { }

bool ConditionOrImpl::are_equal_impl(const ConditionImpl& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const ConditionOrImpl&>(other);
        return get_sorted_vector(m_conditions) == get_sorted_vector(other_derived.m_conditions);
    }
    return false;
}

size_t ConditionOrImpl::hash_impl() const {
    return hash_vector(get_sorted_vector(m_conditions));
}

void ConditionOrImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << "(or ";
    for (size_t i = 0; i < m_conditions.size(); ++i) {
        if (i != 0) out << " ";
        out << *m_conditions[i];
    }
    out << ")";
}

void ConditionOrImpl::accept(ConditionVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}

const ConditionList& ConditionOrImpl::get_conditions() const {
    return m_conditions;
}


/* Not */
ConditionNotImpl::ConditionNotImpl(int identifier, const Condition& condition)
    : ConditionImpl(identifier)
    , m_condition(condition) { }

bool ConditionNotImpl::are_equal_impl(const ConditionImpl& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const ConditionNotImpl&>(other);
        return m_condition == other_derived.m_condition;
    }
    return false;
}

size_t ConditionNotImpl::hash_impl() const {
    return hash_combine(m_condition);
}

void ConditionNotImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << "(not " << *m_condition << ")";
}

void ConditionNotImpl::accept(ConditionVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}

const Condition& ConditionNotImpl::get_condition() const {
    return m_condition;
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

    std::size_t hash<loki::pddl::ConditionOrImpl>::operator()(const loki::pddl::ConditionOrImpl& condition) const {
        return condition.hash_impl();
    }

    std::size_t hash<loki::pddl::ConditionNotImpl>::operator()(const loki::pddl::ConditionNotImpl& condition) const {
        return condition.hash_impl();
    }
}
