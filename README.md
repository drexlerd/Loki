# PDDL-parser

Loki is a C++ library for parsing the PDDL files into an abstract syntax tree.
It supports PDDL version 3.1.


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

