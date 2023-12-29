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

#include "../../../include/domain/pddl/function_expressions.hpp"

#include "../../../include/common/hash.hpp"
#include "../../../include/common/collections.hpp"
#include "../../../include/domain/pddl/function.hpp"
#include "../../../include/common/pddl/visitors.hpp"

#include <cassert>


namespace loki::pddl {

std::unordered_map<BinaryOperatorEnum, std::string> binary_operator_enum_to_string = {
    { BinaryOperatorEnum::MUL, "*" },
    { BinaryOperatorEnum::PLUS, "+" },
    { BinaryOperatorEnum::MINUS, "-" },
    { BinaryOperatorEnum::DIV, "/" },
};

const std::string& to_string(pddl::BinaryOperatorEnum binary_operator) {
    assert(binary_operator_enum_to_string.count(binary_operator));
    return binary_operator_enum_to_string.at(binary_operator);
}


std::unordered_map<MultiOperatorEnum, std::string> multi_operator_enum_to_string = {
    { MultiOperatorEnum::MUL, "*" },
    { MultiOperatorEnum::PLUS, "+" },
};

const std::string& to_string(pddl::MultiOperatorEnum multi_operator) {
    assert(multi_operator_enum_to_string.count(multi_operator));
    return multi_operator_enum_to_string.at(multi_operator);
}


/* FunctionExpressionNumber */
FunctionExpressionNumberImpl::FunctionExpressionNumberImpl(int identifier, double number)
    : Base(identifier), m_number(number) { }

bool FunctionExpressionNumberImpl::are_equal_impl(const FunctionExpressionNumberImpl& other) const {
    if (this != &other) {
        return m_number == other.m_number;
    }
    return true;
}

size_t FunctionExpressionNumberImpl::hash_impl() const {
    return std::hash<double>()(m_number);
}

void FunctionExpressionNumberImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << m_number;
}

double FunctionExpressionNumberImpl::get_number() const {
    return m_number;
}


/* FunctionExpressionBinaryOperator */
FunctionExpressionBinaryOperatorImpl::FunctionExpressionBinaryOperatorImpl(int identifier,
    BinaryOperatorEnum binary_operator,
    FunctionExpression left_function_expression,
    FunctionExpression right_function_expression)
    : Base(identifier)
    , m_binary_operator(binary_operator)
    , m_left_function_expression(std::move(left_function_expression))
    , m_right_function_expression(std::move(right_function_expression)) { }

bool FunctionExpressionBinaryOperatorImpl::are_equal_impl(const FunctionExpressionBinaryOperatorImpl& other) const {
    if (this != &other) {
        return (m_binary_operator == other.m_binary_operator)
            && (m_left_function_expression == other.m_left_function_expression)
            && (m_right_function_expression == other.m_right_function_expression);
    }
    return true;
}

size_t FunctionExpressionBinaryOperatorImpl::hash_impl() const {
    return hash_combine(m_binary_operator, m_left_function_expression, m_right_function_expression);
}

void FunctionExpressionBinaryOperatorImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << "(" << to_string(m_binary_operator) << " ";
    std::visit(StringifyVisitor(out, options), *m_left_function_expression);
    out << " ";
    std::visit(StringifyVisitor(out, options), *m_right_function_expression);
    out << ")";
}

BinaryOperatorEnum FunctionExpressionBinaryOperatorImpl::get_binary_operator() const {
    return m_binary_operator;
}

const FunctionExpression& FunctionExpressionBinaryOperatorImpl::get_left_function_expression() const {
    return m_left_function_expression;
}

const FunctionExpression& FunctionExpressionBinaryOperatorImpl::get_right_function_expression() const {
    return m_right_function_expression;
}


/* FunctionExpressionMultiOperator */
FunctionExpressionMultiOperatorImpl::FunctionExpressionMultiOperatorImpl(int identifier,
    MultiOperatorEnum multi_operator,
    FunctionExpressionList function_expressions)
    : Base(identifier), m_multi_operator(multi_operator), m_function_expressions(function_expressions) { }

bool FunctionExpressionMultiOperatorImpl::are_equal_impl(const FunctionExpressionMultiOperatorImpl& other) const {
    if (this != &other) {
        return (m_multi_operator == other.m_multi_operator)
            && (get_sorted_vector(m_function_expressions) == get_sorted_vector(other.m_function_expressions));
    }
    return true;
}

size_t FunctionExpressionMultiOperatorImpl::hash_impl() const {
    return hash_combine(m_multi_operator, hash_vector(get_sorted_vector(m_function_expressions)));
}

void FunctionExpressionMultiOperatorImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << "(" << to_string(m_multi_operator);
    assert(!m_function_expressions.empty());
    for (const auto& function_expression : m_function_expressions) {
        out << " ";
        std::visit(StringifyVisitor(out, options), *function_expression);
    }
    out << ")";
}

MultiOperatorEnum FunctionExpressionMultiOperatorImpl::get_multi_operator() const{
    return m_multi_operator;
}

const FunctionExpressionList& FunctionExpressionMultiOperatorImpl::get_function_expressions() const {
    return m_function_expressions;
}


/* FunctionExpressionMinus */
FunctionExpressionMinusImpl::FunctionExpressionMinusImpl(int identifier, FunctionExpression function_expression)
    : Base(identifier), m_function_expression(std::move(function_expression)) { }

bool FunctionExpressionMinusImpl::are_equal_impl(const FunctionExpressionMinusImpl& other) const {
    if (this != &other) {
        return m_function_expression == other.m_function_expression;
    }
    return true;
}

size_t FunctionExpressionMinusImpl::hash_impl() const {
    return hash_combine(m_function_expression);
}

void FunctionExpressionMinusImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << "(- " << m_function_expression << ")";
}

const FunctionExpression& FunctionExpressionMinusImpl::get_function_expression() const {
    return m_function_expression;
}


/* FunctionExpressionFunction */
FunctionExpressionFunctionImpl::FunctionExpressionFunctionImpl(int identifier, Function function)
    : Base(identifier), m_function(std::move(function)) { }

bool FunctionExpressionFunctionImpl::are_equal_impl(const FunctionExpressionFunctionImpl& other) const {
    if (this != &other) {
        return m_function == other.m_function;
    }
    return true;
}

size_t FunctionExpressionFunctionImpl::hash_impl() const {
    return hash_combine(m_function);
}

void FunctionExpressionFunctionImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << *m_function;
}

const Function& FunctionExpressionFunctionImpl::get_function() const {
    return m_function;
}

}


namespace std {
    bool less<loki::pddl::FunctionExpression>::operator()(
        const loki::pddl::FunctionExpression& left_function_expression,
        const loki::pddl::FunctionExpression& right_function_expression) const {
        return *left_function_expression < *right_function_expression;
    }

    std::size_t hash<loki::pddl::FunctionExpressionNumberImpl>::operator()(const loki::pddl::FunctionExpressionNumberImpl& function_expression) const {
        return function_expression.hash_impl();
    }

    std::size_t hash<loki::pddl::FunctionExpressionBinaryOperatorImpl>::operator()(const loki::pddl::FunctionExpressionBinaryOperatorImpl& function_expression) const {
        return function_expression.hash_impl();
    }

    std::size_t hash<loki::pddl::FunctionExpressionMultiOperatorImpl>::operator()(const loki::pddl::FunctionExpressionMultiOperatorImpl& function_expression) const {
        return function_expression.hash_impl();
    }

    std::size_t hash<loki::pddl::FunctionExpressionMinusImpl>::operator()(const loki::pddl::FunctionExpressionMinusImpl& function_expression) const {
        return function_expression.hash_impl();
    }

    std::size_t hash<loki::pddl::FunctionExpressionFunctionImpl>::operator()(const loki::pddl::FunctionExpressionFunctionImpl& function_expression) const {
        return function_expression.hash_impl();
    }
}
