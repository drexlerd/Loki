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

#ifndef LOKI_INCLUDE_LOKI_UTILS_VARIADIC_CONTAINER_HPP_
#define LOKI_INCLUDE_LOKI_UTILS_VARIADIC_CONTAINER_HPP_

#include <tuple>

namespace loki
{

template<typename... Containers>
class VariadicContainer
{
private:
    std::tuple<Containers...> m_containers;

public:
    VariadicContainer(Containers&&... containers) : m_containers(std::make_tuple(std::forward<Containers>(containers)...)) {}

    template<typename Container>
    Container& get()
    {
        return std::get<Container>(m_containers);
    }

    template<typename Container>
    const Container& get() const
    {
        return std::get<Container>(m_containers);
    }
};

}

#endif