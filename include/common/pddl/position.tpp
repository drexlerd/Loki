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
PositionCache<Ts...>::PositionCache(const X3ErrorHandler& error_handler, const fs::path& file, int tabs)
    : m_error_handler(PDDLErrorHandler(error_handler.get_error_handler().get_position_cache(), file, tabs)) { }


template<typename... Ts>
template<typename T>
void PositionCache<Ts...>::push_back(const T* object, const PositionType& position) {
    auto& t_positions = std::get<PositionMapType<T>>(m_positions);
    t_positions[object].push_back(position);
}

template<typename... Ts>
template<typename T>
std::vector<PositionType> PositionCache<Ts...>::get(const T* object) const {
    auto& t_positions = std::get<PositionMapType<T>>(m_positions);
    auto it = t_positions.find(object);
    if (it != t_positions.end()) {
        return it->second;
    }
    return {};
}

template<typename... Ts>
const PDDLErrorHandler& PositionCache<Ts...>::get_error_handler() const {
    return m_error_handler;
}

}
