file(GLOB SRC_FILES "*.cpp")
add_executable(parser ${SRC_FILES})
target_link_libraries(parser pddl)

