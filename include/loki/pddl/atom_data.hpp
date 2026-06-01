/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_ATOM_DATA_HPP_
#define LOKI_PDDL_ATOM_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki/pddl/atom_index.hpp"
#include "loki/pddl/predicate_index.hpp"
#include "loki/pddl/term_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::pddl::Atom>
{
    ygg::Index<::loki::pddl::Atom> index;
    ygg::Index<::loki::pddl::Predicate> predicate;
    ygg::IndexList<::loki::pddl::Term> terms;

    Data() = default;
    Data(ygg::Index<::loki::pddl::Predicate> predicate_, ygg::IndexList<::loki::pddl::Term> terms_ = {}) : index(), predicate(predicate_), terms(std::move(terms_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(predicate); ygg::clear(terms); }
    auto cista_members() const noexcept { return std::tie(index, predicate, terms); }
    auto identifying_members() const noexcept { return std::tie(predicate, terms); }
};

}

#endif
