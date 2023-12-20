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

#ifndef LOKI_INCLUDE_LOKI_COMMON_SEGMENTED_VECTOR_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_SEGMENTED_VECTOR_HPP_

#include <array>
#include <memory>
#include <mutex>
#include <vector>


namespace loki {

template<typename T, size_t N>
class SegmentedPersistentVector {
private:
    std::vector<std::array<T, N>> data;

public:

};

}

#endif