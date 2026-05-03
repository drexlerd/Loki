cmake_minimum_required(VERSION 3.21)

project(loki-integration)

set(DEFAULT_BUILD_TYPE "Release")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE ${DEFAULT_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
endif()
set(BUILD_TYPE ${CMAKE_BUILD_TYPE})

option(LOKI_BUILD_SHARED_DEPENDENCIES "Build bundled dependencies as shared libraries." OFF)
message(STATUS "Build shared dependencies: ${LOKI_BUILD_SHARED_DEPENDENCIES}")

if(APPLE)
    set(LOKI_DEPENDENCY_INSTALL_RPATH "@loader_path")
else()
    set(LOKI_DEPENDENCY_INSTALL_RPATH "$ORIGIN")
endif()

add_subdirectory(argparse)
add_subdirectory(boost)
add_subdirectory(fmt)
add_subdirectory(gtl)
add_subdirectory(loki)
