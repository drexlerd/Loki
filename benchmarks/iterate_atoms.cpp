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

struct AtomAccessResult
{
    size_t atom_identifier;
    loki::Predicate atom_predicate;
    loki::TermList atom_terms;
};

static AtomAccessResult access_atom_data(const loki::Atom& atom)
{
    const auto atom_identifier = atom->get_index();
    const auto atom_predicate = atom->get_predicate();
    auto atom_terms = loki::TermList();
    for (const auto& term : atom->get_terms())
    {
        atom_terms.push_back(term);
    }

    return AtomAccessResult {
        atom_identifier,
        atom_predicate,
        atom_terms,
    };
}

/// @brief In this benchmark, we evaluate the performance of accessing data in sequence
static void BM_IterateAtoms(benchmark::State& state)
{
    const size_t num_atoms = state.range(0);
    const size_t num_predicates = 100;
    // Choose num_objects sufficiently large, we cast to size_t to stay slightly below to fit into the cache
    const size_t num_objects = static_cast<size_t>(sqrt(num_atoms / num_predicates));
    auto factories = loki::Repositories();
    auto atoms = create_atoms(num_objects, num_predicates, factories);
    benchmark::DoNotOptimize(atoms);

    for (auto _ : state)
    {
        for (const auto& atom : atoms)
        {
            const auto atom_access_data = access_atom_data(atom);
            benchmark::DoNotOptimize(atom_access_data);
        }
    }

    state.SetBytesProcessed(state.iterations() * atoms.size() * sizeof(loki::ActionImpl));
}

static void BM_RandomlyIterateAtoms(benchmark::State& state)
{
    const size_t num_atoms = state.range(0);
    const size_t num_predicates = 100;
    const size_t num_objects = static_cast<size_t>(sqrt(num_atoms / num_predicates));
    auto factories = loki::Repositories();
    auto atoms = create_atoms(num_objects, num_predicates, factories);
    benchmark::DoNotOptimize(atoms);

    std::random_device rd;   // Obtain a random number from hardware
    std::mt19937 eng(rd());  // Seed the generator

    for (auto _ : state)
    {
        std::vector<int> indices(atoms.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::shuffle(indices.begin(), indices.end(), eng);

        for (int index : indices)
        {
            const auto atom_access_data = access_atom_data(atoms[index]);
            benchmark::DoNotOptimize(atom_access_data);
        }
    }

    state.SetBytesProcessed(state.iterations() * atoms.size() * sizeof(loki::ActionImpl));
}

}

// Tetralith has Intel Xeon Gold 6130 with L1=512KB, L2=8192KB, L3=22528KB
// sizeof(loki::AtomImpl)=56 => 9142 fit into L1, 146285 fit into L2, 402285 fit into L3

// we just choose some reasonable numbers that can be observed in planning tasks
BENCHMARK(loki::benchmarks::BM_IterateAtoms)->Arg(100);     // small tasks
BENCHMARK(loki::benchmarks::BM_IterateAtoms)->Arg(1000);    // medium tasks
BENCHMARK(loki::benchmarks::BM_IterateAtoms)->Arg(10000);   // large tasks
BENCHMARK(loki::benchmarks::BM_IterateAtoms)->Arg(100000);  // huge tasks

BENCHMARK(loki::benchmarks::BM_RandomlyIterateAtoms)->Arg(100);
BENCHMARK(loki::benchmarks::BM_RandomlyIterateAtoms)->Arg(1000);
BENCHMARK(loki::benchmarks::BM_RandomlyIterateAtoms)->Arg(10000);
BENCHMARK(loki::benchmarks::BM_RandomlyIterateAtoms)->Arg(100000);

BENCHMARK_MAIN();
