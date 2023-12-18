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

#ifndef LOKI_INCLUDE_LOKI_COMMON_PDDL_SCOPE_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_PDDL_SCOPE_HPP_

#include "../../common/ast/config.hpp"
#include "../../domain/pddl/type.hpp"
#include "../../domain/pddl/object.hpp"
#include "../../domain/pddl/predicate.hpp"
#include "../../domain/pddl/function_skeleton.hpp"
#include "../../domain/pddl/variable.hpp"

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>

#include <cassert>
#include <unordered_map>
#include <memory>
#include <tuple>
#include <optional>
#include <deque>


namespace loki {

/// @brief Pointer type of the binding
template<typename T>
using BindingPtrType = const T*;

/// @brief Encapsulates binding related information of a type T.
///        The object is the entity bound to the name.
///        The position points to the matched location
///        in the input stream and is used for error reporting.
template<typename T>
using BindingValueType = std::tuple<BindingPtrType<T>, std::optional<PositionType>>;

/// @brief Datastructure to store bindings of a type T.
template<typename T>
using BindingMapType = std::unordered_map<std::string, BindingValueType<T>>;


/// @brief Encapsulates bindings for different types.
template<typename... Ts>
class Bindings {
    private:
        std::tuple<BindingMapType<Ts>...> bindings;

    public:
        /// @brief Returns a binding if it exists.
        template<typename T>
        std::optional<BindingValueType<T>> get(const std::string& key) const;

        /// @brief Inserts a binding of type T
        template<typename T>
        void insert(
            const std::string& key,
            const BindingPtrType<T>& binding,
            const std::optional<PositionType>& position);
};


/// @brief Wraps bindings in a scope with reference to a parent scope.
class Scope {
    private:
        const Scope* m_parent_scope;

        Bindings<pddl::TypeImpl
            , pddl::ObjectImpl
            , pddl::PredicateImpl
            , pddl::FunctionSkeletonImpl
            , pddl::VariableImpl> bindings;

    public:
        explicit Scope(const Scope* parent_scope = nullptr) : m_parent_scope(parent_scope) { }

        // delete copy and move to avoid dangling references.
        Scope(const Scope& other) = delete;
        Scope& operator=(const Scope& other) = delete;
        Scope(Scope&& other) = delete;
        Scope& operator=(Scope&& other) = delete;

        /// @brief Returns a binding if it exists.
        template<typename T>
        std::optional<BindingValueType<T>> get(const std::string& name) const;

        /// @brief Insert a binding of type T.
        template<typename T>
        void insert(const std::string& name, const BindingPtrType<T>& binding, const std::optional<PositionType>& position);
};


/// @brief Encapsulates the result of search for a binding with the corresponding ErrorHandler.
template<typename T>
using ScopeStackSearchResult = std::tuple<const BindingPtrType<T>, const std::optional<PositionType>, const ErrorHandler&>;


/// @brief Implements a scoping mechanism to store bindings which are mappings from name to a pointer to a PDDL object
///        type and a position in the input stream that can be used to construct error messages with the given ErrorHandler.
///
///        We use ScopeStacks indicated with surrounding "[.]" to stack scopes indicated with surrounding "(.)" to store bindings.
///        We initialize the parsing step by creating a ScopeStack with a single scope that we call the global scope.
///        During parsing we can open and close new scopes but we must ensure that only the single global scope remains.
///        We can access bindings by calling the get method and the matching binding in the nearest scope is returned.
///        We can also insert new bindings in the current scope.
///
///        During domain file parsing, we have a single scope stack structured as follows:
///        [ (Domain Scope Global), (Domain Scope Child 1), (Domain Scope Child 2), ... ]
///
///        During problem file parsing, we get access to bindings from the domain through additional composition as follows:
///        [ (Domain Scope Global) ], [ (Problem Scope Global), (Problem Scope Child 1), (Problem Scope Child 2), ... ]
class ScopeStack {
    private:
        std::deque<std::unique_ptr<Scope>> m_stack;

        const ErrorHandler m_error_handler;

        const ScopeStack* m_parent;

    public:
        ScopeStack(ErrorHandler&& error_handler,
            const ScopeStack* parent=nullptr);

        // delete copy and move to avoid dangling references.
        ScopeStack(const ScopeStack& other) = delete;
        ScopeStack& operator=(const ScopeStack& other) = delete;
        ScopeStack(ScopeStack&& other) = delete;
        ScopeStack& operator=(ScopeStack&& other) = delete;

        /// @brief Inserts a new scope on the top of the stack.
        void open_scope();

        /// @brief Deletes the topmost scope from the stack.
        void close_scope();

        /// @brief Returns a binding if it exists.
        template<typename T>
        std::optional<ScopeStackSearchResult<T>> get(const std::string& name) const;

        /// @brief Insert a binding of type T.
        template<typename T>
        void insert(const std::string& name, const BindingPtrType<T>& binding, const std::optional<PositionType>& position);

        /// @brief Get the error handler to print an error message.
        const ErrorHandler& get_error_handler() const;

        // For testing purposes only.
        const std::deque<std::unique_ptr<Scope>>& get_stack() const;
};


}

#include "scope.tpp"

#endif
