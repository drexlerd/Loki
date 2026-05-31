/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PDDL_EFFECT_DATA_HPP_
#define LOKI2_PDDL_EFFECT_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/variant.h>
#include "loki2/pddl/effect_and_index.hpp"
#include "loki2/pddl/effect_forall_index.hpp"
#include "loki2/pddl/effect_index.hpp"
#include "loki2/pddl/effect_literal_index.hpp"
#include "loki2/pddl/effect_numeric_index.hpp"
#include "loki2/pddl/effect_one_of_index.hpp"
#include "loki2/pddl/effect_probabilistic_index.hpp"
#include "loki2/pddl/effect_when_index.hpp"

namespace ygg
{

template<>
struct Data<::loki2::pddl::Effect>
{
    using Variant = ::cista::offset::variant<ygg::Index<::loki2::pddl::EffectLiteral>,
                                             ygg::Index<::loki2::pddl::EffectAnd>,
                                             ygg::Index<::loki2::pddl::EffectNumeric>,
                                             ygg::Index<::loki2::pddl::EffectForall>,
                                             ygg::Index<::loki2::pddl::EffectWhen>,
                                             ygg::Index<::loki2::pddl::EffectOneOf>,
                                             ygg::Index<::loki2::pddl::EffectProbabilistic>>;

    ygg::Index<::loki2::pddl::Effect> index;
    Variant value;

    Data() = default;
    explicit Data(Variant value_) : index(), value(std::move(value_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(value); }
    auto cista_members() const noexcept { return std::tie(index, value); }
    auto identifying_members() const noexcept { return std::tie(value); }
};

}

#endif
