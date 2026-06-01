add_library(parsers INTERFACE)

set(LOKI_YGGDRASIL_INCLUDE_DIRS)
foreach(candidate IN ITEMS
        "${YGGDRASIL_NATIVE_PREFIX}/include"
        "${PROJECT_SOURCE_DIR}/../yggdrasil/include")
    if(candidate AND EXISTS "${candidate}/yggdrasil")
        list(APPEND LOKI_YGGDRASIL_INCLUDE_DIRS "${candidate}")
    endif()
endforeach()

# Create an alias for simpler reference
add_library(loki::parsers ALIAS parsers)

target_compile_definitions(parsers INTERFACE
    BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
    BOOST_MPL_LIMIT_LIST_SIZE=50
)

target_link_libraries(parsers
    INTERFACE
        gtl::gtl
        fmt::fmt)

target_include_directories(parsers
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${LOKI_YGGDRASIL_INCLUDE_DIRS}>"
        "$<INSTALL_INTERFACE:${LOKI_INSTALL_INTERFACE_INCLUDEDIR}>"
)

# Install the target and create export-set
install(
    TARGETS parsers
    EXPORT lokiparsersTargets
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
