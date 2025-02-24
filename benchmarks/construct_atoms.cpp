/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
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

#include "utils.hpp"

#include <benchmark/benchmark.h>
#include <iostream>
#include <loki/loki.hpp>
#include <random>

namespace loki::benchmarks
{

/// @brief In this benchmark, we evaluate the performance of constructing atoms.
static void BM_ConstructAtoms(benchmark::State& state)
{
    const size_t num_objects = 100;
    const size_t num_predicates = 10;

    for (auto _ : state)
    {
        auto factories = loki::Repositories();

        auto atoms = create_atoms(num_objects, num_predicates, factories);
        benchmark::DoNotOptimize(atoms);
    }
}

}

BENCHMARK(loki::benchmarks::BM_ConstructAtoms);

BENCHMARK_MAIN();
