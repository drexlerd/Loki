cmake_minimum_required(VERSION 3.21)
project(downstream_loki_user LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Python 3.10 REQUIRED COMPONENTS Interpreter Development.Module)
find_package(nanobind CONFIG REQUIRED)
find_package(loki CONFIG REQUIRED COMPONENTS parsers)

nanobind_add_module(
    _downstream_loki
    src/downstream_loki_user/_downstream_loki.cpp
    BACKEND_MODULE nanobind_backend
)
target_link_libraries(_downstream_loki PRIVATE loki::parsers)

if(DOWNSTREAM_RUNTIME_LIBRARY_DIRS)
    set_target_properties(
        _downstream_loki
        PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/downstream_loki_user"
            BUILD_RPATH "${DOWNSTREAM_RUNTIME_LIBRARY_DIRS}"
            INSTALL_RPATH "${DOWNSTREAM_RUNTIME_LIBRARY_DIRS}"
    )
else()
    set_target_properties(
        _downstream_loki
        PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/downstream_loki_user"
    )
endif()

install(TARGETS _downstream_loki LIBRARY DESTINATION downstream_loki_user)
install(FILES src/downstream_loki_user/__init__.py DESTINATION downstream_loki_user)
