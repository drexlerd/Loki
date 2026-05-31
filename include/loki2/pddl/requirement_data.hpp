/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PDDL_REQUIREMENT_DATA_HPP_
#define LOKI2_PDDL_REQUIREMENT_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki2/pddl/requirement_index.hpp"

namespace ygg
{

template<>
struct Data<::loki2::pddl::Requirement>
{
    ygg::Index<::loki2::pddl::Requirement> index;
    ::loki2::pddl::RequirementKind kind {};

    Data() = default;
    Data(::loki2::pddl::RequirementKind kind_) : index(), kind(kind_) {}

    void clear() noexcept { ygg::clear(index); kind = {}; }
    auto cista_members() const noexcept { return std::tie(index, kind); }
    auto identifying_members() const noexcept { return std::tie(kind); }
};

}

#endif
