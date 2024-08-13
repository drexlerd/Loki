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

#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/function.hpp"
#include "loki/details/pddl/function_expressions.hpp"
#include "loki/details/pddl/literal.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/pddl/visitors.hpp"

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
EffectLiteralImpl::EffectLiteralImpl(size_t index, Literal literal) : Base(index), m_literal(std::move(literal)) {}

void EffectLiteralImpl::str_impl(std::ostream& out, const FormattingOptions& options) const { m_literal->str(out, options); }

const Literal& EffectLiteralImpl::get_literal() const { return m_literal; }

/* EffectAnd */
EffectAndImpl::EffectAndImpl(size_t index, EffectList effects) : Base(index), m_effects(std::move(effects)) {}

void EffectAndImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(and ";
    for (size_t i = 0; i < m_effects.size(); ++i)
    {
        if (i != 0)
            out << " ";
        std::visit(StringifyVisitor(out, options), *m_effects[i]);
    }
    out << ")";
}

const EffectList& EffectAndImpl::get_effects() const { return m_effects; }

/* EffectNumeric */
EffectNumericImpl::EffectNumericImpl(size_t index, AssignOperatorEnum assign_operator, Function function, FunctionExpression function_expression) :
    Base(index),
    m_assign_operator(assign_operator),
    m_function(std::move(function)),
    m_function_expression(std::move(function_expression))
{
}

void EffectNumericImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(" << to_string(m_assign_operator) << " ";
    m_function->str(out, options);
    out << " ";
    std::visit(StringifyVisitor(out, options), *m_function_expression);
    out << ")";
}

AssignOperatorEnum EffectNumericImpl::get_assign_operator() const { return m_assign_operator; }

const Function& EffectNumericImpl::get_function() const { return m_function; }

const FunctionExpression& EffectNumericImpl::get_function_expression() const { return m_function_expression; }

/* ConditionalConditionalForall */
EffectConditionalForallImpl::EffectConditionalForallImpl(size_t index, ParameterList parameters, Effect effect) :
    Base(index),
    m_parameters(std::move(parameters)),
    m_effect(std::move(effect))
{
}

void EffectConditionalForallImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(forall (";
    for (size_t i = 0; i < m_parameters.size(); ++i)
    {
        if (i != 0)
            out << " ";
        m_parameters[i]->str(out, options);
    }
    out << ") ";
    std::visit(StringifyVisitor(out, options), *m_effect);
    out << ")";
}

const ParameterList& EffectConditionalForallImpl::get_parameters() const { return m_parameters; }

const Effect& EffectConditionalForallImpl::get_effect() const { return m_effect; }

/* EffectConditionalWhen */
EffectConditionalWhenImpl::EffectConditionalWhenImpl(size_t index, Condition condition, Effect effect) :
    Base(index),
    m_condition(std::move(condition)),
    m_effect(std::move(effect))
{
}

void EffectConditionalWhenImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(when ";
    std::visit(StringifyVisitor(out, options), *m_condition);
    out << " ";
    std::visit(StringifyVisitor(out, options), *m_effect);
    out << ")";
}

const Condition& EffectConditionalWhenImpl::get_condition() const { return m_condition; }

const Effect& EffectConditionalWhenImpl::get_effect() const { return m_effect; }

}
