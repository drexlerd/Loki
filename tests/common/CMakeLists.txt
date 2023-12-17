file(GLOB SRC_FILES
    "*.cpp")

add_executable(common_tests ${SRC_FILES})

target_link_libraries(common_tests
    PRIVATE
        loki::parsers
        GTest::GTest)

add_test(common_gtests common_tests)
