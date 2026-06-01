/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_EFFECT_WHEN_DATA_HPP_
#define LOKI_PDDL_EFFECT_WHEN_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki/pddl/condition_index.hpp"
#include "loki/pddl/effect_index.hpp"
#include "loki/pddl/effect_when_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::pddl::EffectWhen>
{
    ygg::Index<::loki::pddl::EffectWhen> index;
    ygg::Index<::loki::pddl::Condition> condition;
    ygg::Index<::loki::pddl::Effect> effect;

    Data() = default;
    Data(ygg::Index<::loki::pddl::Condition> condition_, ygg::Index<::loki::pddl::Effect> effect_) : index(), condition(condition_), effect(effect_) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(condition); ygg::clear(effect); }
    auto cista_members() const noexcept { return std::tie(index, condition, effect); }
    auto identifying_members() const noexcept { return std::tie(condition, effect); }
};

}

#endif
