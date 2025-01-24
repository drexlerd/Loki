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

#ifndef LOKI_INCLUDE_LOKI_PDDL_CONDITIONS_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_CONDITIONS_HPP_

#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/function_expressions.hpp"

#include <string>

namespace loki
{

/* Literal */
class ConditionLiteralImpl
{
private:
    size_t m_index;
    Literal m_literal;

    ConditionLiteralImpl(size_t index, Literal literal);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    ConditionLiteralImpl(const ConditionLiteralImpl& other) = delete;
    ConditionLiteralImpl& operator=(const ConditionLiteralImpl& other) = delete;
    ConditionLiteralImpl(ConditionLiteralImpl&& other) = default;
    ConditionLiteralImpl& operator=(ConditionLiteralImpl&& other) = default;

    size_t get_index() const;
    const Literal& get_literal() const;

    auto identifying_members() const { return std::forward_as_tuple(std::as_const(m_literal)); }
};

/* And */
class ConditionAndImpl
{
private:
    size_t m_index;
    ConditionList m_conditions;

    ConditionAndImpl(size_t index, ConditionList conditions);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    ConditionAndImpl(const ConditionAndImpl& other) = delete;
    ConditionAndImpl& operator=(const ConditionAndImpl& other) = delete;
    ConditionAndImpl(ConditionAndImpl&& other) = default;
    ConditionAndImpl& operator=(ConditionAndImpl&& other) = default;

    size_t get_index() const;
    const ConditionList& get_conditions() const;

    auto identifying_members() const { return std::forward_as_tuple(std::as_const(m_conditions)); }
};

/* Or */
class ConditionOrImpl
{
private:
    size_t m_index;
    ConditionList m_conditions;

    ConditionOrImpl(size_t index, ConditionList conditions);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    ConditionOrImpl(const ConditionOrImpl& other) = delete;
    ConditionOrImpl& operator=(const ConditionOrImpl& other) = delete;
    ConditionOrImpl(ConditionOrImpl&& other) = default;
    ConditionOrImpl& operator=(ConditionOrImpl&& other) = default;

    size_t get_index() const;
    const ConditionList& get_conditions() const;

    auto identifying_members() const { return std::forward_as_tuple(std::as_const(m_conditions)); }
};

/* Not */
class ConditionNotImpl
{
private:
    size_t m_index;
    Condition m_condition;

    ConditionNotImpl(size_t index, Condition condition);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    ConditionNotImpl(const ConditionNotImpl& other) = delete;
    ConditionNotImpl& operator=(const ConditionNotImpl& other) = delete;
    ConditionNotImpl(ConditionNotImpl&& other) = default;
    ConditionNotImpl& operator=(ConditionNotImpl&& other) = default;

    size_t get_index() const;
    const Condition& get_condition() const;

    auto identifying_members() const { return std::forward_as_tuple(std::as_const(m_condition)); }
};

/* Imply */
class ConditionImplyImpl
{
private:
    size_t m_index;
    Condition m_condition_left;
    Condition m_condition_right;

    ConditionImplyImpl(size_t index, Condition condition_left, Condition condition_right);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    ConditionImplyImpl(const ConditionImplyImpl& other) = delete;
    ConditionImplyImpl& operator=(const ConditionImplyImpl& other) = delete;
    ConditionImplyImpl(ConditionImplyImpl&& other) = default;
    ConditionImplyImpl& operator=(ConditionImplyImpl&& other) = default;

    size_t get_index() const;
    const Condition& get_condition_left() const;
    const Condition& get_condition_right() const;

    auto identifying_members() const { return std::forward_as_tuple(std::as_const(m_condition_left), std::as_const(m_condition_right)); }
};

/* Exists */
class ConditionExistsImpl
{
private:
    size_t m_index;
    ParameterList m_parameters;
    Condition m_condition;

    ConditionExistsImpl(size_t index, ParameterList parameters, Condition condition);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    ConditionExistsImpl(const ConditionExistsImpl& other) = delete;
    ConditionExistsImpl& operator=(const ConditionExistsImpl& other) = delete;
    ConditionExistsImpl(ConditionExistsImpl&& other) = default;
    ConditionExistsImpl& operator=(ConditionExistsImpl&& other) = default;

    size_t get_index() const;
    const ParameterList& get_parameters() const;
    const Condition& get_condition() const;

    auto identifying_members() const { return std::forward_as_tuple(std::as_const(m_parameters), std::as_const(m_condition)); }
};

/* Forall */
class ConditionForallImpl
{
private:
    size_t m_index;
    ParameterList m_parameters;
    Condition m_condition;

    ConditionForallImpl(size_t index, ParameterList parameters, Condition condition);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    ConditionForallImpl(const ConditionForallImpl& other) = delete;
    ConditionForallImpl& operator=(const ConditionForallImpl& other) = delete;
    ConditionForallImpl(ConditionForallImpl&& other) = default;
    ConditionForallImpl& operator=(ConditionForallImpl&& other) = default;

    size_t get_index() const;
    const ParameterList& get_parameters() const;
    const Condition& get_condition() const;

    auto identifying_members() const { return std::forward_as_tuple(std::as_const(m_parameters), std::as_const(m_condition)); }
};

class ConditionNumericConstraintImpl
{
private:
    size_t m_index;
    BinaryComparatorEnum m_binary_comparator;
    FunctionExpression m_function_expression_left;
    FunctionExpression m_function_expression_right;

    ConditionNumericConstraintImpl(size_t index,
                                   BinaryComparatorEnum binary_comparator,
                                   FunctionExpression function_expression_left,
                                   FunctionExpression function_expression_right);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    ConditionNumericConstraintImpl(const ConditionNumericConstraintImpl& other) = delete;
    ConditionNumericConstraintImpl& operator=(const ConditionNumericConstraintImpl& other) = delete;
    ConditionNumericConstraintImpl(ConditionNumericConstraintImpl&& other) = default;
    ConditionNumericConstraintImpl& operator=(ConditionNumericConstraintImpl&& other) = default;

    size_t get_index() const;
    const BinaryComparatorEnum& get_binary_comparator() const;
    const FunctionExpression& get_function_expression_left() const;
    const FunctionExpression& get_function_expression_right() const;

    auto identifying_members() const
    {
        return std::forward_as_tuple(std::as_const(m_binary_comparator), std::as_const(m_function_expression_left), std::as_const(m_function_expression_right));
    }
};

/* Condition */
using ConditionVariant =
    std::variant<ConditionLiteral, ConditionAnd, ConditionOr, ConditionNot, ConditionImply, ConditionExists, ConditionForall, ConditionNumericConstraint>;

class ConditionImpl
{
private:
    size_t m_index;
    ConditionVariant m_condition;

    ConditionImpl(size_t index, ConditionVariant condition);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    ConditionImpl(const ConditionImpl& other) = delete;
    ConditionImpl& operator=(const ConditionImpl& other) = delete;
    ConditionImpl(ConditionImpl&& other) = default;
    ConditionImpl& operator=(ConditionImpl&& other) = default;

    size_t get_index() const;
    const ConditionVariant& get_condition() const;

    auto identifying_members() const { return std::forward_as_tuple(std::as_const(m_condition)); }
};

extern std::ostream& operator<<(std::ostream& out, const ConditionLiteralImpl& element);
extern std::ostream& operator<<(std::ostream& out, const ConditionAndImpl& element);
extern std::ostream& operator<<(std::ostream& out, const ConditionOrImpl& element);
extern std::ostream& operator<<(std::ostream& out, const ConditionNotImpl& element);
extern std::ostream& operator<<(std::ostream& out, const ConditionImplyImpl& element);
extern std::ostream& operator<<(std::ostream& out, const ConditionExistsImpl& element);
extern std::ostream& operator<<(std::ostream& out, const ConditionForallImpl& element);
extern std::ostream& operator<<(std::ostream& out, const ConditionNumericConstraintImpl& element);
extern std::ostream& operator<<(std::ostream& out, const ConditionImpl& element);

}

#endif
