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

#include <iostream>
#include <random>


static loki::pddl::AtomList create_atoms(
    size_t num_objects,
    size_t num_predicates,
    loki::CollectionOfPDDLFactories& factories) {
    // Create num_objects-many objects with name object_1,...,object_<num_objects>
    auto objects = loki::pddl::ObjectList();
    for (size_t i = 1; i <= num_objects; ++i) {
        objects.push_back(factories.objects.get_or_create<loki::pddl::ObjectImpl>(
            ("object_" + std::to_string(i)))
        );
    }

    // Create num_predicates-many binary predicates with name predicate_1,...,predicate_<num_predicates>
    auto parameters = loki::pddl::ParameterList{
        factories.parameters.get_or_create<loki::pddl::ParameterImpl>(
            factories.variables.get_or_create<loki::pddl::VariableImpl>("?variable_left"),
            loki::pddl::TypeList{}),
        factories.parameters.get_or_create<loki::pddl::ParameterImpl>(
            factories.variables.get_or_create<loki::pddl::VariableImpl>("?variable_right"),
            loki::pddl::TypeList{})
    };

    auto predicates = loki::pddl::PredicateList();
    for (size_t i = 1; i <= num_predicates; ++i) {
        predicates.push_back(factories.predicates.get_or_create<loki::pddl::PredicateImpl>(
            ("predicate_" + std::to_string(i)),
            parameters));
    }

    auto atoms = loki::pddl::AtomList();
    // Construct num_objects^2 * num_predicates many atoms
    for (const auto& predicate : predicates) {
        for (const auto& object_left : objects) {
            for (const auto& object_right : objects) {
                atoms.push_back(factories.problem_atoms.get_or_create<loki::pddl::AtomImpl>(
                    predicate,
                    loki::pddl::TermList{
                        factories.terms.get_or_create<loki::pddl::TermObjectImpl>(object_left),
                        factories.terms.get_or_create<loki::pddl::TermObjectImpl>(object_right)
                    }));
            }
        }
    }
    return atoms;
}


/// @brief In this benchmark, we evaluate the performance of constructing atoms.
static void BM_ConstructAtoms(benchmark::State& state) {
    const size_t num_objects = 100;
    const size_t num_predicates = 10;

    for (auto _ : state) {
        auto factories = loki::CollectionOfPDDLFactories();

        auto atoms = create_atoms(num_objects, num_predicates, factories);
    }
}
BENCHMARK(BM_ConstructAtoms);


auto identifier_visitor = [](const auto& arg) -> int {
    return arg.get_identifier();
};
auto name_visitor = [](const auto& arg) -> std::string {
    return arg.get_name();
};

static void access_atom_data(const loki::pddl::Atom& atom) {
    const auto& atom_identifier = atom->get_identifier();
    const auto& atom_predicate = atom->get_predicate();
    const auto& atom_terms = atom->get_terms();

    const auto& predicate_identifier = atom_predicate->get_identifier();
    const auto& predicate_name = atom_predicate->get_name();
    const auto& predicate_parameters = atom_predicate->get_parameters();

    for (const auto& term : atom_terms) {
        int term_identifier = std::visit(identifier_visitor, *term);
        const auto& term_name = std::visit(name_visitor, *term);
    }
}


/// @brief In this benchmark, we evaluate the performance of accessing data in sequence
static void BM_IterateAtoms(benchmark::State& state) {
    const size_t num_objects = 100;
    const size_t num_predicates = 10;

    auto factories = loki::CollectionOfPDDLFactories();

    auto atoms = create_atoms(num_objects, num_predicates, factories);

    for (auto _ : state) {
        for (const auto& atom : atoms) {
            access_atom_data(atom);
        }
    }
}
BENCHMARK(BM_IterateAtoms);


static void BM_RandomlyIterateAtoms(benchmark::State& state) {
    const size_t num_objects = 100;
    const size_t num_predicates = 10;

    auto factories = loki::CollectionOfPDDLFactories();

    auto atoms = create_atoms(num_objects, num_predicates, factories);

    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator

    for (auto _ : state) {

        std::vector<int> indices(atoms.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::shuffle(indices.begin(), indices.end(), eng);

        for (int index : indices) {
            access_atom_data(atoms[index]);
        }
    }
}
BENCHMARK(BM_RandomlyIterateAtoms);


BENCHMARK_MAIN();
