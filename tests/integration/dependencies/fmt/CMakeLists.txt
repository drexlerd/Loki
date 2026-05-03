cmake_minimum_required(VERSION 3.21)
project(InstallFMT)

include(ExternalProject)

set(LOKI_FMT_GIT_TAG 11.2.0 CACHE STRING "Git tag used to build fmt.")

message(STATUS "Dependency \"fmt\"...")

list(APPEND CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=ON
        -DBUILD_SHARED_LIBS:BOOL=${LOKI_BUILD_SHARED_DEPENDENCIES}
        -DCMAKE_INSTALL_RPATH=${LOKI_DEPENDENCY_INSTALL_RPATH}
        -DFMT_TEST:BOOL=OFF
)
message(STATUS "Preparing external project \"fmt\" with args:")
foreach (CMAKE_ARG ${CMAKE_ARGS})
    message(STATUS "-- ${CMAKE_ARG}")
endforeach ()

ExternalProject_Add(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG ${LOKI_FMT_GIT_TAG}
        PREFIX ${CMAKE_BINARY_DIR}/fmt
        STAMP_DIR ${CMAKE_BINARY_DIR}/fmt/stamp/${LOKI_FMT_GIT_TAG}
        UPDATE_COMMAND ""
        CMAKE_ARGS ${CMAKE_ARGS}
)
