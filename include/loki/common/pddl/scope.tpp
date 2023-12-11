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
std::optional<SearchResult<T>> Scope::get(const std::string& name) const {
    const auto result = bindings.get<T>(name);
    if (result.has_value()) return SearchResult<T>{result.value(), this->shared_from_this()};
    if (m_parent_scope) {
        return m_parent_scope->get<T>(name);
    }
    return std::optional<SearchResult<T>>();
}


template<typename T>
void Scope::insert(const std::string& name, const BindingPtrType<T>& binding, const std::optional<PositionType>& position) {
    assert(binding);
    assert(!this->get<T>(name));
    bindings.insert<T>(name, binding, position);
}

}