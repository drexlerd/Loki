find_package(benchmark "1.8.3" REQUIRED)

add_executable(construct_atoms "construct_atoms.cpp" "utils.cpp")
target_link_libraries(construct_atoms loki::parsers)
target_link_libraries(construct_atoms benchmark::benchmark)

add_executable(iterate_atoms "iterate_atoms.cpp" "utils.cpp")
target_link_libraries(iterate_atoms loki::parsers)
target_link_libraries(iterate_atoms benchmark::benchmark)