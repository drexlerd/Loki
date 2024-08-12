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

#ifndef LOKI_INCLUDE_LOKI_PDDL_TERM_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_TERM_HPP_

#include "loki/details/pddl/base.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/utils/unique_value_type_factory.hpp"

#include <string>

namespace loki
{

/* TermObjectImpl */

class TermObjectImpl : public Base<TermObjectImpl>
{
private:
    Object m_object;

    TermObjectImpl(size_t index, Object object);

    // Give access to the constructor.
    friend class UniqueValueTypeFactory<TermImpl>;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<TermObjectImpl>;

public:
    const Object& get_object() const;
};

template<>
struct ShallowHash<const TermObjectImpl&>
{
    size_t operator()(const TermObjectImpl& e) const;
};

template<>
struct ShallowEqualTo<const TermObjectImpl&>
{
    bool operator()(const TermObjectImpl& l, const TermObjectImpl& r) const;
};

/* TermVariableImpl */

class TermVariableImpl : public Base<TermVariableImpl>
{
private:
    Variable m_variable;

    TermVariableImpl(size_t index, Variable variable);

    // Give access to the constructor.
    friend class UniqueValueTypeFactory<TermImpl>;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<TermVariableImpl>;

public:
    const Variable& get_variable() const;
};

template<>
struct ShallowHash<const TermVariableImpl&>
{
    size_t operator()(const TermVariableImpl& e) const;
};

template<>
struct ShallowEqualTo<const TermVariableImpl&>
{
    bool operator()(const TermVariableImpl& l, const TermVariableImpl& r) const;
};

/* TermImpl */

template<>
struct ShallowHash<const TermImpl*>
{
    size_t operator()(const TermImpl* e) const;
};

template<>
struct ShallowEqualTo<const TermImpl*>
{
    bool operator()(const TermImpl* l, const TermImpl* r) const;
};

}

#endif
