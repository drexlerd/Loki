find_package(Python 3.8 REQUIRED
  COMPONENTS Interpreter Development.Module
  OPTIONAL_COMPONENTS Development.SABIModule)

find_package(nanobind CONFIG REQUIRED PATHS ${CMAKE_PREFIX_PATH} "${YGGDRASIL_NATIVE_PREFIX}/nanobind/cmake")
message(STATUS "Found nanobind: ${nanobind_DIR} (found version ${nanobind_VERSION})")

nanobind_add_module(_pypddl
    src/module.cpp
    src/index.cpp
    src/data.cpp
    src/view.cpp)
target_link_libraries(_pypddl PRIVATE loki::parsers)
set(PYPDDL_YGGDRASIL_INCLUDE_DIRS)
foreach(candidate IN ITEMS
        "${YGGDRASIL_NATIVE_PREFIX}/include"
        "${PROJECT_SOURCE_DIR}/../yggdrasil/include")
    if(candidate AND EXISTS "${candidate}/yggdrasil")
        list(APPEND PYPDDL_YGGDRASIL_INCLUDE_DIRS "${candidate}")
    endif()
endforeach()
target_include_directories(_pypddl PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${PYPDDL_YGGDRASIL_INCLUDE_DIRS})

if(APPLE)
    set(PYPDDL_MODULE_RPATH "@loader_path/native/${CMAKE_INSTALL_LIBDIR};@loader_path")
elseif(UNIX)
    set(PYPDDL_MODULE_RPATH "$ORIGIN/native/${CMAKE_INSTALL_LIBDIR};$ORIGIN")
endif()

if(PYPDDL_MODULE_RPATH)
    set_target_properties(_pypddl PROPERTIES
        BUILD_RPATH "${PYPDDL_MODULE_RPATH}"
        INSTALL_RPATH "${PYPDDL_MODULE_RPATH}")
endif()

set(PYPDDL_INSTALL_PACKAGEDIR "pypddl")

install(TARGETS _pypddl DESTINATION "${PYPDDL_INSTALL_PACKAGEDIR}" COMPONENT pypddl)

install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pypddl/__init__.py"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/pypddl/__init__.pyi"
        DESTINATION "${PYPDDL_INSTALL_PACKAGEDIR}"
        COMPONENT pypddl)

set(PYPDDL_INSTALL_NATIVE_PREFIXES)
if(YGGDRASIL_NATIVE_PREFIX)
    list(APPEND PYPDDL_INSTALL_NATIVE_PREFIXES "${YGGDRASIL_NATIVE_PREFIX}")
endif()
list(APPEND PYPDDL_INSTALL_NATIVE_PREFIXES ${CMAKE_PREFIX_PATH})
list(REMOVE_DUPLICATES PYPDDL_INSTALL_NATIVE_PREFIXES)

set(PYPDDL_INSTALL_LIBRARY_PATHS)
foreach(native_prefix IN LISTS PYPDDL_INSTALL_NATIVE_PREFIXES)
    if(native_prefix)
        file(GLOB native_lib_dirs LIST_DIRECTORIES true "${native_prefix}/lib*")
        foreach(native_lib_dir IN LISTS native_lib_dirs)
            if(IS_DIRECTORY "${native_lib_dir}")
                list(APPEND PYPDDL_INSTALL_LIBRARY_PATHS "${native_lib_dir}")
            endif()
        endforeach()
    endif()
endforeach()
list(REMOVE_DUPLICATES PYPDDL_INSTALL_LIBRARY_PATHS)
list(JOIN PYPDDL_INSTALL_LIBRARY_PATHS ":" PYPDDL_INSTALL_LIBRARY_PATH)

if(PYPDDL_INSTALL_LIBRARY_PATH)
    install(CODE "
        set(ENV{LD_LIBRARY_PATH} \"${PYPDDL_INSTALL_LIBRARY_PATH}:$ENV{LD_LIBRARY_PATH}\")
        set(ENV{DYLD_LIBRARY_PATH} \"${PYPDDL_INSTALL_LIBRARY_PATH}:$ENV{DYLD_LIBRARY_PATH}\")
    " COMPONENT pypddl)
endif()

nanobind_add_stub(
    pypddl_stubs
    MODULE pypddl._pypddl
    RECURSIVE
    INSTALL_TIME
    OUTPUT_PATH "${PYPDDL_INSTALL_PACKAGEDIR}"
    PYTHON_PATH "."
    COMPONENT pypddl
)

install(SCRIPT "${PROJECT_SOURCE_DIR}/cmake/patch_python_stubs.cmake" COMPONENT pypddl)

if(APPLE)
    install(SCRIPT "${PROJECT_SOURCE_DIR}/cmake/install/fix_macos_runtime_paths.cmake"
            COMPONENT pypddl)
elseif(UNIX)
    install(SCRIPT "${PROJECT_SOURCE_DIR}/cmake/install/fix_unix_runtime_paths.cmake"
            COMPONENT pypddl)
endif()

add_custom_target(pypddl_wheel DEPENDS _pypddl parsers)
