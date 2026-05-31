/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PDDL_EFFECT_NUMERIC_DATA_HPP_
#define LOKI2_PDDL_EFFECT_NUMERIC_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki2/pddl/declarations.hpp"
#include "loki2/pddl/effect_numeric_index.hpp"
#include "loki2/pddl/function_expression_index.hpp"
#include "loki2/pddl/function_skeleton_index.hpp"
#include "loki2/pddl/term_index.hpp"

namespace ygg
{

template<>
struct Data<::loki2::pddl::EffectNumeric>
{
    ygg::Index<::loki2::pddl::EffectNumeric> index;
    ::loki2::pddl::NumericEffectOperator op {};
    ygg::Index<::loki2::pddl::FunctionSkeleton> function;
    ygg::IndexList<::loki2::pddl::Term> terms;
    ygg::Index<::loki2::pddl::FunctionExpression> expression;

    Data() = default;
    Data(::loki2::pddl::NumericEffectOperator op_, ygg::Index<::loki2::pddl::FunctionSkeleton> function_, ygg::IndexList<::loki2::pddl::Term> terms_, ygg::Index<::loki2::pddl::FunctionExpression> expression_) : index(), op(op_), function(function_), terms(std::move(terms_)), expression(expression_) {}

    void clear() noexcept { ygg::clear(index); op = {}; ygg::clear(function); ygg::clear(terms); ygg::clear(expression); }
    auto cista_members() const noexcept { return std::tie(index, op, function, terms, expression); }
    auto identifying_members() const noexcept { return std::tie(op, function, terms, expression); }
};

}

#endif
