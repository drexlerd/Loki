enable_testing()
option(INSTALL_GMOCK "Install Googletest's GMock?" OFF)
option(INSTALL_GTEST "Install Googletest's GTest?" OFF)

set(TEST_NAME ${CMAKE_PROJECT_NAME}_tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)

add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

file(GLOB SRC_FILES
    "common/pddl/*.cpp"
    "domain/ast/*.cpp")

add_executable(domain_tests ${SRC_FILES})

target_link_libraries(domain_tests
    PRIVATE
        loki::parsers
        GTest::GTest)

add_test(domain_gtests domain_tests)
