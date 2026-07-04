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


#ifndef LOKI_FORMALISM_FUNCTION_EXPRESSION_DATA_HPP_
#define LOKI_FORMALISM_FUNCTION_EXPRESSION_DATA_HPP_

#include <tuple>
#include <utility>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/variant.h>
#include "loki/formalism/binary_function_expression_index.hpp"
#include "loki/formalism/function_expression_index.hpp"
#include "loki/formalism/function_expression_number_index.hpp"
#include "loki/formalism/function_term_index.hpp"
#include "loki/formalism/multi_function_expression_index.hpp"
#include "loki/formalism/unary_function_expression_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::formalism::FunctionExpression>
{
    using Variant = ::cista::offset::variant<ygg::Index<::loki::formalism::FunctionExpressionNumber>,
                                             ygg::Index<::loki::formalism::FunctionTerm>,
                                             ygg::Index<::loki::formalism::UnaryFunctionExpression>,
                                             ygg::Index<::loki::formalism::BinaryFunctionExpression>,
                                             ygg::Index<::loki::formalism::MultiFunctionExpression>>;
    template<typename C>
    using ViewVariant = std::variant<::ygg::View<ygg::Index<::loki::formalism::FunctionExpressionNumber>, C>,
                                     ::ygg::View<ygg::Index<::loki::formalism::FunctionTerm>, C>,
                                     ::ygg::View<ygg::Index<::loki::formalism::UnaryFunctionExpression>, C>,
                                     ::ygg::View<ygg::Index<::loki::formalism::BinaryFunctionExpression>, C>,
                                     ::ygg::View<ygg::Index<::loki::formalism::MultiFunctionExpression>, C>>;

    ygg::Index<::loki::formalism::FunctionExpression> index;
    Variant value;

    Data() = default;
    explicit Data(Variant value_) : index(), value(std::move(value_)) {}
    template<typename C>
    explicit Data(ViewVariant<C> value_) : index(), value(std::visit([](const auto& view) -> Variant { return Variant(view.get_index()); }, value_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(value); }
    auto cista_members() const noexcept { return std::tie(index, value); }
    auto identifying_members() const noexcept { return std::tie(value); }
};

}

#endif
