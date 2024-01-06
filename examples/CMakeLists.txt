add_executable(multiple_problems "multiple_problems.cpp")
target_link_libraries(multiple_problems loki::parsers)

add_executable(position_cache "position_cache.cpp")
target_link_libraries(position_cache loki::parsers)

add_executable(single_problem "single_problem.cpp")
target_link_libraries(single_problem loki::parsers)

add_executable(undefined_behavior "undefined_behavior.cpp")
target_link_libraries(undefined_behavior loki::parsers)


add_custom_target(example_domain ALL
    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/data/gripper/domain.pddl" "${CMAKE_BINARY_DIR}/data/gripper/domain.pddl")
add_custom_target(example_problem_0 ALL
    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/data/gripper/p-2-0.pddl" "${CMAKE_BINARY_DIR}/data/gripper/p-2-0.pddl")
add_custom_target(example_problem_1 ALL
    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/data/gripper/p-2-1.pddl" "${CMAKE_BINARY_DIR}/data/gripper/p-2-1.pddl")