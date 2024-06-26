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

/* Literal */
EffectLiteralImpl::EffectLiteralImpl(size_t identifier, Literal literal) : Base(identifier), m_literal(std::move(literal)) {}

bool EffectLiteralImpl::is_structurally_equivalent_to_impl(const EffectLiteralImpl& other) const
{
    if (this != &other)
    {
        return m_literal == other.m_literal;
    }
    return true;
}

size_t EffectLiteralImpl::hash_impl() const { return std::hash<Literal>()(m_literal); }

void EffectLiteralImpl::str_impl(std::ostream& out, const FormattingOptions& options) const { m_literal->str(out, options); }

const Literal& EffectLiteralImpl::get_literal() const { return m_literal; }

EffectAndImpl::EffectAndImpl(size_t identifier, EffectList effects) : Base(identifier), m_effects(std::move(effects)) {}

bool EffectAndImpl::is_structurally_equivalent_to_impl(const EffectAndImpl& other) const
{
    if (this != &other)
    {
        return get_sorted_vector(m_effects) == get_sorted_vector(other.m_effects);
    }
    return true;
}

size_t EffectAndImpl::hash_impl() const { return hash_container(get_sorted_vector(m_effects)); }

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
EffectNumericImpl::EffectNumericImpl(size_t identifier, AssignOperatorEnum assign_operator, Function function, FunctionExpression function_expression) :
    Base(identifier),
    m_assign_operator(assign_operator),
    m_function(std::move(function)),
    m_function_expression(std::move(function_expression))
{
}

bool EffectNumericImpl::is_structurally_equivalent_to_impl(const EffectNumericImpl& other) const
{
    if (this != &other)
    {
        return (m_assign_operator == other.m_assign_operator) && (m_function == other.m_function) && (m_function_expression == other.m_function_expression);
    }
    return true;
}

size_t EffectNumericImpl::hash_impl() const { return hash_combine(m_assign_operator, m_function, m_function_expression); }

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

/* ConditionalForall */
EffectConditionalForallImpl::EffectConditionalForallImpl(size_t identifier, ParameterList parameters, Effect effect) :
    Base(identifier),
    m_parameters(std::move(parameters)),
    m_effect(std::move(effect))
{
}

bool EffectConditionalForallImpl::is_structurally_equivalent_to_impl(const EffectConditionalForallImpl& other) const
{
    if (this != &other)
    {
        return (m_parameters == other.m_parameters) && (m_effect == other.m_effect);
    }
    return true;
}

size_t EffectConditionalForallImpl::hash_impl() const { return hash_combine(hash_container(m_parameters), m_effect); }

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

EffectConditionalWhenImpl::EffectConditionalWhenImpl(size_t identifier, Condition condition, Effect effect) :
    Base(identifier),
    m_condition(std::move(condition)),
    m_effect(std::move(effect))
{
}

bool EffectConditionalWhenImpl::is_structurally_equivalent_to_impl(const EffectConditionalWhenImpl& other) const
{
    if (this != &other)
    {
        return (m_condition == other.m_condition) && (m_effect == other.m_effect);
    }
    return true;
}

size_t EffectConditionalWhenImpl::hash_impl() const { return hash_combine(m_condition, m_effect); }

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
