enable_testing()

find_package(GTest "1.11.0" REQUIRED PATHS ${CMAKE_PREFIX_PATH}/googletest NO_SYSTEM_ENVIRONMENT_PATH)

file(GLOB_RECURSE SRC_FILES
    "common/*.cpp" "common/**/*.cpp"
    "domain/*.cpp" "domain/**/*.cpp"
    "problem/*.cpp" "problem/**/*.cpp")
file(GLOB_RECURSE HEADER_FILES
    "common/*.hpp" "common/**/*.hpp"
    "domain/*.hpp" "domain/**/*.hpp"
    "problem/*.hpp" "problem/**/*.hpp")

set(TEST_NAME ${CMAKE_PROJECT_NAME}_tests)

add_executable(${TEST_NAME} ${SRC_FILES} ${HEADER_FILES})

target_link_libraries(${TEST_NAME}
    PRIVATE
        loki::parsers
        GTest::GTest
        GTest::Main)

add_test(domain_gtests ${TEST_NAME})
