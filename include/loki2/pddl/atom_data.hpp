/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PDDL_ATOM_DATA_HPP_
#define LOKI2_PDDL_ATOM_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki2/pddl/atom_index.hpp"
#include "loki2/pddl/predicate_index.hpp"
#include "loki2/pddl/term_index.hpp"

namespace ygg
{

template<>
struct Data<::loki2::pddl::Atom>
{
    ygg::Index<::loki2::pddl::Atom> index;
    ygg::Index<::loki2::pddl::Predicate> predicate;
    ygg::IndexList<::loki2::pddl::Term> terms;

    Data() = default;
    Data(ygg::Index<::loki2::pddl::Predicate> predicate_, ygg::IndexList<::loki2::pddl::Term> terms_ = {}) : index(), predicate(predicate_), terms(std::move(terms_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(predicate); ygg::clear(terms); }
    auto cista_members() const noexcept { return std::tie(index, predicate, terms); }
    auto identifying_members() const noexcept { return std::tie(predicate, terms); }
};

}

#endif
