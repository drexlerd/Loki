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

#ifndef LOKI_INCLUDE_LOKI_PDDL_FACTORIES_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_FACTORIES_HPP_

#include "loki/pddl/action.hpp"
#include "loki/pddl/atom.hpp"
#include "loki/pddl/axiom.hpp"
#include "loki/pddl/conditions.hpp"
#include "loki/pddl/domain.hpp"
#include "loki/pddl/effects.hpp"
#include "loki/pddl/factory.hpp"
#include "loki/pddl/function.hpp"
#include "loki/pddl/function_expressions.hpp"
#include "loki/pddl/function_skeleton.hpp"
#include "loki/pddl/ground_atom.hpp"
#include "loki/pddl/ground_literal.hpp"
#include "loki/pddl/literal.hpp"
#include "loki/pddl/metric.hpp"
#include "loki/pddl/numeric_fluent.hpp"
#include "loki/pddl/object.hpp"
#include "loki/pddl/parameter.hpp"
#include "loki/pddl/position.hpp"
#include "loki/pddl/predicate.hpp"
#include "loki/pddl/problem.hpp"
#include "loki/pddl/requirements.hpp"
#include "loki/pddl/term.hpp"
#include "loki/pddl/type.hpp"
#include "loki/pddl/variable.hpp"

namespace loki
{
// The segmented sizes are chosen sufficiently large to avoid
// to avoid allocations and for continuous storage.
// The values are just educated guesses based on the knowledge
// that cache line size is 64 Bytes.
using RequirementFactory = PDDLFactory<pddl::RequirementsImpl>;
using TypeFactory = PDDLFactory<pddl::TypeImpl>;
using VariableFactory = PDDLFactory<pddl::VariableImpl>;
using TermFactory = PDDLFactory<pddl::TermImpl>;
using ObjectFactory = PDDLFactory<pddl::ObjectImpl>;
using AtomFactory = PDDLFactory<pddl::AtomImpl>;
using GroundAtomFactory = PDDLFactory<pddl::GroundAtomImpl>;
using LiteralFactory = PDDLFactory<pddl::LiteralImpl>;
using GroundLiteralFactory = PDDLFactory<pddl::GroundLiteralImpl>;
using ParameterFactory = PDDLFactory<pddl::ParameterImpl>;
using PredicateFactory = PDDLFactory<pddl::PredicateImpl>;
using FunctionExpressionFactory = PDDLFactory<pddl::FunctionExpressionImpl>;
using FunctionFactory = PDDLFactory<pddl::FunctionImpl>;
using FunctionSkeletonFactory = PDDLFactory<pddl::FunctionSkeletonImpl>;
using ConditionFactory = PDDLFactory<pddl::ConditionImpl>;
using EffectFactory = PDDLFactory<pddl::EffectImpl>;
using ActionFactory = PDDLFactory<pddl::ActionImpl>;
using DerivedPredicateFactory = PDDLFactory<pddl::AxiomImpl>;
using OptimizationMetricFactory = PDDLFactory<pddl::OptimizationMetricImpl>;
using NumericFluentFactory = PDDLFactory<pddl::NumericFluentImpl>;
using DomainFactory = PDDLFactory<pddl::DomainImpl>;
using ProblemFactory = PDDLFactory<pddl::ProblemImpl>;

using PDDLPositionCache = PositionCache<pddl::RequirementsImpl,
                                        pddl::TypeImpl,
                                        pddl::VariableImpl,
                                        pddl::TermImpl,
                                        pddl::ObjectImpl,
                                        pddl::AtomImpl,
                                        pddl::GroundAtomImpl,
                                        pddl::LiteralImpl,
                                        pddl::GroundLiteralImpl,
                                        pddl::ParameterImpl,
                                        pddl::PredicateImpl,
                                        pddl::FunctionExpressionImpl,
                                        pddl::FunctionImpl,
                                        pddl::FunctionSkeletonImpl,
                                        pddl::ConditionImpl,
                                        pddl::EffectImpl,
                                        pddl::ActionImpl,
                                        pddl::AxiomImpl,
                                        pddl::OptimizationMetricImpl,
                                        pddl::NumericFluentImpl,
                                        pddl::DomainImpl,
                                        pddl::ProblemImpl>;

/// @brief Collection of factories for the unique creation of PDDL objects.
struct PDDLFactories
{
    RequirementFactory requirements;
    TypeFactory types;
    VariableFactory variables;
    TermFactory terms;
    ObjectFactory objects;
    AtomFactory atoms;
    GroundAtomFactory ground_atoms;
    LiteralFactory literals;
    GroundLiteralFactory ground_literals;
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

    PDDLFactories() :
        requirements(RequirementFactory(100)),
        types(TypeFactory(1000)),
        variables(VariableFactory(1000)),
        terms(TermFactory(1000)),
        objects(ObjectFactory(1000)),
        atoms(AtomFactory(1000)),
        ground_atoms(GroundAtomFactory(1000)),
        literals(LiteralFactory(1000)),
        ground_literals(GroundLiteralFactory(1000)),
        parameters(ParameterFactory(1000)),
        predicates(PredicateFactory(1000)),
        function_expressions(FunctionExpressionFactory(1000)),
        functions(FunctionFactory(1000)),
        function_skeletons(FunctionSkeletonFactory(1000)),
        conditions(ConditionFactory(1000)),
        effects(EffectFactory(1000)),
        actions(ActionFactory(100)),
        derived_predicates(DerivedPredicateFactory(100)),
        optimization_metrics(OptimizationMetricFactory(10)),
        numeric_fluents(NumericFluentFactory(1000)),
        domains(DomainFactory(1)),
        problems(ProblemFactory(10))
    {
    }

    // delete copy and move to avoid dangling references.
    PDDLFactories(const PDDLFactories& other) = delete;
    PDDLFactories& operator=(const PDDLFactories& other) = delete;
    PDDLFactories(PDDLFactories&& other) = delete;
    PDDLFactories& operator=(PDDLFactories&& other) = delete;
};

}

#endif