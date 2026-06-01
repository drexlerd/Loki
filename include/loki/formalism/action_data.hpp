/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_FORMALISM_ACTION_DATA_HPP_
#define LOKI_FORMALISM_ACTION_DATA_HPP_

#include <tuple>
#include <utility>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/string.h>
#include <cista/containers/optional.h>
#include "loki/formalism/action_index.hpp"
#include "loki/formalism/condition_index.hpp"
#include "loki/formalism/effect_index.hpp"
#include "loki/formalism/parameter_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::formalism::Action>
{
    ygg::Index<::loki::formalism::Action> index;
    ::cista::offset::string name;
    ygg::IndexList<::loki::formalism::Parameter> parameters;
    ::cista::optional<ygg::Index<::loki::formalism::Condition>> precondition;
    ::cista::optional<ygg::Index<::loki::formalism::Effect>> effect;

    Data() = default;
    Data(::cista::offset::string name_, ygg::IndexList<::loki::formalism::Parameter> parameters_, ::cista::optional<ygg::Index<::loki::formalism::Condition>> precondition_, ::cista::optional<ygg::Index<::loki::formalism::Effect>> effect_) : index(), name(std::move(name_)), parameters(std::move(parameters_)), precondition(precondition_), effect(effect_) {}
    template<typename C>
    Data(const std::string& name_,
         const std::vector<::ygg::View<ygg::Index<::loki::formalism::Parameter>, C>>& parameters_,
         const std::optional<::ygg::View<ygg::Index<::loki::formalism::Condition>, C>>& precondition_,
         const std::optional<::ygg::View<ygg::Index<::loki::formalism::Effect>, C>>& effect_) :
        index(), name(name_), parameters(), precondition(), effect()
    {
        set(parameters_, parameters);
        set(precondition_, precondition);
        set(effect_, effect);
    }

    void clear() noexcept { ygg::clear(index); ygg::clear(name); ygg::clear(parameters); ygg::clear(precondition); ygg::clear(effect); }
    auto cista_members() const noexcept { return std::tie(index, name, parameters, precondition, effect); }
    auto identifying_members() const noexcept { return std::tie(name, parameters, precondition, effect); }
};

}

#endif
