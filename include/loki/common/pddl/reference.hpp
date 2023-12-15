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
#include "../../domain/pddl/function_skeleton.hpp"
#include "../../domain/pddl/variable.hpp"

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>

#include <cassert>
#include <unordered_set>
#include <memory>
#include <tuple>
#include <optional>
#include <deque>


namespace loki {
template<typename T>
using ReferenceSetType = std::unordered_set<const T*>;


/// @brief Encapsulates referenced bindings.
///        We require that each binding must be referenced at least one in a child scope
///        and if it was not referenced then we report an error message.
///        Insert a reference when it is made
///        and delete it after checking whether it was referenced.
template<typename... Ts>
class References {
    private:
        std::tuple<ReferenceSetType<Ts>...> references;

        const ErrorHandler& m_error_handler;

    public:
        explicit References(const ErrorHandler& error_handler);

        /// @brief Returns a binding if it exists.
        template<typename T>
        bool exists(const T* reference) const;

        /// @brief Inserts a binding of type T
        template<typename T>
        void insert(const T* reference);

        /// @brief Erases a binding of Type T
        template<typename T>
        void erase(const T* reference);

        /// @brief Get the error handler to print an error message.
        const ErrorHandler& get_error_handler() const;
};

using ReferencedBindings = References<pddl::TypeImpl
    , pddl::ObjectImpl
    , pddl::PredicateImpl
    , pddl::FunctionSkeletonImpl
    , pddl::VariableImpl>;

}

#include "reference.tpp"

#endif
