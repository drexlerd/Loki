file(GLOB SRC_FILES "*.cpp")
add_executable(exe ${SRC_FILES})
target_link_libraries(exe parsers)

