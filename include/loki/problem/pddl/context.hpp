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

#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_CONTEXT_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_CONTEXT_HPP_

#include "problem.hpp"

#include "../../domain/pddl/requirements.hpp"
#include "../../domain/pddl/context.hpp"
#include "../../domain/pddl/object.hpp"

#include "../../common/ast/config.hpp"
#include "../../common/factory.hpp"
#include "../../common/pddl/scope.hpp"


namespace loki::problem {
    struct Context {
        std::unique_ptr<std::ostringstream> error_stream;
        error_handler_type error_handler;

        const domain::Context& domain_context;

        // We want different pointers for problem specific objects
        ReferenceCountedObjectFactory<pddl::ObjectImpl
            , pddl::TermObjectImpl
            , pddl::AtomImpl
            , pddl::LiteralImpl
            , pddl::ConditionLiteralImpl
            , pddl::ConditionAndImpl
            , pddl::ConditionOrImpl
            , pddl::ConditionNotImpl
            , pddl::RequirementsImpl
            , pddl::ProblemImpl> cache;

        // Track scopes during parsing
        ScopeStack scopes;

        // For convenience.
        pddl::Requirements requirements;

        Context(std::unique_ptr<std::ostringstream>&& error_stream_,
            error_handler_type&& error_handler_,
            const domain::Context& domain_context_)
            : error_stream(std::move(error_stream_))
            , error_handler(std::move(error_handler_))
            , domain_context(domain_context_) {

            // Make constants more easily referenceable in the problem definition
            for (const auto& pair : domain_context.scopes.get_current_scope().get<pddl::ObjectImpl>()) {
                scopes.get_current_scope().insert(pair.first, pair.second.object, {});
            }
        }
    };
}


#endif
