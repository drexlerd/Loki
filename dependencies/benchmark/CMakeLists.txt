cmake_minimum_required(VERSION 3.21)
project(InstallBenchmark)

include(ExternalProject)

set(LOKI_BENCHMARK_GIT_TAG v1.8.3 CACHE STRING "Git tag used to build benchmark.")

list(APPEND CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_BUILD_TYPE=Release
    -DBUILD_SHARED_LIBS=${LOKI_BUILD_SHARED_DEPENDENCIES}
    -DCMAKE_INSTALL_RPATH=${LOKI_DEPENDENCY_INSTALL_RPATH}
    -DBENCHMARK_ENABLE_GTEST_TESTS=OFF
    )

message(STATUS "Preparing external project \"benchmark\" with args:")
foreach(CMAKE_ARG ${CMAKE_ARGS})
    message(STATUS "-- ${CMAKE_ARG}")
endforeach()

ExternalProject_Add(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG ${LOKI_BENCHMARK_GIT_TAG}
    PREFIX ${CMAKE_BINARY_DIR}/benchmark
    STAMP_DIR ${CMAKE_BINARY_DIR}/benchmark/stamp/${LOKI_BENCHMARK_GIT_TAG}
    UPDATE_COMMAND ""
    CMAKE_ARGS ${CMAKE_ARGS}
)
