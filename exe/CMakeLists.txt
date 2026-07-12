add_executable(loki "loki.cpp")
target_compile_definitions(loki PRIVATE LOKI_VERSION="${PROJECT_VERSION}")
target_link_libraries(loki loki::parsers)
