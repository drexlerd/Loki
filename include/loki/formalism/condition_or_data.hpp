/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_FORMALISM_CONDITION_OR_DATA_HPP_
#define LOKI_FORMALISM_CONDITION_OR_DATA_HPP_

#include <tuple>
#include <utility>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki/formalism/condition_index.hpp"
#include "loki/formalism/condition_or_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::formalism::ConditionOr>
{
    ygg::Index<::loki::formalism::ConditionOr> index;
    ygg::IndexList<::loki::formalism::Condition> conditions;

    Data() = default;
    Data(ygg::IndexList<::loki::formalism::Condition> conditions_) : index(), conditions(std::move(conditions_)) {}
    template<typename C>
    Data(const std::vector<::ygg::View<ygg::Index<::loki::formalism::Condition>, C>>& conditions_) : index(), conditions()
    {
        set(conditions_, conditions);
    }

    void clear() noexcept { ygg::clear(index); ygg::clear(conditions); }
    auto cista_members() const noexcept { return std::tie(index, conditions); }
    auto identifying_members() const noexcept { return std::tie(conditions); }
};

}

#endif
