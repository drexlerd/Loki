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
#include "loki/details/pddl/position.hpp"
#include "loki/details/pddl/problem_builder.hpp"
#include "loki/details/pddl/reference.hpp"
#include "loki/details/pddl/repositories.hpp"
#include "loki/details/pddl/scope.hpp"

namespace loki
{

struct ProblemParsingContext
{
    // For referencing to existing bindings
    ScopeStack scopes;
    // For storing the positions in the input PDDL file
    PDDLPositionCache positions;
    // For checking that certain PDDL objects were referenced at least once
    ReferencedPDDLObjects references;
    // Options
    Options options;

    ProblemBuilder builder;     ///< Construct the resulting problem while parsing
    Requirements requirements;  ///< Set this early for convenience

    ProblemParsingContext(const DomainParsingContext& domain_parsing_context, std::shared_ptr<const Domain> domain, const Options& options_) :
        scopes(domain_parsing_context.scopes),
        positions(domain_parsing_context.positions),
        references(domain_parsing_context.references),
        options(options_),
        requirements(nullptr)
    {
    }
};
}

#endif