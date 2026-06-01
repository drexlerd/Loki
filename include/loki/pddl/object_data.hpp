/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_OBJECT_DATA_HPP_
#define LOKI_PDDL_OBJECT_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/string.h>
#include "loki/pddl/object_index.hpp"
#include "loki/pddl/type_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::pddl::Object>
{
    ygg::Index<::loki::pddl::Object> index;
    ::cista::offset::string name;
    ygg::IndexList<::loki::pddl::Type> types;

    Data() = default;
    Data(::cista::offset::string name_, ygg::IndexList<::loki::pddl::Type> types_ = {}) : index(), name(std::move(name_)), types(std::move(types_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(name); ygg::clear(types); }
    auto cista_members() const noexcept { return std::tie(index, name, types); }
    auto identifying_members() const noexcept { return std::tie(name, types); }
};

}

#endif
