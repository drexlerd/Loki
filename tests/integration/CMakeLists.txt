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

project(integration_example VERSION "0.0.1" LANGUAGES C CXX)

# We installed the dependencies into the subdirectory loki-dependencies
# and hence must append it to the cmake_prefix_path.
list(APPEND CMAKE_PREFIX_PATH
    "${CMAKE_PREFIX_PATH}/loki-dependencies")


##############################################################
# CMake modules and macro files
##############################################################

find_package(loki 0.0.1 COMPONENTS parsers REQUIRED)


##############################################################
# Add library and executable targets
##############################################################

add_executable(integration_standalone "main.cpp")
target_link_libraries(integration_standalone loki::parsers)

add_custom_target(example_domain ALL
    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/gripper/domain.pddl" "${CMAKE_BINARY_DIR}/gripper/domain.pddl")
add_custom_target(example_problem_0 ALL
    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/gripper/p-2-0.pddl" "${CMAKE_BINARY_DIR}/gripper/p-2-0.pddl")


