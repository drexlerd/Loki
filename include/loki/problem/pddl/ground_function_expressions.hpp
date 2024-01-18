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

#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_GROUND_FUNCTION_EXPRESSIONS_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_GROUND_FUNCTION_EXPRESSIONS_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"
#include "../../domain/pddl/function_expressions.hpp"

#include <string>


namespace loki {
template<typename HolderType, ElementsPerSegment N>
class PersistentFactory;
}


namespace loki::pddl {
/* GroundFunctionExpressionNumber */
class GroundFunctionExpressionNumberImpl : public Base<GroundFunctionExpressionNumberImpl> {
private:
    double m_number;

    GroundFunctionExpressionNumberImpl(int identifier, double number);

    // Give access to the constructor.
    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

    bool is_structurally_equivalent_to_impl(const GroundFunctionExpressionNumberImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<GroundFunctionExpressionNumberImpl>;

public:
    double get_number() const;
};


/* GroundFunctionExpressionBinaryOperator */
class GroundFunctionExpressionBinaryOperatorImpl : public Base<GroundFunctionExpressionBinaryOperatorImpl> {
private:
    BinaryOperatorEnum m_binary_operator;
    GroundFunctionExpression m_left_function_expression;
    GroundFunctionExpression m_right_function_expression;

    GroundFunctionExpressionBinaryOperatorImpl(int identifier,
        BinaryOperatorEnum binary_operator,
        GroundFunctionExpression left_function_expression,
        GroundFunctionExpression right_function_expression);

    // Give access to the constructor.
    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

    bool is_structurally_equivalent_to_impl(const GroundFunctionExpressionBinaryOperatorImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<GroundFunctionExpressionBinaryOperatorImpl>;

public:
    BinaryOperatorEnum get_binary_operator() const;
    const GroundFunctionExpression& get_left_function_expression() const;
    const GroundFunctionExpression& get_right_function_expression() const;
};


/* GroundFunctionExpressionMultiOperator */
class GroundFunctionExpressionMultiOperatorImpl : public Base<GroundFunctionExpressionMultiOperatorImpl> {
private:
    MultiOperatorEnum m_multi_operator;
    GroundFunctionExpressionList m_function_expressions;

    GroundFunctionExpressionMultiOperatorImpl(int identifier,
        MultiOperatorEnum multi_operator,
        GroundFunctionExpressionList function_expressions);

    // Give access to the constructor.
    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

    bool is_structurally_equivalent_to_impl(const GroundFunctionExpressionMultiOperatorImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<GroundFunctionExpressionMultiOperatorImpl>;

public:
    MultiOperatorEnum get_multi_operator() const;
    const GroundFunctionExpressionList& get_function_expressions() const;
};


/* GroundFunctionExpressionMinus */
class GroundFunctionExpressionMinusImpl : public Base<GroundFunctionExpressionMinusImpl> {
private:
    GroundFunctionExpression m_function_expression;

    GroundFunctionExpressionMinusImpl(int identifier, GroundFunctionExpression function_expression);

    // Give access to the constructor.
    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

    bool is_structurally_equivalent_to_impl(const GroundFunctionExpressionMinusImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<GroundFunctionExpressionMinusImpl>;

public:
    const GroundFunctionExpression& get_function_expression() const;
};


/* GroundFunctionExpressionGroundFunction */
class GroundFunctionExpressionFunctionImpl : public Base<GroundFunctionExpressionFunctionImpl> {
private:
    GroundFunction m_function;

    GroundFunctionExpressionFunctionImpl(int identifier, GroundFunction function);

    // Give access to the constructor.
    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

    bool is_structurally_equivalent_to_impl(const GroundFunctionExpressionFunctionImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<GroundFunctionExpressionFunctionImpl>;

public:
    const GroundFunction& get_function() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::GroundFunctionExpression>
    {
        bool operator()(const loki::pddl::GroundFunctionExpression& left_function_expression, const loki::pddl::GroundFunctionExpression& right_function_expression) const;
    };

    template<>
    struct hash<loki::pddl::GroundFunctionExpressionNumberImpl>
    {
        std::size_t operator()(const loki::pddl::GroundFunctionExpressionNumberImpl& function_expressions) const;
    };

    template<>
    struct hash<loki::pddl::GroundFunctionExpressionBinaryOperatorImpl>
    {
        std::size_t operator()(const loki::pddl::GroundFunctionExpressionBinaryOperatorImpl& function_expressions) const;
    };

    template<>
    struct hash<loki::pddl::GroundFunctionExpressionMultiOperatorImpl>
    {
        std::size_t operator()(const loki::pddl::GroundFunctionExpressionMultiOperatorImpl& function_expressions) const;
    };

    template<>
    struct hash<loki::pddl::GroundFunctionExpressionMinusImpl>
    {
        std::size_t operator()(const loki::pddl::GroundFunctionExpressionMinusImpl& function_expressions) const;
    };

    template<>
    struct hash<loki::pddl::GroundFunctionExpressionFunctionImpl>
    {
        std::size_t operator()(const loki::pddl::GroundFunctionExpressionFunctionImpl& function_expressions) const;
    };
}

#endif
