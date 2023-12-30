# Loki

ATTENTION: Loki's testing framework must be stronger, and Loki itself must still be tested in production. Therefore, we strongly advise against using it already. However, we are thankful for all kinds of feedback, suggestions, and feature requests.

Loki is a C++17 library for efficient syntactic and semantic parsing of PDDL files. Loki implements exhaustive error handling to provide meaningful clang-style error messages for syntactic, semantic, and modeling errors. Loki uniquely constructs PDDL objects over a class of problems from a common domain. The resulting PDDL objects are immutable pointer objects with persistent memory, allowing for constant time hashing and equality comparison operations.

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
