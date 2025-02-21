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

#ifndef LOKI_INCLUDE_LOKI_PDDL_PROBLEM_PARSING_CONTEXT_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_PROBLEM_PARSING_CONTEXT_HPP_

#include "loki/details/pddl/domain_parsing_context.hpp"
#include "loki/details/pddl/parser_options.hpp"
#include "loki/details/pddl/position_cache.hpp"
#include "loki/details/pddl/problem_builder.hpp"
#include "loki/details/pddl/reference.hpp"

namespace loki
{

struct ProblemParsingContext
{
    ScopeStack& scopes;
    PDDLPositionCache& positions;
    ReferencedPDDLObjects references;

    Options options;

    ProblemBuilder builder;  ///< Construct the resulting problem while parsing

    ProblemParsingContext(ScopeStack& scopes, PDDLPositionCache& positions, std::shared_ptr<const Domain> domain, const Options& options) :
        scopes(scopes),
        positions(positions),
        references(),
        options(options),
        builder(domain)
    {
    }
};
}

#endif