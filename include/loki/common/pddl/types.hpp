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

#include "../../domain/pddl/action.hpp"
#include "../../domain/pddl/atom.hpp"
#include "../../domain/pddl/conditions.hpp"
#include "../../domain/pddl/derived_predicate.hpp"
#include "../../domain/pddl/domain.hpp"
#include "../../domain/pddl/effects.hpp"
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


namespace loki {

// For unique creation
using PddlFactory = ReferenceCountedObjectFactory<pddl::RequirementsImpl
    , pddl::TypeImpl
    , pddl::VariableImpl
    , pddl::TermObjectImpl
    , pddl::TermVariableImpl
    , pddl::ObjectImpl
    , pddl::AtomImpl
    , pddl::LiteralImpl
    , pddl::ParameterImpl
    , pddl::PredicateImpl
    , pddl::FunctionImpl
    , pddl::FunctionSkeletonImpl
    , pddl::ConditionLiteralImpl
    , pddl::ConditionAndImpl
    , pddl::ConditionOrImpl
    , pddl::ConditionNotImpl
    , pddl::EffectLiteralImpl
    , pddl::EffectAndImpl
    , pddl::EffectConditionalForallImpl
    , pddl::EffectConditionalWhenImpl
    , pddl::ActionImpl
    , pddl::DerivedPredicateImpl
    , pddl::DomainImpl
    , pddl::ProblemImpl>;

}

#endif