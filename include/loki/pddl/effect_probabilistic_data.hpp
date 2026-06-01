/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_EFFECT_PROBABILISTIC_DATA_HPP_
#define LOKI_PDDL_EFFECT_PROBABILISTIC_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki/pddl/effect_probabilistic_alternative_index.hpp"
#include "loki/pddl/effect_probabilistic_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::pddl::EffectProbabilistic>
{
    ygg::Index<::loki::pddl::EffectProbabilistic> index;
    ygg::IndexList<::loki::pddl::EffectProbabilisticAlternative> alternatives;

    Data() = default;
    Data(ygg::IndexList<::loki::pddl::EffectProbabilisticAlternative> alternatives_) : index(), alternatives(std::move(alternatives_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(alternatives); }
    auto cista_members() const noexcept { return std::tie(index, alternatives); }
    auto identifying_members() const noexcept { return std::tie(alternatives); }
};

}

#endif
