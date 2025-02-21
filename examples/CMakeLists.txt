add_executable(multiple_problems "multiple_problems.cpp")
target_link_libraries(multiple_problems loki::parsers)

add_executable(single_problem "single_problem.cpp")
target_link_libraries(single_problem loki::parsers)

add_executable(undefined_behavior "undefined_behavior.cpp")
target_link_libraries(undefined_behavior loki::parsers)
