/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PDDL_TYPE_DATA_HPP_
#define LOKI2_PDDL_TYPE_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/string.h>
#include "loki2/pddl/type_index.hpp"

namespace ygg
{

template<>
struct Data<::loki2::pddl::Type>
{
    ygg::Index<::loki2::pddl::Type> index;
    ::cista::offset::string name;
    ygg::IndexList<::loki2::pddl::Type> bases;

    Data() = default;
    Data(::cista::offset::string name_, ygg::IndexList<::loki2::pddl::Type> bases_ = {}) : index(), name(std::move(name_)), bases(std::move(bases_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(name); ygg::clear(bases); }
    auto cista_members() const noexcept { return std::tie(index, name, bases); }
    auto identifying_members() const noexcept { return std::tie(name, bases); }
};

}

#endif
