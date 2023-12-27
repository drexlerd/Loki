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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_FUNCTION_EXPRESSIONS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_FUNCTION_EXPRESSIONS_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"

#include <string>


namespace loki {
template<typename HolderType>
class PersistentFactory;
}


namespace loki::pddl {
enum class BinaryOperatorEnum {
    MUL,
    PLUS,
    MINUS,
    DIV,
};

extern std::unordered_map<BinaryOperatorEnum, std::string> binary_operator_enum_to_string;
extern const std::string& to_string(pddl::BinaryOperatorEnum binary_operator);

enum class MultiOperatorEnum {
    MUL,
    PLUS,
};

extern std::unordered_map<MultiOperatorEnum, std::string> multi_operator_enum_to_string;
extern const std::string& to_string(pddl::MultiOperatorEnum multi_operator);


/* FunctionExpressionNumber */
class FunctionExpressionNumberImpl : public Base<FunctionExpressionNumberImpl> {
private:
    double m_number;

    FunctionExpressionNumberImpl(int identifier, double number);

    template<typename HolderType>
    friend class loki::PersistentFactory;

public:
    bool are_equal_impl(const FunctionExpressionNumberImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    double get_number() const;
};


/* FunctionExpressionBinaryOperator */
class FunctionExpressionBinaryOperatorImpl : public Base<FunctionExpressionBinaryOperatorImpl> {
private:
    BinaryOperatorEnum m_binary_operator;
    FunctionExpression m_left_function_expression;
    FunctionExpression m_right_function_expression;

    FunctionExpressionBinaryOperatorImpl(int identifier,
        BinaryOperatorEnum binary_operator,
        FunctionExpression left_function_expression,
        FunctionExpression right_function_expression);

    template<typename HolderType>
    friend class loki::PersistentFactory;

public:
    bool are_equal_impl(const FunctionExpressionBinaryOperatorImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    BinaryOperatorEnum get_binary_operator() const;
    const FunctionExpression& get_left_function_expression() const;
    const FunctionExpression& get_right_function_expression() const;
};


/* FunctionExpressionMultiOperator */
class FunctionExpressionMultiOperatorImpl : public Base<FunctionExpressionMultiOperatorImpl> {
private:
    MultiOperatorEnum m_multi_operator;
    FunctionExpressionList m_function_expressions;

    FunctionExpressionMultiOperatorImpl(int identifier,
        MultiOperatorEnum multi_operator,
        FunctionExpressionList function_expressions);

    template<typename HolderType>
    friend class loki::PersistentFactory;

public:
    bool are_equal_impl(const FunctionExpressionMultiOperatorImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    MultiOperatorEnum get_multi_operator() const;
    const FunctionExpressionList& get_function_expressions() const;
};


/* FunctionExpressionMinus */
class FunctionExpressionMinusImpl : public Base<FunctionExpressionMinusImpl> {
private:
    FunctionExpression m_function_expression;

    FunctionExpressionMinusImpl(int identifier, FunctionExpression function_expression);

    template<typename HolderType>
    friend class loki::PersistentFactory;

public:
    bool are_equal_impl(const FunctionExpressionMinusImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    const FunctionExpression& get_function_expression() const;
};


/* FunctionExpressionFunction */
class FunctionExpressionFunctionImpl : public Base<FunctionExpressionFunctionImpl> {
private:
    Function m_function;

    FunctionExpressionFunctionImpl(int identifier, Function function);

    template<typename HolderType>
    friend class loki::PersistentFactory;

public:
    bool are_equal_impl(const FunctionExpressionFunctionImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    const Function& get_function() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::FunctionExpression>
    {
        bool operator()(const loki::pddl::FunctionExpression& left_function_expression, const loki::pddl::FunctionExpression& right_function_expression) const;
    };

    template<>
    struct hash<loki::pddl::FunctionExpressionNumberImpl>
    {
        std::size_t operator()(const loki::pddl::FunctionExpressionNumberImpl& function_expressions) const;
    };

    template<>
    struct hash<loki::pddl::FunctionExpressionBinaryOperatorImpl>
    {
        std::size_t operator()(const loki::pddl::FunctionExpressionBinaryOperatorImpl& function_expressions) const;
    };

    template<>
    struct hash<loki::pddl::FunctionExpressionMultiOperatorImpl>
    {
        std::size_t operator()(const loki::pddl::FunctionExpressionMultiOperatorImpl& function_expressions) const;
    };

    template<>
    struct hash<loki::pddl::FunctionExpressionMinusImpl>
    {
        std::size_t operator()(const loki::pddl::FunctionExpressionMinusImpl& function_expressions) const;
    };

    template<>
    struct hash<loki::pddl::FunctionExpressionFunctionImpl>
    {
        std::size_t operator()(const loki::pddl::FunctionExpressionFunctionImpl& function_expressions) const;
    };
}

#endif
