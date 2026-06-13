# Loki: PDDL Parsing and Translation in C++ and Python

Loki is a C++20 library with Python bindings for parsing and translation of PDDL files. Loki separates the parsing and translation of domain and problem files, allowing users to effectively work with collection of problems. 

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

Loki depends on a fraction of [Boost's](https://www.boost.org) header-only libraries (Fusion, Spirit x3, Container), its performance benchmarking framework depends on [GoogleBenchmark](https://github.com/google/benchmark), and its testing framework depends on [GoogleTest](https://github.com/google/googletest).

Loki consumes native dependencies from Python packages:

- `pyyggdrasil >= 0.0.17, < 0.1` for shared third-party native dependencies.

The shared workspace layout, layered install order, and the common
build-from-source and CMake-integration patterns are documented in the
[Planning and Learning build instructions](https://github.com/planning-and-learning/.github/blob/main/profile/README.md#building-from-source);
the sections below cover `loki`/`pypddl`-specific details.

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
python -m pip install 'pyyggdrasil>=0.0.17,<0.1'

cmake -S . -B build
```

CMake discovers the installed `pyyggdrasil` automatically through
`cmake/bootstrap_pyyggdrasil.cmake` (which locates the package and adds its
native prefix to `CMAKE_PREFIX_PATH`, after which `find_package(yggdrasil)`
provides the shared helper functions) and links against the
`yggdrasil::yggdrasil` target. To point at a different prefix explicitly:

```console
cmake -S . -B build \
  -DCMAKE_PREFIX_PATH="$(python -m pyyggdrasil --prefix)"

cmake --build build -j4
```

CMake options:

| Option | Default | Description |
| --- | --- | --- |
| `LOKI_BUILD_TESTS` | `OFF` | Build Loki tests. |
| `LOKI_BUILD_EXECUTABLES` | `OFF` | Build Loki executables. |
| `LOKI_BUILD_PROFILING` | `OFF` | Build Loki profiling targets. |
| `LOKI_BUILD_PYPDDL` | `OFF` | Build Loki for the `pypddl` Python wheel. |
| `LOKI_ENABLE_FMT_FORMATTERS` | `ON` | Enable the public `fmt::formatter` specializations of Loki and yggdrasil (`LOKI_`/`YGG_ENABLE_FMT_FORMATTERS` macros, both default-on in the headers). |

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

## Python API

The Python package exposes the semantic parser, translator, and reparseable PDDL
formatter through `pypddl.formalism`:

```python
from pypddl import formalism as pddl

parser = pddl.Parser("""
(define (domain ready-domain)
  (:predicates (ready))
)
""")

translation = pddl.translate_domain(parser.domain())
domain_text = pddl.format_domain(translation.translated_domain)
reparsed = pddl.Parser(domain_text)
assert reparsed.domain().get_name() == "ready-domain"
```

## C++ API

The umbrella header exposes the semantic parser, translator, and reparseable
PDDL formatter through the top-level `loki` namespace:

```cpp
#include <loki/loki.hpp>

#include <string>

int main()
{
    auto parser = loki::Parser(std::string { "(define (domain ready-domain) (:predicates (ready)))" });
    const auto translation = loki::translate_domain(parser.get_domain());
    const auto domain_text = loki::format_domain(translation.get_translated_domain());
    auto reparsed = loki::Parser(domain_text);
    return reparsed.get_domain().get_name() == "ready-domain" ? 0 : 1;
}
```

## CMake Integration

This section covers `pypddl`-specific paths and targets; the general pattern for
consuming the native prefixes from CMake is in the
[common CMake integration instructions](https://github.com/planning-and-learning/.github/blob/main/profile/README.md#cmake-integration).

The Python package `pypddl` installs Loki's native headers, shared library, and
CMake package config under `pypddl.native_prefix()`. It depends on
`pyyggdrasil>=0.0.17,<0.1` for third-party native dependencies:

```python
import pypddl
import pyyggdrasil

print(pypddl.cmake_prefix())       # prefix to put on CMAKE_PREFIX_PATH
print(pypddl.cmake_dir())          # directory containing lokiConfig.cmake
print(pyyggdrasil.cmake_prefix())
```

The same paths are available from the shell via `python -m pypddl --prefix`,
`--cmake-dir`, `--include-dir`, and `--version`.

Downstream CMake projects should include the native prefixes of `pypddl` and
its native package dependencies in `CMAKE_PREFIX_PATH`:

```console
cmake -S . -B build \
  -DCMAKE_PREFIX_PATH="$(python -m pypddl --prefix);$(python -m pyyggdrasil --prefix)"
```

Loki exports the `loki::parsers` target.

## Running the Executables

The executable illustrates how to use Loki. It is disabled by default and can be
enabled with `-DLOKI_BUILD_EXECUTABLES=ON`. Example PDDL inputs are available
from the benchmark submodule:

```console
git submodule update --init --recursive data/planning-benchmarks
```

```console
./build/exe/loki \
  data/planning-benchmarks/tests/classical/gripper/domain.pddl \
  data/planning-benchmarks/tests/classical/gripper/test-1.pddl
```

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
