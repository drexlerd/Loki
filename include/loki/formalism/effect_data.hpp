/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_FORMALISM_EFFECT_DATA_HPP_
#define LOKI_FORMALISM_EFFECT_DATA_HPP_

#include <tuple>
#include <utility>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/variant.h>
#include "loki/formalism/effect_and_index.hpp"
#include "loki/formalism/effect_forall_index.hpp"
#include "loki/formalism/effect_index.hpp"
#include "loki/formalism/effect_literal_index.hpp"
#include "loki/formalism/effect_numeric_index.hpp"
#include "loki/formalism/effect_one_of_index.hpp"
#include "loki/formalism/effect_probabilistic_index.hpp"
#include "loki/formalism/effect_when_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::formalism::Effect>
{
    using Variant = ::cista::offset::variant<ygg::Index<::loki::formalism::EffectLiteral>,
                                             ygg::Index<::loki::formalism::EffectAnd>,
                                             ygg::Index<::loki::formalism::EffectNumeric>,
                                             ygg::Index<::loki::formalism::EffectForall>,
                                             ygg::Index<::loki::formalism::EffectWhen>,
                                             ygg::Index<::loki::formalism::EffectOneOf>,
                                             ygg::Index<::loki::formalism::EffectProbabilistic>>;
    template<typename C>
    using ViewVariant = std::variant<::ygg::View<ygg::Index<::loki::formalism::EffectLiteral>, C>,
                                     ::ygg::View<ygg::Index<::loki::formalism::EffectAnd>, C>,
                                     ::ygg::View<ygg::Index<::loki::formalism::EffectNumeric>, C>,
                                     ::ygg::View<ygg::Index<::loki::formalism::EffectForall>, C>,
                                     ::ygg::View<ygg::Index<::loki::formalism::EffectWhen>, C>,
                                     ::ygg::View<ygg::Index<::loki::formalism::EffectOneOf>, C>,
                                     ::ygg::View<ygg::Index<::loki::formalism::EffectProbabilistic>, C>>;

    ygg::Index<::loki::formalism::Effect> index;
    Variant value;

    Data() = default;
    explicit Data(Variant value_) : index(), value(std::move(value_)) {}
    template<typename C>
    explicit Data(ViewVariant<C> value_) : index(), value(std::visit([](const auto& view) -> Variant { return Variant(view.get_index()); }, value_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(value); }
    auto cista_members() const noexcept { return std::tie(index, value); }
    auto identifying_members() const noexcept { return std::tie(value); }
};

}

#endif
