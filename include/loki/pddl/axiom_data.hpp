/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_AXIOM_DATA_HPP_
#define LOKI_PDDL_AXIOM_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include "loki/pddl/axiom_index.hpp"
#include "loki/pddl/condition_index.hpp"
#include "loki/pddl/literal_index.hpp"
#include "loki/pddl/parameter_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::pddl::Axiom>
{
    ygg::Index<::loki::pddl::Axiom> index;
    ygg::IndexList<::loki::pddl::Parameter> parameters;
    ygg::Index<::loki::pddl::Literal> head;
    ygg::Index<::loki::pddl::Condition> condition;

    Data() = default;
    Data(ygg::IndexList<::loki::pddl::Parameter> parameters_, ygg::Index<::loki::pddl::Literal> head_, ygg::Index<::loki::pddl::Condition> condition_) : index(), parameters(std::move(parameters_)), head(head_), condition(condition_) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(parameters); ygg::clear(head); ygg::clear(condition); }
    auto cista_members() const noexcept { return std::tie(index, parameters, head, condition); }
    auto identifying_members() const noexcept { return std::tie(parameters, head, condition); }
};

}

#endif
