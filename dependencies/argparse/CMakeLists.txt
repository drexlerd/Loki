cmake_minimum_required(VERSION 3.21)
project(InstallArgparse)

include(ExternalProject)

set(LOKI_ARGPARSE_GIT_TAG v3.2 CACHE STRING "Git tag used to build argparse.")

message(STATUS "Dependency \"argparse\"...")

list(APPEND CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DBUILD_SHARED_LIBS=${LOKI_BUILD_SHARED_DEPENDENCIES}
        -DCMAKE_INSTALL_RPATH=${LOKI_DEPENDENCY_INSTALL_RPATH}
)

message(STATUS "Preparing external project \"argparse\" with args:")
foreach(CMAKE_ARG ${CMAKE_ARGS})
    message(STATUS "-- ${CMAKE_ARG}")
endforeach()
ExternalProject_Add(
    argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
        GIT_TAG ${LOKI_ARGPARSE_GIT_TAG}
        PREFIX ${CMAKE_BINARY_DIR}/argparse
        STAMP_DIR ${CMAKE_BINARY_DIR}/argparse/stamp/${LOKI_ARGPARSE_GIT_TAG}
        UPDATE_COMMAND ""
        CMAKE_ARGS ${CMAKE_ARGS}
)
