cmake_minimum_required(VERSION 3.21)

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
# Dependency handling
##############################################################

# We installed the dependencies into the subdirectories under the install prefix.
# Hence must append them to the single cmake_prefix_path.
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
list(LENGTH CMAKE_PREFIX_PATH PREFIX_PATH_COUNT)
if(PREFIX_PATH_COUNT GREATER 1)
    message(FATAL_ERROR "Only one prefix path is allowed. Found multiple paths in CMAKE_PREFIX_PATH. Please add dependencies to the CMake Superbuild.")
endif()

list(APPEND MODIFIED_CMAKE_PREFIX_PATH
    "${CMAKE_PREFIX_PATH}/benchmark"
    "${CMAKE_PREFIX_PATH}/boost"
    "${CMAKE_PREFIX_PATH}/googletest")
message(${MODIFIED_CMAKE_PREFIX_PATH})
set(CMAKE_PREFIX_PATH ${MODIFIED_CMAKE_PREFIX_PATH})
message(STATUS "MODIFIED_CMAKE_PREFIX_PATH:")
foreach(CMAKE_PREFIX_PATH_ARG ${CMAKE_PREFIX_PATH})
    message(STATUS "-- ${CMAKE_PREFIX_PATH_ARG}")
endforeach()
# set(CMAKE_FIND_DEBUG_MODE ON)


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
  VERSION "${loki_VERSION}"
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
