set(PYPDDL_INSTALL_PACKAGEDIR "pypddl")

install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/pypddl/__init__.py"
        DESTINATION "${PYPDDL_INSTALL_PACKAGEDIR}"
        COMPONENT pypddl)

set(PYPDDL_INSTALL_LIBRARY_PATHS)
foreach(native_prefix IN ITEMS "${YGGDRASIL_NATIVE_PREFIX}")
    if(native_prefix)
        file(GLOB native_lib_dirs LIST_DIRECTORIES true "${native_prefix}/lib*")
        foreach(native_lib_dir IN LISTS native_lib_dirs)
            if(IS_DIRECTORY "${native_lib_dir}")
                list(APPEND PYPDDL_INSTALL_LIBRARY_PATHS "${native_lib_dir}")
            endif()
        endforeach()
    endif()
endforeach()
list(JOIN PYPDDL_INSTALL_LIBRARY_PATHS ":" PYPDDL_INSTALL_LIBRARY_PATH)

if(PYPDDL_INSTALL_LIBRARY_PATH)
    install(CODE "
        set(ENV{LD_LIBRARY_PATH} \"${PYPDDL_INSTALL_LIBRARY_PATH}:$ENV{LD_LIBRARY_PATH}\")
        set(ENV{DYLD_LIBRARY_PATH} \"${PYPDDL_INSTALL_LIBRARY_PATH}:$ENV{DYLD_LIBRARY_PATH}\")
    " COMPONENT pypddl)
endif()

if(APPLE)
    install(SCRIPT "${PROJECT_SOURCE_DIR}/cmake/install/fix_macos_runtime_paths.cmake"
            COMPONENT pypddl)
elseif(UNIX)
    install(SCRIPT "${PROJECT_SOURCE_DIR}/cmake/install/fix_unix_runtime_paths.cmake"
            COMPONENT pypddl)
endif()

add_custom_target(pypddl_wheel DEPENDS parsers)
