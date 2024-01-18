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

#include <loki/problem/pddl/ground_conditions.hpp>

#include <loki/common/hash.hpp>
#include <loki/common/collections.hpp>
#include <loki/common/pddl/visitors.hpp>
#include <loki/domain/pddl/parameter.hpp>
#include <loki/problem/pddl/ground_literal.hpp>


namespace loki::pddl {

/* Literal */
GroundConditionLiteralImpl::GroundConditionLiteralImpl(int identifier, GroundLiteral literal)
    : Base(identifier)
    , m_literal(std::move(literal)) { }

bool GroundConditionLiteralImpl::is_structurally_equivalent_to_impl(const GroundConditionLiteralImpl& other) const {
    if (this != &other) {
        return m_literal == other.m_literal;
    }
    return true;
}

size_t GroundConditionLiteralImpl::hash_impl() const {
    return std::hash<GroundLiteral>()(m_literal);
}

void GroundConditionLiteralImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << *m_literal;
}

const GroundLiteral& GroundConditionLiteralImpl::get_literal() const {
    return m_literal;
}


/* And */
GroundConditionAndImpl::GroundConditionAndImpl(int identifier, GroundConditionList conditions)
    : Base(identifier)
    , m_conditions(std::move(conditions)) { }

bool GroundConditionAndImpl::is_structurally_equivalent_to_impl(const GroundConditionAndImpl& other) const {
    if (this != &other) {
        return get_sorted_vector(m_conditions) == get_sorted_vector(other.m_conditions);
    }
    return true;
}

size_t GroundConditionAndImpl::hash_impl() const {
    return hash_container(get_sorted_vector(m_conditions));
}

void GroundConditionAndImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << "(and ";
    for (size_t i = 0; i < m_conditions.size(); ++i) {
        if (i != 0) out << " ";
        std::visit(StringifyVisitor(out, options), *m_conditions[i]);
    }
    out << ")";
}

const GroundConditionList& GroundConditionAndImpl::get_conditions() const {
    return m_conditions;
}


/* Or */
GroundConditionOrImpl::GroundConditionOrImpl(int identifier, GroundConditionList conditions)
    : Base(identifier)
    , m_conditions(std::move(conditions)) { }

bool GroundConditionOrImpl::is_structurally_equivalent_to_impl(const GroundConditionOrImpl& other) const {
    if (this != &other) {
        return get_sorted_vector(m_conditions) == get_sorted_vector(other.m_conditions);
    }
    return true;
}

size_t GroundConditionOrImpl::hash_impl() const {
    return hash_container(get_sorted_vector(m_conditions));
}

void GroundConditionOrImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << "(or ";
    for (size_t i = 0; i < m_conditions.size(); ++i) {
        if (i != 0) out << " ";
        std::visit(StringifyVisitor(out, options), *m_conditions[i]);
    }
    out << ")";
}

const GroundConditionList& GroundConditionOrImpl::get_conditions() const {
    return m_conditions;
}


/* Not */
GroundConditionNotImpl::GroundConditionNotImpl(int identifier, GroundCondition condition)
    : Base(identifier)
    , m_condition(std::move(condition)) { }

bool GroundConditionNotImpl::is_structurally_equivalent_to_impl(const GroundConditionNotImpl& other) const {
    if (this != &other) {
        return m_condition == other.m_condition;
    }
    return true;
}

size_t GroundConditionNotImpl::hash_impl() const {
    return hash_combine(m_condition);
}

void GroundConditionNotImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << "(not ";
    std::visit(StringifyVisitor(out, options), *m_condition);
    out << ")";
}

const GroundCondition& GroundConditionNotImpl::get_condition() const {
    return m_condition;
}


/* Imply */
GroundConditionImplyImpl::GroundConditionImplyImpl(int identifier, GroundCondition condition_left, GroundCondition condition_right)
    : Base(identifier), m_condition_left(std::move(condition_left)), m_condition_right(std::move(condition_right)) { }

bool GroundConditionImplyImpl::is_structurally_equivalent_to_impl(const GroundConditionImplyImpl& other) const {
    if (this != &other) {
        return (m_condition_left == other.m_condition_left)
            && (m_condition_right == other.m_condition_right);
    }
    return true;
}

size_t GroundConditionImplyImpl::hash_impl() const {
    return hash_combine(m_condition_left, m_condition_right);
}

void GroundConditionImplyImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << "(imply ";
    std::visit(StringifyVisitor(out, options), *m_condition_left);
    out << " ";
    std::visit(StringifyVisitor(out, options), *m_condition_right);
    out << ")";
}

const GroundCondition& GroundConditionImplyImpl::get_condition_left() const {
    return m_condition_left;
}

const GroundCondition& GroundConditionImplyImpl::get_condition_right() const {
    return m_condition_right;
}


/* Exists */
GroundConditionExistsImpl::GroundConditionExistsImpl(int identifier, ParameterList parameters, GroundCondition condition)
    : Base(identifier), m_parameters(std::move(parameters)), m_condition(std::move(condition)) { }

bool GroundConditionExistsImpl::is_structurally_equivalent_to_impl(const GroundConditionExistsImpl& other) const {
    if (this != &other) {
        return (m_parameters == other.m_parameters)
            && (m_condition == other.m_condition);
    }
    return true;
}

size_t GroundConditionExistsImpl::hash_impl() const {
    return hash_combine(hash_container(m_parameters), m_condition);
}

void GroundConditionExistsImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << "(exists (";
    for (size_t i = 0; i < m_parameters.size(); ++i) {
        if (i != 0) out << " ";
        out << *m_parameters[i];
    }
    out << ") ";
    std::visit(StringifyVisitor(out, options), *m_condition);
    out << ")";
}

const ParameterList& GroundConditionExistsImpl::get_parameters() const {
    return m_parameters;
}

const GroundCondition& GroundConditionExistsImpl::get_condition() const {
    return m_condition;
}


/* Forall */
GroundConditionForallImpl::GroundConditionForallImpl(int identifier, ParameterList parameters, GroundCondition condition)
    : Base(identifier), m_parameters(std::move(parameters)), m_condition(std::move(condition)) { }

bool GroundConditionForallImpl::is_structurally_equivalent_to_impl(const GroundConditionForallImpl& other) const {
    if (this != &other) {
        return (m_parameters == other.m_parameters)
            && (m_condition == other.m_condition);
    }
    return true;
}

size_t GroundConditionForallImpl::hash_impl() const {
    return hash_combine(hash_container(m_parameters), m_condition);
}

void GroundConditionForallImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << "(forall (";
    for (size_t i = 0; i < m_parameters.size(); ++i) {
        if (i != 0) out << " ";
        out << *m_parameters[i];
    }
    out << ") ";
    std::visit(StringifyVisitor(out, options), *m_condition);
    out << ")";
}

const ParameterList& GroundConditionForallImpl::get_parameters() const {
    return m_parameters;
}

const GroundCondition& GroundConditionForallImpl::get_condition() const {
    return m_condition;
}

}


namespace std {
    bool less<loki::pddl::GroundCondition>::operator()(
        const loki::pddl::GroundCondition& left_condition,
        const loki::pddl::GroundCondition& right_condition) const {
        return std::visit(loki::pddl::LessComparatorVisitor(), *left_condition, *right_condition);
    }

    std::size_t hash<loki::pddl::GroundConditionLiteralImpl>::operator()(const loki::pddl::GroundConditionLiteralImpl& condition) const {
        return condition.hash();
    }

    std::size_t hash<loki::pddl::GroundConditionAndImpl>::operator()(const loki::pddl::GroundConditionAndImpl& condition) const {
        return condition.hash();
    }

    std::size_t hash<loki::pddl::GroundConditionOrImpl>::operator()(const loki::pddl::GroundConditionOrImpl& condition) const {
        return condition.hash();
    }

    std::size_t hash<loki::pddl::GroundConditionNotImpl>::operator()(const loki::pddl::GroundConditionNotImpl& condition) const {
        return condition.hash();
    }

    std::size_t hash<loki::pddl::GroundConditionImplyImpl>::operator()(const loki::pddl::GroundConditionImplyImpl& condition) const {
        return condition.hash();
    }

    std::size_t hash<loki::pddl::GroundConditionExistsImpl>::operator()(const loki::pddl::GroundConditionExistsImpl& condition) const {
        return condition.hash();
    }

    std::size_t hash<loki::pddl::GroundConditionForallImpl>::operator()(const loki::pddl::GroundConditionForallImpl& condition) const {
        return condition.hash();
    }
}
