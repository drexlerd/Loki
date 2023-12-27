# Loki

ATTENTION: Loki's testing framework is still very weak and Loki itself has not been tested in production yet. Therefore, we strongly advise against using it already. However, we are thankful for feedback, suggestions, and feature requests of all kinds.

Loki is a modern C++17 library for efficient syntactic and semantic parsing of PDDL files with exhaustive error handling to provide meaningful clang-style error messages to the user. Loki uniquely constructs PDDL objects across a whole class of problems over a common domain. Loki targets multi-threaded applications and will provide an additional pddl representation in the near future which will be based on [flatbuffers](https://flatbuffers.dev/) for reducing heap allocations to almost zero.

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

The fourth example shows how to find matches positions of each PDDL object in the input stream and report customized clang-style error reports.

```console
./build/examples/position_cache
```


## Running the Executables

Parsing a domain file into an abstract syntax tree and printing it.

```console
./build/exe/domain benchmarks/gripper/domain.pddl
```

Parsing a domain and problem file into an abstract syntax tree and printing it.

```console
./build/exe/problem benchmarks/gripper/domain.pddl benchmarks/gripper/p-2-0.pddl
```


## Acknowledgements

This work was partially supported by the Wallenberg AI, Autonomous Systems and Software Program (WASP) funded by the Knut and Alice Wallenberg Foundation.
