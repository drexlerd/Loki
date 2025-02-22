
add_executable(parser "parser.cpp")
target_link_libraries(parser loki::parsers)

add_executable(translator "translator.cpp")
target_link_libraries(translator loki::parsers)

