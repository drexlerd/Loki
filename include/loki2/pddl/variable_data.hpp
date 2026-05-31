/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PDDL_VARIABLE_DATA_HPP_
#define LOKI2_PDDL_VARIABLE_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/string.h>
#include "loki2/pddl/variable_index.hpp"

namespace ygg
{

template<>
struct Data<::loki2::pddl::Variable>
{
    ygg::Index<::loki2::pddl::Variable> index;
    ::cista::offset::string name;

    Data() = default;
    Data(::cista::offset::string name_) : index(), name(std::move(name_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(name); }
    auto cista_members() const noexcept { return std::tie(index, name); }
    auto identifying_members() const noexcept { return std::tie(name); }
};

}

#endif
