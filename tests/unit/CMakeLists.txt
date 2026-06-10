find_package(GTest "1.11.0" REQUIRED COMPONENTS GTest Main PATHS ${CMAKE_PREFIX_PATH} NO_DEFAULT_PATH)
find_library(Boost_JSON_LIBRARY boost_json PATHS ${CMAKE_PREFIX_PATH} PATH_SUFFIXES lib NO_DEFAULT_PATH REQUIRED)

function(add_gtest test_name source_file)
    add_executable(${test_name} ${source_file} main.cpp)
    target_compile_definitions(${test_name} PRIVATE ROOT_DIR="${CMAKE_SOURCE_DIR}")
    target_link_libraries(${test_name}
        PRIVATE
            loki::parsers
            GTest::gtest
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

add_gtest(parser_suite                         "parser/suite.cpp")
add_gtest(public_facade                        "public_facade.cpp")
add_gtest(public_headers                       "public_headers.cpp")

file(GLOB_RECURSE LOKI_DIRECT_PUBLIC_HEADERS
    CONFIGURE_DEPENDS
    RELATIVE "${CMAKE_SOURCE_DIR}/include"
    "${CMAKE_SOURCE_DIR}/include/loki/*.hpp"
)
list(SORT LOKI_DIRECT_PUBLIC_HEADERS)

set(LOKI_PUBLIC_HEADER_TARGETS)
foreach(header IN LISTS LOKI_DIRECT_PUBLIC_HEADERS)
    string(REGEX REPLACE "[^A-Za-z0-9_]+" "_" header_target_name "${header}")
    set(header_target "public_header_${header_target_name}")
    set(header_test_source "${CMAKE_CURRENT_BINARY_DIR}/${header_target}.cpp")
    file(GENERATE
        OUTPUT "${header_test_source}"
        CONTENT "#include <${header}>
int main() { return 0; }
")
    add_executable(${header_target} "${header_test_source}")
    target_link_libraries(${header_target} PRIVATE loki::parsers)
    list(APPEND LOKI_PUBLIC_HEADER_TARGETS ${header_target})
endforeach()
if(LOKI_PUBLIC_HEADER_TARGETS)
    add_dependencies(public_headers ${LOKI_PUBLIC_HEADER_TARGETS})
endif()
add_gtest(semantic_parser                      "semantic/parser.cpp")
add_gtest(semantic_negative_suite              "semantic/negative_suite.cpp")
add_gtest(semantic_translation_counts_suite    "semantic/translation_counts_suite.cpp")
add_gtest(formalism_parser                     "formalism/parser.cpp")
add_gtest(formalism_translator                 "formalism/translator.cpp")
