/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PDDL_BINARY_FUNCTION_EXPRESSION_DATA_HPP_
#define LOKI2_PDDL_BINARY_FUNCTION_EXPRESSION_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki2/pddl/binary_function_expression_index.hpp"
#include "loki2/pddl/declarations.hpp"
#include "loki2/pddl/function_expression_index.hpp"

namespace ygg
{

template<>
struct Data<::loki2::pddl::BinaryFunctionExpression>
{
    ygg::Index<::loki2::pddl::BinaryFunctionExpression> index;
    ::loki2::pddl::BinaryArithmeticOperator op {};
    ygg::Index<::loki2::pddl::FunctionExpression> left;
    ygg::Index<::loki2::pddl::FunctionExpression> right;

    Data() = default;
    Data(::loki2::pddl::BinaryArithmeticOperator op_, ygg::Index<::loki2::pddl::FunctionExpression> left_, ygg::Index<::loki2::pddl::FunctionExpression> right_) : index(), op(op_), left(left_), right(right_) {}

    void clear() noexcept { ygg::clear(index); op = {}; ygg::clear(left); ygg::clear(right); }
    auto cista_members() const noexcept { return std::tie(index, op, left, right); }
    auto identifying_members() const noexcept { return std::tie(op, left, right); }
};

}

#endif
