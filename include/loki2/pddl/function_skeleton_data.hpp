/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PDDL_FUNCTION_SKELETON_DATA_HPP_
#define LOKI2_PDDL_FUNCTION_SKELETON_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/string.h>
#include "loki2/pddl/function_skeleton_index.hpp"
#include "loki2/pddl/parameter_index.hpp"
#include "loki2/pddl/type_index.hpp"

namespace ygg
{

template<>
struct Data<::loki2::pddl::FunctionSkeleton>
{
    ygg::Index<::loki2::pddl::FunctionSkeleton> index;
    ::cista::offset::string name;
    ygg::IndexList<::loki2::pddl::Parameter> parameters;
    ygg::Index<::loki2::pddl::Type> type;

    Data() = default;
    Data(::cista::offset::string name_, ygg::IndexList<::loki2::pddl::Parameter> parameters_, ygg::Index<::loki2::pddl::Type> type_) : index(), name(std::move(name_)), parameters(std::move(parameters_)), type(type_) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(name); ygg::clear(parameters); ygg::clear(type); }
    auto cista_members() const noexcept { return std::tie(index, name, parameters, type); }
    auto identifying_members() const noexcept { return std::tie(name, parameters, type); }
};

}

#endif
