# Loki

Loki is a C++17 library for efficient syntactic and semantic parsing of PDDL files. Loki implements exhaustive error handling to provide meaningful clang-style messages for syntactic, semantic, and modeling errors. The resulting PDDL objects are structurally uniquely constructed immutable pointer objects stored in persistent and continuous memory to enable hashing in constant time, equality comparison in constant time, and cache-efficient data access.

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
- [ ] :fluents
- [x] :numeric-fluents
- [ ] :object-fluents
- [x] :adl
- [ ] :durative-actions
- [ ] :derived-predicates
- [ ] :timed-initial-literals
- [ ] :preferences
- [ ] :constraints
- [x] :action-costs


## Dependencies

Loki depends on a fraction of Boost's (boost.org) header-only libraries.

- Fusion
- Spirit x3
- Container


## Installation

We provide a cmake project that takes care of downloading, building, and installing [Boost](https://boost.org), [GoogleBenchmark](https://github.com/google/benchmark), and [GoogleTest](https://github.com/google/googletest).

```console
# Configure dependencies
cmake -S dependencies -B dependencies/build
# Build and install dependencies
cmake --build dependencies/build -j16

# Configure Loki with installation prefixes of all dependencies
cmake -S . -B build -DCMAKE_PREFIX_PATH="dependencies/build/boost/boost-prefix/src/boost"
# Build Loki
cmake --build build -j16
# Install Loki (optional)
cmake --install build --prefix=<path/to/installation-directory>
```

The recommended way to integrate Loki in your CMake projects is by adding the installation directory to `CMAKE_PREFIX_PATH` and a combination of `find_package` and `target_link_libraries`:

```cmake
find_package(loki 0.1 COMPONENTS parsers REQUIRED)

target_link_libraries(your_target loki::parsers)
```


## Best Practices for Integration into Planning Systems

Soon :)


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

The testing framework depends on [GoogleTest](https://github.com/google/googletest). Use the following altered cmake configure step.

```console 
cmake -S . -B build \
    -DENABLE_TESTING=ON \
    -DCMAKE_PREFIX_PATH="dependencies/build/boost/boost-prefix/src/boost;\
        dependencies/build/googletest/googletest-prefix/src/googletest"
```


## Performance Benchmarks

The benchmark framework depends on [GoogleBenchmark](https://github.com/google/benchmark). Use the following altered cmake configure step.

```console 
cmake -S . -B build \
    -DENABLE_TESTING=ON \
    -DCMAKE_PREFIX_PATH="dependencies/build/boost/boost-prefix/src/boost;\
        dependencies/build/benchmark/benchmark-prefix/src/benchmark"
```


## Acknowledgements

This work was partially supported by the Wallenberg AI, Autonomous Systems and Software Program (WASP) funded by the Knut and Alice Wallenberg Foundation.
