file(GLOB SRC_FILES
    "common/*.cpp"
    "common/ast/*.cpp"
    "common/pddl/*.cpp"
    "common/pddl/parser/*.cpp"
    "domain/*.cpp"
    "domain/ast/*.cpp"
    "domain/pddl/*.cpp"
    "domain/pddl/parser/*.cpp"
    "problem/*.cpp"
    "problem/ast/*.cpp"
    "problem/pddl/*.cpp"
    "problem/pddl/parser/*.cpp")
add_library(parsers STATIC ${SRC_FILES})
# target_link_libraries(parsers PRIVATE flatbuffers)

# Create an alias for simpler reference
add_library(loki::parsers ALIAS parsers)
