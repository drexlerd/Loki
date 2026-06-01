/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_PREDICATE_DATA_HPP_
#define LOKI_PDDL_PREDICATE_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/string.h>
#include "loki/pddl/parameter_index.hpp"
#include "loki/pddl/predicate_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::pddl::Predicate>
{
    ygg::Index<::loki::pddl::Predicate> index;
    ::cista::offset::string name;
    ygg::IndexList<::loki::pddl::Parameter> parameters;

    Data() = default;
    Data(::cista::offset::string name_, ygg::IndexList<::loki::pddl::Parameter> parameters_ = {}) : index(), name(std::move(name_)), parameters(std::move(parameters_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(name); ygg::clear(parameters); }
    auto cista_members() const noexcept { return std::tie(index, name, parameters); }
    auto identifying_members() const noexcept { return std::tie(name, parameters); }
};

}

#endif
