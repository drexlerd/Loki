/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PDDL_ACTION_DATA_HPP_
#define LOKI2_PDDL_ACTION_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/string.h>
#include <cista/containers/optional.h>
#include "loki2/pddl/action_index.hpp"
#include "loki2/pddl/condition_index.hpp"
#include "loki2/pddl/effect_index.hpp"
#include "loki2/pddl/parameter_index.hpp"

namespace ygg
{

template<>
struct Data<::loki2::pddl::Action>
{
    ygg::Index<::loki2::pddl::Action> index;
    ::cista::offset::string name;
    ygg::IndexList<::loki2::pddl::Parameter> parameters;
    ::cista::optional<ygg::Index<::loki2::pddl::Condition>> precondition;
    ::cista::optional<ygg::Index<::loki2::pddl::Effect>> effect;

    Data() = default;
    Data(::cista::offset::string name_, ygg::IndexList<::loki2::pddl::Parameter> parameters_, ::cista::optional<ygg::Index<::loki2::pddl::Condition>> precondition_, ::cista::optional<ygg::Index<::loki2::pddl::Effect>> effect_) : index(), name(std::move(name_)), parameters(std::move(parameters_)), precondition(precondition_), effect(effect_) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(name); ygg::clear(parameters); ygg::clear(precondition); ygg::clear(effect); }
    auto cista_members() const noexcept { return std::tie(index, name, parameters, precondition, effect); }
    auto identifying_members() const noexcept { return std::tie(name, parameters, precondition, effect); }
};

}

#endif
