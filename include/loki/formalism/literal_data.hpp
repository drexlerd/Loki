/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_FORMALISM_LITERAL_DATA_HPP_
#define LOKI_FORMALISM_LITERAL_DATA_HPP_

#include <tuple>
#include <utility>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki/formalism/atom_index.hpp"
#include "loki/formalism/literal_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::formalism::Literal>
{
    ygg::Index<::loki::formalism::Literal> index;
    bool positive = true;
    ygg::Index<::loki::formalism::Atom> atom;

    Data() = default;
    Data(bool positive_, ygg::Index<::loki::formalism::Atom> atom_) : index(), positive(positive_), atom(atom_) {}
    template<typename C>
    Data(bool positive_, ::ygg::View<ygg::Index<::loki::formalism::Atom>, C> atom_) : index(), positive(positive_), atom()
    {
        set(atom_, atom);
    }

    void clear() noexcept { ygg::clear(index); positive = true; ygg::clear(atom); }
    auto cista_members() const noexcept { return std::tie(index, positive, atom); }
    auto identifying_members() const noexcept { return std::tie(positive, atom); }
};

}

#endif
