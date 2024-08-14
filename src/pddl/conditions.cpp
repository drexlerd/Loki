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
#include "loki/details/utils/collections.hpp"

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

size_t UniquePDDLHasher<const ConditionLiteralImpl&>::operator()(const ConditionLiteralImpl& e) const { return UniquePDDLHashCombiner()(e.get_literal()); }

size_t UniquePDDLHasher<const ConditionAndImpl&>::operator()(const ConditionAndImpl& e) const
{
    return UniquePDDLHashCombiner()(get_sorted_vector(e.get_conditions()));
}

size_t UniquePDDLHasher<const ConditionOrImpl&>::operator()(const ConditionOrImpl& e) const
{
    return UniquePDDLHashCombiner()(get_sorted_vector(e.get_conditions()));
}

size_t UniquePDDLHasher<const ConditionNotImpl&>::operator()(const ConditionNotImpl& e) const { return UniquePDDLHashCombiner()(e.get_condition()); }

size_t UniquePDDLHasher<const ConditionImplyImpl&>::operator()(const ConditionImplyImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_condition_left(), e.get_condition_right());
}

size_t UniquePDDLHasher<const ConditionExistsImpl&>::operator()(const ConditionExistsImpl& e) const
{
    return UniquePDDLHashCombiner()(get_sorted_vector(e.get_parameters()), e.get_condition());
}

size_t UniquePDDLHasher<const ConditionForallImpl&>::operator()(const ConditionForallImpl& e) const
{
    return UniquePDDLHashCombiner()(get_sorted_vector(e.get_parameters()), e.get_condition());
}

size_t UniquePDDLHasher<const ConditionImpl*>::operator()(const ConditionImpl* e) const
{
    return std::visit([](const auto& arg) { return UniquePDDLHasher<decltype(arg)>()(arg); }, *e);
}

bool UniquePDDLEqualTo<const ConditionLiteralImpl&>::operator()(const ConditionLiteralImpl& l, const ConditionLiteralImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_literal() == r.get_literal());
    }
    return true;
}

bool UniquePDDLEqualTo<const ConditionAndImpl&>::operator()(const ConditionAndImpl& l, const ConditionAndImpl& r) const
{
    if (&l != &r)
    {
        return (get_sorted_vector(l.get_conditions()) == get_sorted_vector(r.get_conditions()));
    }
    return true;
}

bool UniquePDDLEqualTo<const ConditionOrImpl&>::operator()(const ConditionOrImpl& l, const ConditionOrImpl& r) const
{
    if (&l != &r)
    {
        return (get_sorted_vector(l.get_conditions()) == get_sorted_vector(r.get_conditions()));
    }
    return true;
}

bool UniquePDDLEqualTo<const ConditionNotImpl&>::operator()(const ConditionNotImpl& l, const ConditionNotImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_condition() == r.get_condition());
    }
    return true;
}

bool UniquePDDLEqualTo<const ConditionImplyImpl&>::operator()(const ConditionImplyImpl& l, const ConditionImplyImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_condition_left() == r.get_condition_left()) && (l.get_condition_left() == r.get_condition_left());
    }
    return true;
}

bool UniquePDDLEqualTo<const ConditionExistsImpl&>::operator()(const ConditionExistsImpl& l, const ConditionExistsImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_condition() == r.get_condition()) && (get_sorted_vector(l.get_parameters()) == get_sorted_vector(r.get_parameters()));
    }
    return true;
}

bool UniquePDDLEqualTo<const ConditionForallImpl&>::operator()(const ConditionForallImpl& l, const ConditionForallImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_condition() == r.get_condition()) && (get_sorted_vector(l.get_parameters()) == get_sorted_vector(r.get_parameters()));
    }
    return true;
}

bool UniquePDDLEqualTo<const ConditionImpl*>::operator()(const ConditionImpl* l, const ConditionImpl* r) const
{
    return UniquePDDLEqualTo<ConditionImpl>()(*l, *r);
}

std::ostream& operator<<(std::ostream& out, const ConditionLiteralImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ConditionAndImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ConditionOrImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ConditionNotImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ConditionImplyImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ConditionExistsImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ConditionForallImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const ConditionImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}
}
