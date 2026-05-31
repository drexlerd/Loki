/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PDDL_CONDITION_NOT_DATA_HPP_
#define LOKI2_PDDL_CONDITION_NOT_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki2/pddl/condition_index.hpp"
#include "loki2/pddl/condition_not_index.hpp"

namespace ygg
{

template<>
struct Data<::loki2::pddl::ConditionNot>
{
    ygg::Index<::loki2::pddl::ConditionNot> index;
    ygg::Index<::loki2::pddl::Condition> condition;

    Data() = default;
    Data(ygg::Index<::loki2::pddl::Condition> condition_) : index(), condition(condition_) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(condition); }
    auto cista_members() const noexcept { return std::tie(index, condition); }
    auto identifying_members() const noexcept { return std::tie(condition); }
};

}

#endif
