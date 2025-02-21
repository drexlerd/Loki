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

#ifndef LOKI_INCLUDE_LOKI_PDDL_DOMAIN_PARSING_CONTEXT_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_DOMAIN_PARSING_CONTEXT_HPP_

#include "loki/details/pddl/domain_builder.hpp"
#include "loki/details/pddl/parser_options.hpp"
#include "loki/details/pddl/position.hpp"
#include "loki/details/pddl/reference.hpp"
#include "loki/details/pddl/repositories.hpp"
#include "loki/details/pddl/scope.hpp"

namespace loki
{

struct DomainParsingContext
{
    // FOr error handling
    PDDLErrorHandler error_handler;
    // For referencing to existing bindings
    ScopeStack scopes;
    // For storing the positions in the input PDDL file
    PDDLPositionCache positions;
    // For checking that certain PDDL objects were referenced at least once
    ReferencedPDDLObjects references;
    // Options
    Options options;

    DomainBuilder builder;

    Requirements requirements;

    DomainParsingContext(X3ErrorHandler& x3_error_handler, const Options& options_) :
        error_handler(x3_error_handler.get_error_handler().get_position_cache()),
        scopes(),
        positions(),
        references(),
        options(options_),
        builder(),
        requirements(nullptr)
    {
    }
};
}

#endif