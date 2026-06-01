/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_FORMALISM_CONDITION_DATA_HPP_
#define LOKI_FORMALISM_CONDITION_DATA_HPP_

#include <tuple>
#include <utility>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/variant.h>
#include "loki/formalism/condition_and_index.hpp"
#include "loki/formalism/condition_exists_index.hpp"
#include "loki/formalism/condition_forall_index.hpp"
#include "loki/formalism/condition_imply_index.hpp"
#include "loki/formalism/condition_index.hpp"
#include "loki/formalism/condition_literal_index.hpp"
#include "loki/formalism/condition_not_index.hpp"
#include "loki/formalism/condition_numeric_constraint_index.hpp"
#include "loki/formalism/condition_or_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::formalism::Condition>
{
    using Variant = ::cista::offset::variant<ygg::Index<::loki::formalism::ConditionLiteral>,
                                             ygg::Index<::loki::formalism::ConditionAnd>,
                                             ygg::Index<::loki::formalism::ConditionOr>,
                                             ygg::Index<::loki::formalism::ConditionNot>,
                                             ygg::Index<::loki::formalism::ConditionImply>,
                                             ygg::Index<::loki::formalism::ConditionExists>,
                                             ygg::Index<::loki::formalism::ConditionForall>,
                                             ygg::Index<::loki::formalism::ConditionNumericConstraint>>;
    template<typename C>
    using ViewVariant = std::variant<::ygg::View<ygg::Index<::loki::formalism::ConditionLiteral>, C>,
                                     ::ygg::View<ygg::Index<::loki::formalism::ConditionAnd>, C>,
                                     ::ygg::View<ygg::Index<::loki::formalism::ConditionOr>, C>,
                                     ::ygg::View<ygg::Index<::loki::formalism::ConditionNot>, C>,
                                     ::ygg::View<ygg::Index<::loki::formalism::ConditionImply>, C>,
                                     ::ygg::View<ygg::Index<::loki::formalism::ConditionExists>, C>,
                                     ::ygg::View<ygg::Index<::loki::formalism::ConditionForall>, C>,
                                     ::ygg::View<ygg::Index<::loki::formalism::ConditionNumericConstraint>, C>>;

    ygg::Index<::loki::formalism::Condition> index;
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
