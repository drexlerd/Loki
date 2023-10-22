file(GLOB SRC_FILES
    "common/*.cpp"
    "domain/*.cpp"
    "domain/stage_1_ast/*.cpp"
    "domain/stage_2_sketch/*.cpp"
    "problem/*.cpp"
    "problem/stage_1_ast/*.cpp"
    "problem/stage_2_sketch/*.cpp")
add_library(parsers STATIC ${SRC_FILES})

