/*
 * Copyright (C) 2023 Dominik Drexler
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

#ifndef LOKI_INCLUDE_LOKI_PDDL_EFFECTS_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_EFFECTS_HPP_

#include "loki/details/pddl/base.hpp"
#include "loki/details/pddl/declarations.hpp"

#include <string>

namespace loki
{
enum class AssignOperatorEnum
{
    ASSIGN,
    SCALE_UP,
    SCALE_DOWN,
    INCREASE,
    DECREASE
};

extern std::unordered_map<AssignOperatorEnum, std::string> assign_operator_enum_to_string;
extern const std::string& to_string(AssignOperatorEnum assign_operator);

/* Literal */
class EffectLiteralImpl : public Base<EffectLiteralImpl>
{
private:
    Literal m_literal;

    EffectLiteralImpl(size_t index, Literal literal);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<EffectLiteralImpl>;

public:
    const Literal& get_literal() const;
};

template<>
struct ShallowHash<const EffectLiteralImpl&>
{
    size_t operator()(const EffectLiteralImpl& e) const;
};

template<>
struct ShallowEqualTo<const EffectLiteralImpl&>
{
    bool operator()(const EffectLiteralImpl& l, const EffectLiteralImpl& r) const;
};

/* And */
class EffectAndImpl : public Base<EffectAndImpl>
{
private:
    EffectList m_effects;

    EffectAndImpl(size_t index, EffectList effects);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<EffectAndImpl>;

public:
    const EffectList& get_effects() const;
};

template<>
struct ShallowHash<const EffectAndImpl&>
{
    size_t operator()(const EffectAndImpl& e) const;
};

template<>
struct ShallowEqualTo<const EffectAndImpl&>
{
    bool operator()(const EffectAndImpl& l, const EffectAndImpl& r) const;
};

/* EffectNumeric */
class EffectNumericImpl : public Base<EffectNumericImpl>
{
private:
    AssignOperatorEnum m_assign_operator;
    Function m_function;
    FunctionExpression m_function_expression;

    EffectNumericImpl(size_t index, AssignOperatorEnum assign_operator, Function function, FunctionExpression function_expression);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<EffectNumericImpl>;

public:
    AssignOperatorEnum get_assign_operator() const;
    const Function& get_function() const;
    const FunctionExpression& get_function_expression() const;
};

template<>
struct ShallowHash<const EffectNumericImpl&>
{
    size_t operator()(const EffectNumericImpl& e) const;
};

template<>
struct ShallowEqualTo<const EffectNumericImpl&>
{
    bool operator()(const EffectNumericImpl& l, const EffectNumericImpl& r) const;
};

/* ConditionalForall */
class EffectConditionalForallImpl : public Base<EffectConditionalForallImpl>
{
private:
    ParameterList m_parameters;
    Effect m_effect;

    EffectConditionalForallImpl(size_t index, ParameterList parameters, Effect effect);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<EffectConditionalForallImpl>;

public:
    const ParameterList& get_parameters() const;
    const Effect& get_effect() const;
};

template<>
struct ShallowHash<const EffectConditionalForallImpl&>
{
    size_t operator()(const EffectConditionalForallImpl& e) const;
};

template<>
struct ShallowEqualTo<const EffectConditionalForallImpl&>
{
    bool operator()(const EffectConditionalForallImpl& l, const EffectConditionalForallImpl& r) const;
};

/* ConditionalWhen */
class EffectConditionalWhenImpl : public Base<EffectConditionalWhenImpl>
{
private:
    Condition m_condition;
    Effect m_effect;

    EffectConditionalWhenImpl(size_t index, Condition condition, Effect effect);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<EffectConditionalWhenImpl>;

public:
    const Condition& get_condition() const;
    const Effect& get_effect() const;
};

template<>
struct ShallowHash<const EffectConditionalWhenImpl&>
{
    size_t operator()(const EffectConditionalWhenImpl& e) const;
};

template<>
struct ShallowEqualTo<const EffectConditionalWhenImpl&>
{
    bool operator()(const EffectConditionalWhenImpl& l, const EffectConditionalWhenImpl& r) const;
};

/* EffectImpl */

template<>
struct ShallowHash<const EffectImpl*>
{
    size_t operator()(const EffectImpl* e) const;
};

template<>
struct ShallowEqualTo<const EffectImpl*>
{
    bool operator()(const EffectImpl* l, const EffectImpl* r) const;
};

}

#endif
