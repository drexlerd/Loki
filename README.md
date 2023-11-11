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
