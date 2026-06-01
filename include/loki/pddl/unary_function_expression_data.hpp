/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_UNARY_FUNCTION_EXPRESSION_DATA_HPP_
#define LOKI_PDDL_UNARY_FUNCTION_EXPRESSION_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki/pddl/declarations.hpp"
#include "loki/pddl/function_expression_index.hpp"
#include "loki/pddl/unary_function_expression_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::pddl::UnaryFunctionExpression>
{
    ygg::Index<::loki::pddl::UnaryFunctionExpression> index;
    ::loki::pddl::UnaryArithmeticOperator op {};
    ygg::Index<::loki::pddl::FunctionExpression> expression;

    Data() = default;
    Data(::loki::pddl::UnaryArithmeticOperator op_, ygg::Index<::loki::pddl::FunctionExpression> expression_) : index(), op(op_), expression(expression_) {}

    void clear() noexcept { ygg::clear(index); op = {}; ygg::clear(expression); }
    auto cista_members() const noexcept { return std::tie(index, op, expression); }
    auto identifying_members() const noexcept { return std::tie(op, expression); }
};

}

#endif
