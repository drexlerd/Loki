find_package(GTest "1.11.0" REQUIRED COMPONENTS GTest Main PATHS ${CMAKE_PREFIX_PATH} NO_DEFAULT_PATH)
find_library(Boost_JSON_LIBRARY boost_json PATHS ${CMAKE_PREFIX_PATH} PATH_SUFFIXES lib NO_DEFAULT_PATH REQUIRED)

set(TEST_NAME ${CMAKE_PROJECT_NAME}_tests)

set(SRC_FILES
    main.cpp
    loki/parser_suite.cpp
    loki/public_facade.cpp
    loki/semantic_parser.cpp
    pddl/parser.cpp
    pddl/translator.cpp)

add_executable(${TEST_NAME} ${SRC_FILES})

target_link_libraries(${TEST_NAME}
    PRIVATE
        loki::parsers
        GTest::gtest
        ${Boost_JSON_LIBRARY})

set(LOKI_TEST_BUILD_RPATH)
if(YGGDRASIL_NATIVE_PREFIX)
    list(APPEND LOKI_TEST_BUILD_RPATH "${YGGDRASIL_NATIVE_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
endif()
if(LOKI_TEST_BUILD_RPATH)
    set_target_properties(${TEST_NAME} PROPERTIES BUILD_RPATH "${LOKI_TEST_BUILD_RPATH}")
endif()

add_test(domain_gtests ${TEST_NAME})
