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

#ifndef LOKI_INCLUDE_LOKI_PDDL_CONTEXT_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_CONTEXT_HPP_

#include "loki/details/pddl/factories.hpp"
#include "loki/details/pddl/position.hpp"
#include "loki/details/pddl/reference.hpp"
#include "loki/details/pddl/scope.hpp"

namespace loki
{

struct Context
{
    // For the unique construction of PDDL objects
    PDDLFactories& factories;
    // For storing the positions in the input PDDL file
    PDDLPositionCache& positions;
    // For referencing to existing bindings
    ScopeStack& scopes;
    // For strict error checking
    bool strict;
    // For checking that certain PDDL objects were referenced at least once
    ReferencedPDDLObjects references;
    // For convenience, to avoid an additional parameter during semantic parsing
    Requirements requirements;
    std::unordered_set<Predicate> derived_predicates;

    Context(PDDLFactories& factories_, PDDLPositionCache& positions_, ScopeStack& scopes_, bool strict_ = false) :
        factories(factories_),
        positions(positions_),
        scopes(scopes_),
        strict(strict_),
        references(ReferencedPDDLObjects()),
        requirements(nullptr)
    {
    }
};

}

#endif