cmake_minimum_required(VERSION 3.21)
project(InstallGoogletest)

include(ExternalProject)

set(LOKI_GOOGLETEST_GIT_TAG release-1.12.1 CACHE STRING "Git tag used to build googletest.")

list(APPEND CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    -DBUILD_SHARED_LIBS=${LOKI_BUILD_SHARED_DEPENDENCIES}
    -DCMAKE_INSTALL_RPATH=${LOKI_DEPENDENCY_INSTALL_RPATH}
)

message(STATUS "Preparing external project \"googletest\" with args:")
foreach(CMAKE_ARG ${CMAKE_ARGS})
    message(STATUS "-- ${CMAKE_ARG}")
endforeach()

ExternalProject_Add(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG ${LOKI_GOOGLETEST_GIT_TAG}
    PREFIX ${CMAKE_BINARY_DIR}/googletest
    STAMP_DIR ${CMAKE_BINARY_DIR}/googletest/stamp/${LOKI_GOOGLETEST_GIT_TAG}
    UPDATE_COMMAND ""
    CMAKE_ARGS ${CMAKE_ARGS}
)
