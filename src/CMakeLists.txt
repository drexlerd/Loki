add_library(parsers SHARED
    semantic/builder.cpp
    semantic/context.cpp
    semantic/diagnostics.cpp
    semantic/mappings.cpp
    semantic/parser.cpp
    semantic/translator.cpp
)
set_target_properties(parsers PROPERTIES OUTPUT_NAME loki_parsers)

if(APPLE)
    set(LOKI_LIBRARY_ORIGIN "@loader_path")
elseif(UNIX)
    set(LOKI_LIBRARY_ORIGIN "$ORIGIN")
endif()

foreach(prefix IN LISTS CMAKE_PREFIX_PATH)
    if(EXISTS "${prefix}/${CMAKE_INSTALL_LIBDIR}")
        list(APPEND LOKI_PARSERS_BUILD_RPATHS "${prefix}/${CMAKE_INSTALL_LIBDIR}")
    endif()
endforeach()

set_target_properties(parsers PROPERTIES
    BUILD_RPATH "${LOKI_PARSERS_BUILD_RPATHS}"
    INSTALL_RPATH "${LOKI_LIBRARY_ORIGIN}")

# Create an alias for simpler reference
add_library(loki::parsers ALIAS parsers)

target_compile_definitions(parsers
    PUBLIC
        BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
        BOOST_MPL_LIMIT_LIST_SIZE=50
)

target_compile_features(parsers
    PUBLIC
        cxx_std_20
)

target_link_libraries(parsers
    PUBLIC
        yggdrasil::yggdrasil
        gtl::gtl
        fmt::fmt)

target_include_directories(parsers
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${YGGDRASIL_NATIVE_DEPENDENCY_INCLUDE_DIRECTORIES}>"
        "$<INSTALL_INTERFACE:${LOKI_INSTALL_INTERFACE_INCLUDEDIR}>"
)

if(NOT LOKI_BUILD_PYPDDL)
    # Install the target and create export-set
    install(
        TARGETS parsers
        EXPORT lokiparsersTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT loki
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT loki
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT loki
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    # Generate and install export file
    install(EXPORT lokiparsersTargets
        NAMESPACE loki::
        FILE "lokiparsersTargets.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/loki"
        COMPONENT loki
    )
    # Generate build tree export file
    export(EXPORT lokiparsersTargets
           FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/lokiparsersTargets.cmake"
           NAMESPACE loki::
    )
endif()
