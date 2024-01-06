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
set_target_properties(parsers PROPERTIES OUTPUT_NAME loki_parsers)

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
    EXPORT lokiparsersTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Generate and install export file
install(EXPORT lokiparsersTargets
    NAMESPACE loki::
    COMPONENT parsers
    FILE lokiparsersTargets.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/loki"
)

# Generate build tree export file
export(EXPORT lokiparsersTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/lokiparsersTargets.cmake"
       NAMESPACE loki::
)
