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
References<Ts...>::References(const ErrorHandler& error_handler) : m_error_handler(error_handler) { }

template<typename... Ts>
template<typename T>
std::optional<PositionType> References<Ts...>::get(const std::string& key) const {
    const auto& t_references = std::get<MapType<T>>(references);
    auto it = t_references.find(key);
    if (it != t_references.end()) {
        return {it->second};
    }
    return std::nullopt;
}


template<typename... Ts>
template<typename T>
void References<Ts...>::insert(const std::string& key, const PositionType& position) {
    auto& t_references = std::get<MapType<T>>(references);
    assert(!t_references.count(key));
    t_references.emplace(key, position);
}


template<typename... Ts>
template<typename T>
void References<Ts...>::erase(const std::string& key) {
    auto& t_references = std::get<MapType<T>>(references);
    assert(t_references.count(key));
    t_references.erase(key);
}


template<typename... Ts>
const ErrorHandler& References<Ts...>::get_error_handler() const {
    return m_error_handler;
}


}
