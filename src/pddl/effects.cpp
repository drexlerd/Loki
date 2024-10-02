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

/* CompositeForall */
EffectCompositeForallImpl::EffectCompositeForallImpl(size_t index, ParameterList parameters, Effect effect) :
    m_index(index),
    m_parameters(std::move(parameters)),
    m_effect(std::move(effect))
{
}

size_t EffectCompositeForallImpl::get_index() const { return m_index; }

const ParameterList& EffectCompositeForallImpl::get_parameters() const { return m_parameters; }

const Effect& EffectCompositeForallImpl::get_effect() const { return m_effect; }

/* EffectCompositeWhen */
EffectCompositeWhenImpl::EffectCompositeWhenImpl(size_t index, Condition condition, Effect effect) :
    m_index(index),
    m_condition(std::move(condition)),
    m_effect(std::move(effect))
{
}

size_t EffectCompositeWhenImpl::get_index() const { return m_index; }

const Condition& EffectCompositeWhenImpl::get_condition() const { return m_condition; }

const Effect& EffectCompositeWhenImpl::get_effect() const { return m_effect; }

/* EffectCompositeOneofImpl */
EffectCompositeOneofImpl::EffectCompositeOneofImpl(size_t index, EffectList effects) :
    m_index(index),
    m_effects(std::move(effects))
{
}

size_t EffectCompositeOneofImpl::get_index() const { return m_index; }

const EffectList& EffectCompositeOneofImpl::get_effects() const { return m_effects; }


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

std::ostream& operator<<(std::ostream& out, const EffectCompositeForallImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const EffectCompositeWhenImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, const EffectCompositeOneofImpl& element)
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
