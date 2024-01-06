# Loki

Loki is a C++17 library for efficient syntactic and semantic parsing of PDDL files. Loki implements exhaustive error handling to provide meaningful clang-style messages for syntactic, semantic, and modeling errors. The resulting PDDL objects are structurally uniquely constructed immutable pointer objects stored in persistent and continuous memory to enable hashing in constant time, equality comparison in constant time, and cache-efficient data access.

## Supported PDDL Requirements

- [x] :strips
- [x] :typing
- [x] :negative-preconditions
- [x] :disjunctive-preconditions
- [x] :equality
- [x] :existential-preconditions
- [x] :universal-preconditions
- [x] :quantified-preconditions
- [x] :conditional-effects
- [ ] :fluents
- [x] :numeric-fluents
- [ ] :object-fluents
- [x] :adl
- [ ] :durative-actions
- [ ] :derived-predicates
- [ ] :timed-initial-literals
- [ ] :preferences
- [ ] :constraints
- [x] :action-costs


## Dependencies

Loki depends on a fraction of Boost's (boost.org) header-only libraries.

- Fusion
- Spirit x3
- Container


## Installation

```console
cmake -S . -B build
cmake --build build -j16
```


## Running the Examples

The examples illustrate best practices on how to use Loki.

The first example shows the incorrect handling of the ownership semantics. The example is supposed to crash when trying to print the domain for the second time.

```console
./build/examples/undefined_behavior
```

The second example shows how to parse a domain and problem file which is supposed to be used in a planning system where a non-fragmented indexing of atoms and literals is preferred.

```console
./build/examples/single_problem
```

The third example shows how to detect structurally equivalent problems over a common domain.

```console
./build/examples/multiple_problems
```

The fourth example shows how to find the matched positions of each PDDL object in the input stream and how to report customized clang-style error reports.

```console
./build/examples/position_cache
```


## Running the Executables

Parsing a domain file and printing it.

```console
./build/exe/domain benchmarks/gripper/domain.pddl
```

Parsing a domain and a problem file and printing both.

```console
./build/exe/problem benchmarks/gripper/domain.pddl benchmarks/gripper/p-2-0.pddl
```


## Running the Tests

The testing framework depends on [GoogleTest](https://github.com/google/googletest) and requires the `-DENABLE_TESTING=ON` flag to be passed to the cmake configure step.


## Performance Benchmarks

The benchmark framework depends on [GoogleBenchmark](https://github.com/google/benchmark) and requires the `-DENABLE_BENCHMARKING=ON` flag to be passed to the cmake configure step. The results can be viewed [here](https://drexlerd.github.io/Loki/dev/bench/).


## Acknowledgements

This work was partially supported by the Wallenberg AI, Autonomous Systems and Software Program (WASP) funded by the Knut and Alice Wallenberg Foundation.
