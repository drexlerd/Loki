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

#ifndef LOKI_INCLUDE_LOKI_PDDL_FUNCTION_EXPRESSIONS_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_FUNCTION_EXPRESSIONS_HPP_

#include "loki/details/pddl/base.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/utils/unique_value_type_factory.hpp"

#include <string>
#include <unordered_map>

namespace loki
{
enum class BinaryOperatorEnum
{
    MUL,
    PLUS,
    MINUS,
    DIV,
};

extern std::unordered_map<BinaryOperatorEnum, std::string> binary_operator_enum_to_string;
extern const std::string& to_string(BinaryOperatorEnum binary_operator);

enum class MultiOperatorEnum
{
    MUL,
    PLUS,
};

extern std::unordered_map<MultiOperatorEnum, std::string> multi_operator_enum_to_string;
extern const std::string& to_string(MultiOperatorEnum multi_operator);

/* FunctionExpressionNumber */
class FunctionExpressionNumberImpl : public Base<FunctionExpressionNumberImpl>
{
private:
    double m_number;

    FunctionExpressionNumberImpl(size_t index, double number);

    // Give access to the constructor.
    friend class UniqueValueTypeFactory<FunctionExpressionImpl>;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<FunctionExpressionNumberImpl>;

public:
    double get_number() const;
};

template<>
struct ShallowHash<const FunctionExpressionNumberImpl&>
{
    size_t operator()(const FunctionExpressionNumberImpl& e) const;
};

template<>
struct ShallowEqualTo<const FunctionExpressionNumberImpl&>
{
    bool operator()(const FunctionExpressionNumberImpl& l, const FunctionExpressionNumberImpl& r) const;
};

/* FunctionExpressionBinaryOperator */
class FunctionExpressionBinaryOperatorImpl : public Base<FunctionExpressionBinaryOperatorImpl>
{
private:
    BinaryOperatorEnum m_binary_operator;
    FunctionExpression m_left_function_expression;
    FunctionExpression m_right_function_expression;

    FunctionExpressionBinaryOperatorImpl(size_t index,
                                         BinaryOperatorEnum binary_operator,
                                         FunctionExpression left_function_expression,
                                         FunctionExpression right_function_expression);

    // Give access to the constructor.
    friend class UniqueValueTypeFactory<FunctionExpressionImpl>;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<FunctionExpressionBinaryOperatorImpl>;

public:
    BinaryOperatorEnum get_binary_operator() const;
    const FunctionExpression& get_left_function_expression() const;
    const FunctionExpression& get_right_function_expression() const;
};

template<>
struct ShallowHash<const FunctionExpressionBinaryOperatorImpl&>
{
    size_t operator()(const FunctionExpressionBinaryOperatorImpl& e) const;
};

template<>
struct ShallowEqualTo<const FunctionExpressionBinaryOperatorImpl&>
{
    bool operator()(const FunctionExpressionBinaryOperatorImpl& l, const FunctionExpressionBinaryOperatorImpl& r) const;
};

/* FunctionExpressionMultiOperator */
class FunctionExpressionMultiOperatorImpl : public Base<FunctionExpressionMultiOperatorImpl>
{
private:
    MultiOperatorEnum m_multi_operator;
    FunctionExpressionList m_function_expressions;

    FunctionExpressionMultiOperatorImpl(size_t index, MultiOperatorEnum multi_operator, FunctionExpressionList function_expressions);

    // Give access to the constructor.
    friend class UniqueValueTypeFactory<FunctionExpressionImpl>;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<FunctionExpressionMultiOperatorImpl>;

public:
    MultiOperatorEnum get_multi_operator() const;
    const FunctionExpressionList& get_function_expressions() const;
};

template<>
struct ShallowHash<const FunctionExpressionMultiOperatorImpl&>
{
    size_t operator()(const FunctionExpressionMultiOperatorImpl& e) const;
};

template<>
struct ShallowEqualTo<const FunctionExpressionMultiOperatorImpl&>
{
    bool operator()(const FunctionExpressionMultiOperatorImpl& l, const FunctionExpressionMultiOperatorImpl& r) const;
};

/* FunctionExpressionMinus */
class FunctionExpressionMinusImpl : public Base<FunctionExpressionMinusImpl>
{
private:
    FunctionExpression m_function_expression;

    FunctionExpressionMinusImpl(size_t index, FunctionExpression function_expression);

    // Give access to the constructor.
    friend class UniqueValueTypeFactory<FunctionExpressionImpl>;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<FunctionExpressionMinusImpl>;

public:
    const FunctionExpression& get_function_expression() const;
};

template<>
struct ShallowHash<const FunctionExpressionMinusImpl&>
{
    size_t operator()(const FunctionExpressionMinusImpl& e) const;
};

template<>
struct ShallowEqualTo<const FunctionExpressionMinusImpl&>
{
    bool operator()(const FunctionExpressionMinusImpl& l, const FunctionExpressionMinusImpl& r) const;
};

/* FunctionExpressionFunction */
class FunctionExpressionFunctionImpl : public Base<FunctionExpressionFunctionImpl>
{
private:
    Function m_function;

    FunctionExpressionFunctionImpl(size_t index, Function function);

    // Give access to the constructor.
    friend class UniqueValueTypeFactory<FunctionExpressionImpl>;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<FunctionExpressionFunctionImpl>;

public:
    const Function& get_function() const;
};

template<>
struct ShallowHash<const FunctionExpressionFunctionImpl&>
{
    size_t operator()(const FunctionExpressionFunctionImpl& e) const;
};

template<>
struct ShallowEqualTo<const FunctionExpressionFunctionImpl&>
{
    bool operator()(const FunctionExpressionFunctionImpl& l, const FunctionExpressionFunctionImpl& r) const;
};

/* FunctionExpressionImpl */

template<>
struct ShallowHash<const FunctionExpressionImpl*>
{
    size_t operator()(const FunctionExpressionImpl* e) const;
};

template<>
struct ShallowEqualTo<const FunctionExpressionImpl*>
{
    bool operator()(const FunctionExpressionImpl* l, const FunctionExpressionImpl* r) const;
};

}

#endif
