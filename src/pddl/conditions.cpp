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
#include "loki/details/utils/collections.hpp"
#include "loki/details/utils/equal_to.hpp"
#include "loki/details/utils/hash.hpp"

namespace loki
{

/* Literal */
ConditionLiteralImpl::ConditionLiteralImpl(size_t index, Literal literal) : Base(index), m_literal(std::move(literal)) {}

void ConditionLiteralImpl::str_impl(std::ostream& out, const FormattingOptions& options) const { m_literal->str(out, options); }

const Literal& ConditionLiteralImpl::get_literal() const { return m_literal; }

size_t ShallowHash<const ConditionLiteralImpl&>::operator()(const ConditionLiteralImpl& e) const { return ShallowHashCombiner()(e.get_literal()); }

bool ShallowEqualTo<const ConditionLiteralImpl&>::operator()(const ConditionLiteralImpl& l, const ConditionLiteralImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_literal() == r.get_literal());
    }
    return true;
}

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

size_t ShallowHash<const ConditionAndImpl&>::operator()(const ConditionAndImpl& e) const
{
    return ShallowHashCombiner()(get_sorted_vector(e.get_conditions()));
}

bool ShallowEqualTo<const ConditionAndImpl&>::operator()(const ConditionAndImpl& l, const ConditionAndImpl& r) const
{
    if (&l != &r)
    {
        return (get_sorted_vector(l.get_conditions()) == get_sorted_vector(r.get_conditions()));
    }
    return true;
}

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

size_t ShallowHash<const ConditionOrImpl&>::operator()(const ConditionOrImpl& e) const { return ShallowHashCombiner()(get_sorted_vector(e.get_conditions())); }

bool ShallowEqualTo<const ConditionOrImpl&>::operator()(const ConditionOrImpl& l, const ConditionOrImpl& r) const
{
    if (&l != &r)
    {
        return (get_sorted_vector(l.get_conditions()) == get_sorted_vector(r.get_conditions()));
    }
    return true;
}

/* Not */
ConditionNotImpl::ConditionNotImpl(size_t index, Condition condition) : Base(index), m_condition(std::move(condition)) {}

void ConditionNotImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(not ";
    std::visit(StringifyVisitor(out, options), *m_condition);
    out << ")";
}

const Condition& ConditionNotImpl::get_condition() const { return m_condition; }

size_t ShallowHash<const ConditionNotImpl&>::operator()(const ConditionNotImpl& e) const { return ShallowHashCombiner()(e.get_condition()); }

bool ShallowEqualTo<const ConditionNotImpl&>::operator()(const ConditionNotImpl& l, const ConditionNotImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_condition() == r.get_condition());
    }
    return true;
}

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

size_t ShallowHash<const ConditionImplyImpl&>::operator()(const ConditionImplyImpl& e) const
{
    return ShallowHashCombiner()(e.get_condition_left(), e.get_condition_right());
}

bool ShallowEqualTo<const ConditionImplyImpl&>::operator()(const ConditionImplyImpl& l, const ConditionImplyImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_condition_left() == r.get_condition_left()) && (l.get_condition_left() == r.get_condition_left());
    }
    return true;
}

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

size_t ShallowHash<const ConditionExistsImpl&>::operator()(const ConditionExistsImpl& e) const
{
    return ShallowHashCombiner()(get_sorted_vector(e.get_parameters()), e.get_condition());
}

bool ShallowEqualTo<const ConditionExistsImpl&>::operator()(const ConditionExistsImpl& l, const ConditionExistsImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_condition() == r.get_condition()) && (get_sorted_vector(l.get_parameters()) == get_sorted_vector(r.get_parameters()));
    }
    return true;
}

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

size_t ShallowHash<const ConditionForallImpl&>::operator()(const ConditionForallImpl& e) const
{
    return ShallowHashCombiner()(get_sorted_vector(e.get_parameters()), e.get_condition());
}

bool ShallowEqualTo<const ConditionForallImpl&>::operator()(const ConditionForallImpl& l, const ConditionForallImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_condition() == r.get_condition()) && (get_sorted_vector(l.get_parameters()) == get_sorted_vector(r.get_parameters()));
    }
    return true;
}

/* ConditionImpl */

size_t ShallowHash<const ConditionImpl*>::operator()(const ConditionImpl* e) const
{
    return std::visit([](const auto& arg) { return ShallowHash<decltype(arg)>()(arg); }, *e);
}

bool ShallowEqualTo<const ConditionImpl*>::operator()(const ConditionImpl* l, const ConditionImpl* r) const { return ShallowEqualTo<ConditionImpl>()(*l, *r); }

}
