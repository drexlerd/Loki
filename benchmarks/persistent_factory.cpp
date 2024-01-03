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

#include "../include/loki/common/pddl/types.hpp"
#include "../include/loki/common/memory.hpp"

#include <benchmark/benchmark.h>

#include <chrono>
#include <iostream>
#include <random>


/// @brief In this experiment, we evaluate the performance of constructing atoms.

// Define another benchmark
static void BM_ConstructAtoms(benchmark::State& state) {
    const size_t num_objects = 100;
    const size_t num_predicates = 100;

    for (auto _ : state) {
        // Create num_objects-many objects with name object_1,...,object_<num_objects>
        auto object_factory = loki::ObjectFactory();
        auto objects = loki::pddl::ObjectList();
        for (size_t i = 1; i <= num_objects; ++i) {
            objects.push_back(object_factory.get_or_create<loki::pddl::ObjectImpl>(
                ("object_" + std::to_string(i)))
            );
        }

        // Create num_predicates-many binary predicates with name predicate_1,...,predicate_<num_predicates>
        auto predicate_factory = loki::PredicateFactory();
        auto parameter_factory = loki::ParameterFactory();
        auto variable_factory = loki::VariableFactory();
        auto parameters = loki::pddl::ParameterList{
            parameter_factory.get_or_create<loki::pddl::ParameterImpl>(
                variable_factory.get_or_create<loki::pddl::VariableImpl>("?variable_left"),
                loki::pddl::TypeList{}),
            parameter_factory.get_or_create<loki::pddl::ParameterImpl>(
                variable_factory.get_or_create<loki::pddl::VariableImpl>("?variable_right"),
                loki::pddl::TypeList{})
        };

        auto predicates = loki::pddl::PredicateList();
        for (size_t i = 1; i <= num_predicates; ++i) {
            predicates.push_back(predicate_factory.get_or_create<loki::pddl::PredicateImpl>(
                ("predicate_" + std::to_string(i)),
                parameters));
        }

        auto atom_factory = loki::AtomFactory();
        auto term_factory = loki::TermFactory();
        auto atoms = loki::pddl::AtomList();
        // Construct num_objects^2 * num_predicates many atoms
        for (const auto& predicate : predicates) {
            for (const auto& object_left : objects) {
                for (const auto& object_right : objects) {
                    atoms.push_back(atom_factory.get_or_create<loki::pddl::AtomImpl>(
                        predicate,
                        loki::pddl::TermList{
                            term_factory.get_or_create<loki::pddl::TermObjectImpl>(object_left),
                            term_factory.get_or_create<loki::pddl::TermObjectImpl>(object_right)
                        }));
                }
            }
        }
    }
}
BENCHMARK(BM_ConstructAtoms);

BENCHMARK_MAIN();
