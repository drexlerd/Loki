# PDDL-parser

Loki is a C++ library for parsing the PDDL files.
Loki aims to achieve four goals:
1. Support all concepts of PDDL,
2. Provide meaningful error messages to the user,
3. Provide easy integration into existing planning systems, and
4. Provide methods to compile away requirements.


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
