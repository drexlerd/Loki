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

# Use include depending on building or using from installed location
target_include_directories(parsers
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

# Install the target and create export-set
install(
    TARGETS parsers
    EXPORT parsersTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Generate and install export file
install(EXPORT parsersTargets
    NAMESPACE loki::
    COMPONENT parsers
    FILE parsersTargets.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/loki"
)

# Generate build tree export file
export(EXPORT parsersTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/parsersTargets.cmake"
       NAMESPACE dlplan::
)
