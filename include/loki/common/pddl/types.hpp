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

#include "../factory.hpp"
#include "../persistent_factory.hpp"

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
#include "../../problem/pddl/numeric_fluent.hpp"


namespace loki {

/// @brief Collection of factories for the unique creation of PDDL objects.
struct CollectionOfPDDLFactories {
    PersistentFactory<pddl::RequirementEnum> requirement_enums;
    PersistentFactory<pddl::RequirementsImpl> requirements;
    PersistentFactory<pddl::TypeImpl> types;
    PersistentFactory<pddl::VariableImpl> variables;
    PersistentFactory<pddl::TermObjectImpl
        , pddl::TermVariableImpl> terms;
    PersistentFactory<pddl::ObjectImpl> objects;
    PersistentFactory<pddl::AtomImpl> domain_atoms;
    PersistentFactory<pddl::AtomImpl> problem_atoms;  // ground atoms
    PersistentFactory<pddl::LiteralImpl> domain_literals;
    PersistentFactory<pddl::LiteralImpl> problem_literals;  // ground literals
    PersistentFactory<pddl::ParameterImpl> parameters;
    PersistentFactory<pddl::PredicateImpl> predicates;
    PersistentFactory<pddl::FunctionExpressionNumberImpl
        , pddl::FunctionExpressionBinaryOperatorImpl
        , pddl::FunctionExpressionMinusImpl
        , pddl::FunctionExpressionFunctionImpl> function_expressions;
    PersistentFactory<pddl::FunctionImpl> functions;
    PersistentFactory<pddl::FunctionSkeletonImpl> function_skeletons;
    PersistentFactory<pddl::ConditionLiteralImpl
        , pddl::ConditionAndImpl
        , pddl::ConditionOrImpl
        , pddl::ConditionNotImpl> conditions;
    PersistentFactory<pddl::EffectLiteralImpl
        , pddl::EffectAndImpl
        , pddl::EffectNumericImpl
        , pddl::EffectConditionalForallImpl
        , pddl::EffectConditionalWhenImpl> effects;
    PersistentFactory<pddl::ActionImpl> actions;
    PersistentFactory<pddl::DerivedPredicateImpl> derived_predicates;
    PersistentFactory<pddl::NumericFluentImpl> numeric_fluents;
    PersistentFactory<pddl::DomainImpl> domains;
    PersistentFactory<pddl::ProblemImpl> problems;

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
    PersistentFactory<pddl::RequirementEnum>& requirement_enums;
    PersistentFactory<pddl::RequirementsImpl>& requirements;
    PersistentFactory<pddl::TypeImpl>& types;
    PersistentFactory<pddl::VariableImpl>& variables;
    PersistentFactory<pddl::TermObjectImpl
        , pddl::TermVariableImpl>& terms;
    PersistentFactory<pddl::ObjectImpl>& objects;
    PersistentFactory<pddl::AtomImpl>& atoms;
    PersistentFactory<pddl::LiteralImpl>& literals;
    PersistentFactory<pddl::ParameterImpl>& parameters;
    PersistentFactory<pddl::PredicateImpl>& predicates;
    PersistentFactory<pddl::FunctionExpressionNumberImpl
        , pddl::FunctionExpressionBinaryOperatorImpl
        , pddl::FunctionExpressionMinusImpl
        , pddl::FunctionExpressionFunctionImpl>& function_expressions;
    PersistentFactory<pddl::FunctionImpl>& functions;
    PersistentFactory<pddl::FunctionSkeletonImpl>& function_skeletons;
    PersistentFactory<pddl::ConditionLiteralImpl
        , pddl::ConditionAndImpl
        , pddl::ConditionOrImpl
        , pddl::ConditionNotImpl>& conditions;
    PersistentFactory<pddl::EffectLiteralImpl
        , pddl::EffectAndImpl
        , pddl::EffectNumericImpl
        , pddl::EffectConditionalForallImpl
        , pddl::EffectConditionalWhenImpl>& effects;
    PersistentFactory<pddl::ActionImpl>& actions;
    PersistentFactory<pddl::DerivedPredicateImpl>& derived_predicates;
    PersistentFactory<pddl::NumericFluentImpl>& numeric_fluents;
    PersistentFactory<pddl::DomainImpl>& domains;
    PersistentFactory<pddl::ProblemImpl>& problems;
};

}

#endif