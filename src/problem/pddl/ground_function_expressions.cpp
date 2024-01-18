/*
 * Copyright (C) 2023 Dominik Drexler
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

#include <loki/problem/pddl/ground_function_expressions.hpp>

#include <loki/common/hash.hpp>
#include <loki/common/collections.hpp>
#include <loki/common/pddl/visitors.hpp>
#include <loki/problem/pddl/ground_function.hpp>

#include <cassert>


namespace loki::pddl {

/* GroundFunctionExpressionNumber */
GroundFunctionExpressionNumberImpl::GroundFunctionExpressionNumberImpl(int identifier, double number)
    : Base(identifier), m_number(number) { }

bool GroundFunctionExpressionNumberImpl::is_structurally_equivalent_to_impl(const GroundFunctionExpressionNumberImpl& other) const {
    if (this != &other) {
        return m_number == other.m_number;
    }
    return true;
}

size_t GroundFunctionExpressionNumberImpl::hash_impl() const {
    return std::hash<double>()(m_number);
}

void GroundFunctionExpressionNumberImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << m_number;
}

double GroundFunctionExpressionNumberImpl::get_number() const {
    return m_number;
}


/* GroundFunctionExpressionBinaryOperator */
GroundFunctionExpressionBinaryOperatorImpl::GroundFunctionExpressionBinaryOperatorImpl(int identifier,
    BinaryOperatorEnum binary_operator,
    GroundFunctionExpression left_function_expression,
    GroundFunctionExpression right_function_expression)
    : Base(identifier)
    , m_binary_operator(binary_operator)
    , m_left_function_expression(std::move(left_function_expression))
    , m_right_function_expression(std::move(right_function_expression)) { }

bool GroundFunctionExpressionBinaryOperatorImpl::is_structurally_equivalent_to_impl(const GroundFunctionExpressionBinaryOperatorImpl& other) const {
    if (this != &other) {
        return (m_binary_operator == other.m_binary_operator)
            && (m_left_function_expression == other.m_left_function_expression)
            && (m_right_function_expression == other.m_right_function_expression);
    }
    return true;
}

size_t GroundFunctionExpressionBinaryOperatorImpl::hash_impl() const {
    return hash_combine(m_binary_operator, m_left_function_expression, m_right_function_expression);
}

void GroundFunctionExpressionBinaryOperatorImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << "(" << to_string(m_binary_operator) << " ";
    std::visit(StringifyVisitor(out, options), *m_left_function_expression);
    out << " ";
    std::visit(StringifyVisitor(out, options), *m_right_function_expression);
    out << ")";
}

BinaryOperatorEnum GroundFunctionExpressionBinaryOperatorImpl::get_binary_operator() const {
    return m_binary_operator;
}

const GroundFunctionExpression& GroundFunctionExpressionBinaryOperatorImpl::get_left_function_expression() const {
    return m_left_function_expression;
}

const GroundFunctionExpression& GroundFunctionExpressionBinaryOperatorImpl::get_right_function_expression() const {
    return m_right_function_expression;
}


/* GroundFunctionExpressionMultiOperator */
GroundFunctionExpressionMultiOperatorImpl::GroundFunctionExpressionMultiOperatorImpl(int identifier,
    MultiOperatorEnum multi_operator,
    GroundFunctionExpressionList function_expressions)
    : Base(identifier), m_multi_operator(multi_operator), m_function_expressions(function_expressions) { }

bool GroundFunctionExpressionMultiOperatorImpl::is_structurally_equivalent_to_impl(const GroundFunctionExpressionMultiOperatorImpl& other) const {
    if (this != &other) {
        return (m_multi_operator == other.m_multi_operator)
            && (get_sorted_vector(m_function_expressions) == get_sorted_vector(other.m_function_expressions));
    }
    return true;
}

size_t GroundFunctionExpressionMultiOperatorImpl::hash_impl() const {
    return hash_combine(m_multi_operator, hash_container(get_sorted_vector(m_function_expressions)));
}

void GroundFunctionExpressionMultiOperatorImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << "(" << to_string(m_multi_operator);
    assert(!m_function_expressions.empty());
    for (const auto& function_expression : m_function_expressions) {
        out << " ";
        std::visit(StringifyVisitor(out, options), *function_expression);
    }
    out << ")";
}

MultiOperatorEnum GroundFunctionExpressionMultiOperatorImpl::get_multi_operator() const{
    return m_multi_operator;
}

const GroundFunctionExpressionList& GroundFunctionExpressionMultiOperatorImpl::get_function_expressions() const {
    return m_function_expressions;
}


/* GroundFunctionExpressionMinus */
GroundFunctionExpressionMinusImpl::GroundFunctionExpressionMinusImpl(int identifier, GroundFunctionExpression function_expression)
    : Base(identifier), m_function_expression(std::move(function_expression)) { }

bool GroundFunctionExpressionMinusImpl::is_structurally_equivalent_to_impl(const GroundFunctionExpressionMinusImpl& other) const {
    if (this != &other) {
        return m_function_expression == other.m_function_expression;
    }
    return true;
}

size_t GroundFunctionExpressionMinusImpl::hash_impl() const {
    return hash_combine(m_function_expression);
}

void GroundFunctionExpressionMinusImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << "(- " << m_function_expression << ")";
}

const GroundFunctionExpression& GroundFunctionExpressionMinusImpl::get_function_expression() const {
    return m_function_expression;
}


/* GroundFunctionExpressionFunction */
GroundFunctionExpressionFunctionImpl::GroundFunctionExpressionFunctionImpl(int identifier, GroundFunction function)
    : Base(identifier), m_function(std::move(function)) { }

bool GroundFunctionExpressionFunctionImpl::is_structurally_equivalent_to_impl(const GroundFunctionExpressionFunctionImpl& other) const {
    if (this != &other) {
        return m_function == other.m_function;
    }
    return true;
}

size_t GroundFunctionExpressionFunctionImpl::hash_impl() const {
    return hash_combine(m_function);
}

void GroundFunctionExpressionFunctionImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << *m_function;
}

const GroundFunction& GroundFunctionExpressionFunctionImpl::get_function() const {
    return m_function;
}

}


namespace std {
    bool less<loki::pddl::GroundFunctionExpression>::operator()(
        const loki::pddl::GroundFunctionExpression& left_function_expression,
        const loki::pddl::GroundFunctionExpression& right_function_expression) const {
        return *left_function_expression < *right_function_expression;
    }

    std::size_t hash<loki::pddl::GroundFunctionExpressionNumberImpl>::operator()(const loki::pddl::GroundFunctionExpressionNumberImpl& function_expression) const {
        return function_expression.hash();
    }

    std::size_t hash<loki::pddl::GroundFunctionExpressionBinaryOperatorImpl>::operator()(const loki::pddl::GroundFunctionExpressionBinaryOperatorImpl& function_expression) const {
        return function_expression.hash();
    }

    std::size_t hash<loki::pddl::GroundFunctionExpressionMultiOperatorImpl>::operator()(const loki::pddl::GroundFunctionExpressionMultiOperatorImpl& function_expression) const {
        return function_expression.hash();
    }

    std::size_t hash<loki::pddl::GroundFunctionExpressionMinusImpl>::operator()(const loki::pddl::GroundFunctionExpressionMinusImpl& function_expression) const {
        return function_expression.hash();
    }

    std::size_t hash<loki::pddl::GroundFunctionExpressionFunctionImpl>::operator()(const loki::pddl::GroundFunctionExpressionFunctionImpl& function_expression) const {
        return function_expression.hash();
    }
}
