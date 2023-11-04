# PDDL-parser

Loki is a C++ library for parsing the PDDL files into an abstract syntax tree.
It supports PDDL version 3.1.

## Grammar details

The grammar implementation is mostly similar to the BNF description of PDDL 3.0 by Gerevini and Long (2005)
with object fluents and actions costs of PDDL 3.1 and the following additional details

- We allow for nested either in types
- We allow nested conditional effects


## Installation

```console
cmake -S . -B build
cmake --build build -j16
```

## Running

A successful parse:

```console
./build/exe/exe benchmarks/gripper/success.pddl
```

A failed parse:

```console
./build/exe/exe benchmarks/gripper/fail.pddl
```
