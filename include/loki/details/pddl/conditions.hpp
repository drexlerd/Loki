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

#ifndef LOKI_INCLUDE_LOKI_PDDL_CONDITIONS_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_CONDITIONS_HPP_

#include "loki/details/pddl/base.hpp"
#include "loki/details/pddl/declarations.hpp"

#include <string>

namespace loki
{

/* Literal */
class ConditionLiteralImpl : public Base<ConditionLiteralImpl>
{
private:
    Literal m_literal;

    ConditionLiteralImpl(size_t index, Literal literal);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<ConditionLiteralImpl>;

public:
    const Literal& get_literal() const;
};

template<>
struct ShallowHash<const ConditionLiteralImpl&>
{
    size_t operator()(const ConditionLiteralImpl& e) const;
};

template<>
struct ShallowEqualTo<const ConditionLiteralImpl&>
{
    bool operator()(const ConditionLiteralImpl& l, const ConditionLiteralImpl& r) const;
};

/* And */
class ConditionAndImpl : public Base<ConditionAndImpl>
{
private:
    ConditionList m_conditions;

    ConditionAndImpl(size_t index, ConditionList conditions);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<ConditionAndImpl>;

public:
    const ConditionList& get_conditions() const;
};

template<>
struct ShallowHash<const ConditionAndImpl&>
{
    size_t operator()(const ConditionAndImpl& e) const;
};

template<>
struct ShallowEqualTo<const ConditionAndImpl&>
{
    bool operator()(const ConditionAndImpl& l, const ConditionAndImpl& r) const;
};

/* Or */
class ConditionOrImpl : public Base<ConditionOrImpl>
{
private:
    ConditionList m_conditions;

    ConditionOrImpl(size_t index, ConditionList conditions);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

    bool is_structurally_equivalent_to_impl(const ConditionOrImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<ConditionOrImpl>;

public:
    const ConditionList& get_conditions() const;
};

template<>
struct ShallowHash<const ConditionOrImpl&>
{
    size_t operator()(const ConditionOrImpl& e) const;
};

template<>
struct ShallowEqualTo<const ConditionOrImpl&>
{
    bool operator()(const ConditionOrImpl& l, const ConditionOrImpl& r) const;
};

/* Not */
class ConditionNotImpl : public Base<ConditionNotImpl>
{
private:
    Condition m_condition;

    ConditionNotImpl(size_t index, Condition condition);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<ConditionNotImpl>;

public:
    const Condition& get_condition() const;
};

template<>
struct ShallowHash<const ConditionNotImpl&>
{
    size_t operator()(const ConditionNotImpl& e) const;
};

template<>
struct ShallowEqualTo<const ConditionNotImpl&>
{
    bool operator()(const ConditionNotImpl& l, const ConditionNotImpl& r) const;
};

/* Imply */
class ConditionImplyImpl : public Base<ConditionImplyImpl>
{
private:
    Condition m_condition_left;
    Condition m_condition_right;

    ConditionImplyImpl(size_t index, Condition condition_left, Condition condition_right);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<ConditionImplyImpl>;

public:
    const Condition& get_condition_left() const;
    const Condition& get_condition_right() const;
};

template<>
struct ShallowHash<const ConditionImplyImpl&>
{
    size_t operator()(const ConditionImplyImpl& e) const;
};

template<>
struct ShallowEqualTo<const ConditionImplyImpl&>
{
    bool operator()(const ConditionImplyImpl& l, const ConditionImplyImpl& r) const;
};

/* Exists */
class ConditionExistsImpl : public Base<ConditionExistsImpl>
{
private:
    ParameterList m_parameters;
    Condition m_condition;

    ConditionExistsImpl(size_t index, ParameterList parameters, Condition condition);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<ConditionExistsImpl>;

public:
    const ParameterList& get_parameters() const;
    const Condition& get_condition() const;
};

template<>
struct ShallowHash<const ConditionExistsImpl&>
{
    size_t operator()(const ConditionExistsImpl& e) const;
};

template<>
struct ShallowEqualTo<const ConditionExistsImpl&>
{
    bool operator()(const ConditionExistsImpl& l, const ConditionExistsImpl& r) const;
};

/* Forall */
class ConditionForallImpl : public Base<ConditionForallImpl>
{
private:
    ParameterList m_parameters;
    Condition m_condition;

    ConditionForallImpl(size_t index, ParameterList parameters, Condition condition);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<ConditionForallImpl>;

public:
    const ParameterList& get_parameters() const;
    const Condition& get_condition() const;
};

template<>
struct ShallowHash<const ConditionForallImpl&>
{
    size_t operator()(const ConditionForallImpl& e) const;
};

template<>
struct ShallowEqualTo<const ConditionForallImpl&>
{
    bool operator()(const ConditionForallImpl& l, const ConditionForallImpl& r) const;
};

/* ConditionImpl */

template<>
struct ShallowHash<const ConditionImpl*>
{
    size_t operator()(const ConditionImpl* e) const;
};

template<>
struct ShallowEqualTo<const ConditionImpl*>
{
    bool operator()(const ConditionImpl* l, const ConditionImpl* r) const;
};

}

#endif
