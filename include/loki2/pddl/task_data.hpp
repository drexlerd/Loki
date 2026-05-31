/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PDDL_TASK_DATA_HPP_
#define LOKI2_PDDL_TASK_DATA_HPP_

#include <tuple>
#include <utility>
#include <yggdrasil/core/types.hpp>
#include <yggdrasil/core/types_utils.hpp>
#include <cista/containers/string.h>
#include <cista/containers/optional.h>
#include "loki2/pddl/axiom_index.hpp"
#include "loki2/pddl/condition_index.hpp"
#include "loki2/pddl/domain_index.hpp"
#include "loki2/pddl/initial_function_value_index.hpp"
#include "loki2/pddl/literal_index.hpp"
#include "loki2/pddl/metric_index.hpp"
#include "loki2/pddl/object_index.hpp"
#include "loki2/pddl/requirement_index.hpp"
#include "loki2/pddl/task_index.hpp"

namespace ygg
{

template<>
struct Data<::loki2::pddl::Task>
{
    ygg::Index<::loki2::pddl::Task> index;
    ::cista::offset::string name;
    ygg::Index<::loki2::pddl::Domain> domain;
    ygg::IndexList<::loki2::pddl::Requirement> requirements;
    ygg::IndexList<::loki2::pddl::Object> objects;
    ygg::IndexList<::loki2::pddl::Literal> initial_literals;
    ygg::IndexList<::loki2::pddl::InitialFunctionValue> initial_function_values;
    ::cista::optional<ygg::Index<::loki2::pddl::Condition>> goal;
    ::cista::optional<ygg::Index<::loki2::pddl::Metric>> metric;
    ygg::IndexList<::loki2::pddl::Axiom> axioms;

    Data() = default;
    Data(::cista::offset::string name_, ygg::Index<::loki2::pddl::Domain> domain_, ygg::IndexList<::loki2::pddl::Requirement> requirements_, ygg::IndexList<::loki2::pddl::Object> objects_, ygg::IndexList<::loki2::pddl::Literal> initial_literals_, ygg::IndexList<::loki2::pddl::InitialFunctionValue> initial_function_values_, ::cista::optional<ygg::Index<::loki2::pddl::Condition>> goal_, ::cista::optional<ygg::Index<::loki2::pddl::Metric>> metric_, ygg::IndexList<::loki2::pddl::Axiom> axioms_) : index(), name(std::move(name_)), domain(domain_), requirements(std::move(requirements_)), objects(std::move(objects_)), initial_literals(std::move(initial_literals_)), initial_function_values(std::move(initial_function_values_)), goal(goal_), metric(metric_), axioms(std::move(axioms_)) {}

    void clear() noexcept { ygg::clear(index); ygg::clear(name); ygg::clear(domain); ygg::clear(requirements); ygg::clear(objects); ygg::clear(initial_literals); ygg::clear(initial_function_values); ygg::clear(goal); ygg::clear(metric); ygg::clear(axioms); }
    auto cista_members() const noexcept { return std::tie(index, name, domain, requirements, objects, initial_literals, initial_function_values, goal, metric, axioms); }
    auto identifying_members() const noexcept { return std::tie(name, domain, requirements, objects, initial_literals, initial_function_values, goal, metric, axioms); }
};

}

#endif
