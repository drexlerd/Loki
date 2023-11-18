# Loki

Loki is a C++ library for syntactic and semantic parsing of PDDL files.
Loki aims to provide meaningful error messages.

## Supported PDDL Requirements

TODO

- [ ] :strips
- [ ] :typing
- [ ] :negative-preconditions
- [ ] :disjunctive-preconditions
- [ ] :equality
- [ ] :existential-preconditions
- [ ] :universal-preconditions
- [ ] :quantified-preconditions
- [ ] :conditional-effects
- [ ] :fluents
- [ ] :numeric-fluents
- [ ] :object-fluents
- [ ] :adl
- [ ] :durative-actions
- [ ] :derived-predicates
- [ ] :timed-initial-literals
- [ ] :preferences
- [ ] :constraints
- [ ] :action-costs


## Grammar Simplifications

Loki simplifies the PDDL grammar to make the resulting data structure less complex
but easier to work with while preserving the same expressivity.
Therefore, Loki might throw an error even for PDDL input files that conform to the PDDL specification.

- In the PDDL specification, "object" is a reserved PDDL type, and "object" and "number"
are reserved PDDL function types. Loki additionally reserves "number" as a PDDL type
to reuse the standard PDDL types as function types as well.
Hence, Loki throws an error if "number" is used in the types specification
even if it is legal according to the PDDL specification.


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
./build/exe/domain benchmarks/schedule/domain.pddl
```

Parsing a problem file into an abstract syntax tree and printing it.

```console
./build/exe/problem benchmarks/schedule/domain.pddl benchmarks/schedule/probschedule-51-2.pddl
```
