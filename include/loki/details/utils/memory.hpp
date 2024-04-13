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

#ifndef LOKI_INCLUDE_LOKI_UTILS_MEMORY_HPP_
#define LOKI_INCLUDE_LOKI_UTILS_MEMORY_HPP_

// Taken from: https://stackoverflow.com/questions/669438/how-to-get-memory-usage-at-runtime-using-c

#include <tuple>

//////////////////////////////////////////////////////////////////////////////
//
// Read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in KB.
//
// On failure, returns 0.0, 0.0
namespace loki
{

extern std::tuple<double, double> process_mem_usage();

}

#endif
