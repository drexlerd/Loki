# Loki

Loki is a C++20 library for syntactic and semantic parsing and translation of PDDL files. Loki separates the parsing and translation of domain and problem files, allowing users to effectively work with collection of problems.

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
- [x] :numeric-fluents
- [x] :adl
- [x] :derived-predicates
- [x] :action-costs
- [x] :non-deterministic (unsupported in the translator)
- [x] :probabilistic-effects (unsupported in the translator)


## Dependencies

Loki depends on a fraction of [Boost's](boost.org) header-only libraries (Fusion, Spirit x3, Container), its performance benchmarking framework depends on [GoogleBenchmark](https://github.com/google/benchmark), and its testing framework depends on [GoogleTest](https://github.com/google/googletest).

We provide a CMake Superbuild project that takes care of downloading, building, and installing all dependencies.

```console
# Configure dependencies
cmake -S dependencies -B dependencies/build -DCMAKE_INSTALL_PREFIX=dependencies/installs
# Build and install dependencies
cmake --build dependencies/build -j16
```


## Build Instructions

```console
# Configure with installation prefixes of all dependencies
cmake -S . -B build -DCMAKE_PREFIX_PATH=${PWD}/dependencies/installs
# Build
cmake --build build -j16
# Install (optional)
cmake --install build --prefix=<path/to/installation-directory>
```


## Integration Instructions

We provide a CMake Superbuild project [here](https://github.com/drexlerd/Loki/tree/main/tests/integration/dependencies) that takes care of downloading, building, and installing Loki together and its dependencies. You can simply copy it to your project or integrate it in your own Superbuild and run it similarly to the Superbuild project from above. An example planning library based on Loki is available [here](https://github.com/simon-stahlberg/mimir).


## Running the Examples

The examples illustrate best practices on how to use Loki.

The first example shows the incorrect handling of the ownership semantics. The example is supposed to crash when trying to print the domain for the second time.

```console
./build/examples/undefined_behavior
```

The second example shows how to parse a domain and problem file which is supposed to be used in a planning system where a non-fragmented indexing of atoms and literals is preferred.

```console
./build/examples/single_problem
```

The third example shows how to detect structurally equivalent problems over a common domain.

```console
./build/examples/multiple_problems
```

The fourth example shows how to find the matched positions of each PDDL object in the input stream and how to report customized clang-style error reports.

```console
./build/examples/position_cache
```


## Running the Executables

Parsing a domain file and printing it.

```console
./build/exe/domain benchmarks/gripper/domain.pddl
```

Parsing a domain and a problem file and printing both.

```console
./build/exe/problem benchmarks/gripper/domain.pddl benchmarks/gripper/p-2-0.pddl
```

## Running the Tests

The testing framework depends on [GoogleTest](https://github.com/google/googletest) and requires the additional compile flag `-DBUILD_TESTS=ON` to be set in the cmake configure step.

## Performance Benchmarks

The benchmark framework depends on [GoogleBenchmark](https://github.com/google/benchmark) and requires the additional compile flag `-DBUILD_BENCHMARKS=ON` to be set in the cmake configure step. The results from the GitHub action can be viewed [here](https://drexlerd.github.io/Loki/dev/bench/).

## IDE Support

We developed Loki in Visual Studio Code. We recommend the `C/C++` and `CMake Tools` extensions by Microsoft. To get maximum IDE support, you should set the following `Cmake: Configure Args` in the `CMake Tools` extension settings under `Workspace`:

- `-DCMAKE_PREFIX_PATH=${workspaceFolder}/dependencies/installs`
- `-DBUILD_TESTS=ON`
- `-DBUILD_BENCHMARKS=ON`

After running `CMake: Configure` in Visual Studio Code (ctrl + shift + p), you should see all include paths being correctly resolved.

## Acknowledgements

This work was partially supported by the Wallenberg AI, Autonomous Systems and Software Program (WASP) funded by the Knut and Alice Wallenberg Foundation.
