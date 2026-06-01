/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_FORMALISM_PREDICATE_DATA_HPP_
#define LOKI_FORMALISM_PREDICATE_DATA_HPP_

#include <tuple>
#include <utility>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/string.h>
#include "loki/formalism/parameter_index.hpp"
#include "loki/formalism/predicate_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::formalism::Predicate>
{
    ygg::Index<::loki::formalism::Predicate> index;
    ::cista::offset::string name;
    ygg::IndexList<::loki::formalism::Parameter> parameters;

    Data() = default;
    Data(::cista::offset::string name_, ygg::IndexList<::loki::formalism::Parameter> parameters_ = {}) : index(), name(std::move(name_)), parameters(std::move(parameters_)) {}
    template<typename C>
    Data(const std::string& name_, const std::vector<::ygg::View<ygg::Index<::loki::formalism::Parameter>, C>>& parameters_ = {}) : index(), name(name_), parameters()
    {
        set(parameters_, parameters);
    }

    void clear() noexcept { ygg::clear(index); ygg::clear(name); ygg::clear(parameters); }
    auto cista_members() const noexcept { return std::tie(index, name, parameters); }
    auto identifying_members() const noexcept { return std::tie(name, parameters); }
};

}

#endif
