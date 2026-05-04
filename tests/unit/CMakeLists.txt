find_package(GTest "1.11.0" REQUIRED COMPONENTS GTest Main PATHS ${CMAKE_PREFIX_PATH} NO_DEFAULT_PATH)

file(GLOB_RECURSE SRC_FILES
    "*.cpp" "**/*.cpp")
file(GLOB_RECURSE HEADER_FILES
    "*.hpp" "**/*.hpp")

set(TEST_NAME ${CMAKE_PROJECT_NAME}_tests)

add_executable(${TEST_NAME} ${SRC_FILES} ${HEADER_FILES})

target_include_directories(${TEST_NAME}
    PRIVATE
        "${PROJECT_SOURCE_DIR}/src")

target_link_libraries(${TEST_NAME}
    PRIVATE
        loki::parsers
        GTest::gtest)

if(BUILD_SHARED_LIBS)
    set(LOKI_TEST_BUILD_RPATH "$<TARGET_FILE_DIR:parsers>")
    if(YGGDRASIL_NATIVE_PREFIX)
        list(APPEND LOKI_TEST_BUILD_RPATH "${YGGDRASIL_NATIVE_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
    endif()
    set_target_properties(${TEST_NAME} PROPERTIES BUILD_RPATH "${LOKI_TEST_BUILD_RPATH}")
endif()

add_test(domain_gtests ${TEST_NAME})
