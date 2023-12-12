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
class Scope;

template<typename T>
using BindingPtrType = std::shared_ptr<const T>;

using PositionType = boost::spirit::x3::position_tagged;

/// @brief Encapsulates binding related information of a type T.
///        The object is the entity bound to the name.
///        The position points to the matched location
///        in the input stream and is used for error reporting.
template<typename T>
struct ValueType {
    BindingPtrType<T> object;
    std::optional<PositionType> position;
};

/// @brief Datastructure to store bindings of a type T.
template<typename T>
using MapType = std::unordered_map<std::string, ValueType<T>>;


/// @brief Encapsulates bindings for different types.
template<typename... Ts>
class Bindings {
    private:
        std::tuple<MapType<Ts>...> bindings;

    public:
        /// @brief Returns a binding if it exists.
        template<typename T>
        std::optional<ValueType<T>> get(const std::string& key) const;

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
        Scope const *m_parent_scope;

        Bindings<pddl::TypeImpl
            , pddl::ObjectImpl
            , pddl::PredicateImpl
            , pddl::FunctionSkeletonImpl
            , pddl::VariableImpl> bindings;

    public:
        explicit Scope(Scope const *parent_scope = nullptr) : m_parent_scope(parent_scope) { }

        /// @brief Returns a binding if it exists.
        template<typename T>
        std::optional<ValueType<T>> get(const std::string& name) const;

        /// @brief Insert a binding of type T.
        template<typename T>
        void insert(const std::string& name, const BindingPtrType<T>& binding, const std::optional<PositionType>& position);
};


/// @brief Combined information of ValueType and ErrorHandler
template<typename T>
struct SearchResult {
    ValueType<T> value;
    const ErrorHandler& error_handler;
};


/// @brief Implements a scoping mechanism with a given ErrorHandler.
///        Allows stacking of ScopeStacks because each ScopeStack has a different ErrorHandler.
class ScopeStack {
    private:
        std::deque<std::unique_ptr<Scope>> m_stack;

        ErrorHandler m_error_handler;

        ScopeStack const *m_parent;

    public:
        ScopeStack(
            ErrorHandler&& error_handler,
            ScopeStack const *parent=nullptr);

        /// @brief Inserts a new scope on the top of the stack.
        void open_scope();

        /// @brief Deletes the topmost scope from the stack.
        void close_scope();

        /// @brief Returns a binding if it exists.
        template<typename T>
        std::optional<SearchResult<T>> get(const std::string& name) const;

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
