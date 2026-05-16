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

Loki consumes native dependencies from Python packages:

- `pyyggdrasil >= 0.0.9` for shared third-party native dependencies.

The shared workspace layout and general Python/CMake integration pattern are
documented in the
[Planning and Learning build instructions](https://github.com/planning-and-learning/.github/blob/main/profile/README.md#local-development).

For offline/local development, install `pyyggdrasil` from the sibling source
checkout instead:

```console
cd ../yggdrasil
uv pip install --python ../loki/.venv/bin/python .
```

## Build C++

Install Loki's native dependency providers into the active Python environment,
then configure CMake with their native prefixes:

```console
python -m pip install 'pyyggdrasil>=0.0.9'

cmake -S . -B build \
  -DCMAKE_PREFIX_PATH="$(python -c 'import pyyggdrasil; print(pyyggdrasil.native_prefix())')"

cmake --build build -j4
```

CMake options:

| Option | Default | Description |
| --- | --- | --- |
| `LOKI_BUILD_TESTS` | `OFF` | Build Loki tests. |
| `LOKI_BUILD_EXECUTABLES` | `OFF` | Build Loki executables. |
| `LOKI_BUILD_BENCHMARKS` | `OFF` | Build Loki benchmarks. |
| `BUILD_PYPDDL` | `OFF` | Build Loki for the `pypddl` Python wheel. |

Run tests from a build configured with `-DLOKI_BUILD_TESTS=ON`:

```console
ctest --test-dir build --output-on-failure
```

Install Loki from a configured build directory with:

```console
cmake --install build --prefix=<path/to/installation-directory>
```

## Build Python

```console
python -m pip install .
```

## CMake Integration

The Python package `pypddl` installs Loki's native headers, shared library, and
CMake package config under `pypddl.native_prefix()`. It depends on
`pyyggdrasil>=0.0.9` for third-party native dependencies:

```python
import pypddl
import pyyggdrasil

print(pypddl.native_prefix())
print(pyyggdrasil.native_prefix())
```

Downstream CMake projects should include the native prefixes of `pypddl` and
its native package dependencies in `CMAKE_PREFIX_PATH`:

```console
cmake -S . -B build \
  -DCMAKE_PREFIX_PATH="$(python -c 'import os, pyyggdrasil, pypddl; print(os.pathsep.join(map(str, [pyyggdrasil.native_prefix(), pypddl.native_prefix()])))')"
```

Loki exports the `loki::parsers` target.

## Running the Executables

The executable illustrates how to use Loki. It is disabled by default and can be
enabled with `-DLOKI_BUILD_EXECUTABLES=ON`.

```console
./build/exe/loki data/gripper/domain.pddl data/gripper/p-2-0.pddl
```

## IDE Support

We developed Loki in Visual Studio Code. We recommend the `C/C++` and `CMake Tools` extensions by Microsoft. Install `pyyggdrasil>=0.0.9` into the Python environment used for configuring CMake, then set the following `Cmake: Configure Args` in the `CMake Tools` extension settings under `Workspace`:

- `-DCMAKE_PREFIX_PATH=<output of python -c 'import pyyggdrasil; print(pyyggdrasil.native_prefix())'>`
- `-DLOKI_BUILD_TESTS=ON`
- `-DLOKI_BUILD_BENCHMARKS=ON`

After running `CMake: Configure` in Visual Studio Code (ctrl + shift + p), you should see all include paths being correctly resolved.

## Citing Loki

If you use Loki in your research, please cite it as follows:

```bibtex
@misc{drexler-zenodo2026,
  author =       "Dominik Drexler",
  title =        "{Loki}: A {PDDL} Parser and Normalizer",
  publisher =    "Zenodo",
  year =         "2026",
  doi =          "10.5281/zenodo.20081136",
  url =          "https://doi.org/10.5281/zenodo.20081136",
}
```

## Acknowledgements

This work was partially supported by the Wallenberg AI, Autonomous Systems and Software Program (WASP) funded by the Knut and Alice Wallenberg Foundation.
