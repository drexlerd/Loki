/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PDDL_LITERAL_DATA_HPP_
#define LOKI2_PDDL_LITERAL_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki2/pddl/atom_index.hpp"
#include "loki2/pddl/literal_index.hpp"

namespace ygg
{

template<>
struct Data<::loki2::pddl::Literal>
{
    ygg::Index<::loki2::pddl::Literal> index;
    bool positive = true;
    ygg::Index<::loki2::pddl::Atom> atom;

    Data() = default;
    Data(bool positive_, ygg::Index<::loki2::pddl::Atom> atom_) : index(), positive(positive_), atom(atom_) {}

    void clear() noexcept { ygg::clear(index); positive = true; ygg::clear(atom); }
    auto cista_members() const noexcept { return std::tie(index, positive, atom); }
    auto identifying_members() const noexcept { return std::tie(positive, atom); }
};

}

#endif
