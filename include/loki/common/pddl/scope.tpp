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

namespace loki {

template<typename... Ts>
template<typename T>
std::optional<ValueType<T>> Bindings<Ts...>::get(const std::string& key) const {
    const auto& t_bindings = std::get<MapType<T>>(bindings);
    auto it = t_bindings.find(key);
    if (it != t_bindings.end()) {
        return {it->second};
    }
    return std::optional<ValueType<T>>();
}


template<typename... Ts>
template<typename T>
void Bindings<Ts...>::insert(const std::string& key, const BindingPtrType<T>& binding, const std::optional<PositionType>& position) {
    assert(binding);
    auto& t_bindings = std::get<MapType<T>>(bindings);
    assert(!t_bindings.count(key));
    t_bindings.emplace(key, ValueType<T>{binding, position});
}


template<typename T>
std::optional<ValueType<T>> Scope::get(const std::string& name) const {
    const auto result = bindings.get<T>(name);
    if (result.has_value()) return result.value();
    if (m_parent_scope) {
        return m_parent_scope->get<T>(name);
    }
    return std::nullopt;
}


template<typename T>
void Scope::insert(const std::string& name, const BindingPtrType<T>& binding, const std::optional<PositionType>& position) {
    assert(binding);
    assert(!this->get<T>(name));
    bindings.insert<T>(name, binding, position);
}


ScopeStack::ScopeStack(
    ErrorHandler&& error_handler,
    const ScopeStack* parent)
    : m_error_handler(std::move(error_handler))
    , m_parent(parent) { }

void ScopeStack::open_scope() {
    m_stack.push_back(m_stack.empty()
        ? std::make_shared<Scope>()
        : std::make_shared<Scope>(m_stack.back()));
}

void ScopeStack::close_scope() {
    assert(!m_stack.empty());
    m_stack.pop_back();
}

/// @brief Returns a binding if it exists.
template<typename T>
std::optional<SearchResult<T>> ScopeStack::get(const std::string& name) const {
    assert(!m_stack.empty());
    auto result = m_stack.back()->get<T>(name);
    if (result.has_value()) return SearchResult<T>{result.value(), m_error_handler};
    if (m_parent) return m_parent->get<T>(name);
    return std::nullopt;
}

/// @brief Insert a binding of type T.
template<typename T>
void ScopeStack::insert(const std::string& name, const BindingPtrType<T>& binding, const std::optional<PositionType>& position) {
    assert(!m_stack.empty());
    m_stack.back()->insert(name, binding, position);
}

const ErrorHandler& ScopeStack::get_error_handler() const {
    return m_error_handler;
}

}