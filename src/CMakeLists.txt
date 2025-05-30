file(GLOB_RECURSE SRC_FILES
    "*.cpp" "**/*.cpp")
file(GLOB_RECURSE PRIVATE_HEADER_FILES
    "*.hpp" "**/*.hpp")
file(GLOB_RECURSE PUBLIC_HEADER_FILES
    "../include/*.hpp" "../include/**/*.hpp")
add_library(parsers STATIC ${SRC_FILES} ${PRIVATE_HEADER_FILES} ${PUBLIC_HEADER_FILES})
set_target_properties(parsers PROPERTIES OUTPUT_NAME loki_parsers)

# Add compile definitions for the parsers target
target_compile_definitions(parsers PUBLIC
    BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
    BOOST_MPL_LIMIT_LIST_SIZE=50
)

# Create an alias for simpler reference
add_library(loki::parsers ALIAS parsers)

target_link_libraries(parsers absl::flat_hash_set)

# Use include depending on building or using from installed location
target_include_directories(parsers
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
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
    FILE "lokiparsersTargets.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/loki"
)

# Generate build tree export file
export(EXPORT lokiparsersTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/lokiparsersTargets.cmake"
       NAMESPACE loki::
)
