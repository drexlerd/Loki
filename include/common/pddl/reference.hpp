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

#ifndef LOKI_INCLUDE_LOKI_COMMON_PDDL_REFERENCE_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_PDDL_REFERENCE_HPP_

#include "../../common/ast/config.hpp"
#include "../../domain/pddl/type.hpp"
#include "../../domain/pddl/object.hpp"
#include "../../domain/pddl/predicate.hpp"
#include "../../domain/pddl/requirements.hpp"
#include "../../domain/pddl/function_skeleton.hpp"
#include "../../domain/pddl/variable.hpp"
#include "../../domain/pddl/requirements.hpp"

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>

#include <cassert>
#include <unordered_set>
#include <memory>
#include <tuple>
#include <optional>
#include <deque>


namespace loki {
/// @brief Encapsulates references.
///
///        Example usage:
///        1. Track all variables of the parameters of an action.
///        2. Parse the conditions and effects while untracking
///           variables that are referenced.
///        3. Verify that all variables are untracked, meaning
///           that they were referenced at least once.
template<typename... Ts>
class References {
    private:
        std::tuple<std::unordered_set<Ts>...> references;

    public:
        /// @brief Returns a pointer if it exists.
        template<typename T>
        bool exists(T reference) const;

        /// @brief Inserts a pointer of type T
        template<typename T>
        void track(T reference);

        /// @brief Erases a pointer of Type T
        template<typename T>
        void untrack(T reference);
};

using ReferencedPDDLObjects = References<pddl::Type
    , pddl::Object
    , pddl::Predicate
    , pddl::FunctionSkeleton
    , pddl::Variable
    , pddl::RequirementEnum>;

}

#include "reference.tpp"

#endif
