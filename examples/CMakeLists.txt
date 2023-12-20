add_executable(multiple_problems "multiple_problems.cpp")
target_link_libraries(multiple_problems parsers)

add_executable(position_cache "position_cache.cpp")
target_link_libraries(position_cache parsers)

add_executable(single_problem "single_problem.cpp")
target_link_libraries(single_problem parsers)

add_executable(undefined_behavior "undefined_behavior.cpp")
target_link_libraries(undefined_behavior parsers)


add_custom_target(state_space_delivery_example_domain ALL
    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/benchmarks/gripper/domain.pddl" "${CMAKE_BINARY_DIR}/benchmarks/gripper/domain.pddl")
add_custom_target(state_space_delivery_example_instance_1 ALL
    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/benchmarks/gripper/p-2-0.pddl" "${CMAKE_BINARY_DIR}/benchmarks/gripper/p-2-0.pddl")
add_custom_target(state_space_delivery_example_instance_2 ALL
    COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/benchmarks/gripper/p-2-1.pddl" "${CMAKE_BINARY_DIR}/benchmarks/gripper/p-2-1.pddl")