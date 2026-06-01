/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_PDDL_DOMAIN_DATA_HPP_
#define LOKI_PDDL_DOMAIN_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/string.h>
#include "loki/pddl/action_index.hpp"
#include "loki/pddl/axiom_index.hpp"
#include "loki/pddl/domain_index.hpp"
#include "loki/pddl/function_skeleton_index.hpp"
#include "loki/pddl/object_index.hpp"
#include "loki/pddl/predicate_index.hpp"
#include "loki/pddl/requirement_index.hpp"
#include "loki/pddl/type_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::pddl::Domain>
{
    ygg::Index<::loki::pddl::Domain> index;
    ::cista::offset::string name;
    ygg::IndexList<::loki::pddl::Requirement> requirements;
    ygg::IndexList<::loki::pddl::Type> types;
    ygg::IndexList<::loki::pddl::Object> constants;
    ygg::IndexList<::loki::pddl::Predicate> predicates;
    ygg::IndexList<::loki::pddl::FunctionSkeleton> functions;
    ygg::IndexList<::loki::pddl::Action> actions;
    ygg::IndexList<::loki::pddl::Axiom> axioms;

    Data() = default;
    Data(::cista::offset::string name_, ygg::IndexList<::loki::pddl::Requirement> requirements_, ygg::IndexList<::loki::pddl::Type> types_, ygg::IndexList<::loki::pddl::Object> constants_, ygg::IndexList<::loki::pddl::Predicate> predicates_, ygg::IndexList<::loki::pddl::FunctionSkeleton> functions_, ygg::IndexList<::loki::pddl::Action> actions_, ygg::IndexList<::loki::pddl::Axiom> axioms_) : index(), name(std::move(name_)), requirements(std::move(requirements_)), types(std::move(types_)), constants(std::move(constants_)), predicates(std::move(predicates_)), functions(std::move(functions_)), actions(std::move(actions_)), axioms(std::move(axioms_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(name); ygg::clear(requirements); ygg::clear(types); ygg::clear(constants); ygg::clear(predicates); ygg::clear(functions); ygg::clear(actions); ygg::clear(axioms); }
    auto cista_members() const noexcept { return std::tie(index, name, requirements, types, constants, predicates, functions, actions, axioms); }
    auto identifying_members() const noexcept { return std::tie(name, requirements, types, constants, predicates, functions, actions, axioms); }
};

}

#endif
