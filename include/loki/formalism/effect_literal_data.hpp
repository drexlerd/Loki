/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_FORMALISM_EFFECT_LITERAL_DATA_HPP_
#define LOKI_FORMALISM_EFFECT_LITERAL_DATA_HPP_

#include <tuple>
#include <utility>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki/formalism/effect_literal_index.hpp"
#include "loki/formalism/literal_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::formalism::EffectLiteral>
{
    ygg::Index<::loki::formalism::EffectLiteral> index;
    ygg::Index<::loki::formalism::Literal> literal;

    Data() = default;
    Data(ygg::Index<::loki::formalism::Literal> literal_) : index(), literal(literal_) {}
    template<typename C>
    Data(::ygg::View<ygg::Index<::loki::formalism::Literal>, C> literal_) : index(), literal()
    {
        set(literal_, literal);
    }

    void clear() noexcept { ygg::clear(index); ygg::clear(literal); }
    auto cista_members() const noexcept { return std::tie(index, literal); }
    auto identifying_members() const noexcept { return std::tie(literal); }
};

}

#endif
