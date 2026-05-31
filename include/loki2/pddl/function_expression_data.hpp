/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PDDL_FUNCTION_EXPRESSION_DATA_HPP_
#define LOKI2_PDDL_FUNCTION_EXPRESSION_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/variant.h>
#include "loki2/pddl/binary_function_expression_index.hpp"
#include "loki2/pddl/function_expression_index.hpp"
#include "loki2/pddl/function_expression_number_index.hpp"
#include "loki2/pddl/function_term_index.hpp"
#include "loki2/pddl/multi_function_expression_index.hpp"
#include "loki2/pddl/unary_function_expression_index.hpp"

namespace ygg
{

template<>
struct Data<::loki2::pddl::FunctionExpression>
{
    using Variant = ::cista::offset::variant<ygg::Index<::loki2::pddl::FunctionExpressionNumber>,
                                             ygg::Index<::loki2::pddl::FunctionTerm>,
                                             ygg::Index<::loki2::pddl::UnaryFunctionExpression>,
                                             ygg::Index<::loki2::pddl::BinaryFunctionExpression>,
                                             ygg::Index<::loki2::pddl::MultiFunctionExpression>>;

    ygg::Index<::loki2::pddl::FunctionExpression> index;
    Variant value;

    Data() = default;
    explicit Data(Variant value_) : index(), value(std::move(value_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(value); }
    auto cista_members() const noexcept { return std::tie(index, value); }
    auto identifying_members() const noexcept { return std::tie(value); }
};

}

#endif
