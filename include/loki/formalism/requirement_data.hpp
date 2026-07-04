/*
 * Copyright (C) 2024-2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef LOKI_FORMALISM_REQUIREMENT_DATA_HPP_
#define LOKI_FORMALISM_REQUIREMENT_DATA_HPP_

#include <tuple>
#include <utility>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki/formalism/requirement_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::formalism::Requirement>
{
    ygg::Index<::loki::formalism::Requirement> index;
    ::loki::formalism::RequirementKind kind {};

    Data() = default;
    Data(::loki::formalism::RequirementKind kind_) : index(), kind(kind_) {}

    void clear() noexcept { ygg::clear(index); kind = {}; }
    auto cista_members() const noexcept { return std::tie(index, kind); }
    auto identifying_members() const noexcept { return std::tie(kind); }
};

}

#endif
