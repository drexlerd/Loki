/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_FUNCTION_TERM_DATA_HPP_
#define LOKI_PDDL_FUNCTION_TERM_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki/pddl/function_skeleton_index.hpp"
#include "loki/pddl/function_term_index.hpp"
#include "loki/pddl/term_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::pddl::FunctionTerm>
{
    ygg::Index<::loki::pddl::FunctionTerm> index;
    ygg::Index<::loki::pddl::FunctionSkeleton> function;
    ygg::IndexList<::loki::pddl::Term> terms;

    Data() = default;
    Data(ygg::Index<::loki::pddl::FunctionSkeleton> function_, ygg::IndexList<::loki::pddl::Term> terms_ = {}) : index(), function(function_), terms(std::move(terms_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(function); ygg::clear(terms); }
    auto cista_members() const noexcept { return std::tie(index, function, terms); }
    auto identifying_members() const noexcept { return std::tie(function, terms); }
};

}

#endif
