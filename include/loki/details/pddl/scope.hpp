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

#ifndef LOKI_INCLUDE_LOKI_PDDL_SCOPE_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_SCOPE_HPP_

#include "loki/details/ast/config.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/error_reporting.hpp"
#include "loki/details/pddl/function_skeleton.hpp"
#include "loki/details/pddl/object.hpp"
#include "loki/details/pddl/predicate.hpp"
#include "loki/details/pddl/type.hpp"
#include "loki/details/pddl/variable.hpp"

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <cassert>
#include <deque>
#include <memory>
#include <optional>
#include <tuple>
#include <unordered_map>

namespace loki
{

/// @brief Encapsulates binding related information of a type T.
///        The object is the entity bound to the name.
///        The position points to the matched location
///        in the input stream and is used for error reporting.
template<typename T>
using BindingValueType = std::pair<T, std::optional<Position>>;

/// @brief Datastructure to store bindings of a type T.
template<typename T>
using Bindings = std::unordered_map<std::string, BindingValueType<T>>;

/// @brief Wraps bindings in a scope with reference to a parent scope.
class Scope
{
private:
    const Scope* m_parent_scope;

    Bindings<Type> m_types;
    Bindings<Object> m_objects;
    Bindings<FunctionSkeleton> m_function_skeletons;
    Bindings<Variable> m_variables;
    Bindings<Predicate> m_predicates;

public:
    explicit Scope(const Scope* parent_scope = nullptr);
    Scope(Bindings<Type> types,
          Bindings<Object> objects,
          Bindings<FunctionSkeleton> function_skeletons,
          Bindings<Variable> variables,
          Bindings<Predicate> predicates,
          const Scope* parent_scope = nullptr);
    Scope(const Scope& other) = default;
    Scope& operator=(const Scope& other) = default;
    Scope(Scope&& other) = default;
    Scope& operator=(Scope&& other) = default;

    std::unique_ptr<Scope> clone(const Scope* parent = nullptr) const;

    /// @brief Return a binding if it exists.
    std::optional<BindingValueType<Type>> get_type(const std::string& name) const;
    std::optional<BindingValueType<Object>> get_object(const std::string& name) const;
    std::optional<BindingValueType<FunctionSkeleton>> get_function_skeleton(const std::string& name) const;
    std::optional<BindingValueType<Variable>> get_variable(const std::string& name) const;
    std::optional<BindingValueType<Predicate>> get_predicate(const std::string& name) const;

    /// @brief Insert a binding.
    void insert_type(const std::string& name, const Type& type, const std::optional<Position>& position);
    void insert_object(const std::string& name, const Object& object, const std::optional<Position>& position);
    void insert_function_skeleton(const std::string& name, const FunctionSkeleton& function_skeleton, const std::optional<Position>& position);
    void insert_variable(const std::string& name, const Variable& variable, const std::optional<Position>& position);
    void insert_predicate(const std::string& name, const Predicate& predicate, const std::optional<Position>& position);
};

class ScopeStack
{
private:
    std::deque<std::unique_ptr<Scope>> m_stack;

public:
    ScopeStack() = default;
    ScopeStack(const ScopeStack& other) : m_stack()
    {
        const Scope* cur = nullptr;
        for (const auto& scope : other.m_stack)
        {
            m_stack.push_back(scope->clone(cur));
            cur = m_stack.back().get();
        }
    }
    ScopeStack& operator=(const ScopeStack& other)
    {
        m_stack.clear();
        const Scope* cur = nullptr;
        for (const auto& scope : other.m_stack)
        {
            m_stack.push_back(scope->clone(cur));
            cur = m_stack.back().get();
        }
    }
    ScopeStack(ScopeStack&& other) = default;
    ScopeStack& operator=(ScopeStack&& other) = default;

    /// @brief Inserts a new scope on the top of the stack.
    void open_scope();

    /// @brief Deletes the topmost scope from the stack.
    void close_scope();

    /// @brief Return a binding if it exists.
    Scope& top();
    const Scope& top() const;

    // For testing purposes only.
    const std::deque<std::unique_ptr<Scope>>& get_stack() const;
};

}

#endif