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

#ifndef LOKI_INCLUDE_LOKI_PDDL_FUNCTION_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_FUNCTION_HPP_

#include "loki/details/pddl/base.hpp"
#include "loki/details/pddl/declarations.hpp"

#include <string>

namespace loki
{
class FunctionImpl : public Base<FunctionImpl>
{
private:
    FunctionSkeleton m_function_skeleton;
    TermList m_terms;

    FunctionImpl(size_t index, FunctionSkeleton function_skeleton, TermList terms);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<FunctionImpl>;

public:
    const FunctionSkeleton& get_function_skeleton() const;
    const TermList& get_terms() const;
};

template<>
struct ShallowHash<const FunctionImpl*>
{
    size_t operator()(const FunctionImpl* e) const;
};

template<>
struct ShallowEqualTo<const FunctionImpl*>
{
    bool operator()(const FunctionImpl* l, const FunctionImpl* r) const;
};

}

#endif
