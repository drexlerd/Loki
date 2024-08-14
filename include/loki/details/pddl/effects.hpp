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

#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/equal_to.hpp"
#include "loki/details/pddl/hash.hpp"

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
class EffectLiteralImpl
{
private:
    size_t m_index;
    Literal m_literal;

    EffectLiteralImpl(size_t index, Literal literal);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

public:
    // moveable but not copyable
    EffectLiteralImpl(const EffectLiteralImpl& other) = delete;
    EffectLiteralImpl& operator=(const EffectLiteralImpl& other) = delete;
    EffectLiteralImpl(EffectLiteralImpl&& other) = default;
    EffectLiteralImpl& operator=(EffectLiteralImpl&& other) = default;

    size_t get_index() const;
    const Literal& get_literal() const;
};

/* And */
class EffectAndImpl
{
private:
    size_t m_index;
    EffectList m_effects;

    EffectAndImpl(size_t index, EffectList effects);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

public:
    // moveable but not copyable
    EffectAndImpl(const EffectAndImpl& other) = delete;
    EffectAndImpl& operator=(const EffectAndImpl& other) = delete;
    EffectAndImpl(EffectAndImpl&& other) = default;
    EffectAndImpl& operator=(EffectAndImpl&& other) = default;

    size_t get_index() const;
    const EffectList& get_effects() const;
};

/* EffectNumeric */
class EffectNumericImpl
{
private:
    size_t m_index;
    AssignOperatorEnum m_assign_operator;
    Function m_function;
    FunctionExpression m_function_expression;

    EffectNumericImpl(size_t index, AssignOperatorEnum assign_operator, Function function, FunctionExpression function_expression);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

public:
    // moveable but not copyable
    EffectNumericImpl(const EffectNumericImpl& other) = delete;
    EffectNumericImpl& operator=(const EffectNumericImpl& other) = delete;
    EffectNumericImpl(EffectNumericImpl&& other) = default;
    EffectNumericImpl& operator=(EffectNumericImpl&& other) = default;

    size_t get_index() const;
    AssignOperatorEnum get_assign_operator() const;
    const Function& get_function() const;
    const FunctionExpression& get_function_expression() const;
};

/* ConditionalForall */
class EffectConditionalForallImpl
{
private:
    size_t m_index;
    ParameterList m_parameters;
    Effect m_effect;

    EffectConditionalForallImpl(size_t index, ParameterList parameters, Effect effect);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

public:
    // moveable but not copyable
    EffectConditionalForallImpl(const EffectConditionalForallImpl& other) = delete;
    EffectConditionalForallImpl& operator=(const EffectConditionalForallImpl& other) = delete;
    EffectConditionalForallImpl(EffectConditionalForallImpl&& other) = default;
    EffectConditionalForallImpl& operator=(EffectConditionalForallImpl&& other) = default;

    size_t get_index() const;
    const ParameterList& get_parameters() const;
    const Effect& get_effect() const;
};

/* ConditionalWhen */
class EffectConditionalWhenImpl
{
private:
    size_t m_index;
    Condition m_condition;
    Effect m_effect;

    EffectConditionalWhenImpl(size_t index, Condition condition, Effect effect);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

public:
    // moveable but not copyable
    EffectConditionalWhenImpl(const EffectConditionalWhenImpl& other) = delete;
    EffectConditionalWhenImpl& operator=(const EffectConditionalWhenImpl& other) = delete;
    EffectConditionalWhenImpl(EffectConditionalWhenImpl&& other) = default;
    EffectConditionalWhenImpl& operator=(EffectConditionalWhenImpl&& other) = default;

    size_t get_index() const;
    const Condition& get_condition() const;
    const Effect& get_effect() const;
};

template<>
struct UniquePDDLHasher<const EffectLiteralImpl&>
{
    size_t operator()(const EffectLiteralImpl& e) const;
};

template<>
struct UniquePDDLHasher<const EffectAndImpl&>
{
    size_t operator()(const EffectAndImpl& e) const;
};

template<>
struct UniquePDDLHasher<const EffectNumericImpl&>
{
    size_t operator()(const EffectNumericImpl& e) const;
};

template<>
struct UniquePDDLHasher<const EffectConditionalForallImpl&>
{
    size_t operator()(const EffectConditionalForallImpl& e) const;
};

template<>
struct UniquePDDLHasher<const EffectConditionalWhenImpl&>
{
    size_t operator()(const EffectConditionalWhenImpl& e) const;
};

template<>
struct UniquePDDLHasher<const EffectImpl*>
{
    size_t operator()(const EffectImpl* e) const;
};

template<>
struct UniquePDDLEqualTo<const EffectLiteralImpl&>
{
    bool operator()(const EffectLiteralImpl& l, const EffectLiteralImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const EffectAndImpl&>
{
    bool operator()(const EffectAndImpl& l, const EffectAndImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const EffectNumericImpl&>
{
    bool operator()(const EffectNumericImpl& l, const EffectNumericImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const EffectConditionalForallImpl&>
{
    bool operator()(const EffectConditionalForallImpl& l, const EffectConditionalForallImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const EffectConditionalWhenImpl&>
{
    bool operator()(const EffectConditionalWhenImpl& l, const EffectConditionalWhenImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const EffectImpl*>
{
    bool operator()(const EffectImpl* l, const EffectImpl* r) const;
};

extern std::ostream& operator<<(std::ostream& out, const EffectLiteralImpl& element);
extern std::ostream& operator<<(std::ostream& out, const EffectAndImpl& element);
extern std::ostream& operator<<(std::ostream& out, const EffectNumericImpl& element);
extern std::ostream& operator<<(std::ostream& out, const EffectConditionalForallImpl& element);
extern std::ostream& operator<<(std::ostream& out, const EffectConditionalWhenImpl& element);
extern std::ostream& operator<<(std::ostream& out, const EffectImpl& element);

}

#endif
