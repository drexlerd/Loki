/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_CONDITION_IMPLY_DATA_HPP_
#define LOKI_PDDL_CONDITION_IMPLY_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki/pddl/condition_imply_index.hpp"
#include "loki/pddl/condition_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::pddl::ConditionImply>
{
    ygg::Index<::loki::pddl::ConditionImply> index;
    ygg::Index<::loki::pddl::Condition> left;
    ygg::Index<::loki::pddl::Condition> right;

    Data() = default;
    Data(ygg::Index<::loki::pddl::Condition> left_, ygg::Index<::loki::pddl::Condition> right_) : index(), left(left_), right(right_) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(left); ygg::clear(right); }
    auto cista_members() const noexcept { return std::tie(index, left, right); }
    auto identifying_members() const noexcept { return std::tie(left, right); }
};

}

#endif
