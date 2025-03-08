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

#include "loki/details/pddl/declarations.hpp"

#include <string>
#include <unordered_map>

namespace loki
{
enum class BinaryComparatorEnum
{
    GREATER,        ///< ">"
    LESS,           ///< "<"
    EQUAL,          ///< "="
    GREATER_EQUAL,  ///< ">="
    LESS_EQUAL,     ///< "<="
};

extern std::unordered_map<BinaryComparatorEnum, std::string> binary_comparator_enum_to_string;
extern const std::string& to_string(BinaryComparatorEnum binary_comparator);

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
class FunctionExpressionNumberImpl
{
private:
    size_t m_index;
    double m_number;

    FunctionExpressionNumberImpl(size_t index, double number);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    FunctionExpressionNumberImpl(const FunctionExpressionNumberImpl& other) = delete;
    FunctionExpressionNumberImpl& operator=(const FunctionExpressionNumberImpl& other) = delete;
    FunctionExpressionNumberImpl(FunctionExpressionNumberImpl&& other) = default;
    FunctionExpressionNumberImpl& operator=(FunctionExpressionNumberImpl&& other) = default;

    size_t get_index() const;
    double get_number() const;

    auto identifying_members() const { return std::tuple(get_number()); }
};

/* FunctionExpressionBinaryOperator */
class FunctionExpressionBinaryOperatorImpl
{
private:
    size_t m_index;
    BinaryOperatorEnum m_binary_operator;
    FunctionExpression m_left_function_expression;
    FunctionExpression m_right_function_expression;

    FunctionExpressionBinaryOperatorImpl(size_t index,
                                         BinaryOperatorEnum binary_operator,
                                         FunctionExpression left_function_expression,
                                         FunctionExpression right_function_expression);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    FunctionExpressionBinaryOperatorImpl(const FunctionExpressionBinaryOperatorImpl& other) = delete;
    FunctionExpressionBinaryOperatorImpl& operator=(const FunctionExpressionBinaryOperatorImpl& other) = delete;
    FunctionExpressionBinaryOperatorImpl(FunctionExpressionBinaryOperatorImpl&& other) = default;
    FunctionExpressionBinaryOperatorImpl& operator=(FunctionExpressionBinaryOperatorImpl&& other) = default;

    size_t get_index() const;
    BinaryOperatorEnum get_binary_operator() const;
    FunctionExpression get_left_function_expression() const;
    FunctionExpression get_right_function_expression() const;

    auto identifying_members() const { return std::tuple(get_binary_operator(), get_left_function_expression(), get_right_function_expression()); }
};

/* FunctionExpressionMultiOperator */
class FunctionExpressionMultiOperatorImpl
{
private:
    size_t m_index;
    MultiOperatorEnum m_multi_operator;
    FunctionExpressionList m_function_expressions;

    FunctionExpressionMultiOperatorImpl(size_t index, MultiOperatorEnum multi_operator, FunctionExpressionList function_expressions);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    FunctionExpressionMultiOperatorImpl(const FunctionExpressionMultiOperatorImpl& other) = delete;
    FunctionExpressionMultiOperatorImpl& operator=(const FunctionExpressionMultiOperatorImpl& other) = delete;
    FunctionExpressionMultiOperatorImpl(FunctionExpressionMultiOperatorImpl&& other) = default;
    FunctionExpressionMultiOperatorImpl& operator=(FunctionExpressionMultiOperatorImpl&& other) = default;

    size_t get_index() const;
    MultiOperatorEnum get_multi_operator() const;
    const FunctionExpressionList& get_function_expressions() const;

    auto identifying_members() const { return std::tuple(get_multi_operator(), std::cref(get_function_expressions())); }
};

/* FunctionExpressionMinus */
class FunctionExpressionMinusImpl
{
private:
    size_t m_index;
    FunctionExpression m_function_expression;

    FunctionExpressionMinusImpl(size_t index, FunctionExpression function_expression);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    FunctionExpressionMinusImpl(const FunctionExpressionMinusImpl& other) = delete;
    FunctionExpressionMinusImpl& operator=(const FunctionExpressionMinusImpl& other) = delete;
    FunctionExpressionMinusImpl(FunctionExpressionMinusImpl&& other) = default;
    FunctionExpressionMinusImpl& operator=(FunctionExpressionMinusImpl&& other) = default;

    size_t get_index() const;
    FunctionExpression get_function_expression() const;

    auto identifying_members() const { return std::tuple(get_function_expression()); }
};

/* FunctionExpressionFunction */
class FunctionExpressionFunctionImpl
{
private:
    size_t m_index;
    Function m_function;

    FunctionExpressionFunctionImpl(size_t index, Function function);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    FunctionExpressionFunctionImpl(const FunctionExpressionFunctionImpl& other) = delete;
    FunctionExpressionFunctionImpl& operator=(const FunctionExpressionFunctionImpl& other) = delete;
    FunctionExpressionFunctionImpl(FunctionExpressionFunctionImpl&& other) = default;
    FunctionExpressionFunctionImpl& operator=(FunctionExpressionFunctionImpl&& other) = default;

    size_t get_index() const;
    Function get_function() const;

    auto identifying_members() const { return std::tuple(get_function()); }
};

/* FunctionExpression */
class FunctionExpressionImpl
{
private:
    size_t m_index;
    std::variant<FunctionExpressionNumber,
                 FunctionExpressionBinaryOperator,
                 FunctionExpressionMultiOperator,
                 FunctionExpressionMinus,
                 FunctionExpressionFunction>
        m_function_expression;

    FunctionExpressionImpl(size_t index,
                           std::variant<FunctionExpressionNumber,
                                        FunctionExpressionBinaryOperator,
                                        FunctionExpressionMultiOperator,
                                        FunctionExpressionMinus,
                                        FunctionExpressionFunction> function_expression);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    FunctionExpressionImpl(const FunctionExpressionImpl& other) = delete;
    FunctionExpressionImpl& operator=(const FunctionExpressionImpl& other) = delete;
    FunctionExpressionImpl(FunctionExpressionImpl&& other) = default;
    FunctionExpressionImpl& operator=(FunctionExpressionImpl&& other) = default;

    size_t get_index() const;
    const std::variant<FunctionExpressionNumber,
                       FunctionExpressionBinaryOperator,
                       FunctionExpressionMultiOperator,
                       FunctionExpressionMinus,
                       FunctionExpressionFunction>&
    get_function_expression() const;

    auto identifying_members() const { return std::tuple(get_function_expression()); }
};

extern std::ostream& operator<<(std::ostream& out, const FunctionExpressionNumberImpl& element);
extern std::ostream& operator<<(std::ostream& out, const FunctionExpressionBinaryOperatorImpl& element);
extern std::ostream& operator<<(std::ostream& out, const FunctionExpressionMultiOperatorImpl& element);
extern std::ostream& operator<<(std::ostream& out, const FunctionExpressionMinusImpl& element);
extern std::ostream& operator<<(std::ostream& out, const FunctionExpressionFunctionImpl& element);
extern std::ostream& operator<<(std::ostream& out, const FunctionExpressionImpl& element);

extern std::ostream& operator<<(std::ostream& out, FunctionExpressionNumber element);
extern std::ostream& operator<<(std::ostream& out, FunctionExpressionBinaryOperator element);
extern std::ostream& operator<<(std::ostream& out, FunctionExpressionMultiOperator element);
extern std::ostream& operator<<(std::ostream& out, FunctionExpressionMinus element);
extern std::ostream& operator<<(std::ostream& out, FunctionExpressionFunction element);
extern std::ostream& operator<<(std::ostream& out, FunctionExpression element);

}

#endif
