# Loki

Loki is a C++ library for efficient syntactic and semantic parsing of PDDL files with exhaustive error handling to provide meaningful error messages to the user. Loki uniquely constructs PDDL objects across a whole class of problems over a common domain allowing for the detection of syntactically equivalent problems (up to reordering). Loki also implements an automatic garbage collection which can be useful for many sorts of compilations where intermediate results are supposed to be recycled.

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
- [ ] :action-costs


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


## Running

Parsing a domain file into an abstract syntax tree and printing it.

```console
./build/exe/domain benchmarks/gripper/domain.pddl
```

Parsing a problem file into an abstract syntax tree and printing it.

```console
./build/exe/problem benchmarks/gripper/domain.pddl benchmarks/gripper/p-2-0.pddl
```
