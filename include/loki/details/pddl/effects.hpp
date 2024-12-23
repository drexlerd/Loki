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
    template<HasIdentifiableMembers T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    EffectLiteralImpl(const EffectLiteralImpl& other) = delete;
    EffectLiteralImpl& operator=(const EffectLiteralImpl& other) = delete;
    EffectLiteralImpl(EffectLiteralImpl&& other) = default;
    EffectLiteralImpl& operator=(EffectLiteralImpl&& other) = default;

    size_t get_index() const;
    const Literal& get_literal() const;

    auto identifiable_members() const { return std::forward_as_tuple(std::as_const(m_literal)); }
};

/* And */
class EffectAndImpl
{
private:
    size_t m_index;
    EffectList m_effects;

    EffectAndImpl(size_t index, EffectList effects);

    // Give access to the constructor.
    template<HasIdentifiableMembers T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    EffectAndImpl(const EffectAndImpl& other) = delete;
    EffectAndImpl& operator=(const EffectAndImpl& other) = delete;
    EffectAndImpl(EffectAndImpl&& other) = default;
    EffectAndImpl& operator=(EffectAndImpl&& other) = default;

    size_t get_index() const;
    const EffectList& get_effects() const;

    auto identifiable_members() const { return std::forward_as_tuple(std::as_const(m_effects)); }
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
    template<HasIdentifiableMembers T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

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

    auto identifiable_members() const
    {
        return std::forward_as_tuple(std::as_const(m_assign_operator), std::as_const(m_function), std::as_const(m_function_expression));
    }
};

/* CompositeForall */
class EffectCompositeForallImpl
{
private:
    size_t m_index;
    ParameterList m_parameters;
    Effect m_effect;

    EffectCompositeForallImpl(size_t index, ParameterList parameters, Effect effect);

    // Give access to the constructor.
    template<HasIdentifiableMembers T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    EffectCompositeForallImpl(const EffectCompositeForallImpl& other) = delete;
    EffectCompositeForallImpl& operator=(const EffectCompositeForallImpl& other) = delete;
    EffectCompositeForallImpl(EffectCompositeForallImpl&& other) = default;
    EffectCompositeForallImpl& operator=(EffectCompositeForallImpl&& other) = default;

    size_t get_index() const;
    const ParameterList& get_parameters() const;
    const Effect& get_effect() const;

    auto identifiable_members() const { return std::forward_as_tuple(std::as_const(m_parameters), std::as_const(m_effect)); }
};

/* CompositeWhen */
class EffectCompositeWhenImpl
{
private:
    size_t m_index;
    Condition m_condition;
    Effect m_effect;

    EffectCompositeWhenImpl(size_t index, Condition condition, Effect effect);

    // Give access to the constructor.
    template<HasIdentifiableMembers T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    EffectCompositeWhenImpl(const EffectCompositeWhenImpl& other) = delete;
    EffectCompositeWhenImpl& operator=(const EffectCompositeWhenImpl& other) = delete;
    EffectCompositeWhenImpl(EffectCompositeWhenImpl&& other) = default;
    EffectCompositeWhenImpl& operator=(EffectCompositeWhenImpl&& other) = default;

    size_t get_index() const;
    const Condition& get_condition() const;
    const Effect& get_effect() const;

    auto identifiable_members() const { return std::forward_as_tuple(std::as_const(m_condition), std::as_const(m_effect)); }
};

class EffectCompositeOneofImpl
{
private:
    size_t m_index;
    EffectList m_effects;

    EffectCompositeOneofImpl(size_t index, EffectList effects);

    // Give access to the constructor.
    template<HasIdentifiableMembers T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    EffectCompositeOneofImpl(const EffectCompositeOneofImpl& other) = delete;
    EffectCompositeOneofImpl& operator=(const EffectCompositeOneofImpl& other) = delete;
    EffectCompositeOneofImpl(EffectCompositeOneofImpl&& other) = default;
    EffectCompositeOneofImpl& operator=(EffectCompositeOneofImpl&& other) = default;

    size_t get_index() const;
    const EffectList& get_effects() const;

    auto identifiable_members() const { return std::forward_as_tuple(std::as_const(m_effects)); }
};

/* EffectImpl */
class EffectImpl
{
private:
    size_t m_index;
    std::variant<EffectLiteral, EffectAnd, EffectNumeric, EffectCompositeForall, EffectCompositeWhen, EffectCompositeOneof> m_effect;

    EffectImpl(size_t index, std::variant<EffectLiteral, EffectAnd, EffectNumeric, EffectCompositeForall, EffectCompositeWhen, EffectCompositeOneof> effect);

    // Give access to the constructor.
    template<HasIdentifiableMembers T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    EffectImpl(const EffectImpl& other) = delete;
    EffectImpl& operator=(const EffectImpl& other) = delete;
    EffectImpl(EffectImpl&& other) = default;
    EffectImpl& operator=(EffectImpl&& other) = default;

    size_t get_index() const;
    const std::variant<EffectLiteral, EffectAnd, EffectNumeric, EffectCompositeForall, EffectCompositeWhen, EffectCompositeOneof>& get_effect() const;

    auto identifiable_members() const { return std::forward_as_tuple(std::as_const(m_effect)); }
};

extern std::ostream& operator<<(std::ostream& out, const EffectLiteralImpl& element);
extern std::ostream& operator<<(std::ostream& out, const EffectAndImpl& element);
extern std::ostream& operator<<(std::ostream& out, const EffectNumericImpl& element);
extern std::ostream& operator<<(std::ostream& out, const EffectCompositeForallImpl& element);
extern std::ostream& operator<<(std::ostream& out, const EffectCompositeWhenImpl& element);
extern std::ostream& operator<<(std::ostream& out, const EffectCompositeOneofImpl& element);
extern std::ostream& operator<<(std::ostream& out, const EffectImpl& element);

}

#endif
