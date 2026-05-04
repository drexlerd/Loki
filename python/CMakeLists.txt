set(PYPDDL_INSTALL_PACKAGEDIR "pypddl")

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/pypddl/__init__.py"
        DESTINATION "${PYPDDL_INSTALL_PACKAGEDIR}"
        COMPONENT pypddl)

if(APPLE)
    install(SCRIPT "${PROJECT_SOURCE_DIR}/cmake/install/fix_macos_runtime_paths.cmake"
            COMPONENT pypddl)
elseif(UNIX)
    install(SCRIPT "${PROJECT_SOURCE_DIR}/cmake/install/fix_unix_runtime_paths.cmake"
            COMPONENT pypddl)
endif()

add_custom_target(pypddl_wheel DEPENDS parsers)
