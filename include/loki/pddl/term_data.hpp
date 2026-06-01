/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_TERM_DATA_HPP_
#define LOKI_PDDL_TERM_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/variant.h>
#include "loki/pddl/object_index.hpp"
#include "loki/pddl/term_index.hpp"
#include "loki/pddl/variable_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::pddl::Term>
{
    using Variant = ::cista::offset::variant<ygg::Index<::loki::pddl::Variable>, ygg::Index<::loki::pddl::Object>>;

    ygg::Index<::loki::pddl::Term> index;
    Variant value;

    Data() = default;
    explicit Data(Variant value_) : index(), value(std::move(value_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(value); }
    auto cista_members() const noexcept { return std::tie(index, value); }
    auto identifying_members() const noexcept { return std::tie(value); }
};

}

#endif
