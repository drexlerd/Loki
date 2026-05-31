/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PDDL_PARAMETER_DATA_HPP_
#define LOKI2_PDDL_PARAMETER_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki2/pddl/parameter_index.hpp"
#include "loki2/pddl/type_index.hpp"
#include "loki2/pddl/variable_index.hpp"

namespace ygg
{

template<>
struct Data<::loki2::pddl::Parameter>
{
    ygg::Index<::loki2::pddl::Parameter> index;
    ygg::Index<::loki2::pddl::Variable> variable;
    ygg::IndexList<::loki2::pddl::Type> types;

    Data() = default;
    Data(ygg::Index<::loki2::pddl::Variable> variable_, ygg::IndexList<::loki2::pddl::Type> types_ = {}) : index(), variable(variable_), types(std::move(types_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(variable); ygg::clear(types); }
    auto cista_members() const noexcept { return std::tie(index, variable, types); }
    auto identifying_members() const noexcept { return std::tie(variable, types); }
};

}

#endif
