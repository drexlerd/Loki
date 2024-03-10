enable_testing()

find_package(GTest "1.11.0" REQUIRED COMPONENTS GTest Main PATHS ${CMAKE_PREFIX_PATH} NO_DEFAULT_PATH)
# Set result variables
find_package(GTest)

file(GLOB_RECURSE SRC_FILES
    "*.cpp" "**/*.cpp")
file(GLOB_RECURSE HEADER_FILES
    "*.hpp" "**/*.hpp")

set(TEST_NAME ${CMAKE_PROJECT_NAME}_tests)

add_executable(${TEST_NAME} ${SRC_FILES} ${HEADER_FILES})

target_link_libraries(${TEST_NAME}
    PRIVATE
        loki::parsers
        GTest::GTest
        GTest::Main)

add_test(domain_gtests ${TEST_NAME})
