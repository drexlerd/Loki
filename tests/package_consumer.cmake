set(install_prefix "${LOKI_BINARY_DIR}/package-consumer/install")
set(work_dir "${LOKI_BINARY_DIR}/package-consumer/work")
set(consumer_dir "${work_dir}/consumer")
set(consumer_build_dir "${work_dir}/consumer-build")
set(bad_consumer_dir "${work_dir}/bad-consumer")
set(bad_consumer_build_dir "${work_dir}/bad-consumer-build")

file(REMOVE_RECURSE "${LOKI_BINARY_DIR}/package-consumer")
file(MAKE_DIRECTORY "${consumer_dir}" "${bad_consumer_dir}")

set(install_command "${CMAKE_COMMAND}" --install "${LOKI_BINARY_DIR}" --prefix "${install_prefix}" --component loki)
if(LOKI_CONFIG)
    list(APPEND install_command --config "${LOKI_CONFIG}")
endif()
execute_process(
    COMMAND ${install_command}
    RESULT_VARIABLE install_result
    OUTPUT_VARIABLE install_output
    ERROR_VARIABLE install_error
)
if(NOT install_result EQUAL 0)
    message(FATAL_ERROR "Failed to install loki package for consumer test:\n${install_output}\n${install_error}")
endif()

file(WRITE "${consumer_dir}/CMakeLists.txt" [[
cmake_minimum_required(VERSION 3.21)

project(loki_consumer LANGUAGES CXX)

find_package(loki CONFIG REQUIRED)

add_executable(loki_consumer main.cpp)
target_link_libraries(loki_consumer PRIVATE loki::parsers)
]])

file(WRITE "${consumer_dir}/main.cpp" [[
#include <loki/loki.hpp>

#include <string>

int main()
{
    auto parser = loki::Parser(std::string { "(define (domain c) (:predicates (p)))" });
    const auto domain = parser.get_domain();
    if (std::string(domain.get_name()) != "c")
    {
        return 1;
    }

    const auto task = parser.parse_task(std::string { "(define (problem p) (:domain c) (:init) (:goal (p)))" });
    const auto domain_translation = loki::translate_domain(domain);
    const auto task_translation = loki::translate_task(task, domain_translation);
    const auto domain_text = loki::format_domain(domain_translation.get_translated_domain());
    auto reparsed = loki::Parser(domain_text);
    if (std::string(reparsed.get_domain().get_name()) != "c")
    {
        return 2;
    }
    if (std::string(task_translation.get_original_task().get_name()) != "p")
    {
        return 3;
    }

    loki::SourceRange range {};
    static_cast<void>(range);
    return 0;
}
]])

set(prefix_path "${install_prefix}")
if(LOKI_YGGDRASIL_PREFIX)
    list(APPEND prefix_path "${LOKI_YGGDRASIL_PREFIX}")
endif()

execute_process(
    COMMAND
        "${CMAKE_COMMAND}"
            -S "${consumer_dir}"
            -B "${consumer_build_dir}"
            -G "${LOKI_GENERATOR}"
            "-DCMAKE_PREFIX_PATH=${prefix_path}"
    RESULT_VARIABLE configure_result
    OUTPUT_VARIABLE configure_output
    ERROR_VARIABLE configure_error
)
if(NOT configure_result EQUAL 0)
    message(FATAL_ERROR "Failed to configure loki consumer:\n${configure_output}\n${configure_error}")
endif()

set(build_command "${CMAKE_COMMAND}" --build "${consumer_build_dir}" --parallel 6)
if(LOKI_CONFIG)
    list(APPEND build_command --config "${LOKI_CONFIG}")
endif()
execute_process(
    COMMAND ${build_command}
    RESULT_VARIABLE build_result
    OUTPUT_VARIABLE build_output
    ERROR_VARIABLE build_error
)
if(NOT build_result EQUAL 0)
    message(FATAL_ERROR "Failed to build loki consumer:\n${build_output}\n${build_error}")
endif()

if(NOT LOKI_INSTALL_LIBDIR)
    set(LOKI_INSTALL_LIBDIR "lib")
endif()

set(runtime_library_path "${install_prefix}/${LOKI_INSTALL_LIBDIR}")
if(LOKI_YGGDRASIL_PREFIX)
    list(APPEND runtime_library_path "${LOKI_YGGDRASIL_PREFIX}/${LOKI_INSTALL_LIBDIR}")
endif()
if(UNIX)
    string(REPLACE ";" ":" runtime_library_path_env "${runtime_library_path}")
else()
    set(runtime_library_path_env "${runtime_library_path}")
endif()

set(consumer_executable "${consumer_build_dir}/loki_consumer")
if(WIN32 AND LOKI_CONFIG)
    set(consumer_executable "${consumer_build_dir}/${LOKI_CONFIG}/loki_consumer.exe")
endif()

if(UNIX)
    execute_process(
        COMMAND
            "${CMAKE_COMMAND}" -E env
                "LD_LIBRARY_PATH=${runtime_library_path_env}"
                "DYLD_LIBRARY_PATH=${runtime_library_path_env}"
                "${consumer_executable}"
        RESULT_VARIABLE run_result
        OUTPUT_VARIABLE run_output
        ERROR_VARIABLE run_error
    )
else()
    execute_process(
        COMMAND "${consumer_executable}"
        RESULT_VARIABLE run_result
        OUTPUT_VARIABLE run_output
        ERROR_VARIABLE run_error
    )
endif()
if(NOT run_result EQUAL 0)
    message(FATAL_ERROR "Failed to run loki consumer:\n${run_output}\n${run_error}")
endif()

file(WRITE "${bad_consumer_dir}/CMakeLists.txt" [[
cmake_minimum_required(VERSION 3.21)

project(loki_bad_consumer LANGUAGES CXX)

find_package(loki CONFIG REQUIRED COMPONENTS missing)
]])

execute_process(
    COMMAND
        "${CMAKE_COMMAND}"
            -S "${bad_consumer_dir}"
            -B "${bad_consumer_build_dir}"
            -G "${LOKI_GENERATOR}"
            "-DCMAKE_PREFIX_PATH=${prefix_path}"
    RESULT_VARIABLE bad_configure_result
    OUTPUT_VARIABLE bad_configure_output
    ERROR_VARIABLE bad_configure_error
)
set(bad_configure_log "${bad_configure_output}\n${bad_configure_error}")
if(bad_configure_result EQUAL 0)
    message(FATAL_ERROR "Unsupported loki component unexpectedly configured successfully.")
endif()
if(NOT bad_configure_log MATCHES "Unsupported component: missing")
    message(FATAL_ERROR "Unsupported component diagnostic was missing:\n${bad_configure_log}")
endif()
