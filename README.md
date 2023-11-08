# PDDL-parser

Loki is a C++ library for parsing the PDDL files.
Loki aims to achieve three goals: 
1. Support all concepts of PDDL,
2. Provide meaningful error messages to the user,
3. Provide easy integration into existing planning systems, and
4. Provide methods to compile away requirements.

The parsing is split into two phases: syntactic and semantic parsing.
The syntactic parser constructs an abstract syntax tree while performing error handling locally on the basis of recursively applied grammar rules.
The semantic parsing performs error handling globally on the basis of the whole abstract syntax tree.

## Dependencies

Loki depends on a fraction of Boost's (boost.org) header only libraries.

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
./build/exe/problem benchmarks/schedule/probschedule-51-2.pddl
```
