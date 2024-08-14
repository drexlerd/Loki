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

#include "loki/details/pddl/effects.hpp"

#include "formatter.hpp"
#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/function.hpp"
#include "loki/details/pddl/function_expressions.hpp"
#include "loki/details/pddl/literal.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/utils/collections.hpp"

#include <cassert>

namespace loki
{

std::unordered_map<AssignOperatorEnum, std::string> assign_operator_enum_to_string = {
    { AssignOperatorEnum::ASSIGN, "assign" },     { AssignOperatorEnum::SCALE_UP, "scale-up" }, { AssignOperatorEnum::SCALE_DOWN, "scale-down" },
    { AssignOperatorEnum::INCREASE, "increase" }, { AssignOperatorEnum::DECREASE, "decrease" },
};

const std::string& to_string(AssignOperatorEnum assign_operator)
{
    assert(assign_operator_enum_to_string.count(assign_operator));
    return assign_operator_enum_to_string.at(assign_operator);
}

/* EffectLiteral */
EffectLiteralImpl::EffectLiteralImpl(size_t index, Literal literal) : m_index(index), m_literal(std::move(literal)) {}

size_t EffectLiteralImpl::get_index() const { return m_index; }

const Literal& EffectLiteralImpl::get_literal() const { return m_literal; }

/* EffectAnd */
EffectAndImpl::EffectAndImpl(size_t index, EffectList effects) : m_index(index), m_effects(std::move(effects)) {}

size_t EffectAndImpl::get_index() const { return m_index; }

const EffectList& EffectAndImpl::get_effects() const { return m_effects; }

/* EffectNumeric */
EffectNumericImpl::EffectNumericImpl(size_t index, AssignOperatorEnum assign_operator, Function function, FunctionExpression function_expression) :
    m_index(index),
    m_assign_operator(assign_operator),
    m_function(std::move(function)),
    m_function_expression(std::move(function_expression))
{
}

size_t EffectNumericImpl::get_index() const { return m_index; }

AssignOperatorEnum EffectNumericImpl::get_assign_operator() const { return m_assign_operator; }

const Function& EffectNumericImpl::get_function() const { return m_function; }

const FunctionExpression& EffectNumericImpl::get_function_expression() const { return m_function_expression; }

/* ConditionalConditionalForall */
EffectConditionalForallImpl::EffectConditionalForallImpl(size_t index, ParameterList parameters, Effect effect) :
    m_index(index),
    m_parameters(std::move(parameters)),
    m_effect(std::move(effect))
{
}

size_t EffectConditionalForallImpl::get_index() const { return m_index; }

const ParameterList& EffectConditionalForallImpl::get_parameters() const { return m_parameters; }

const Effect& EffectConditionalForallImpl::get_effect() const { return m_effect; }

/* EffectConditionalWhen */
EffectConditionalWhenImpl::EffectConditionalWhenImpl(size_t index, Condition condition, Effect effect) :
    m_index(index),
    m_condition(std::move(condition)),
    m_effect(std::move(effect))
{
}

size_t EffectConditionalWhenImpl::get_index() const { return m_index; }

const Condition& EffectConditionalWhenImpl::get_condition() const { return m_condition; }

const Effect& EffectConditionalWhenImpl::get_effect() const { return m_effect; }

size_t UniquePDDLHasher<const EffectLiteralImpl&>::operator()(const EffectLiteralImpl& e) const { return UniquePDDLHashCombiner()(e.get_literal()); }

size_t UniquePDDLHasher<const EffectAndImpl&>::operator()(const EffectAndImpl& e) const { return UniquePDDLHashCombiner()(get_sorted_vector(e.get_effects())); }

size_t UniquePDDLHasher<const EffectNumericImpl&>::operator()(const EffectNumericImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_assign_operator(), e.get_function(), e.get_function_expression());
}

size_t UniquePDDLHasher<const EffectConditionalForallImpl&>::operator()(const EffectConditionalForallImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_effect(), get_sorted_vector(e.get_parameters()));
}

size_t UniquePDDLHasher<const EffectConditionalWhenImpl&>::operator()(const EffectConditionalWhenImpl& e) const
{
    return UniquePDDLHashCombiner()(e.get_condition(), e.get_effect());
}

size_t UniquePDDLHasher<const EffectImpl*>::operator()(const EffectImpl* e) const
{
    return std::visit([](const auto& arg) { return UniquePDDLHasher<decltype(arg)>()(arg); }, *e);
}

bool UniquePDDLEqualTo<const EffectLiteralImpl&>::operator()(const EffectLiteralImpl& l, const EffectLiteralImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_literal() == r.get_literal());
    }
    return true;
}

bool UniquePDDLEqualTo<const EffectAndImpl&>::operator()(const EffectAndImpl& l, const EffectAndImpl& r) const
{
    if (&l != &r)
    {
        return (get_sorted_vector(l.get_effects()) == get_sorted_vector(r.get_effects()));
    }
    return true;
}

bool UniquePDDLEqualTo<const EffectNumericImpl&>::operator()(const EffectNumericImpl& l, const EffectNumericImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_assign_operator() == r.get_assign_operator()) && (l.get_function() == r.get_function())
               && (l.get_function_expression() == r.get_function_expression());
    }
    return true;
}

bool UniquePDDLEqualTo<const EffectConditionalForallImpl&>::operator()(const EffectConditionalForallImpl& l, const EffectConditionalForallImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_effect() == r.get_effect()) && (get_sorted_vector(l.get_parameters()) == get_sorted_vector(r.get_parameters()));
    }
    return true;
}

bool UniquePDDLEqualTo<const EffectConditionalWhenImpl&>::operator()(const EffectConditionalWhenImpl& l, const EffectConditionalWhenImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_condition() == r.get_condition()) && (l.get_effect() == r.get_effect());
    }
    return true;
}

bool UniquePDDLEqualTo<const EffectImpl*>::operator()(const EffectImpl* l, const EffectImpl* r) const { return UniquePDDLEqualTo<EffectImpl>()(*l, *r); }

std::ostream& operator<<(std::ostream& out, const EffectLiteralImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const EffectAndImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const EffectNumericImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const EffectConditionalForallImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const EffectConditionalWhenImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const EffectImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

}
