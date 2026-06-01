/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_EFFECT_DATA_HPP_
#define LOKI_PDDL_EFFECT_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/variant.h>
#include "loki/pddl/effect_and_index.hpp"
#include "loki/pddl/effect_forall_index.hpp"
#include "loki/pddl/effect_index.hpp"
#include "loki/pddl/effect_literal_index.hpp"
#include "loki/pddl/effect_numeric_index.hpp"
#include "loki/pddl/effect_one_of_index.hpp"
#include "loki/pddl/effect_probabilistic_index.hpp"
#include "loki/pddl/effect_when_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::pddl::Effect>
{
    using Variant = ::cista::offset::variant<ygg::Index<::loki::pddl::EffectLiteral>,
                                             ygg::Index<::loki::pddl::EffectAnd>,
                                             ygg::Index<::loki::pddl::EffectNumeric>,
                                             ygg::Index<::loki::pddl::EffectForall>,
                                             ygg::Index<::loki::pddl::EffectWhen>,
                                             ygg::Index<::loki::pddl::EffectOneOf>,
                                             ygg::Index<::loki::pddl::EffectProbabilistic>>;

    ygg::Index<::loki::pddl::Effect> index;
    Variant value;

    Data() = default;
    explicit Data(Variant value_) : index(), value(std::move(value_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(value); }
    auto cista_members() const noexcept { return std::tie(index, value); }
    auto identifying_members() const noexcept { return std::tie(value); }
};

}

#endif
