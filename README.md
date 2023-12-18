# Loki

ATTENTION: Loki's testing framework is still very weak and Loki itself has not been tested in production yet. Therefore, we strongly advise against using it already. However, we are thankful for feedback, suggestions, and feature requests of all kinds.

Loki is a modern C++ library for efficient syntactic and semantic parsing of PDDL files with exhaustive error handling to provide meaningful error messages to the user. Loki uniquely constructs PDDL objects across a whole class of problems over a common domain.

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

## Design Decisions and its Implied Properties

### Syntactic Parsing

Loki implements the canonical parser structure of boost spirit x3 described [here](https://www.boost.org/doc/libs/1_83_0/libs/spirit/doc/x3/html/spirit_x3/tutorials/rexpr.html)

### Immutability

Loki ensures that each PDDL object is immutable to reduce usage errors and is especially important in multi-threaded applications.

### Private Constructors

Loki ensures the controlled creation of PDDL objects through a factory to ensure uniqueness (up to reordering). Uniqueness ensures other important properties such as efficient hashing (just take the pointer) and comparison for equality (just compare the pointers) or the detection of structurally equivalent problems.

### Shared Owners vs. Single Owner of PDDL objects

Loki represents each PDDL object as a non-owning raw pointer to avoid mutex locks due to atomic increments of reference counters. This gives the user a little bit more responsibility, i.e., the user has to keep the parsers in memory to avoid the deallocation of PDDL objects, illustrated [here](https://github.com/drexlerd/Loki/blob/main/examples/undefined_behavior.cpp).

### Non-fragmented Indexing Schemes

Loki ensures that objects from each class of PDDL objects (Object, Predicate, Atom, Conditions, ...) get assigned identifiers following a non-fragmented indexing scheme (0,1,2,...) allowing for simple association in bitsets and vectors which are commonly used in planning systems for efficiency reasons.

## Grammar Simplifications

Loki simplifies the PDDL grammar to make the resulting data structure less complex but easier to work with while preserving the same expressivity. Therefore, Loki might throw an error even for PDDL input files that conform to the PDDL specification.

- In the PDDL specification, "object" is a reserved PDDL type, and "object" and "number" are reserved PDDL function types. Loki additionally reserves "number" as a PDDL type to reuse the standard PDDL types as function types as well. Hence, Loki throws an error if "number" is used in the types specification even if it is legal according to the PDDL specification.


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

The examples illustrate best practises on how to use Loki.

The first examples shows how to get the ownership semantics correct.

```console
./build/examples/undefined_behavior
```

The second example shows how to parse a domain and problem file which is supposed to be used in a planning system where a non-fragmented indexing of atoms and literals is preferred.

```console
./build/examples/single_problem
```

The third examples shows how to detect structurally equivalent problems over a common domain.

```console
./build/examples/multiple_problems
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
