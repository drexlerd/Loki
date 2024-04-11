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

#include "loki/pddl/scope.hpp"

namespace loki
{
Scope::Scope(const Scope* parent_scope) : m_parent_scope(parent_scope) {}

std::optional<BindingValueType<pddl::Type>> Scope::get_type(const std::string& name) const
{
    const auto it = m_types.find(name);
    if (it != m_types.end())
        return it->second;
    if (m_parent_scope)
    {
        return m_parent_scope->get_type(name);
    }
    return std::nullopt;
}

std::optional<BindingValueType<pddl::Object>> Scope::get_object(const std::string& name) const
{
    const auto it = m_objects.find(name);
    if (it != m_objects.end())
        return it->second;
    if (m_parent_scope)
    {
        return m_parent_scope->get_object(name);
    }
    return std::nullopt;
}

std::optional<BindingValueType<pddl::FunctionSkeleton>> Scope::get_function_skeleton(const std::string& name) const
{
    const auto it = m_function_skeletons.find(name);
    if (it != m_function_skeletons.end())
        return it->second;
    if (m_parent_scope)
    {
        return m_parent_scope->get_function_skeleton(name);
    }
    return std::nullopt;
}

std::optional<BindingValueType<pddl::Variable>> Scope::get_variable(const std::string& name) const
{
    const auto it = m_variables.find(name);
    if (it != m_variables.end())
        return it->second;
    if (m_parent_scope)
    {
        return m_parent_scope->get_variable(name);
    }
    return std::nullopt;
}

std::optional<BindingValueType<pddl::Predicate>> Scope::get_predicate(const std::string& name) const
{
    const auto it = m_predicates.find(name);
    if (it != m_predicates.end())
        return it->second;
    if (m_parent_scope)
    {
        return m_parent_scope->get_predicate(name);
    }
    return std::nullopt;
}

std::optional<BindingValueType<pddl::Predicate>> Scope::get_derived_predicate(const std::string& name) const
{
    const auto it = m_derived_predicates.find(name);
    if (it != m_derived_predicates.end())
        return it->second;
    if (m_parent_scope)
    {
        return m_parent_scope->get_derived_predicate(name);
    }
    return std::nullopt;
}

void Scope::insert_type(const std::string& name, const pddl::Type& element, const std::optional<Position>& position)
{
    assert(element);
    assert(!this->get_type(name));
    m_types.emplace(name, BindingValueType<pddl::Type>(element, position));
}

void Scope::insert_object(const std::string& name, const pddl::Object& element, const std::optional<Position>& position)
{
    assert(element);
    assert(!this->get_object(name));
    m_objects.emplace(name, BindingValueType<pddl::Object>(element, position));
}

void Scope::insert_function_skeleton(const std::string& name, const pddl::FunctionSkeleton& element, const std::optional<Position>& position)
{
    assert(element);
    assert(!this->get_function_skeleton(name));
    m_function_skeletons.emplace(name, BindingValueType<pddl::FunctionSkeleton>(element, position));
}

void Scope::insert_variable(const std::string& name, const pddl::Variable& element, const std::optional<Position>& position)
{
    assert(element);
    assert(!this->get_variable(name));
    m_variables.emplace(name, BindingValueType<pddl::Variable>(element, position));
}

void Scope::insert_predicate(const std::string& name, const pddl::Predicate& element, const std::optional<Position>& position)
{
    assert(element);
    assert(!this->get_predicate(name));
    m_predicates.emplace(name, BindingValueType<pddl::Predicate>(element, position));
}

void Scope::insert_derived_predicate(const std::string& name, const pddl::Predicate& element, const std::optional<Position>& position)
{
    assert(element);
    assert(!this->get_derived_predicate(name));
    m_derived_predicates.emplace(name, BindingValueType<pddl::Predicate>(element, position));
}

ScopeStack::ScopeStack(const PDDLErrorHandler& error_handler, const ScopeStack* parent) : m_error_handler(error_handler), m_parent(parent) {}

void ScopeStack::open_scope() { m_stack.push_back(m_stack.empty() ? std::make_unique<Scope>() : std::make_unique<Scope>(m_stack.back().get())); }

void ScopeStack::close_scope()
{
    assert(!m_stack.empty());
    m_stack.pop_back();
}

std::optional<ScopeStackSearchResult<pddl::Type>> ScopeStack::get_type(const std::string& name) const
{
    assert(!m_stack.empty());
    auto result = m_stack.back()->get_type(name);
    if (result.has_value())
    {
        return std::make_tuple(std::get<0>(result.value()), std::get<1>(result.value()), std::cref(m_error_handler));
    }
    if (m_parent)
        return m_parent->get_type(name);
    return std::nullopt;
}

std::optional<ScopeStackSearchResult<pddl::Object>> ScopeStack::get_object(const std::string& name) const
{
    assert(!m_stack.empty());
    auto result = m_stack.back()->get_object(name);
    if (result.has_value())
    {
        return std::make_tuple(std::get<0>(result.value()), std::get<1>(result.value()), std::cref(m_error_handler));
    }
    if (m_parent)
        return m_parent->get_object(name);
    return std::nullopt;
}

std::optional<ScopeStackSearchResult<pddl::FunctionSkeleton>> ScopeStack::get_function_skeleton(const std::string& name) const
{
    assert(!m_stack.empty());
    auto result = m_stack.back()->get_function_skeleton(name);
    if (result.has_value())
    {
        return std::make_tuple(std::get<0>(result.value()), std::get<1>(result.value()), std::cref(m_error_handler));
    }
    if (m_parent)
        return m_parent->get_function_skeleton(name);
    return std::nullopt;
}

std::optional<ScopeStackSearchResult<pddl::Variable>> ScopeStack::get_variable(const std::string& name) const
{
    assert(!m_stack.empty());
    auto result = m_stack.back()->get_variable(name);
    if (result.has_value())
    {
        return std::make_tuple(std::get<0>(result.value()), std::get<1>(result.value()), std::cref(m_error_handler));
    }
    if (m_parent)
        return m_parent->get_variable(name);
    return std::nullopt;
}

std::optional<ScopeStackSearchResult<pddl::Predicate>> ScopeStack::get_predicate(const std::string& name) const
{
    assert(!m_stack.empty());
    auto result = m_stack.back()->get_predicate(name);
    if (result.has_value())
    {
        return std::make_tuple(std::get<0>(result.value()), std::get<1>(result.value()), std::cref(m_error_handler));
    }
    if (m_parent)
        return m_parent->get_predicate(name);
    return std::nullopt;
}

std::optional<ScopeStackSearchResult<pddl::Predicate>> ScopeStack::get_derived_predicate(const std::string& name) const
{
    assert(!m_stack.empty());
    auto result = m_stack.back()->get_derived_predicate(name);
    if (result.has_value())
    {
        return std::make_tuple(std::get<0>(result.value()), std::get<1>(result.value()), std::cref(m_error_handler));
    }
    if (m_parent)
        return m_parent->get_derived_predicate(name);
    return std::nullopt;
}

void ScopeStack::insert_type(const std::string& name, const pddl::Type& element, const std::optional<Position>& position)
{
    assert(!m_stack.empty());
    m_stack.back()->insert_type(name, element, position);
}

void ScopeStack::insert_object(const std::string& name, const pddl::Object& element, const std::optional<Position>& position)
{
    assert(!m_stack.empty());
    m_stack.back()->insert_object(name, element, position);
}

void ScopeStack::insert_function_skeleton(const std::string& name, const pddl::FunctionSkeleton& element, const std::optional<Position>& position)
{
    assert(!m_stack.empty());
    m_stack.back()->insert_function_skeleton(name, element, position);
}

void ScopeStack::insert_variable(const std::string& name, const pddl::Variable& element, const std::optional<Position>& position)
{
    assert(!m_stack.empty());
    m_stack.back()->insert_variable(name, element, position);
}

void ScopeStack::insert_predicate(const std::string& name, const pddl::Predicate& element, const std::optional<Position>& position)
{
    assert(!m_stack.empty());
    m_stack.back()->insert_predicate(name, element, position);
}

void ScopeStack::insert_derived_predicate(const std::string& name, const pddl::Predicate& element, const std::optional<Position>& position)
{
    assert(!m_stack.empty());
    m_stack.back()->insert_derived_predicate(name, element, position);
}

const PDDLErrorHandler& ScopeStack::get_error_handler() const { return m_error_handler; }

const std::deque<std::unique_ptr<Scope>>& ScopeStack::get_stack() const { return m_stack; }

}