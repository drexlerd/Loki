/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_INITIAL_FUNCTION_VALUE_DATA_HPP_
#define LOKI_PDDL_INITIAL_FUNCTION_VALUE_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki/pddl/function_expression_index.hpp"
#include "loki/pddl/function_term_index.hpp"
#include "loki/pddl/initial_function_value_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::pddl::InitialFunctionValue>
{
    ygg::Index<::loki::pddl::InitialFunctionValue> index;
    ygg::Index<::loki::pddl::FunctionTerm> function;
    ygg::Index<::loki::pddl::FunctionExpression> value;

    Data() = default;
    Data(ygg::Index<::loki::pddl::FunctionTerm> function_, ygg::Index<::loki::pddl::FunctionExpression> value_) : index(), function(function_), value(value_) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(function); ygg::clear(value); }
    auto cista_members() const noexcept { return std::tie(index, function, value); }
    auto identifying_members() const noexcept { return std::tie(function, value); }
};

}

#endif
