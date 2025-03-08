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
    Literal get_literal() const;

    auto identifying_members() const { return std::tuple(get_literal()); }
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

    auto identifying_members() const { return std::tuple(std::cref(get_conditions())); }
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

    auto identifying_members() const { return std::tuple(std::cref(get_conditions())); }
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
    Condition get_condition() const;

    auto identifying_members() const { return std::tuple(get_condition()); }
};

/* Imply */
class ConditionImplyImpl
{
private:
    size_t m_index;
    Condition m_left_condition;
    Condition m_right_condition;

    ConditionImplyImpl(size_t index, Condition left_condition, Condition right_condition);

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
    Condition get_left_condition() const;
    Condition get_right_condition() const;

    auto identifying_members() const { return std::tuple(get_left_condition(), get_right_condition()); }
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
    Condition get_condition() const;

    auto identifying_members() const { return std::tuple(std::cref(get_parameters()), get_condition()); }
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
    Condition get_condition() const;

    auto identifying_members() const { return std::tuple(std::cref(get_parameters()), get_condition()); }
};

class ConditionNumericConstraintImpl
{
private:
    size_t m_index;
    BinaryComparatorEnum m_binary_comparator;
    FunctionExpression m_left_function_expression;
    FunctionExpression m_right_function_expression;

    ConditionNumericConstraintImpl(size_t index,
                                   BinaryComparatorEnum binary_comparator,
                                   FunctionExpression left_function_expression,
                                   FunctionExpression right_function_expression);

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
    BinaryComparatorEnum get_binary_comparator() const;
    FunctionExpression get_left_function_expression() const;
    FunctionExpression get_right_function_expression() const;

    auto identifying_members() const { return std::tuple(get_binary_comparator(), get_left_function_expression(), get_right_function_expression()); }
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

    auto identifying_members() const { return std::tuple(get_condition()); }
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

extern std::ostream& operator<<(std::ostream& out, ConditionLiteral element);
extern std::ostream& operator<<(std::ostream& out, ConditionAnd element);
extern std::ostream& operator<<(std::ostream& out, ConditionOr element);
extern std::ostream& operator<<(std::ostream& out, ConditionNot element);
extern std::ostream& operator<<(std::ostream& out, ConditionImply element);
extern std::ostream& operator<<(std::ostream& out, ConditionExists element);
extern std::ostream& operator<<(std::ostream& out, ConditionForall element);
extern std::ostream& operator<<(std::ostream& out, ConditionNumericConstraint element);
extern std::ostream& operator<<(std::ostream& out, Condition element);

}

#endif
