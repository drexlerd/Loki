file(GLOB_RECURSE SRC_FILES
    "common/*.cpp"
    "common/**/*.cpp"
    "domain/*.cpp"
    "domain/**/*.cpp"
    "problem/*.cpp"
    "problem/**/*.cpp")
file(GLOB_RECURSE PRIVATE_HEADER_FILES
    "common/*.hpp"
    "common/**/*.hpp"
    "domain/*.hpp"
    "domain/**/*.hpp"
    "problem/*.hpp"
    "problem/**/*.hpp")
file(GLOB_RECURSE PUBLIC_HEADER_FILES
    "../include/loki/common/*.hpp"
    "../include/loki/common/**/*.hpp"
    "../include/loki/domain/*.hpp"
    "../include/loki/domain/**/*.hpp"
    "../include/loki/problem/*.hpp"
    "../include/loki/problem/**/*.hpp")
add_library(parsers STATIC ${SRC_FILES} ${PRIVATE_HEADER_FILES} ${PUBLIC_HEADER_FILES})
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
