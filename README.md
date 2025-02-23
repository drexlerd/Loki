# Loki

Loki is a C++20 library for syntactic and semantic parsing and translation of PDDL files. Loki separates the parsing and translation of domain and problem files, allowing users to effectively work with collection of problems. 

The parser is based on the canonical parser structure proposed in the [Boost Spirit X3 library](https://beta.boost.org/doc/libs/1_82_0/libs/spirit/doc/x3/html/spirit_x3/tutorials/rexpr.html).

The translator is based on the method presented in section four of the paper [*"Concise finite-domain representations for PDDL planning tasks by Malte Helmert (AIJ 2009)"*](https://ai.dmi.unibas.ch/papers/helmert-aij2009.pdf).

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


## Running the Executables

The executable illustrate how to use Loki.

The first example shows the incorrect handling of the ownership semantics. The example is supposed to crash when trying to print the domain for the second time.

```console
./build/exe/loki data/gripper/domain.pddl data/gripper/p-2-0.pddl
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
