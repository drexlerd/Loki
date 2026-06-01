cmake_minimum_required(VERSION 3.21)

##############################################################
# Language setup
##############################################################

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


##############################################################
# Establish project
##############################################################

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/pyproject.toml" LOKI_PROJECT_VERSION_LINE REGEX "^version[ \t]*=" LIMIT_COUNT 1)
if(NOT LOKI_PROJECT_VERSION_LINE)
    message(FATAL_ERROR "Could not read project version from pyproject.toml")
endif()
string(REGEX REPLACE "^version[ \t]*=[ \t]*\"([^\"]+)\".*$" "\\1" LOKI_PROJECT_VERSION "${LOKI_PROJECT_VERSION_LINE}")

project(loki VERSION "${LOKI_PROJECT_VERSION}" LANGUAGES C CXX)

# Compilation flags, some configuration-specific
# When applying -fsanitize=address, also set it in dependencies via -DCMAKE_CXX_FLAGS="-fsanitize=address"
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wuninitialized -pedantic -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -fomit-frame-pointer")
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -DDEBUG")

# Set a default build type if none was specified
set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}', as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)
endif()

message(STATUS "Build configuration: ${CMAKE_BUILD_TYPE}")

# make cache variables for install destinations
include(GNUInstallDirs)

option(LOKI_BUILD_TESTS "Enables compilation of Loki tests." OFF)
if (LOKI_BUILD_TESTS)
    message("Build tests enabled.")
    enable_testing()
else()
    message("Build tests disabled.")
endif()

option(LOKI_BUILD_PROFILING "Build Loki profiling targets." OFF)
if (LOKI_BUILD_PROFILING)
    message("Build profiling targets enabled.")
else()
    message("Build profiling targets disabled.")
endif()

option(LOKI_BUILD_EXECUTABLES "Enables compilation of Loki executables." OFF)
if (LOKI_BUILD_EXECUTABLES)
    message("Build executables enabled.")
else()
    message("Build executables disabled.")
endif()

message("Build shared libraries enabled.")

option(LOKI_BUILD_PYPDDL "Build Loki for the pypddl Python wheel." OFF)

set(LOKI_INSTALL_INTERFACE_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}")


set(DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data/")
add_definitions(-DDATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data/")
message("DATA_DIR: ${DATA_DIR}")



##############################################################
# CMake modules and macro files
##############################################################

list(APPEND CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/cmake"
)
include("configure_yggdrasil")
include("loki_native_dependencies")


##############################################################
# Dependency Handling
##############################################################

# set(CMAKE_FIND_DEBUG_MODE ON)

configure_yggdrasil()

loki_register_native_dependency_prefix("${YGGDRASIL_NATIVE_PREFIX}")
loki_register_python_native_runtime_prefix("pyyggdrasil")

# -----
# Boost
# -----

# Find Boost headers only according to https://cmake.org/cmake/help/latest/module/FindBoost.html
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.74.0 REQUIRED PATHS ${CMAKE_PREFIX_PATH} NO_DEFAULT_PATH)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  message(STATUS "Found Boost: ${Boost_DIR} (found version ${Boost_VERSION})")
endif()

# -----
# GTL
# -----

find_package(gtl CONFIG REQUIRED PATHS ${CMAKE_PREFIX_PATH} NO_DEFAULT_PATH)
if(gtl_FOUND)
  message(STATUS "Found gtl: ${gtl_DIR}")
endif()

# -----
# Fmt
# -----

find_package(fmt CONFIG REQUIRED PATHS ${CMAKE_PREFIX_PATH} NO_DEFAULT_PATH)
if(fmt_FOUND)
  message(STATUS "Found fmt: ${fmt_DIR} (found version ${fmt_VERSION})")
endif()


##############################################################
# Add library and executable targets
##############################################################

add_subdirectory(src)

if(LOKI_BUILD_PYPDDL)
  add_subdirectory(python)
endif()

if (LOKI_BUILD_EXECUTABLES)
  add_subdirectory(exe)
endif()

if (LOKI_BUILD_TESTS)
  add_subdirectory(tests)
endif()

if (LOKI_BUILD_PROFILING)
  add_subdirectory(profiling)
endif()


###########
# Install #
###########

if(NOT LOKI_BUILD_PYPDDL)
    # Install header files
    install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/loki"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        COMPONENT loki)

    # Install cmake scripts
    install(
        DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/cmake/"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/loki/cmake"
        COMPONENT loki
    )
endif()


###########
# Exports #
###########

# https://cmake.org/cmake/help/latest/guide/importing-exporting/index.html

include(CMakePackageConfigHelpers)

# Generate the version file for the config file
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/lokiConfigVersion.cmake"
  VERSION ${loki_VERSION}
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

if(NOT LOKI_BUILD_PYPDDL)
    # Install config files
    install(
      FILES
        "${CMAKE_CURRENT_BINARY_DIR}/lokiConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/lokiConfigVersion.cmake"
      DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/loki"
      COMPONENT loki
    )
endif()
