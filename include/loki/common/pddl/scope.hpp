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


/// @brief Combined information of value and scope
template<typename T>
struct SearchResult {
    ValueType<T> value;
    std::shared_ptr<const Scope> scope;
};


/// @brief Wraps bindings in a scope with reference to a parent scope.
class Scope : public std::enable_shared_from_this<Scope> {
    private:
        std::shared_ptr<error_handler_type> m_error_handler;
        std::shared_ptr<std::ostringstream> m_error_stream;

        std::shared_ptr<const Scope> m_parent_scope;

        Bindings<pddl::TypeImpl
            , pddl::ObjectImpl
            , pddl::PredicateImpl
            , pddl::FunctionSkeletonImpl
            , pddl::VariableImpl> bindings;

    public:
        Scope(
            std::shared_ptr<error_handler_type> error_handler,
            std::shared_ptr<std::ostringstream> error_stream,
            std::shared_ptr<const Scope> parent_scope = nullptr)
            : m_error_handler(error_handler)
            , m_error_stream(error_stream)
            , m_parent_scope(parent_scope) { }

        /// @brief Returns a binding if it exists.
        template<typename T>
        std::optional<SearchResult<T>> get(const std::string& name) const;

        /// @brief Insert a binding of type T.
        template<typename T>
        void insert(const std::string& name, const BindingPtrType<T>& binding, const std::optional<PositionType>& position);

        /// @brief Returns a human readable error message.
        std::string get_error_message(const PositionType& position, const std::string& message) const {
            m_error_stream->clear();
            m_error_handler->operator()(position, message);
            return m_error_stream->str();
        }
};


class ScopeStack {
    private:
        std::deque<std::shared_ptr<Scope>> m_stack;

        std::shared_ptr<error_handler_type> m_error_handler;
        std::shared_ptr<std::ostringstream> m_error_stream;

    public:
        ScopeStack(
            std::unique_ptr<error_handler_type>&& error_handler,
            std::unique_ptr<std::ostringstream>&& error_stream)
            : m_error_handler(std::move(error_handler))
            , m_error_stream(std::move(error_stream)) { }

        void open_scope() {
            m_stack.push_back(m_stack.empty()
                ? std::make_shared<Scope>(m_error_handler, m_error_stream)
                : std::make_shared<Scope>(m_error_handler, m_error_stream, m_stack.back()));
        }

        void close_scope() {
            assert(!m_stack.empty());
            m_stack.pop_back();
        }

        Scope& get_current_scope() {
            assert(!m_stack.empty());
            return *m_stack.back();
        }

        const Scope& get_current_scope() const {
            assert(!m_stack.empty());
            return *m_stack.back();
        }

        void push_scope(std::shared_ptr<Scope> scope) {
            m_stack.push_back(scope);
        }

        const std::deque<std::shared_ptr<Scope>>& get_stack() const {
            return m_stack;
        }
};

}

#include "scope.tpp"

#endif
