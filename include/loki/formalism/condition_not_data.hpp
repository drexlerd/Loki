/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_FORMALISM_CONDITION_NOT_DATA_HPP_
#define LOKI_FORMALISM_CONDITION_NOT_DATA_HPP_

#include <tuple>
#include <utility>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki/formalism/condition_index.hpp"
#include "loki/formalism/condition_not_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::formalism::ConditionNot>
{
    ygg::Index<::loki::formalism::ConditionNot> index;
    ygg::Index<::loki::formalism::Condition> condition;

    Data() = default;
    Data(ygg::Index<::loki::formalism::Condition> condition_) : index(), condition(condition_) {}
    template<typename C>
    Data(::ygg::View<ygg::Index<::loki::formalism::Condition>, C> condition_) : index(), condition()
    {
        set(condition_, condition);
    }

    void clear() noexcept { ygg::clear(index); ygg::clear(condition); }
    auto cista_members() const noexcept { return std::tie(index, condition); }
    auto identifying_members() const noexcept { return std::tie(condition); }
};

}

#endif
