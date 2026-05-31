/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PDDL_EFFECT_PROBABILISTIC_ALTERNATIVE_DATA_HPP_
#define LOKI2_PDDL_EFFECT_PROBABILISTIC_ALTERNATIVE_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki2/pddl/effect_index.hpp"
#include "loki2/pddl/effect_probabilistic_alternative_index.hpp"

namespace ygg
{

template<>
struct Data<::loki2::pddl::EffectProbabilisticAlternative>
{
    ygg::Index<::loki2::pddl::EffectProbabilisticAlternative> index;
    double probability = 0.0;
    ygg::Index<::loki2::pddl::Effect> effect;

    Data() = default;
    Data(double probability_, ygg::Index<::loki2::pddl::Effect> effect_) : index(), probability(probability_), effect(effect_) {}

    void clear() noexcept { ygg::clear(index); probability = 0.0; ygg::clear(effect); }
    auto cista_members() const noexcept { return std::tie(index, probability, effect); }
    auto identifying_members() const noexcept { return std::tie(probability, effect); }
};

}

#endif
