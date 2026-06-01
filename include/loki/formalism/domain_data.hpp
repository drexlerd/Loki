/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_FORMALISM_DOMAIN_DATA_HPP_
#define LOKI_FORMALISM_DOMAIN_DATA_HPP_

#include <tuple>
#include <utility>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/string.h>
#include "loki/formalism/action_index.hpp"
#include "loki/formalism/axiom_index.hpp"
#include "loki/formalism/domain_index.hpp"
#include "loki/formalism/function_skeleton_index.hpp"
#include "loki/formalism/object_index.hpp"
#include "loki/formalism/predicate_index.hpp"
#include "loki/formalism/requirement_index.hpp"
#include "loki/formalism/type_index.hpp"

namespace ygg
{

template<>
struct Data<::loki::formalism::Domain>
{
    ygg::Index<::loki::formalism::Domain> index;
    ::cista::offset::string name;
    ygg::IndexList<::loki::formalism::Requirement> requirements;
    ygg::IndexList<::loki::formalism::Type> types;
    ygg::IndexList<::loki::formalism::Object> constants;
    ygg::IndexList<::loki::formalism::Predicate> predicates;
    ygg::IndexList<::loki::formalism::FunctionSkeleton> functions;
    ygg::IndexList<::loki::formalism::Action> actions;
    ygg::IndexList<::loki::formalism::Axiom> axioms;

    Data() = default;
    Data(::cista::offset::string name_, ygg::IndexList<::loki::formalism::Requirement> requirements_, ygg::IndexList<::loki::formalism::Type> types_, ygg::IndexList<::loki::formalism::Object> constants_, ygg::IndexList<::loki::formalism::Predicate> predicates_, ygg::IndexList<::loki::formalism::FunctionSkeleton> functions_, ygg::IndexList<::loki::formalism::Action> actions_, ygg::IndexList<::loki::formalism::Axiom> axioms_) : index(), name(std::move(name_)), requirements(std::move(requirements_)), types(std::move(types_)), constants(std::move(constants_)), predicates(std::move(predicates_)), functions(std::move(functions_)), actions(std::move(actions_)), axioms(std::move(axioms_)) {}
    template<typename C>
    Data(const std::string& name_,
         const std::vector<::ygg::View<ygg::Index<::loki::formalism::Requirement>, C>>& requirements_,
         const std::vector<::ygg::View<ygg::Index<::loki::formalism::Type>, C>>& types_,
         const std::vector<::ygg::View<ygg::Index<::loki::formalism::Object>, C>>& constants_,
         const std::vector<::ygg::View<ygg::Index<::loki::formalism::Predicate>, C>>& predicates_,
         const std::vector<::ygg::View<ygg::Index<::loki::formalism::FunctionSkeleton>, C>>& functions_,
         const std::vector<::ygg::View<ygg::Index<::loki::formalism::Action>, C>>& actions_,
         const std::vector<::ygg::View<ygg::Index<::loki::formalism::Axiom>, C>>& axioms_) :
        index(), name(name_), requirements(), types(), constants(), predicates(), functions(), actions(), axioms()
    {
        set(requirements_, requirements);
        set(types_, types);
        set(constants_, constants);
        set(predicates_, predicates);
        set(functions_, functions);
        set(actions_, actions);
        set(axioms_, axioms);
    }

    void clear() noexcept { ygg::clear(index); ygg::clear(name); ygg::clear(requirements); ygg::clear(types); ygg::clear(constants); ygg::clear(predicates); ygg::clear(functions); ygg::clear(actions); ygg::clear(axioms); }
    auto cista_members() const noexcept { return std::tie(index, name, requirements, types, constants, predicates, functions, actions, axioms); }
    auto identifying_members() const noexcept { return std::tie(name, requirements, types, constants, predicates, functions, actions, axioms); }
};

}

#endif
