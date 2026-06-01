/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_CONDITION_DATA_HPP_
#define LOKI_PDDL_CONDITION_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/variant.h>
#include "loki/pddl/condition_and_index.hpp"
#include "loki/pddl/condition_exists_index.hpp"
#include "loki/pddl/condition_forall_index.hpp"
#include "loki/pddl/condition_imply_index.hpp"
#include "loki/pddl/condition_index.hpp"
#include "loki/pddl/condition_literal_index.hpp"
#include "loki/pddl/condition_not_index.hpp"
#include "loki/pddl/condition_numeric_constraint_index.hpp"
#include "loki/pddl/condition_or_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::pddl::Condition>
{
    using Variant = ::cista::offset::variant<ygg::Index<::loki::pddl::ConditionLiteral>,
                                             ygg::Index<::loki::pddl::ConditionAnd>,
                                             ygg::Index<::loki::pddl::ConditionOr>,
                                             ygg::Index<::loki::pddl::ConditionNot>,
                                             ygg::Index<::loki::pddl::ConditionImply>,
                                             ygg::Index<::loki::pddl::ConditionExists>,
                                             ygg::Index<::loki::pddl::ConditionForall>,
                                             ygg::Index<::loki::pddl::ConditionNumericConstraint>>;

    ygg::Index<::loki::pddl::Condition> index;
    Variant value;

    Data() = default;
    explicit Data(Variant value_) : index(), value(std::move(value_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(value); }
    auto cista_members() const noexcept { return std::tie(index, value); }
    auto identifying_members() const noexcept { return std::tie(value); }
};

}

#endif
