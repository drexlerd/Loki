file(GLOB SRC_FILES
    "common/*.cpp"
    "common/pddl/*.cpp"
    "domain/ast/*.cpp"
    "domain/pddl/*.cpp"
    "domain/pddl/parser/*.cpp"
    "problem/ast/*.cpp"
    "problem/pddl/*.cpp"
    "problem/pddl/parser/*.cpp")
add_library(parsers STATIC ${SRC_FILES})

# Create an alias for simpler reference
add_library(loki::parsers ALIAS parsers)
