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

#include "loki/details/pddl/function.hpp"
#include "loki/details/pddl/visitors.hpp"
#include "loki/details/utils/collections.hpp"
#include "loki/details/utils/hash.hpp"

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
FunctionExpressionNumberImpl::FunctionExpressionNumberImpl(size_t index, double number) : Base(index), m_number(number) {}

void FunctionExpressionNumberImpl::str_impl(std::ostream& out, const FormattingOptions& /*options*/) const { out << m_number; }

double FunctionExpressionNumberImpl::get_number() const { return m_number; }

size_t ShallowHash<const FunctionExpressionNumberImpl&>::operator()(const FunctionExpressionNumberImpl& e) const
{
    return ShallowHashCombiner()(e.get_number());
}

bool ShallowEqualTo<const FunctionExpressionNumberImpl&>::operator()(const FunctionExpressionNumberImpl& l, const FunctionExpressionNumberImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_number() == r.get_number());
    }
    return true;
}

/* FunctionExpressionBinaryOperator */
FunctionExpressionBinaryOperatorImpl::FunctionExpressionBinaryOperatorImpl(size_t index,
                                                                           BinaryOperatorEnum binary_operator,
                                                                           FunctionExpression left_function_expression,
                                                                           FunctionExpression right_function_expression) :
    Base(index),
    m_binary_operator(binary_operator),
    m_left_function_expression(std::move(left_function_expression)),
    m_right_function_expression(std::move(right_function_expression))
{
}

void FunctionExpressionBinaryOperatorImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(" << to_string(m_binary_operator) << " ";
    std::visit(StringifyVisitor(out, options), *m_left_function_expression);
    out << " ";
    std::visit(StringifyVisitor(out, options), *m_right_function_expression);
    out << ")";
}

BinaryOperatorEnum FunctionExpressionBinaryOperatorImpl::get_binary_operator() const { return m_binary_operator; }

const FunctionExpression& FunctionExpressionBinaryOperatorImpl::get_left_function_expression() const { return m_left_function_expression; }

const FunctionExpression& FunctionExpressionBinaryOperatorImpl::get_right_function_expression() const { return m_right_function_expression; }

size_t ShallowHash<const FunctionExpressionBinaryOperatorImpl&>::operator()(const FunctionExpressionBinaryOperatorImpl& e) const
{
    return ShallowHashCombiner()(e.get_binary_operator(), e.get_left_function_expression(), e.get_right_function_expression());
}

bool ShallowEqualTo<const FunctionExpressionBinaryOperatorImpl&>::operator()(const FunctionExpressionBinaryOperatorImpl& l,
                                                                             const FunctionExpressionBinaryOperatorImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_binary_operator() == r.get_binary_operator()) && (l.get_left_function_expression() == r.get_left_function_expression())
               && (l.get_right_function_expression() == r.get_right_function_expression());
    }
    return true;
}

/* FunctionExpressionMultiOperator */
FunctionExpressionMultiOperatorImpl::FunctionExpressionMultiOperatorImpl(size_t index,
                                                                         MultiOperatorEnum multi_operator,
                                                                         FunctionExpressionList function_expressions) :
    Base(index),
    m_multi_operator(multi_operator),
    m_function_expressions(function_expressions)
{
}

void FunctionExpressionMultiOperatorImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(" << to_string(m_multi_operator);
    assert(!m_function_expressions.empty());
    for (const auto& function_expression : m_function_expressions)
    {
        out << " ";
        std::visit(StringifyVisitor(out, options), *function_expression);
    }
    out << ")";
}

MultiOperatorEnum FunctionExpressionMultiOperatorImpl::get_multi_operator() const { return m_multi_operator; }

const FunctionExpressionList& FunctionExpressionMultiOperatorImpl::get_function_expressions() const { return m_function_expressions; }

size_t ShallowHash<const FunctionExpressionMultiOperatorImpl&>::operator()(const FunctionExpressionMultiOperatorImpl& e) const
{
    return ShallowHashCombiner()(e.get_multi_operator(), get_sorted_vector(e.get_function_expressions()));
}

bool ShallowEqualTo<const FunctionExpressionMultiOperatorImpl&>::operator()(const FunctionExpressionMultiOperatorImpl& l,
                                                                            const FunctionExpressionMultiOperatorImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_multi_operator() == r.get_multi_operator())
               && (get_sorted_vector(l.get_function_expressions()) == get_sorted_vector(r.get_function_expressions()));
    }
    return true;
}

/* FunctionExpressionMinus */
FunctionExpressionMinusImpl::FunctionExpressionMinusImpl(size_t index, FunctionExpression function_expression) :
    Base(index),
    m_function_expression(std::move(function_expression))
{
}

void FunctionExpressionMinusImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(- ";
    std::visit(StringifyVisitor(out, options), *m_function_expression);
    out << ")";
}

const FunctionExpression& FunctionExpressionMinusImpl::get_function_expression() const { return m_function_expression; }

size_t ShallowHash<const FunctionExpressionMinusImpl&>::operator()(const FunctionExpressionMinusImpl& e) const
{
    return ShallowHashCombiner()(e.get_function_expression());
}

bool ShallowEqualTo<const FunctionExpressionMinusImpl&>::operator()(const FunctionExpressionMinusImpl& l, const FunctionExpressionMinusImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_function_expression() == r.get_function_expression());
    }
    return true;
}

/* FunctionExpressionFunction */
FunctionExpressionFunctionImpl::FunctionExpressionFunctionImpl(size_t index, Function function) : Base(index), m_function(std::move(function)) {}

void FunctionExpressionFunctionImpl::str_impl(std::ostream& out, const FormattingOptions& options) const { m_function->str(out, options); }

const Function& FunctionExpressionFunctionImpl::get_function() const { return m_function; }

size_t ShallowHash<const FunctionExpressionFunctionImpl&>::operator()(const FunctionExpressionFunctionImpl& e) const
{
    return ShallowHashCombiner()(e.get_function());
}

bool ShallowEqualTo<const FunctionExpressionFunctionImpl&>::operator()(const FunctionExpressionFunctionImpl& l, const FunctionExpressionFunctionImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_function() == r.get_function());
    }
    return true;
}

/* FunctionExpressionImpl */

size_t ShallowHash<const FunctionExpressionImpl*>::operator()(const FunctionExpressionImpl* e) const
{
    return std::visit([](const auto& arg) { return ShallowHash<decltype(arg)>()(arg); }, *e);
}

bool ShallowEqualTo<const FunctionExpressionImpl*>::operator()(const FunctionExpressionImpl* l, const FunctionExpressionImpl* r) const
{
    return ShallowEqualTo<FunctionExpressionImpl>()(*l, *r);
}

}
