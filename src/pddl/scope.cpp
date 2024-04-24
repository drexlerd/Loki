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

#include "loki/details/pddl/scope.hpp"

namespace loki
{
Scope::Scope(const PDDLErrorHandler& error_handler, const Scope* parent_scope) : m_error_handler(error_handler), m_parent_scope(parent_scope) {}

std::optional<BindingSearchResult<Type>> Scope::get_type(const std::string& name) const
{
    const auto it = m_types.find(name);
    if (it != m_types.end())
        return std::make_tuple(it->second.first, it->second.second, std::cref(m_error_handler));
    if (m_parent_scope)
    {
        return m_parent_scope->get_type(name);
    }
    return std::nullopt;
}

std::optional<BindingSearchResult<Object>> Scope::get_object(const std::string& name) const
{
    const auto it = m_objects.find(name);
    if (it != m_objects.end())
        return std::make_tuple(it->second.first, it->second.second, std::cref(m_error_handler));
    if (m_parent_scope)
    {
        return m_parent_scope->get_object(name);
    }
    return std::nullopt;
}

std::optional<BindingSearchResult<FunctionSkeleton>> Scope::get_function_skeleton(const std::string& name) const
{
    const auto it = m_function_skeletons.find(name);
    if (it != m_function_skeletons.end())
        return std::make_tuple(it->second.first, it->second.second, std::cref(m_error_handler));
    if (m_parent_scope)
    {
        return m_parent_scope->get_function_skeleton(name);
    }
    return std::nullopt;
}

std::optional<BindingSearchResult<Variable>> Scope::get_variable(const std::string& name) const
{
    const auto it = m_variables.find(name);
    if (it != m_variables.end())
        return std::make_tuple(it->second.first, it->second.second, std::cref(m_error_handler));
    if (m_parent_scope)
    {
        return m_parent_scope->get_variable(name);
    }
    return std::nullopt;
}

std::optional<BindingSearchResult<Predicate>> Scope::get_predicate(const std::string& name) const
{
    const auto it = m_predicates.find(name);
    if (it != m_predicates.end())
        return std::make_tuple(it->second.first, it->second.second, std::cref(m_error_handler));
    if (m_parent_scope)
    {
        return m_parent_scope->get_predicate(name);
    }
    return std::nullopt;
}

std::optional<BindingSearchResult<Predicate>> Scope::get_derived_predicate(const std::string& name) const
{
    const auto it = m_derived_predicates.find(name);
    if (it != m_derived_predicates.end())
        return std::make_tuple(it->second.first, it->second.second, std::cref(m_error_handler));
    if (m_parent_scope)
    {
        return m_parent_scope->get_derived_predicate(name);
    }
    return std::nullopt;
}

void Scope::insert_type(const std::string& name, const Type& element, const std::optional<Position>& position)
{
    assert(!this->get_type(name));
    m_types.emplace(name, BindingValueType<Type>(element, position));
}

void Scope::insert_object(const std::string& name, const Object& element, const std::optional<Position>& position)
{
    assert(!this->get_object(name));
    m_objects.emplace(name, BindingValueType<Object>(element, position));
}

void Scope::insert_function_skeleton(const std::string& name, const FunctionSkeleton& element, const std::optional<Position>& position)
{
    assert(!this->get_function_skeleton(name));
    m_function_skeletons.emplace(name, BindingValueType<FunctionSkeleton>(element, position));
}

void Scope::insert_variable(const std::string& name, const Variable& element, const std::optional<Position>& position)
{
    assert(!this->get_variable(name));
    m_variables.emplace(name, BindingValueType<Variable>(element, position));
}

void Scope::insert_predicate(const std::string& name, const Predicate& element, const std::optional<Position>& position)
{
    assert(!this->get_predicate(name));
    m_predicates.emplace(name, BindingValueType<Predicate>(element, position));
}

void Scope::insert_derived_predicate(const std::string& name, const Predicate& element, const std::optional<Position>& position)
{
    assert(!this->get_derived_predicate(name));
    m_derived_predicates.emplace(name, BindingValueType<Predicate>(element, position));
}

const PDDLErrorHandler& Scope::get_error_handler() const { return m_error_handler; }

ScopeStack::ScopeStack(const PDDLErrorHandler& error_handler, const ScopeStack* parent) : m_error_handler(error_handler), m_parent(parent) {}

void ScopeStack::open_scope()
{
    // Link to parent Scope across parent ScopeStacks.
    m_stack.push_back(m_stack.empty() ?
                          (m_parent ? std::make_unique<Scope>(m_error_handler, m_parent->m_stack.back().get()) : std::make_unique<Scope>(m_error_handler)) :
                          std::make_unique<Scope>(m_error_handler, m_stack.back().get()));
}

void ScopeStack::close_scope()
{
    assert(!m_stack.empty());
    m_stack.pop_back();
}

Scope& ScopeStack::top()
{
    assert(!m_stack.empty());
    return *m_stack.back();
}
const Scope& ScopeStack::top() const
{
    assert(!m_stack.empty());
    return *m_stack.back();
}

const std::deque<std::unique_ptr<Scope>>& ScopeStack::get_stack() const { return m_stack; }

}