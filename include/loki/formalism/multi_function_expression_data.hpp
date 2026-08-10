/*
 * Copyright (C) 2024-2026 Dominik Drexler
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

#ifndef LOKI_FORMALISM_MULTI_FUNCTION_EXPRESSION_DATA_HPP_
#define LOKI_FORMALISM_MULTI_FUNCTION_EXPRESSION_DATA_HPP_

#include "loki/formalism/declarations.hpp"
#include "loki/formalism/function_expression_index.hpp"
#include "loki/formalism/multi_function_expression_index.hpp"

#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>

namespace ygg
{

template<>
struct Data<::loki::formalism::MultiFunctionExpression>
{
    ygg::Index<::loki::formalism::MultiFunctionExpression> index;
    ::loki::formalism::MultiArithmeticOperator op {};
    ygg::Index<::loki::formalism::FunctionExpression> first;
    ygg::Index<::loki::formalism::FunctionExpression> second;
    ygg::IndexList<::loki::formalism::FunctionExpression> remaining;

    Data() = default;
    Data(::loki::formalism::MultiArithmeticOperator op_,
         ygg::Index<::loki::formalism::FunctionExpression> first_,
         ygg::Index<::loki::formalism::FunctionExpression> second_,
         ygg::IndexList<::loki::formalism::FunctionExpression> remaining_ = {}) :
        index(),
        op(op_),
        first(first_),
        second(second_),
        remaining(std::move(remaining_))
    {
    }
    template<typename C>
    Data(::loki::formalism::MultiArithmeticOperator op_,
         ::ygg::View<ygg::Index<::loki::formalism::FunctionExpression>, C> first_,
         ::ygg::View<ygg::Index<::loki::formalism::FunctionExpression>, C> second_,
         const std::vector<::ygg::View<ygg::Index<::loki::formalism::FunctionExpression>, C>>& remaining_ = {}) :
        index(),
        op(op_),
        first(),
        second(),
        remaining()
    {
        set(first_, first);
        set(second_, second);
        set(remaining_, remaining);
    }

    void clear() noexcept
    {
        ygg::clear(index);
        op = {};
        ygg::clear(first);
        ygg::clear(second);
        ygg::clear(remaining);
    }
    auto cista_members() const noexcept { return std::tie(index, op, first, second, remaining); }
    auto identifying_members() const noexcept { return std::tie(op, first, second, remaining); }
};

}

#endif
