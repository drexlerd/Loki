file(GLOB SRC_FILES
    "ast/*.cpp")

add_executable(domain_tests ${SRC_FILES})

target_link_libraries(domain_tests
    PRIVATE
        loki::parsers
        GTest::GTest)

add_test(domain_gtests domain_tests)
