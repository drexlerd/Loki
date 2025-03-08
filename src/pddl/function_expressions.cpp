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

#include "loki/details/pddl/function_expressions.hpp"

#include "formatter.hpp"
#include "loki/details/pddl/function.hpp"

#include <cassert>

namespace loki
{

std::unordered_map<BinaryComparatorEnum, std::string> binary_comparator_enum_to_string = {
    { BinaryComparatorEnum::GREATER, ">" },         //
    { BinaryComparatorEnum::LESS, "<" },            //
    { BinaryComparatorEnum::EQUAL, "=" },           //
    { BinaryComparatorEnum::GREATER_EQUAL, ">=" },  //
    { BinaryComparatorEnum::LESS_EQUAL, "<=" },
};

const std::string& to_string(BinaryComparatorEnum binary_comparator)
{
    assert(binary_comparator_enum_to_string.count(binary_comparator));
    return binary_comparator_enum_to_string.at(binary_comparator);
}

std::unordered_map<BinaryOperatorEnum, std::string> binary_operator_enum_to_string = {
    { BinaryOperatorEnum::MUL, "*" },
    { BinaryOperatorEnum::PLUS, "+" },
    { BinaryOperatorEnum::MINUS, "-" },
    { BinaryOperatorEnum::DIV, "/" },
};

const std::string& to_string(BinaryOperatorEnum binary_operator)
{
    assert(binary_operator_enum_to_string.count(binary_operator));
    return binary_operator_enum_to_string.at(binary_operator);
}

std::unordered_map<MultiOperatorEnum, std::string> multi_operator_enum_to_string = {
    { MultiOperatorEnum::MUL, "*" },
    { MultiOperatorEnum::PLUS, "+" },
};

const std::string& to_string(MultiOperatorEnum multi_operator)
{
    assert(multi_operator_enum_to_string.count(multi_operator));
    return multi_operator_enum_to_string.at(multi_operator);
}

/* FunctionExpressionNumber */
FunctionExpressionNumberImpl::FunctionExpressionNumberImpl(size_t index, double number) : m_index(index), m_number(number) {}

size_t FunctionExpressionNumberImpl::get_index() const { return m_index; }

double FunctionExpressionNumberImpl::get_number() const { return m_number; }

/* FunctionExpressionBinaryOperator */
FunctionExpressionBinaryOperatorImpl::FunctionExpressionBinaryOperatorImpl(size_t index,
                                                                           BinaryOperatorEnum binary_operator,
                                                                           FunctionExpression left_function_expression,
                                                                           FunctionExpression right_function_expression) :
    m_index(index),
    m_binary_operator(binary_operator),
    m_left_function_expression(std::move(left_function_expression)),
    m_right_function_expression(std::move(right_function_expression))
{
}

size_t FunctionExpressionBinaryOperatorImpl::get_index() const { return m_index; }

BinaryOperatorEnum FunctionExpressionBinaryOperatorImpl::get_binary_operator() const { return m_binary_operator; }

FunctionExpression FunctionExpressionBinaryOperatorImpl::get_left_function_expression() const { return m_left_function_expression; }

FunctionExpression FunctionExpressionBinaryOperatorImpl::get_right_function_expression() const { return m_right_function_expression; }

/* FunctionExpressionMultiOperator */
FunctionExpressionMultiOperatorImpl::FunctionExpressionMultiOperatorImpl(size_t index,
                                                                         MultiOperatorEnum multi_operator,
                                                                         FunctionExpressionList function_expressions) :
    m_index(index),
    m_multi_operator(multi_operator),
    m_function_expressions(function_expressions)
{
}

size_t FunctionExpressionMultiOperatorImpl::get_index() const { return m_index; }

MultiOperatorEnum FunctionExpressionMultiOperatorImpl::get_multi_operator() const { return m_multi_operator; }

const FunctionExpressionList& FunctionExpressionMultiOperatorImpl::get_function_expressions() const { return m_function_expressions; }

/* FunctionExpressionMinus */
FunctionExpressionMinusImpl::FunctionExpressionMinusImpl(size_t index, FunctionExpression function_expression) :
    m_index(index),
    m_function_expression(std::move(function_expression))
{
}

size_t FunctionExpressionMinusImpl::get_index() const { return m_index; }

FunctionExpression FunctionExpressionMinusImpl::get_function_expression() const { return m_function_expression; }

/* FunctionExpressionFunction */
FunctionExpressionFunctionImpl::FunctionExpressionFunctionImpl(size_t index, Function function) : m_index(index), m_function(std::move(function)) {}

size_t FunctionExpressionFunctionImpl::get_index() const { return m_index; }

Function FunctionExpressionFunctionImpl::get_function() const { return m_function; }

/* FunctionExpression */
FunctionExpressionImpl::FunctionExpressionImpl(size_t index,
                                               std::variant<FunctionExpressionNumber,
                                                            FunctionExpressionBinaryOperator,
                                                            FunctionExpressionMultiOperator,
                                                            FunctionExpressionMinus,
                                                            FunctionExpressionFunction> function_expression) :
    m_index(index),
    m_function_expression(function_expression)
{
}

size_t FunctionExpressionImpl::get_index() const { return m_index; }

const std::
    variant<FunctionExpressionNumber, FunctionExpressionBinaryOperator, FunctionExpressionMultiOperator, FunctionExpressionMinus, FunctionExpressionFunction>&
    FunctionExpressionImpl::get_function_expression() const
{
    return m_function_expression;
}

std::ostream& operator<<(std::ostream& out, const FunctionExpressionNumberImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const FunctionExpressionBinaryOperatorImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const FunctionExpressionMultiOperatorImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const FunctionExpressionMinusImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const FunctionExpressionFunctionImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const FunctionExpressionImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, FunctionExpressionNumber element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, FunctionExpressionBinaryOperator element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, FunctionExpressionMultiOperator element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, FunctionExpressionMinus element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, FunctionExpressionFunction element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, FunctionExpression element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

}
