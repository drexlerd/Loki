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

#include "loki/details/pddl/literal.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/pddl/visitors.hpp"

namespace loki
{

/* Literal */
ConditionLiteralImpl::ConditionLiteralImpl(size_t index, Literal literal) : Base(index), m_literal(std::move(literal)) {}

void ConditionLiteralImpl::str_impl(std::ostream& out, const FormattingOptions& options) const { m_literal->str(out, options); }

const Literal& ConditionLiteralImpl::get_literal() const { return m_literal; }

/* And */
ConditionAndImpl::ConditionAndImpl(size_t index, ConditionList conditions) : Base(index), m_conditions(std::move(conditions)) {}

void ConditionAndImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(and ";
    for (size_t i = 0; i < m_conditions.size(); ++i)
    {
        if (i != 0)
            out << " ";
        std::visit(StringifyVisitor(out, options), *m_conditions[i]);
    }
    out << ")";
}

const ConditionList& ConditionAndImpl::get_conditions() const { return m_conditions; }

/* Or */
ConditionOrImpl::ConditionOrImpl(size_t index, ConditionList conditions) : Base(index), m_conditions(std::move(conditions)) {}

void ConditionOrImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(or ";
    for (size_t i = 0; i < m_conditions.size(); ++i)
    {
        if (i != 0)
            out << " ";
        std::visit(StringifyVisitor(out, options), *m_conditions[i]);
    }
    out << ")";
}

const ConditionList& ConditionOrImpl::get_conditions() const { return m_conditions; }

/* Not */
ConditionNotImpl::ConditionNotImpl(size_t index, Condition condition) : Base(index), m_condition(std::move(condition)) {}

void ConditionNotImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(not ";
    std::visit(StringifyVisitor(out, options), *m_condition);
    out << ")";
}

const Condition& ConditionNotImpl::get_condition() const { return m_condition; }

/* Imply */
ConditionImplyImpl::ConditionImplyImpl(size_t index, Condition condition_left, Condition condition_right) :
    Base(index),
    m_condition_left(std::move(condition_left)),
    m_condition_right(std::move(condition_right))
{
}

void ConditionImplyImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(imply ";
    std::visit(StringifyVisitor(out, options), *m_condition_left);
    out << " ";
    std::visit(StringifyVisitor(out, options), *m_condition_right);
    out << ")";
}

const Condition& ConditionImplyImpl::get_condition_left() const { return m_condition_left; }

const Condition& ConditionImplyImpl::get_condition_right() const { return m_condition_right; }

/* Exists */
ConditionExistsImpl::ConditionExistsImpl(size_t index, ParameterList parameters, Condition condition) :
    Base(index),
    m_parameters(std::move(parameters)),
    m_condition(std::move(condition))
{
}

void ConditionExistsImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(exists (";
    for (size_t i = 0; i < m_parameters.size(); ++i)
    {
        if (i != 0)
            out << " ";
        m_parameters[i]->str(out, options);
    }
    out << ") ";
    std::visit(StringifyVisitor(out, options), *m_condition);
    out << ")";
}

const ParameterList& ConditionExistsImpl::get_parameters() const { return m_parameters; }

const Condition& ConditionExistsImpl::get_condition() const { return m_condition; }

/* Forall */
ConditionForallImpl::ConditionForallImpl(size_t index, ParameterList parameters, Condition condition) :
    Base(index),
    m_parameters(std::move(parameters)),
    m_condition(std::move(condition))
{
}

void ConditionForallImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(forall (";
    for (size_t i = 0; i < m_parameters.size(); ++i)
    {
        if (i != 0)
            out << " ";
        m_parameters[i]->str(out, options);
    }
    out << ") ";
    std::visit(StringifyVisitor(out, options), *m_condition);
    out << ")";
}

const ParameterList& ConditionForallImpl::get_parameters() const { return m_parameters; }

const Condition& ConditionForallImpl::get_condition() const { return m_condition; }

}
