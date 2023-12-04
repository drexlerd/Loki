/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
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

#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_DECLARATIONS_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_DECLARATIONS_HPP_

#include "../../domain/pddl/declarations.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>


namespace loki::pddl {
    class GroundAtomImpl;
    using GroundAtom = std::shared_ptr<const GroundAtomImpl>;
    using GroundAtomList = std::vector<GroundAtom>;

    class ProblemImpl;
    using Problem = std::shared_ptr<const ProblemImpl>;
    using ProblemList = std::vector<Problem>;
}

#endif
