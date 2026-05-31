file(GLOB_RECURSE SRC_FILES
    "*.cpp" "**/*.cpp")
file(GLOB_RECURSE PRIVATE_HEADER_FILES
    "*.hpp" "**/*.hpp")
file(GLOB_RECURSE PUBLIC_HEADER_FILES
    "../include/*.hpp" "../include/**/*.hpp")
add_library(parsers SHARED ${SRC_FILES} ${PRIVATE_HEADER_FILES} ${PUBLIC_HEADER_FILES})
set_target_properties(parsers PROPERTIES OUTPUT_NAME loki_parsers)

if(LOKI_BUILD_PYPDDL)
    if(APPLE)
        set(LOKI_PARSERS_INSTALL_RPATH "@loader_path;@loader_path/../../../pyyggdrasil/lib")
    else()
        set(LOKI_PARSERS_INSTALL_RPATH "$ORIGIN;$ORIGIN/../../../pyyggdrasil/lib")
    endif()
elseif(APPLE)
    set(LOKI_PARSERS_INSTALL_RPATH "@loader_path")
else()
    set(LOKI_PARSERS_INSTALL_RPATH "$ORIGIN")
endif()

if(LOKI_PARSERS_INSTALL_RPATH)
    set_target_properties(parsers PROPERTIES INSTALL_RPATH "${LOKI_PARSERS_INSTALL_RPATH}")
endif()

if(YGGDRASIL_NATIVE_PREFIX)
    set_target_properties(parsers PROPERTIES BUILD_RPATH "${YGGDRASIL_NATIVE_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
endif()

# Add compile definitions for the parsers target
target_compile_definitions(parsers PUBLIC
    BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
    BOOST_MPL_LIMIT_LIST_SIZE=50
)

# Create an alias for simpler reference
add_library(loki::parsers ALIAS parsers)

target_link_libraries(parsers
  PUBLIC
    gtl::gtl
    fmt::fmt)

# Use include depending on building or using from installed location
target_include_directories(parsers
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${LOKI_INSTALL_INTERFACE_INCLUDEDIR}>"
)

# Install the target and create export-set
install(
    TARGETS parsers
    EXPORT lokiparsersTargets
    LIBRARY DESTINATION ${LOKI_INSTALL_PREFIX}${CMAKE_INSTALL_LIBDIR} COMPONENT ${LOKI_INSTALL_COMPONENT}
    ARCHIVE DESTINATION ${LOKI_INSTALL_PREFIX}${CMAKE_INSTALL_LIBDIR} COMPONENT ${LOKI_INSTALL_COMPONENT}
    RUNTIME DESTINATION ${LOKI_INSTALL_PREFIX}${CMAKE_INSTALL_BINDIR} COMPONENT ${LOKI_INSTALL_COMPONENT}
    INCLUDES DESTINATION ${LOKI_INSTALL_PREFIX}${CMAKE_INSTALL_INCLUDEDIR}
)

# Generate and install export file
install(EXPORT lokiparsersTargets
    NAMESPACE loki::
    FILE "lokiparsersTargets.cmake"
    DESTINATION "${LOKI_INSTALL_PREFIX}${CMAKE_INSTALL_LIBDIR}/cmake/loki"
    COMPONENT ${LOKI_INSTALL_COMPONENT}
)

# Generate build tree export file
export(EXPORT lokiparsersTargets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/lokiparsersTargets.cmake"
       NAMESPACE loki::
)
