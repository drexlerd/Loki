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
template<typename... Ts>
class ReferenceCountedObjectFactory;
}

namespace loki::pddl {
enum class ArithmeticOperatorEnum {
    MUL,
    PLUS,
    MINUS,
    DIV,
};

extern std::unordered_map<ArithmeticOperatorEnum, std::string> binary_operator_enum_to_string;
extern const std::string& to_string(pddl::ArithmeticOperatorEnum binary_operator);


class FunctionExpressionVisitor {
public:
    virtual ~FunctionExpressionVisitor() = default;

    virtual void visit(const FunctionExpressionNumber& function_expression) = 0;
    virtual void visit(const FunctionExpressionBinaryOperator& function_expression) = 0;
    virtual void visit(const FunctionExpressionMinus& function_expression) = 0;
    virtual void visit(const FunctionExpressionFunction& function_expression) = 0;
};


/* FunctionExpression */
class FunctionExpressionImpl : public Base<FunctionExpressionImpl> {
protected:
    FunctionExpressionImpl(int identifier);

    // protected copy/move to prevent accidental object slicing when passed by value
    FunctionExpressionImpl(const FunctionExpressionImpl& other) = default;
    FunctionExpressionImpl& operator=(const FunctionExpressionImpl& other) = default;
    FunctionExpressionImpl(FunctionExpressionImpl&& other) = default;
    FunctionExpressionImpl& operator=(FunctionExpressionImpl&& other) = default;

public:
    virtual ~FunctionExpressionImpl();

    /// @brief Test for structural equivalence
    virtual bool are_equal_impl(const FunctionExpressionImpl& other) const = 0;
    virtual void str_impl(std::ostringstream& out, const FormattingOptions& options) const = 0;

    /// @brief Accepts the visitor by calling the visit overload.
    virtual void accept(FunctionExpressionVisitor& visitor) const = 0;
};


/* FunctionExpressionNumber */
class FunctionExpressionNumberImpl : public FunctionExpressionImpl, public std::enable_shared_from_this<FunctionExpressionNumberImpl> {
private:
    double m_number;

    FunctionExpressionNumberImpl(int identifier, double number);

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const FunctionExpressionImpl& other) const override;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const override;

    void accept(FunctionExpressionVisitor& visitor) const override;

    double get_number() const;
};


/* FunctionExpressionBinaryOperator */
class FunctionExpressionBinaryOperatorImpl : public FunctionExpressionImpl, public std::enable_shared_from_this<FunctionExpressionBinaryOperatorImpl> {
private:
    ArithmeticOperatorEnum m_binary_operator;
    FunctionExpression m_left_function_expression;
    FunctionExpression m_right_function_expression;

    FunctionExpressionBinaryOperatorImpl(int identifier,
        ArithmeticOperatorEnum binary_operator,
        const FunctionExpression& left_function_expression,
        const FunctionExpression& right_function_expression);

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const FunctionExpressionImpl& other) const override;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const override;

    void accept(FunctionExpressionVisitor& visitor) const override;

    ArithmeticOperatorEnum get_binary_operator() const;
    const FunctionExpression& get_left_function_expression() const;
    const FunctionExpression& get_right_function_expression() const;
};


/* FunctionExpressionMinus */
class FunctionExpressionMinusImpl : public FunctionExpressionImpl, public std::enable_shared_from_this<FunctionExpressionMinusImpl> {
private:
    FunctionExpression m_function_expression;

    FunctionExpressionMinusImpl(int identifier, const FunctionExpression& function_expression);

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const FunctionExpressionImpl& other) const override;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const override;

    void accept(FunctionExpressionVisitor& visitor) const override;

    const FunctionExpression& get_function_expression() const;
};


/* FunctionExpressionFunction */
class FunctionExpressionFunctionImpl : public FunctionExpressionImpl, public std::enable_shared_from_this<FunctionExpressionFunctionImpl> {
private:
    Function m_function;

    FunctionExpressionFunctionImpl(int identifier, const Function& function);

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const FunctionExpressionImpl& other) const override;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const override;

    void accept(FunctionExpressionVisitor& visitor) const override;

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
