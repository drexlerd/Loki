find_package(Python 3.8 REQUIRED
  COMPONENTS Interpreter Development.Module
  OPTIONAL_COMPONENTS Development.SABIModule)

include(CMakePackageConfigHelpers)

find_package(nanobind CONFIG REQUIRED PATHS ${CMAKE_PREFIX_PATH})
message(STATUS "Found nanobind: ${nanobind_DIR} (found version ${nanobind_VERSION})")

set(PYPDDL_NATIVE_DIR "pypddl/native")
set(PYPDDL_NATIVE_INCLUDEDIR "${PYPDDL_NATIVE_DIR}/${CMAKE_INSTALL_INCLUDEDIR}")
set(PYPDDL_NATIVE_LIBDIR "${PYPDDL_NATIVE_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(PYPDDL_NATIVE_BINDIR "${PYPDDL_NATIVE_DIR}/${CMAKE_INSTALL_BINDIR}")
set(PYPDDL_NATIVE_CMAKEDIR "${PYPDDL_NATIVE_LIBDIR}/cmake")

nanobind_add_module(_pypddl
    src/module.cpp
    src/pypddl/formalism/module.cpp
    src/pypddl/formalism/data.cpp
    src/pypddl/formalism/index.cpp
    src/pypddl/formalism/view.cpp)

target_link_libraries(_pypddl PRIVATE loki::parsers)
target_include_directories(_pypddl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(APPLE)
    loki_make_python_native_runtime_rpaths(PYPDDL_MODULE_RPATH "@loader_path" "../")
    list(APPEND PYPDDL_MODULE_RPATH "@loader_path/native/${CMAKE_INSTALL_LIBDIR}")
    loki_make_python_native_runtime_rpaths(PYPDDL_PARSERS_RPATH "@loader_path" "../../../")
    loki_make_python_native_runtime_rpaths(PYPDDL_INSTALL_NATIVE_LIBRARY_RPATHS "@loader_path" "../../../")
elseif(UNIX)
    loki_make_python_native_runtime_rpaths(PYPDDL_MODULE_RPATH "$ORIGIN" "../")
    list(APPEND PYPDDL_MODULE_RPATH "$ORIGIN/native/${CMAKE_INSTALL_LIBDIR}")
    loki_make_python_native_runtime_rpaths(PYPDDL_PARSERS_RPATH "$ORIGIN" "../../../")
    loki_make_python_native_runtime_rpath_string(PYPDDL_INSTALL_NATIVE_LIBRARY_RPATH "$ORIGIN" "../../../")
endif()

if(PYPDDL_MODULE_RPATH)
    set_target_properties(_pypddl PROPERTIES
        BUILD_RPATH "${PYPDDL_MODULE_RPATH}"
        INSTALL_RPATH "${PYPDDL_MODULE_RPATH}")
endif()

install(TARGETS _pypddl DESTINATION "pypddl" COMPONENT pypddl)
set_property(TARGET parsers PROPERTY INTERFACE_INCLUDE_DIRECTORIES
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${LOKI_NATIVE_DEPENDENCY_INCLUDE_DIRECTORIES}>")
set_property(TARGET parsers PROPERTY INSTALL_RPATH "${PYPDDL_PARSERS_RPATH}")
install(TARGETS parsers
        EXPORT pypddlLokiparsersTargets
        LIBRARY DESTINATION "${PYPDDL_NATIVE_LIBDIR}" COMPONENT pypddl
        ARCHIVE DESTINATION "${PYPDDL_NATIVE_LIBDIR}" COMPONENT pypddl
        RUNTIME DESTINATION "${PYPDDL_NATIVE_BINDIR}" COMPONENT pypddl
        INCLUDES DESTINATION "${PYPDDL_NATIVE_INCLUDEDIR}")
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/loki"
        DESTINATION "${PYPDDL_NATIVE_INCLUDEDIR}"
        COMPONENT pypddl)
configure_package_config_file("${CMAKE_SOURCE_DIR}/Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/lokiConfig.cmake"
        INSTALL_DESTINATION "${PYPDDL_NATIVE_CMAKEDIR}/loki"
        NO_CHECK_REQUIRED_COMPONENTS_MACRO)
install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/lokiConfig.cmake"
            "${CMAKE_BINARY_DIR}/lokiConfigVersion.cmake"
        DESTINATION "${PYPDDL_NATIVE_CMAKEDIR}/loki"
        COMPONENT pypddl)
install(EXPORT pypddlLokiparsersTargets
        NAMESPACE loki::
        FILE "lokiparsersTargets.cmake"
        DESTINATION "${PYPDDL_NATIVE_CMAKEDIR}/loki"
        COMPONENT pypddl)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/cmake/"
        DESTINATION "${PYPDDL_NATIVE_CMAKEDIR}/loki/cmake"
        COMPONENT pypddl)

if(APPLE)
    install(CODE
            "set(PYPDDL_PYTHON_NATIVE_LIBRARY_RPATHS \"${PYPDDL_INSTALL_NATIVE_LIBRARY_RPATHS}\")
             include(\"${PROJECT_SOURCE_DIR}/cmake/install/fix_macos_runtime_paths.cmake\")"
            COMPONENT pypddl)
elseif(UNIX)
    install(CODE
            "set(PYPDDL_PYTHON_NATIVE_LIBRARY_RPATH \"${PYPDDL_INSTALL_NATIVE_LIBRARY_RPATH}\")
             include(\"${PROJECT_SOURCE_DIR}/cmake/install/fix_unix_runtime_paths.cmake\")"
            COMPONENT pypddl)
endif()

set(PYPDDL_PYTHON_FILES
    pypddl/__init__.py
    pypddl/__init__.pyi
    pypddl/formalism/__init__.py
)

foreach(python_file IN LISTS PYPDDL_PYTHON_FILES)
    get_filename_component(python_file_directory "${python_file}" DIRECTORY)
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/${python_file}"
            DESTINATION "${python_file_directory}"
            COMPONENT pypddl)
endforeach()

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
    install(CODE
        "set(ENV{LD_LIBRARY_PATH} \"${PYPDDL_INSTALL_LIBRARY_PATH}:$ENV{LD_LIBRARY_PATH}\")
         set(ENV{DYLD_LIBRARY_PATH} \"${PYPDDL_INSTALL_LIBRARY_PATH}:$ENV{DYLD_LIBRARY_PATH}\")"
        COMPONENT pypddl)
endif()

nanobind_add_stub(
    pypddl_stubs
    MODULE pypddl._pypddl
    RECURSIVE
    INSTALL_TIME
    OUTPUT_PATH "pypddl"
    PYTHON_PATH "."
    COMPONENT pypddl
)

install(SCRIPT "${PROJECT_SOURCE_DIR}/cmake/patch_python_stubs.cmake" COMPONENT pypddl)

add_custom_target(pypddl_wheel DEPENDS _pypddl parsers)
