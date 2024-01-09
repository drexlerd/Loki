
enable_testing()

find_package(GTest "1.11.0" REQUIRED)

file(GLOB SRC_FILES
    "common/pddl/*.cpp"
    "domain/ast/*.cpp")

set(TEST_NAME ${CMAKE_PROJECT_NAME}_tests)

add_executable(${TEST_NAME} ${SRC_FILES})

target_link_libraries(${TEST_NAME}
    PRIVATE
        loki::parsers
        GTest::GTest
        GTest::Main)

add_test(domain_gtests ${TEST_NAME})
