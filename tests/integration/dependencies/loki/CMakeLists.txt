cmake_minimum_required(VERSION 3.21)
project(InstallLoki)

include(ExternalProject)


list(APPEND CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_PREFIX_PATH=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_BUILD_TYPE=Release
    -DBUILD_SHARED_LIBS=ON
    )

message(STATUS "Preparing external project \"loki\" with args:")
foreach(CMAKE_ARG ${CMAKE_ARGS})
    message(STATUS "-- ${CMAKE_ARG}")
endforeach()

ExternalProject_Add(
    loki
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../..
    PREFIX ${CMAKE_BINARY_DIR}/loki
    DEPENDS boost fmt gtl
    CMAKE_ARGS ${CMAKE_ARGS}
)
