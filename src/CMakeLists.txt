file(GLOB HEADER_FILES
    "../include/loki/common/*.hpp"
    "../include/loki/common/ast/*.hpp"
    "../include/loki/common/pddl/*.hpp"
    "../include/loki/common/pddl/parser/*.hpp"
    "../include/loki/domain/*.hpp"
    "../include/loki/domain/ast/*.hpp"
    "../include/loki/domain/pddl/*.hpp"
    "../include/loki/domain/pddl/parser/*.hpp"
    "../include/loki/problem/*.hpp"
    "../include/loki/problem/ast/*.hpp"
    "../include/loki/problem/pddl/*.hpp"
    "../include/loki/problem/pddl/parser/*.hpp")
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
add_library(parsers STATIC ${SRC_FILES} ${HEADER_FILES})
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
