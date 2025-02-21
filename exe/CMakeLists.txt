add_executable(domain "domain.cpp")
target_link_libraries(domain loki::parsers)

add_executable(problem "problem.cpp")
target_link_libraries(problem loki::parsers)
