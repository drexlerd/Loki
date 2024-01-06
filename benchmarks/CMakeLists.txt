find_package(benchmark "1.8.3" REQUIRED)

add_executable(persistent_factory "persistent_factory.cpp")
target_link_libraries(persistent_factory loki::parsers)
target_link_libraries(persistent_factory benchmark::benchmark)