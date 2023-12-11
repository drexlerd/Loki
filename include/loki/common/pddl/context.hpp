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

#ifndef LOKI_INCLUDE_LOKI_COMMON_PDDL_CONTEXT_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_PDDL_CONTEXT_HPP_

#include "scope.hpp"
#include "types.hpp"


namespace loki {

struct Context {
    // For the unique construction of PDDL objects
    PddlFactory cache;
    // For referencing to existing bindings
    std::unique_ptr<ScopeStack> scopes;
    // For convenience, to avoid an additional parameter
    pddl::Requirements requirements;
};

}

#endif