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

#ifndef LOKI_INCLUDE_LOKI_COMMON_PDDL_PRINTER_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_PDDL_PRINTER_HPP_

#include <ostream>
#include <vector>
#include <functional>


namespace loki {

/// @brief Writes the string representation of all elements in the given vector
///        with a space separator into the given output stream.
/// @tparam T is the type of the elements in the vector
/// @tparam Function is a callable function that accepts an element of type T and the output buffer.
/// @param vec
/// @param out
/// @param function
template<typename T, typename Function>
void write_vector_to_buffer(const std::vector<T>& vec, std::ostream& out, const Function& function) {
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i != 0) out << " ";
        function(vec[i], out);
    }
}

/// @brief Writes the string representation of all elements in the given vector
///        with a space separator into the given output stream.
/// @tparam T is the type of the elements in the vector
/// @param vec
/// @param out
template<typename T>
void write_vector_to_buffer(const std::vector<T>& vec, std::ostream& out) {
    write_vector_to_buffer(vec, out, [](const T& element, std::ostream& out){ out << *element; });
}

}

#endif