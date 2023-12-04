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

#include "ground_atom.hpp"
#include "problem.hpp"

#include "../../domain/pddl/context.hpp"
#include "../../domain/pddl/object.hpp"

#include "../../common/ast/config.hpp"
#include "../../common/factory.hpp"
#include "../../common/cache.hpp"
#include "../../common/pddl/scope.hpp"


namespace loki::problem {
    struct Context {
        std::unique_ptr<std::ostringstream> error_stream;

        std::unique_ptr<domain::Context> domain_context;

        ReferenceCountedObjectFactory<pddl::GroundAtomImpl
            , pddl::ProblemImpl> cache;

        // Track scopes during parsing
        std::shared_ptr<Scope> global_scope;
        std::deque<std::shared_ptr<Scope>> scopes; 

        // For convenience.
        pddl::Requirements requirements;

        Context(std::unique_ptr<std::ostringstream>&& error_stream_,
            std::unique_ptr<domain::Context>&& domain_context_)
            : error_stream(std::move(error_stream_)),
              domain_context(std::move(domain_context_)) {
            // Initialize the global scope
            global_scope = std::make_shared<Scope>(nullptr);
            scopes.push_back(global_scope);

            // Make constants referenceable in the problem definition
            //for (const auto& pair : domain_context->global_scope->get<pddl::ObjectImpl>()) {
            //    global_scope->insert(pair.first, pair.second);
            //}
        }
    };
}


#endif
