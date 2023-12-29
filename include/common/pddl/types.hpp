/*
 * Copyright (C) 2023 Dominik Drexler
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

#ifndef LOKI_INCLUDE_LOKI_COMMON_PDDL_TYPES_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_PDDL_TYPES_HPP_

#include "position.hpp"
#include "persistent_factory.hpp"

#include "../../domain/pddl/action.hpp"
#include "../../domain/pddl/atom.hpp"
#include "../../domain/pddl/conditions.hpp"
#include "../../domain/pddl/derived_predicate.hpp"
#include "../../domain/pddl/domain.hpp"
#include "../../domain/pddl/effects.hpp"
#include "../../domain/pddl/function_expressions.hpp"
#include "../../domain/pddl/function_skeleton.hpp"
#include "../../domain/pddl/function.hpp"
#include "../../domain/pddl/literal.hpp"
#include "../../domain/pddl/object.hpp"
#include "../../domain/pddl/parameter.hpp"
#include "../../domain/pddl/predicate.hpp"
#include "../../domain/pddl/requirements.hpp"
#include "../../domain/pddl/term.hpp"
#include "../../domain/pddl/type.hpp"
#include "../../domain/pddl/variable.hpp"
#include "../../problem/pddl/problem.hpp"
#include "../../problem/pddl/metric.hpp"
#include "../../problem/pddl/numeric_fluent.hpp"


namespace loki {
using RequirementFactory = PersistentFactory<pddl::RequirementsImpl, 100>;
using TypeFactory = PersistentFactory<pddl::TypeImpl, 1000>;
using VariableFactory = PersistentFactory<pddl::VariableImpl, 1000>;
using TermFactory = PersistentFactory<pddl::TermImpl, 1000>;
using ObjectFactory = PersistentFactory<pddl::ObjectImpl, 1000>;
using AtomFactory = PersistentFactory<pddl::AtomImpl, 1000>;
using LiteralFactory = PersistentFactory<pddl::LiteralImpl, 1000>;
using ParameterFactory = PersistentFactory<pddl::ParameterImpl, 1000>;
using PredicateFactory = PersistentFactory<pddl::PredicateImpl, 100>;
using FunctionExpressionFactory = PersistentFactory<pddl::FunctionExpressionImpl, 100>;
using FunctionFactory = PersistentFactory<pddl::FunctionImpl, 100>;
using FunctionSkeletonFactory = PersistentFactory<pddl::FunctionSkeletonImpl, 100>;
using ConditionFactory = PersistentFactory<pddl::ConditionImpl, 100>;
using EffectFactory = PersistentFactory<pddl::EffectImpl, 100>;
using ActionFactory = PersistentFactory<pddl::ActionImpl, 100>;
using DerivedPredicateFactory = PersistentFactory<pddl::DerivedPredicateImpl, 100>;
using OptimizationMetricFactory = PersistentFactory<pddl::OptimizationMetricImpl, 100>;
using NumericFluentFactory = PersistentFactory<pddl::NumericFluentImpl, 100>;
using DomainFactory = PersistentFactory<pddl::DomainImpl, 1>;
using ProblemFactory = PersistentFactory<pddl::ProblemImpl, 100>;


using PDDLPositionCache = PositionCache<pddl::RequirementsImpl
    , pddl::TypeImpl
    , pddl::VariableImpl
    , pddl::TermImpl
    , pddl::ObjectImpl
    , pddl::AtomImpl
    , pddl::LiteralImpl
    , pddl::ParameterImpl
    , pddl::PredicateImpl
    , pddl::FunctionExpressionImpl
    , pddl::FunctionImpl
    , pddl::FunctionSkeletonImpl
    , pddl::ConditionImpl
    , pddl::EffectImpl
    , pddl::ActionImpl
    , pddl::DerivedPredicateImpl
    , pddl::OptimizationMetricImpl
    , pddl::NumericFluentImpl
    , pddl::DomainImpl
    , pddl::ProblemImpl>;


/// @brief Collection of factories for the unique creation of PDDL objects.
struct CollectionOfPDDLFactories {
    RequirementFactory requirements;
    TypeFactory types;
    VariableFactory variables;
    TermFactory terms;
    ObjectFactory objects;
    AtomFactory domain_atoms;
    AtomFactory problem_atoms;  // ground atoms
    LiteralFactory domain_literals;
    LiteralFactory problem_literals;  // ground literals
    ParameterFactory parameters;
    PredicateFactory predicates;
    FunctionExpressionFactory function_expressions;
    FunctionFactory functions;
    FunctionSkeletonFactory function_skeletons;
    ConditionFactory conditions;
    EffectFactory effects;
    ActionFactory actions;
    DerivedPredicateFactory derived_predicates;
    OptimizationMetricFactory optimization_metrics;
    NumericFluentFactory numeric_fluents;
    DomainFactory domains;
    ProblemFactory problems;

    CollectionOfPDDLFactories() = default;

    // delete copy and move to avoid dangling references.
    CollectionOfPDDLFactories(const CollectionOfPDDLFactories& other) = delete;
    CollectionOfPDDLFactories& operator=(const CollectionOfPDDLFactories& other) = delete;
    CollectionOfPDDLFactories(CollectionOfPDDLFactories&& other) = delete;
    CollectionOfPDDLFactories& operator=(CollectionOfPDDLFactories&& other) = delete;
};

/// @brief Composition of factories used for parsing the domain.
///        Allows to obtain problem specific indexing schemes
///        by using problem specific factories.
///        We currently use problem specific factories for atoms and literals
struct CompositeOfPDDLFactories {
    RequirementFactory& requirements;
    TypeFactory& types;
    VariableFactory& variables;
    TermFactory& terms;
    ObjectFactory& objects;
    AtomFactory& atoms;
    LiteralFactory& literals;
    ParameterFactory& parameters;
    PredicateFactory& predicates;
    FunctionExpressionFactory& function_expressions;
    FunctionFactory& functions;
    FunctionSkeletonFactory& function_skeletons;
    ConditionFactory& conditions;
    EffectFactory& effects;
    ActionFactory& actions;
    DerivedPredicateFactory& derived_predicates;
    OptimizationMetricFactory& optimization_metrics;
    NumericFluentFactory& numeric_fluents;
    DomainFactory& domains;
    ProblemFactory& problems;
};

}

#endif