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
#include "loki/details/utils/value_type_factory.hpp"

#include <string>

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
    friend class UniqueValueTypeFactory<FunctionExpressionImpl, Hash<const FunctionExpressionImpl*, true>, EqualTo<const FunctionExpressionImpl*, true>>;

    bool is_structurally_equivalent_to_impl(const FunctionExpressionNumberImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<FunctionExpressionNumberImpl>;

public:
    double get_number() const;
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
    friend class UniqueValueTypeFactory<FunctionExpressionImpl, Hash<const FunctionExpressionImpl*, true>, EqualTo<const FunctionExpressionImpl*, true>>;

    bool is_structurally_equivalent_to_impl(const FunctionExpressionBinaryOperatorImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<FunctionExpressionBinaryOperatorImpl>;

public:
    BinaryOperatorEnum get_binary_operator() const;
    const FunctionExpression& get_left_function_expression() const;
    const FunctionExpression& get_right_function_expression() const;
};

/* FunctionExpressionMultiOperator */
class FunctionExpressionMultiOperatorImpl : public Base<FunctionExpressionMultiOperatorImpl>
{
private:
    MultiOperatorEnum m_multi_operator;
    FunctionExpressionList m_function_expressions;

    FunctionExpressionMultiOperatorImpl(size_t index, MultiOperatorEnum multi_operator, FunctionExpressionList function_expressions);

    // Give access to the constructor.
    friend class UniqueValueTypeFactory<FunctionExpressionImpl, Hash<const FunctionExpressionImpl*, true>, EqualTo<const FunctionExpressionImpl*, true>>;

    bool is_structurally_equivalent_to_impl(const FunctionExpressionMultiOperatorImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<FunctionExpressionMultiOperatorImpl>;

public:
    MultiOperatorEnum get_multi_operator() const;
    const FunctionExpressionList& get_function_expressions() const;
};

/* FunctionExpressionMinus */
class FunctionExpressionMinusImpl : public Base<FunctionExpressionMinusImpl>
{
private:
    FunctionExpression m_function_expression;

    FunctionExpressionMinusImpl(size_t index, FunctionExpression function_expression);

    // Give access to the constructor.
    friend class UniqueValueTypeFactory<FunctionExpressionImpl, Hash<const FunctionExpressionImpl*, true>, EqualTo<const FunctionExpressionImpl*, true>>;

    bool is_structurally_equivalent_to_impl(const FunctionExpressionMinusImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<FunctionExpressionMinusImpl>;

public:
    const FunctionExpression& get_function_expression() const;
};

/* FunctionExpressionFunction */
class FunctionExpressionFunctionImpl : public Base<FunctionExpressionFunctionImpl>
{
private:
    Function m_function;

    FunctionExpressionFunctionImpl(size_t index, Function function);

    // Give access to the constructor.
    friend class UniqueValueTypeFactory<FunctionExpressionImpl, Hash<const FunctionExpressionImpl*, true>, EqualTo<const FunctionExpressionImpl*, true>>;

    bool is_structurally_equivalent_to_impl(const FunctionExpressionFunctionImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<FunctionExpressionFunctionImpl>;

public:
    const Function& get_function() const;
};

}

#endif
