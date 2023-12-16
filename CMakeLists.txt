cmake_minimum_required(VERSION 3.13)

##############################################################
# Language setup
##############################################################

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

##############################################################
# Establish project
##############################################################

project(sketches VERSION 0.1 LANGUAGES C CXX)

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
    message("Building tests enabled.")
else()
    message("Building tests disabled.")
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

# Python
#find_package(Python REQUIRED)
#find_package(PythonLibs REQUIRED)

# Boost
configure_boost()
find_package(Boost ${BOOST_MIN_VERSION})
include_directories(${Boost_INCLUDE_DIRS})

##############################################################
# Add library and executable targets
##############################################################

add_compile_definitions(BOOST_PHOENIX_STL_TUPLE_H_)

include_directories(include/)
include_directories("${PROJECT_SOURCE_DIR}")

add_subdirectory(src)

add_subdirectory(exe)

add_subdirectory(exe_v2)

add_subdirectory(examples)

set(ENABLE_TESTING True)
if (ENABLE_TESTING)
    add_subdirectory(tests)
endif()

###########
# Install #
###########

# Install cmake scripts
install(
    DIRECTORY "${PROJECT_SOURCE_DIR}/cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/sketches"
)
