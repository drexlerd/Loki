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

#include "../../../include/loki/domain/pddl/effects.hpp"

#include "../../../include/loki/domain/pddl/conditions.hpp"
#include "../../../include/loki/domain/pddl/literal.hpp"
#include "../../../include/loki/domain/pddl/parameter.hpp"
#include "../../../include/loki/domain/pddl/function.hpp"
#include "../../../include/loki/domain/pddl/function_expressions.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"

#include <cassert>


namespace loki::pddl {

std::unordered_map<AssignOperatorEnum, std::string> assign_operator_enum_to_string = {
    { AssignOperatorEnum::ASSIGN, "assign" },
    { AssignOperatorEnum::SCALE_UP, "scale-up" },
    { AssignOperatorEnum::SCALE_DOWN, "scale-down" },
    { AssignOperatorEnum::INCREASE, "increase" },
    { AssignOperatorEnum::DECREASE, "decrease" },
};

const std::string& to_string(pddl::AssignOperatorEnum assign_operator) {
    assert(assign_operator_enum_to_string.count(assign_operator));
    return assign_operator_enum_to_string.at(assign_operator);
}


/* BaseCondition */
EffectImpl::EffectImpl(int identifier)
    : Base(identifier) { }

EffectImpl::~EffectImpl() = default;

/* Literal */
EffectLiteralImpl::EffectLiteralImpl(int identifier, Literal literal)
    : EffectImpl(identifier)
    , m_literal(std::move(literal)) { }

bool EffectLiteralImpl::are_equal_impl(const EffectImpl& other) const {
    // https://stackoverflow.com/questions/11332075/comparing-polymorphic-base-types-in-c-without-rtti
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const EffectLiteralImpl&>(other);
        return m_literal == other_derived.m_literal;
    }
    return false;
}

size_t EffectLiteralImpl::hash_impl() const {
    return std::hash<Literal>()(m_literal);
}

void EffectLiteralImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << *m_literal;
}

void EffectLiteralImpl::accept(EffectVisitor& visitor) const {
    visitor.visit(this);
}

const Literal& EffectLiteralImpl::get_literal() const {
    return m_literal;
}


EffectAndImpl::EffectAndImpl(int identifier, EffectList effects)
    : EffectImpl(identifier), m_effects(std::move(effects)) { }

bool EffectAndImpl::are_equal_impl(const EffectImpl& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const EffectAndImpl&>(other);
        return get_sorted_vector(m_effects) == get_sorted_vector(other_derived.m_effects);
    }
    return false;
}

size_t EffectAndImpl::hash_impl() const {
    return hash_vector(get_sorted_vector(m_effects));
}

void EffectAndImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << "(and ";
    for (size_t i = 0; i < m_effects.size(); ++i) {
        if (i != 0) out << " ";
        out << *m_effects[i];
    }
    out << ")";
}

void EffectAndImpl::accept(EffectVisitor& visitor) const {
    visitor.visit(this);
}

const EffectList& EffectAndImpl::get_effects() const {
    return m_effects;
}


/* EffectNumeric */
EffectNumericImpl::EffectNumericImpl(int identifier, AssignOperatorEnum assign_operator, Function function, FunctionExpression function_expression)
    : EffectImpl(identifier)
    , m_assign_operator(assign_operator)
    , m_function(std::move(function))
    , m_function_expression(std::move(function_expression)) { }

bool EffectNumericImpl::are_equal_impl(const EffectImpl& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const EffectNumericImpl&>(other);
        return m_assign_operator == other_derived.m_assign_operator
            && m_function == other_derived.m_function
            && m_function_expression == other_derived.m_function_expression;
    }
    return false;
}

size_t EffectNumericImpl::hash_impl() const {
    return hash_combine(m_assign_operator, m_function, m_function_expression);
}

void EffectNumericImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << "(" << to_string(m_assign_operator) << " " << *m_function << " " << *m_function_expression << ")";
}

void EffectNumericImpl::accept(EffectVisitor& visitor) const {
    visitor.visit(this);
}

AssignOperatorEnum EffectNumericImpl::get_assign_operator() const {
    return m_assign_operator;
}

const Function& EffectNumericImpl::get_function() const {
    return m_function;
}

const FunctionExpression& EffectNumericImpl::get_function_expression() const {
    return m_function_expression;
}


/* ConditionalForall */
EffectConditionalForallImpl::EffectConditionalForallImpl(int identifier, ParameterList parameters, Effect effect)
    : EffectImpl(identifier), m_parameters(std::move(parameters)), m_effect(std::move(effect)) { }

bool EffectConditionalForallImpl::are_equal_impl(const EffectImpl& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const EffectConditionalForallImpl&>(other);
        return m_parameters == other_derived.m_parameters
            && m_effect == other_derived.m_effect;
    }
    return false;
}

size_t EffectConditionalForallImpl::hash_impl() const {
    return hash_combine(hash_vector(m_parameters), m_effect);
}

void EffectConditionalForallImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << "(forall (";
    for (size_t i = 0; i < m_parameters.size(); ++i) {
        if (i != 0) out << " ";
        out << *m_parameters[i];
    }
    out << ") " << *m_effect << ")";
}

void EffectConditionalForallImpl::accept(EffectVisitor& visitor) const {
    visitor.visit(this);
}

const ParameterList& EffectConditionalForallImpl::get_parameters() const {
    return m_parameters;
}

const Effect& EffectConditionalForallImpl::get_effect() const {
    return m_effect;
}


EffectConditionalWhenImpl::EffectConditionalWhenImpl(int identifier, Condition condition, Effect effect)
    : EffectImpl(identifier), m_condition(std::move(condition)), m_effect(std::move(effect)) { }

bool EffectConditionalWhenImpl::are_equal_impl(const EffectImpl& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const EffectConditionalWhenImpl&>(other);
        return m_condition == other_derived.m_condition
            && m_effect == other_derived.m_effect;
    }
    return false;
}

size_t EffectConditionalWhenImpl::hash_impl() const {
    return hash_combine(m_condition, m_effect);
}

void EffectConditionalWhenImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << "(when " << *m_condition << " " << *m_effect << ")";
}

void EffectConditionalWhenImpl::accept(EffectVisitor& visitor) const {
    visitor.visit(this);
}

const Condition& EffectConditionalWhenImpl::get_condition() const {
    return m_condition;
}

const Effect& EffectConditionalWhenImpl::get_effect() const {
    return m_effect;
}

}


namespace std {
    bool less<loki::pddl::Effect>::operator()(
        const loki::pddl::Effect& left_effect,
        const loki::pddl::Effect& right_effect) const {
        return *left_effect < *right_effect;
    }

    std::size_t hash<loki::pddl::EffectLiteralImpl>::operator()(const loki::pddl::EffectLiteralImpl& effect) const {
        return effect.hash_impl();
    }

    std::size_t hash<loki::pddl::EffectAndImpl>::operator()(const loki::pddl::EffectAndImpl& effect) const {
        return effect.hash_impl();
    }

    std::size_t hash<loki::pddl::EffectNumericImpl>::operator()(const loki::pddl::EffectNumericImpl& effect) const {
        return effect.hash_impl();
    }

    std::size_t hash<loki::pddl::EffectConditionalForallImpl>::operator()(const loki::pddl::EffectConditionalForallImpl& effect) const {
        return effect.hash_impl();
    }

    std::size_t hash<loki::pddl::EffectConditionalWhenImpl>::operator()(const loki::pddl::EffectConditionalWhenImpl& effect) const {
        return effect.hash_impl();
    }
}
