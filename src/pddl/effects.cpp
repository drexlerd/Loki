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
#include "loki/details/utils/collections.hpp"
#include "loki/details/utils/equal_to.hpp"
#include "loki/details/utils/hash.hpp"

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

size_t ShallowHash<const EffectLiteralImpl&>::operator()(const EffectLiteralImpl& e) const { return ShallowHashCombiner()(e.get_literal()); }

bool ShallowEqualTo<const EffectLiteralImpl&>::operator()(const EffectLiteralImpl& l, const EffectLiteralImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_literal() == r.get_literal());
    }
    return true;
}

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

size_t ShallowHash<const EffectAndImpl&>::operator()(const EffectAndImpl& e) const { return ShallowHashCombiner()(get_sorted_vector(e.get_effects())); }

bool ShallowEqualTo<const EffectAndImpl&>::operator()(const EffectAndImpl& l, const EffectAndImpl& r) const
{
    if (&l != &r)
    {
        return (get_sorted_vector(l.get_effects()) == get_sorted_vector(r.get_effects()));
    }
    return true;
}

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

size_t ShallowHash<const EffectNumericImpl&>::operator()(const EffectNumericImpl& e) const
{
    return ShallowHashCombiner()(e.get_assign_operator(), e.get_function(), e.get_function_expression());
}

bool ShallowEqualTo<const EffectNumericImpl&>::operator()(const EffectNumericImpl& l, const EffectNumericImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_assign_operator() == r.get_assign_operator()) && (l.get_function() == r.get_function())
               && (l.get_function_expression() == r.get_function_expression());
    }
    return true;
}

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

size_t ShallowHash<const EffectConditionalForallImpl&>::operator()(const EffectConditionalForallImpl& e) const
{
    return ShallowHashCombiner()(e.get_effect(), get_sorted_vector(e.get_parameters()));
}

bool ShallowEqualTo<const EffectConditionalForallImpl&>::operator()(const EffectConditionalForallImpl& l, const EffectConditionalForallImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_effect() == r.get_effect()) && (get_sorted_vector(l.get_parameters()) == get_sorted_vector(r.get_parameters()));
    }
    return true;
}

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

size_t ShallowHash<const EffectConditionalWhenImpl&>::operator()(const EffectConditionalWhenImpl& e) const
{
    return ShallowHashCombiner()(e.get_condition(), e.get_effect());
}

bool ShallowEqualTo<const EffectConditionalWhenImpl&>::operator()(const EffectConditionalWhenImpl& l, const EffectConditionalWhenImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_condition() == r.get_condition()) && (l.get_effect() == r.get_effect());
    }
    return true;
}

/* EffectImpl */

size_t ShallowHash<const EffectImpl*>::operator()(const EffectImpl* e) const
{
    return std::visit([](const auto& arg) { return ShallowHash<decltype(arg)>()(arg); }, *e);
}

bool ShallowEqualTo<const EffectImpl*>::operator()(const EffectImpl* l, const EffectImpl* r) const { return ShallowEqualTo<EffectImpl>()(*l, *r); }

}
