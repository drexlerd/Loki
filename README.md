# PDDL-parser

## Installation

```console
cmake -S . -B build
cmake --build build -j16
```

## Running

A successful parse:

```console
./build/executable/parser benchmarks/gripper/success.pddl
```

A failed parse:

```console
./build/executable/parser benchmarks/gripper/fail.pddl
```

