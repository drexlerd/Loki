/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
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

#include "loki/details/pddl/conditions.hpp"

#include "formatter.hpp"
#include "loki/details/pddl/literal.hpp"
#include "loki/details/pddl/parameter.hpp"

namespace loki
{

/* Literal */
ConditionLiteralImpl::ConditionLiteralImpl(size_t index, Literal literal) : m_index(index), m_literal(std::move(literal)) {}

size_t ConditionLiteralImpl::get_index() const { return m_index; }

const Literal& ConditionLiteralImpl::get_literal() const { return m_literal; }

/* And */
ConditionAndImpl::ConditionAndImpl(size_t index, ConditionList conditions) : m_index(index), m_conditions(std::move(conditions)) {}

size_t ConditionAndImpl::get_index() const { return m_index; }

const ConditionList& ConditionAndImpl::get_conditions() const { return m_conditions; }

/* Or */
ConditionOrImpl::ConditionOrImpl(size_t index, ConditionList conditions) : m_index(index), m_conditions(std::move(conditions)) {}

size_t ConditionOrImpl::get_index() const { return m_index; }

const ConditionList& ConditionOrImpl::get_conditions() const { return m_conditions; }

/* Not */
ConditionNotImpl::ConditionNotImpl(size_t index, Condition condition) : m_index(index), m_condition(std::move(condition)) {}

size_t ConditionNotImpl::get_index() const { return m_index; }

const Condition& ConditionNotImpl::get_condition() const { return m_condition; }

/* Imply */
ConditionImplyImpl::ConditionImplyImpl(size_t index, Condition condition_left, Condition condition_right) :
    m_index(index),
    m_condition_left(std::move(condition_left)),
    m_condition_right(std::move(condition_right))
{
}

size_t ConditionImplyImpl::get_index() const { return m_index; }

const Condition& ConditionImplyImpl::get_condition_left() const { return m_condition_left; }

const Condition& ConditionImplyImpl::get_condition_right() const { return m_condition_right; }

/* Exists */
ConditionExistsImpl::ConditionExistsImpl(size_t index, ParameterList parameters, Condition condition) :
    m_index(index),
    m_parameters(std::move(parameters)),
    m_condition(std::move(condition))
{
}

size_t ConditionExistsImpl::get_index() const { return m_index; }

const ParameterList& ConditionExistsImpl::get_parameters() const { return m_parameters; }

const Condition& ConditionExistsImpl::get_condition() const { return m_condition; }

/* Forall */
ConditionForallImpl::ConditionForallImpl(size_t index, ParameterList parameters, Condition condition) :
    m_index(index),
    m_parameters(std::move(parameters)),
    m_condition(std::move(condition))
{
}

size_t ConditionForallImpl::get_index() const { return m_index; }

const ParameterList& ConditionForallImpl::get_parameters() const { return m_parameters; }

const Condition& ConditionForallImpl::get_condition() const { return m_condition; }

/* ConditionNumericConstraint */

ConditionNumericConstraintImpl::ConditionNumericConstraintImpl(size_t index,
                                                               BinaryComparatorEnum binary_comparator,
                                                               FunctionExpression left_function_expression,
                                                               FunctionExpression right_function_expression) :
    m_index(index),
    m_binary_comparator(binary_comparator),
    m_left_function_expression(left_function_expression),
    m_right_function_expression(right_function_expression)
{
}

size_t ConditionNumericConstraintImpl::get_index() const { return m_index; }

const BinaryComparatorEnum& ConditionNumericConstraintImpl::get_binary_comparator() const { return m_binary_comparator; }

const FunctionExpression& ConditionNumericConstraintImpl::get_left_function_expression() const { return m_left_function_expression; }

const FunctionExpression& ConditionNumericConstraintImpl::get_right_function_expression() const { return m_right_function_expression; }

/* Condition */
ConditionImpl::ConditionImpl(size_t index, ConditionVariant condition) : m_index(index), m_condition(std::move(condition)) {}

size_t ConditionImpl::get_index() const { return m_index; }

const ConditionVariant& ConditionImpl::get_condition() const { return m_condition; }

std::ostream& operator<<(std::ostream& out, const ConditionLiteralImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ConditionAndImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ConditionOrImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ConditionNotImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ConditionImplyImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ConditionExistsImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ConditionForallImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ConditionNumericConstraintImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ConditionImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, ConditionLiteral element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, ConditionAnd element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, ConditionOr element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, ConditionNot element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, ConditionImply element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, ConditionExists element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, ConditionForall element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, ConditionNumericConstraint element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, Condition element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

}
