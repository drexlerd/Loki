find_package(GTest "1.11.0" REQUIRED COMPONENTS GTest Main PATHS ${CMAKE_PREFIX_PATH} NO_DEFAULT_PATH)
find_library(Boost_JSON_LIBRARY boost_json PATHS ${CMAKE_PREFIX_PATH} PATH_SUFFIXES lib NO_DEFAULT_PATH REQUIRED)

function(add_gtest test_name source_file)
    add_executable(${test_name} ${source_file})
    target_compile_definitions(${test_name} PRIVATE ROOT_DIR="${CMAKE_SOURCE_DIR}")
    target_link_libraries(${test_name}
        PRIVATE
            loki::parsers
            GTest::gtest_main
            ${Boost_JSON_LIBRARY})

    set(LOKI_TEST_BUILD_RPATH)
    if(YGGDRASIL_NATIVE_PREFIX)
        list(APPEND LOKI_TEST_BUILD_RPATH "${YGGDRASIL_NATIVE_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
    endif()
    if(LOKI_TEST_BUILD_RPATH)
        set_target_properties(${test_name} PROPERTIES BUILD_RPATH "${LOKI_TEST_BUILD_RPATH}")
    endif()

    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_gtest(parser_counts_suite                  "parser/parser_counts_suite.cpp")
add_gtest(public_facade                        "public_facade.cpp")
add_gtest(public_headers                       "public_headers.cpp")

file(GLOB_RECURSE LOKI_DIRECT_PUBLIC_HEADERS
    CONFIGURE_DEPENDS
    RELATIVE "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/include/loki/*.hpp"
)
list(SORT LOKI_DIRECT_PUBLIC_HEADERS)

set(LOKI_PUBLIC_HEADER_SOURCES)
foreach(header IN LISTS LOKI_DIRECT_PUBLIC_HEADERS)
    string(REGEX REPLACE "[^A-Za-z0-9_]+" "_" header_target_name "${header}")
    set(header_test_source "${CMAKE_CURRENT_BINARY_DIR}/public_header_${header_target_name}.cpp")
    file(GENERATE
        OUTPUT "${header_test_source}"
        CONTENT "#include <${header}>
")
    list(APPEND LOKI_PUBLIC_HEADER_SOURCES "${header_test_source}")
endforeach()
if(LOKI_PUBLIC_HEADER_SOURCES)
    add_library(public_header_compile_checks OBJECT ${LOKI_PUBLIC_HEADER_SOURCES})
    target_link_libraries(public_header_compile_checks PRIVATE loki::parsers)
    add_dependencies(public_headers public_header_compile_checks)
endif()
add_gtest(semantic_parser_tests                "semantic/parser_tests.cpp")
add_gtest(semantic_translator_tests            "semantic/translator_tests.cpp")
add_gtest(semantic_diagnostics_tests           "semantic/diagnostics_tests.cpp")
add_gtest(semantic_action_costs_tests          "semantic/action_costs_tests.cpp")
add_gtest(semantic_negative_suite              "semantic/negative_suite.cpp")
add_gtest(semantic_invariant_suite             "semantic/invariant_suite.cpp")
add_gtest(semantic_translation_counts_suite    "semantic/translation_counts_suite.cpp")
add_gtest(formalism_translator_tests           "formalism/translator_tests.cpp")
