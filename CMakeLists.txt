cmake_minimum_required(VERSION 3.26)

##############################################################
# Language setup
##############################################################

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


##############################################################
# Establish project
##############################################################

project(loki VERSION "0.0.1" LANGUAGES C CXX)

# Compilation flags, some configuration-specific
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -pedantic -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -DDEBUG")

# Set a default build type if none was specified
set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}', as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)
endif()

option(ENABLE_TESTING "Enables compilation of tests." OFF)
if (ENABLE_TESTING)
    message("Testing enabled.")
else()
    message("Testing disabled.")
endif()

option(ENABLE_BENCHMARKING "Enables compilation of tests." OFF)
if (ENABLE_BENCHMARKING)
    message("Benchmarking enabled.")
else()
    message("Benchmarking disabled.")
endif()


##############################################################
# CMake modules and macro files
##############################################################

# make cache variables for install destinations
include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/cmake"
)
include("configure_boost")
include("configure_ccache")

# CCache
configure_ccache()

# Boost
# Find Boost headers only according to https://cmake.org/cmake/help/latest/module/FindBoost.html
configure_boost()
# set(CMAKE_FIND_DEBUG_MODE ON)
# set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
include_directories("${CMAKE_PREFIX_PATH}/include")
message("${CMAKE_PREFIX_PATH}/include")
find_package(Boost ${BOOST_MIN_VERSION} REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()


##############################################################
# Add library and executable targets
##############################################################

add_subdirectory(src)

add_subdirectory(exe)

add_subdirectory(examples)

if (ENABLE_TESTING)
  add_subdirectory(tests)
endif()

if (ENABLE_BENCHMARKING)
  add_subdirectory(benchmarks)
endif()


###########
# Install #
###########

# Install header files
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/loki"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install cmake scripts
install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake/"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/loki/cmake"
)


###########
# Exports #
###########

# https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html

include(CMakePackageConfigHelpers)

# Generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/lokiConfigVersion.cmake"
  VERSION "${loki_VERSION}.${loki_VERSION_MAJOR}.${loki_VERSION_MINOR}"
  COMPATIBILITY ExactVersion
)

# Create config file
# https://cmake.org/cmake/help/book/mastering-cmake/cmake/Help/guide/importing-exporting/index.html
# https://cmake.org/cmake/help/latest/module/CMakePackageConfigHelpers.html#generating-a-package-configuration-file
configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/lokiConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/loki"
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# Install config files
install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/lokiConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/lokiConfigVersion.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/loki"
)
