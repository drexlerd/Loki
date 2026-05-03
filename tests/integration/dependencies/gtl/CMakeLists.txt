cmake_minimum_required(VERSION 3.21)
project(InstallGTL)

include(ExternalProject)

set(LOKI_GTL_GIT_TAG dc7a24fe4b7f2f94480d954da6562c7381ace41f CACHE STRING "Git tag used to build gtl.")

message(STATUS "Dependency \"gtl\"...")

list(APPEND CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_POSITION_INDEPENDENT_CODE=ON
        -DBUILD_SHARED_LIBS=${LOKI_BUILD_SHARED_DEPENDENCIES}
        -DCMAKE_INSTALL_RPATH=${LOKI_DEPENDENCY_INSTALL_RPATH}
        # -DCMAKE_CXX_FLAGS="-fsanitize=address"
        # Disable extras that drag in the failing example
        -DGTL_BUILD_EXAMPLES=OFF
        -DGTL_BUILD_TESTS=OFF
        -DGTL_BUILD_BENCHMARKS=OFF
)

message(STATUS "Preparing external project \"gtl\" with args:")
foreach(CMAKE_ARG ${CMAKE_ARGS})
    message(STATUS "-- ${CMAKE_ARG}")
endforeach()
ExternalProject_Add(
    gtl
    GIT_REPOSITORY https://github.com/greg7mdp/gtl.git
    GIT_TAG ${LOKI_GTL_GIT_TAG}
    PREFIX ${CMAKE_BINARY_DIR}/gtl
    STAMP_DIR ${CMAKE_BINARY_DIR}/gtl/stamp/${LOKI_GTL_GIT_TAG}
    UPDATE_COMMAND ""
    CMAKE_ARGS ${CMAKE_ARGS}
)
ExternalProject_Add_Step(
    gtl install-cmake-package
    COMMAND ${CMAKE_COMMAND}
            -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
            -S ${CMAKE_CURRENT_LIST_DIR}/package
            -B ${CMAKE_BINARY_DIR}/gtl-package
    COMMAND ${CMAKE_COMMAND}
            --install ${CMAKE_BINARY_DIR}/gtl-package
    DEPENDEES install
)
