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

#include "loki/pddl/base.hpp"
#include "loki/pddl/declarations.hpp"
#include "loki/pddl/factory.hpp"

#include <string>

namespace loki
{

/* Literal */
class ConditionLiteralImpl : public Base<ConditionLiteralImpl>
{
private:
    Literal m_literal;

    ConditionLiteralImpl(int identifier, Literal literal);

    // Give access to the constructor.
    friend class PDDLFactory<ConditionImpl, Hash<ConditionImpl*>, EqualTo<ConditionImpl*>>;

    bool is_structurally_equivalent_to_impl(const ConditionLiteralImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<ConditionLiteralImpl>;

public:
    const Literal& get_literal() const;
};

/* And */
class ConditionAndImpl : public Base<ConditionAndImpl>
{
private:
    ConditionList m_conditions;

    ConditionAndImpl(int identifier, ConditionList conditions);

    // Give access to the constructor.
    friend class PDDLFactory<ConditionImpl, Hash<ConditionImpl*>, EqualTo<ConditionImpl*>>;

    bool is_structurally_equivalent_to_impl(const ConditionAndImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<ConditionAndImpl>;

public:
    const ConditionList& get_conditions() const;
};

/* Or */
class ConditionOrImpl : public Base<ConditionOrImpl>
{
private:
    ConditionList m_conditions;

    ConditionOrImpl(int identifier, ConditionList conditions);

    // Give access to the constructor.
    friend class PDDLFactory<ConditionImpl, Hash<ConditionImpl*>, EqualTo<ConditionImpl*>>;

    bool is_structurally_equivalent_to_impl(const ConditionOrImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<ConditionOrImpl>;

public:
    const ConditionList& get_conditions() const;
};

/* Not */
class ConditionNotImpl : public Base<ConditionNotImpl>
{
private:
    Condition m_condition;

    ConditionNotImpl(int identifier, Condition condition);

    // Give access to the constructor.
    friend class PDDLFactory<ConditionImpl, Hash<ConditionImpl*>, EqualTo<ConditionImpl*>>;

    bool is_structurally_equivalent_to_impl(const ConditionNotImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<ConditionNotImpl>;

public:
    const Condition& get_condition() const;
};

/* Imply */
class ConditionImplyImpl : public Base<ConditionNotImpl>
{
private:
    Condition m_condition_left;
    Condition m_condition_right;

    ConditionImplyImpl(int identifier, Condition condition_left, Condition condition_right);

    // Give access to the constructor.
    friend class PDDLFactory<ConditionImpl, Hash<ConditionImpl*>, EqualTo<ConditionImpl*>>;

    bool is_structurally_equivalent_to_impl(const ConditionImplyImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<ConditionImplyImpl>;

public:
    const Condition& get_condition_left() const;
    const Condition& get_condition_right() const;
};

/* Exists */
class ConditionExistsImpl : public Base<ConditionExistsImpl>
{
private:
    ParameterList m_parameters;
    Condition m_condition;

    ConditionExistsImpl(int identifier, ParameterList parameters, Condition condition);

    // Give access to the constructor.
    friend class PDDLFactory<ConditionImpl, Hash<ConditionImpl*>, EqualTo<ConditionImpl*>>;

    bool is_structurally_equivalent_to_impl(const ConditionExistsImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<ConditionExistsImpl>;

public:
    const ParameterList& get_parameters() const;
    const Condition& get_condition() const;
};

/* Forall */
class ConditionForallImpl : public Base<ConditionForallImpl>
{
private:
    ParameterList m_parameters;
    Condition m_condition;

    ConditionForallImpl(int identifier, ParameterList parameters, Condition condition);

    // Give access to the constructor.
    friend class PDDLFactory<ConditionImpl, Hash<ConditionImpl*>, EqualTo<ConditionImpl*>>;

    bool is_structurally_equivalent_to_impl(const ConditionForallImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<ConditionForallImpl>;

public:
    const ParameterList& get_parameters() const;
    const Condition& get_condition() const;
};

}

#endif
