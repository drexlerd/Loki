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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_CONTEXT_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_CONTEXT_HPP_

#include "action.hpp"
#include "atom.hpp"
#include "conditions.hpp"
#include "declarations.hpp"
#include "derived_predicate.hpp"
#include "domain.hpp"
#include "effects.hpp"
#include "function.hpp"
#include "function_skeleton.hpp"
#include "literal.hpp"
#include "object.hpp"
#include "parameter.hpp"
#include "predicate.hpp"
#include "requirements.hpp"
#include "term.hpp"
#include "type.hpp"
#include "variable.hpp"

#include "../../common/factory.hpp"
#include "../../common/ast/config.hpp"
#include "../../common/pddl/scope.hpp"

#include <cassert>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <memory>
#include <tuple>


namespace loki::domain {
    struct Context {
        // For retrieving error messages of the x3 error handler.
        std::unique_ptr<std::ostringstream> error_stream;
        error_handler_type error_handler;

        // For unique creation
        ReferenceCountedObjectFactory<pddl::RequirementsImpl
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
            , pddl::DomainImpl> cache;

        // Track scopes during parsing
        ScopeStack scopes;

        // For convenience.
        pddl::Requirements requirements;
        pddl::Type base_type_object;
        pddl::Type base_type_number;
        pddl::Predicate equal_predicate;

        Context(
            std::unique_ptr<std::ostringstream>&& error_stream_,
            error_handler_type&& error_handler_)
            : error_stream(std::move(error_stream_))
            , error_handler(std::move(error_handler_)) {

            // create base types.
            base_type_object = cache.get_or_create<pddl::TypeImpl>("object");
            base_type_number = cache.get_or_create<pddl::TypeImpl>("number");
            scopes.get_current_scope().insert("object", base_type_object, {});
            scopes.get_current_scope().insert("number", base_type_number, {});

            // add equal predicate with name "=" and two parameters "?left_arg" and "?right_arg"
            auto binary_parameterlist = pddl::ParameterList{
                cache.get_or_create<pddl::ParameterImpl>(
                    cache.get_or_create<pddl::VariableImpl>("?left_arg"),
                    pddl::TypeList{base_type_object}),
                cache.get_or_create<pddl::ParameterImpl>(
                    cache.get_or_create<pddl::VariableImpl>("?right_arg"),
                    pddl::TypeList{base_type_object})

            };
            equal_predicate = cache.get_or_create<pddl::PredicateImpl>("=", binary_parameterlist);
                scopes.get_current_scope().insert<pddl::PredicateImpl>("=", equal_predicate, {});
        }
    };
}


#endif
