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

#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/equal_to.hpp"
#include "loki/details/pddl/hash.hpp"

#include <string>

namespace loki
{

/* TermObjectImpl */

class TermObjectImpl
{
private:
    size_t m_index;
    Object m_object;

    TermObjectImpl(size_t index, Object object);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

public:
    // moveable but not copyable
    TermObjectImpl(const TermObjectImpl& other) = delete;
    TermObjectImpl& operator=(const TermObjectImpl& other) = delete;
    TermObjectImpl(TermObjectImpl&& other) = default;
    TermObjectImpl& operator=(TermObjectImpl&& other) = default;

    size_t get_index() const;
    const Object& get_object() const;
};

/* TermVariableImpl */

class TermVariableImpl
{
private:
    size_t m_index;
    Variable m_variable;

    TermVariableImpl(size_t index, Variable variable);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

public:
    // moveable but not copyable
    TermVariableImpl(const TermVariableImpl& other) = delete;
    TermVariableImpl& operator=(const TermVariableImpl& other) = delete;
    TermVariableImpl(TermVariableImpl&& other) = default;
    TermVariableImpl& operator=(TermVariableImpl&& other) = default;

    size_t get_index() const;
    const Variable& get_variable() const;
};

template<>
struct UniquePDDLHasher<const TermObjectImpl&>
{
    size_t operator()(const TermObjectImpl& e) const;
};

template<>
struct UniquePDDLHasher<const TermVariableImpl&>
{
    size_t operator()(const TermVariableImpl& e) const;
};

template<>
struct UniquePDDLHasher<const TermImpl*>
{
    size_t operator()(const TermImpl* e) const;
};

template<>
struct UniquePDDLEqualTo<const TermObjectImpl&>
{
    bool operator()(const TermObjectImpl& l, const TermObjectImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const TermVariableImpl&>
{
    bool operator()(const TermVariableImpl& l, const TermVariableImpl& r) const;
};

template<>
struct UniquePDDLEqualTo<const TermImpl*>
{
    bool operator()(const TermImpl* l, const TermImpl* r) const;
};

extern std::ostream& operator<<(std::ostream& out, const TermObjectImpl& element);
extern std::ostream& operator<<(std::ostream& out, const TermVariableImpl& element);
extern std::ostream& operator<<(std::ostream& out, const TermImpl& element);

}

#endif
