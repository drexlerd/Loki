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
#include "loki/details/pddl/equal_to.hpp"
#include "loki/details/pddl/hash.hpp"

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
class FunctionExpressionNumberImpl
{
private:
    size_t m_index;
    double m_number;

    FunctionExpressionNumberImpl(size_t index, double number);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

public:
    // moveable but not copyable
    FunctionExpressionNumberImpl(const FunctionExpressionNumberImpl& other) = delete;
    FunctionExpressionNumberImpl& operator=(const FunctionExpressionNumberImpl& other) = delete;
    FunctionExpressionNumberImpl(FunctionExpressionNumberImpl&& other) = default;
    FunctionExpressionNumberImpl& operator=(FunctionExpressionNumberImpl&& other) = default;

    size_t get_index() const;
    double get_number() const;
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
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

public:
    // moveable but not copyable
    FunctionExpressionBinaryOperatorImpl(const FunctionExpressionBinaryOperatorImpl& other) = delete;
    FunctionExpressionBinaryOperatorImpl& operator=(const FunctionExpressionBinaryOperatorImpl& other) = delete;
    FunctionExpressionBinaryOperatorImpl(FunctionExpressionBinaryOperatorImpl&& other) = default;
    FunctionExpressionBinaryOperatorImpl& operator=(FunctionExpressionBinaryOperatorImpl&& other) = default;

    size_t get_index() const;
    BinaryOperatorEnum get_binary_operator() const;
    const FunctionExpression& get_left_function_expression() const;
    const FunctionExpression& get_right_function_expression() const;
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
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

public:
    // moveable but not copyable
    FunctionExpressionMultiOperatorImpl(const FunctionExpressionMultiOperatorImpl& other) = delete;
    FunctionExpressionMultiOperatorImpl& operator=(const FunctionExpressionMultiOperatorImpl& other) = delete;
    FunctionExpressionMultiOperatorImpl(FunctionExpressionMultiOperatorImpl&& other) = default;
    FunctionExpressionMultiOperatorImpl& operator=(FunctionExpressionMultiOperatorImpl&& other) = default;

    size_t get_index() const;
    MultiOperatorEnum get_multi_operator() const;
    const FunctionExpressionList& get_function_expressions() const;
};

/* FunctionExpressionMinus */
class FunctionExpressionMinusImpl
{
private:
    size_t m_index;
    FunctionExpression m_function_expression;

    FunctionExpressionMinusImpl(size_t index, FunctionExpression function_expression);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

public:
    // moveable but not copyable
    FunctionExpressionMinusImpl(const FunctionExpressionMinusImpl& other) = delete;
    FunctionExpressionMinusImpl& operator=(const FunctionExpressionMinusImpl& other) = delete;
    FunctionExpressionMinusImpl(FunctionExpressionMinusImpl&& other) = default;
    FunctionExpressionMinusImpl& operator=(FunctionExpressionMinusImpl&& other) = default;

    size_t get_index() const;
    const FunctionExpression& get_function_expression() const;
};

/* FunctionExpressionFunction */
class FunctionExpressionFunctionImpl
{
private:
    size_t m_index;
    Function m_function;

    FunctionExpressionFunctionImpl(size_t index, Function function);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

public:
    // moveable but not copyable
    FunctionExpressionFunctionImpl(const FunctionExpressionFunctionImpl& other) = delete;
    FunctionExpressionFunctionImpl& operator=(const FunctionExpressionFunctionImpl& other) = delete;
    FunctionExpressionFunctionImpl(FunctionExpressionFunctionImpl&& other) = default;
    FunctionExpressionFunctionImpl& operator=(FunctionExpressionFunctionImpl&& other) = default;

    size_t get_index() const;
    const Function& get_function() const;
};

template<>
struct UniquePDDLHasher<const FunctionExpressionNumberImpl&>
{
    size_t operator()(const FunctionExpressionNumberImpl& e) const;
};

template<>
struct UniquePDDLHasher<const FunctionExpressionBinaryOperatorImpl&>
{
    size_t operator()(const FunctionExpressionBinaryOperatorImpl& e) const;
};

template<>
struct UniquePDDLHasher<const FunctionExpressionMultiOperatorImpl&>
{
    size_t operator()(const FunctionExpressionMultiOperatorImpl& e) const;
};

template<>
struct UniquePDDLHasher<const FunctionExpressionMinusImpl&>
{
    size_t operator()(const FunctionExpressionMinusImpl& e) const;
};

template<>
struct UniquePDDLHasher<const FunctionExpressionFunctionImpl&>
{
    size_t operator()(const FunctionExpressionFunctionImpl& e) const;
};

template<>
struct UniquePDDLHasher<const FunctionExpressionImpl*>
{
    size_t operator()(const FunctionExpressionImpl* e) const;
};

template<>
struct UniquePDDLEqualTo<const FunctionExpressionNumberImpl&>
{
    bool operator()(const FunctionExpressionNumberImpl& l, const FunctionExpressionNumberImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const FunctionExpressionBinaryOperatorImpl&>
{
    bool operator()(const FunctionExpressionBinaryOperatorImpl& l, const FunctionExpressionBinaryOperatorImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const FunctionExpressionMultiOperatorImpl&>
{
    bool operator()(const FunctionExpressionMultiOperatorImpl& l, const FunctionExpressionMultiOperatorImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const FunctionExpressionMinusImpl&>
{
    bool operator()(const FunctionExpressionMinusImpl& l, const FunctionExpressionMinusImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const FunctionExpressionFunctionImpl&>
{
    bool operator()(const FunctionExpressionFunctionImpl& l, const FunctionExpressionFunctionImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const FunctionExpressionImpl*>
{
    bool operator()(const FunctionExpressionImpl* l, const FunctionExpressionImpl* r) const;
};

extern std::ostream& operator<<(std::ostream& out, const FunctionExpressionNumberImpl& element);
extern std::ostream& operator<<(std::ostream& out, const FunctionExpressionBinaryOperatorImpl& element);
extern std::ostream& operator<<(std::ostream& out, const FunctionExpressionMultiOperatorImpl& element);
extern std::ostream& operator<<(std::ostream& out, const FunctionExpressionMinusImpl& element);
extern std::ostream& operator<<(std::ostream& out, const FunctionExpressionFunctionImpl& element);
extern std::ostream& operator<<(std::ostream& out, const FunctionExpressionImpl& element);

}

#endif
