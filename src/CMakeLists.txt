file(GLOB SRC_FILES
    "common/*.cpp"
    "domain/ast/*.cpp"
    "problem/ast/*.cpp")
add_library(parsers STATIC ${SRC_FILES})

# Create an alias for simpler reference
add_library(loki::parsers ALIAS parsers)