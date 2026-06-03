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
add_gtest(semantic_parser                      "semantic/parser.cpp")
add_gtest(semantic_negative_suite              "semantic/negative_suite.cpp")
add_gtest(semantic_translation_counts_suite    "semantic/translation_counts_suite.cpp")
add_gtest(formalism_parser                     "formalism/parser.cpp")
add_gtest(formalism_translator                 "formalism/translator.cpp")
