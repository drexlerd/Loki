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

#ifndef LOKI_INCLUDE_LOKI_COLLECTIONS_HPP_
#define LOKI_INCLUDE_LOKI_COLLECTIONS_HPP_

#include <algorithm>
#include <boost/container/small_vector.hpp>
#include <vector>

namespace loki
{

/// @brief Returns the sorted vector
/// @tparam T
/// @param vec
/// @return
template<typename Collection>
boost::container::small_vector<typename Collection::value_type, 100> get_sorted_vector(const Collection& collection)
{
    boost::container::small_vector<typename Collection::value_type, 100> result(collection.begin(), collection.end());
    std::sort(result.begin(), result.end());
    return result;
}

}

#endif