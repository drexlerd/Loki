# PDDL-parser

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

