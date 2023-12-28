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

#ifndef LOKI_INCLUDE_LOKI_COMMON_PDDL_POSITION_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_PDDL_POSITION_HPP_

#include "config.hpp"
#include "../ast/error_reporting.hpp"
#include "../filesystem.hpp"

#include <unordered_map>


namespace loki {
template<typename T>
using PositionMapType = std::unordered_map<const T*, std::vector<PositionType>>;

/// @brief Stores all occurrences of a PDDL object in the input file for each PDDL type T.
template<typename... Ts>
class PositionCache {
    private:
        std::tuple<PositionMapType<Ts>...> m_positions;

        PDDLErrorHandler m_error_handler;

    public:
        PositionCache(const X3ErrorHandler& error_handler, const fs::path& file, int tabs=4);

        template<typename T>
        void push_back(const T* object, const PositionType& position);

        template<typename T>
        std::vector<PositionType> get(const T* object) const;

        const PDDLErrorHandler& get_error_handler() const;
};

}

#include "position.tpp"

#endif