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

namespace loki
{

template<typename... Ts>
template<typename T>
std::optional<BindingValueType<T>> Bindings<Ts...>::get(const std::string& key) const
{
    const auto& t_bindings = std::get<BindingMapType<T>>(bindings);
    auto it = t_bindings.find(key);
    if (it != t_bindings.end())
    {
        return { it->second };
    }
    return std::nullopt;
}

template<typename... Ts>
template<typename T>
void Bindings<Ts...>::insert(const std::string& key, const PDDLElement<T>& element, const std::optional<Position>& position)
{
    assert(element);
    auto& t_bindings = std::get<BindingMapType<T>>(bindings);
    assert(!t_bindings.count(key));
    t_bindings.emplace(key, std::make_tuple(element, position));
}

template<typename T>
std::optional<BindingValueType<T>> Scope::get(const std::string& name) const
{
    const auto result = bindings.get<T>(name);
    if (result.has_value())
        return result.value();
    if (m_parent_scope)
    {
        return m_parent_scope->get<T>(name);
    }
    return std::nullopt;
}

template<typename T>
void Scope::insert(const std::string& name, const PDDLElement<T>& element, const std::optional<Position>& position)
{
    assert(element);
    assert(!this->get<T>(name));
    bindings.insert<T>(name, element, position);
}

template<typename T>
std::optional<ScopeStackSearchResult<T>> ScopeStack::get(const std::string& name) const
{
    assert(!m_stack.empty());
    auto result = m_stack.back()->get<T>(name);
    if (result.has_value())
    {
        return std::make_tuple(std::get<0>(result.value()), std::get<1>(result.value()), std::cref(m_error_handler));
    }
    if (m_parent)
        return m_parent->get<T>(name);
    return std::nullopt;
}

/// @brief Insert a binding of type T.
template<typename T>
void ScopeStack::insert(const std::string& name, const PDDLElement<T>& element, const std::optional<Position>& position)
{
    assert(!m_stack.empty());
    m_stack.back()->insert(name, element, position);
}

}