/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_MULTI_FUNCTION_EXPRESSION_DATA_HPP_
#define LOKI_PDDL_MULTI_FUNCTION_EXPRESSION_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki/pddl/declarations.hpp"
#include "loki/pddl/function_expression_index.hpp"
#include "loki/pddl/multi_function_expression_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::pddl::MultiFunctionExpression>
{
    ygg::Index<::loki::pddl::MultiFunctionExpression> index;
    ::loki::pddl::MultiArithmeticOperator op {};
    ygg::IndexList<::loki::pddl::FunctionExpression> expressions;

    Data() = default;
    Data(::loki::pddl::MultiArithmeticOperator op_, ygg::IndexList<::loki::pddl::FunctionExpression> expressions_) : index(), op(op_), expressions(std::move(expressions_)) {}

    void clear() noexcept { ygg::clear(index); op = {}; ygg::clear(expressions); }
    auto cista_members() const noexcept { return std::tie(index, op, expressions); }
    auto identifying_members() const noexcept { return std::tie(op, expressions); }
};

}

#endif
