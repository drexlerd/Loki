/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_CONDITION_NUMERIC_CONSTRAINT_DATA_HPP_
#define LOKI_PDDL_CONDITION_NUMERIC_CONSTRAINT_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki/pddl/condition_numeric_constraint_index.hpp"
#include "loki/pddl/declarations.hpp"
#include "loki/pddl/function_expression_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::pddl::ConditionNumericConstraint>
{
    ygg::Index<::loki::pddl::ConditionNumericConstraint> index;
    ::loki::pddl::BinaryComparator comparator {};
    ygg::Index<::loki::pddl::FunctionExpression> left;
    ygg::Index<::loki::pddl::FunctionExpression> right;

    Data() = default;
    Data(::loki::pddl::BinaryComparator comparator_, ygg::Index<::loki::pddl::FunctionExpression> left_, ygg::Index<::loki::pddl::FunctionExpression> right_) : index(), comparator(comparator_), left(left_), right(right_) {}

    void clear() noexcept { ygg::clear(index); comparator = {}; ygg::clear(left); ygg::clear(right); }
    auto cista_members() const noexcept { return std::tie(index, comparator, left, right); }
    auto identifying_members() const noexcept { return std::tie(comparator, left, right); }
};

}

#endif
