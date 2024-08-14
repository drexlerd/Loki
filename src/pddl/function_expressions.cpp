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
#include "loki/details/utils/collections.hpp"

#include <cassert>

namespace loki
{

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

const FunctionExpression& FunctionExpressionBinaryOperatorImpl::get_left_function_expression() const { return m_left_function_expression; }

const FunctionExpression& FunctionExpressionBinaryOperatorImpl::get_right_function_expression() const { return m_right_function_expression; }

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

const FunctionExpression& FunctionExpressionMinusImpl::get_function_expression() const { return m_function_expression; }

/* FunctionExpressionFunction */
FunctionExpressionFunctionImpl::FunctionExpressionFunctionImpl(size_t index, Function function) : m_index(index), m_function(std::move(function)) {}

size_t FunctionExpressionFunctionImpl::get_index() const { return m_index; }

const Function& FunctionExpressionFunctionImpl::get_function() const { return m_function; }

size_t UniquePDDLHasher<const FunctionExpressionNumberImpl&>::operator()(const FunctionExpressionNumberImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_number());
}

size_t UniquePDDLHasher<const FunctionExpressionBinaryOperatorImpl&>::operator()(const FunctionExpressionBinaryOperatorImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_binary_operator(), e.get_left_function_expression(), e.get_right_function_expression());
}

size_t UniquePDDLHasher<const FunctionExpressionMultiOperatorImpl&>::operator()(const FunctionExpressionMultiOperatorImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_multi_operator(), get_sorted_vector(e.get_function_expressions()));
}

size_t UniquePDDLHasher<const FunctionExpressionMinusImpl&>::operator()(const FunctionExpressionMinusImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_function_expression());
}

size_t UniquePDDLHasher<const FunctionExpressionFunctionImpl&>::operator()(const FunctionExpressionFunctionImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_function());
}

size_t UniquePDDLHasher<const FunctionExpressionImpl*>::operator()(const FunctionExpressionImpl* e) const
{
    return std::visit([](const auto& arg) { return UniquePDDLHasher<decltype(arg)>()(arg); }, *e);
}

bool UniquePDDLEqualTo<const FunctionExpressionNumberImpl&>::operator()(const FunctionExpressionNumberImpl& l, const FunctionExpressionNumberImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_number() == r.get_number());
    }
    return true;
}

bool UniquePDDLEqualTo<const FunctionExpressionBinaryOperatorImpl&>::operator()(const FunctionExpressionBinaryOperatorImpl& l,
                                                                                const FunctionExpressionBinaryOperatorImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_binary_operator() == r.get_binary_operator()) && (l.get_left_function_expression() == r.get_left_function_expression())
               && (l.get_right_function_expression() == r.get_right_function_expression());
    }
    return true;
}

bool UniquePDDLEqualTo<const FunctionExpressionMultiOperatorImpl&>::operator()(const FunctionExpressionMultiOperatorImpl& l,
                                                                               const FunctionExpressionMultiOperatorImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_multi_operator() == r.get_multi_operator())
               && (get_sorted_vector(l.get_function_expressions()) == get_sorted_vector(r.get_function_expressions()));
    }
    return true;
}

bool UniquePDDLEqualTo<const FunctionExpressionMinusImpl&>::operator()(const FunctionExpressionMinusImpl& l, const FunctionExpressionMinusImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_function_expression() == r.get_function_expression());
    }
    return true;
}

bool UniquePDDLEqualTo<const FunctionExpressionFunctionImpl&>::operator()(const FunctionExpressionFunctionImpl& l,
                                                                          const FunctionExpressionFunctionImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_function() == r.get_function());
    }
    return true;
}

bool UniquePDDLEqualTo<const FunctionExpressionImpl*>::operator()(const FunctionExpressionImpl* l, const FunctionExpressionImpl* r) const
{
    return UniquePDDLEqualTo<FunctionExpressionImpl>()(*l, *r);
}

std::ostream& operator<<(std::ostream& out, const FunctionExpressionNumberImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const FunctionExpressionBinaryOperatorImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const FunctionExpressionMultiOperatorImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const FunctionExpressionMinusImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const FunctionExpressionFunctionImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const FunctionExpressionImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

}
