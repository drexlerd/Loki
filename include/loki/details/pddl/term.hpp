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
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

public:
    const Object& get_object() const;
};

/* TermVariableImpl */

class TermVariableImpl : public Base<TermVariableImpl>
{
private:
    Variable m_variable;

    TermVariableImpl(size_t index, Variable variable);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

public:
    const Variable& get_variable() const;
};

}

#endif
