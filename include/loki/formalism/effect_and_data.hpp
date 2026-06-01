/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_FORMALISM_EFFECT_AND_DATA_HPP_
#define LOKI_FORMALISM_EFFECT_AND_DATA_HPP_

#include <tuple>
#include <utility>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki/formalism/effect_and_index.hpp"
#include "loki/formalism/effect_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::formalism::EffectAnd>
{
    ygg::Index<::loki::formalism::EffectAnd> index;
    ygg::IndexList<::loki::formalism::Effect> effects;

    Data() = default;
    Data(ygg::IndexList<::loki::formalism::Effect> effects_) : index(), effects(std::move(effects_)) {}
    template<typename C>
    Data(const std::vector<::ygg::View<ygg::Index<::loki::formalism::Effect>, C>>& effects_) : index(), effects()
    {
        set(effects_, effects);
    }

    void clear() noexcept { ygg::clear(index); ygg::clear(effects); }
    auto cista_members() const noexcept { return std::tie(index, effects); }
    auto identifying_members() const noexcept { return std::tie(effects); }
};

}

#endif
