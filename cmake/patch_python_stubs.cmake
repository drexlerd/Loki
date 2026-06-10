set(PYPDDL_STUB_ROOT "${CMAKE_INSTALL_PREFIX}/pypddl")
set(PYPDDL_PRIVATE_STUB_ROOT "${PYPDDL_STUB_ROOT}/_pypddl")

function(pypddl_patch_stub_file stub_file)
    file(READ "${stub_file}" PYPDDL_STUB_CONTENT)
    set(PYPDDL_PATCHED_STUB_CONTENT "${PYPDDL_STUB_CONTENT}")

    string(REPLACE "pypddl._pypddl." "pypddl." PYPDDL_PATCHED_STUB_CONTENT "${PYPDDL_PATCHED_STUB_CONTENT}")
    string(REPLACE "pyyggdrasil._pyyggdrasil." "pyyggdrasil." PYPDDL_PATCHED_STUB_CONTENT "${PYPDDL_PATCHED_STUB_CONTENT}")
    string(REPLACE "pypddl._pypddl" "pypddl" PYPDDL_PATCHED_STUB_CONTENT "${PYPDDL_PATCHED_STUB_CONTENT}")
    string(REPLACE "pyyggdrasil._pyyggdrasil" "pyyggdrasil" PYPDDL_PATCHED_STUB_CONTENT "${PYPDDL_PATCHED_STUB_CONTENT}")

    if(NOT PYPDDL_STUB_CONTENT STREQUAL PYPDDL_PATCHED_STUB_CONTENT)
        file(WRITE "${stub_file}" "${PYPDDL_PATCHED_STUB_CONTENT}")
    endif()
endfunction()

function(pypddl_install_public_stub private_stub_file public_stub_file)
    string(REGEX REPLACE "\\.pyi$" "" PYPDDL_PUBLIC_PACKAGE_DIR "${public_stub_file}")
    if(IS_DIRECTORY "${PYPDDL_PUBLIC_PACKAGE_DIR}")
        set(public_stub_file "${PYPDDL_PUBLIC_PACKAGE_DIR}/__init__.pyi")
    endif()

    if(public_stub_file STREQUAL "${PYPDDL_STUB_ROOT}/__init__.pyi" AND EXISTS "${public_stub_file}")
        return()
    endif()

    get_filename_component(PYPDDL_PUBLIC_STUB_DIR "${public_stub_file}" DIRECTORY)
    file(MAKE_DIRECTORY "${PYPDDL_PUBLIC_STUB_DIR}")
    file(READ "${private_stub_file}" PYPDDL_PRIVATE_STUB_CONTENT)
    file(WRITE "${public_stub_file}" "${PYPDDL_PRIVATE_STUB_CONTENT}")
endfunction()

if(EXISTS "${PYPDDL_PRIVATE_STUB_ROOT}")
    file(GLOB_RECURSE PYPDDL_PRIVATE_STUB_FILES "${PYPDDL_PRIVATE_STUB_ROOT}/*.pyi")

    foreach(PYPDDL_PRIVATE_STUB_FILE IN LISTS PYPDDL_PRIVATE_STUB_FILES)
        file(RELATIVE_PATH PYPDDL_PUBLIC_STUB_RELATIVE_PATH "${PYPDDL_PRIVATE_STUB_ROOT}" "${PYPDDL_PRIVATE_STUB_FILE}")
        pypddl_install_public_stub("${PYPDDL_PRIVATE_STUB_FILE}" "${PYPDDL_STUB_ROOT}/${PYPDDL_PUBLIC_STUB_RELATIVE_PATH}")
    endforeach()

    file(REMOVE_RECURSE "${PYPDDL_PRIVATE_STUB_ROOT}")
endif()

if(EXISTS "${PYPDDL_STUB_ROOT}")
    file(GLOB_RECURSE PYPDDL_STUB_FILES "${PYPDDL_STUB_ROOT}/*.pyi")

    foreach(PYPDDL_STUB_FILE IN LISTS PYPDDL_STUB_FILES)
        pypddl_patch_stub_file("${PYPDDL_STUB_FILE}")
    endforeach()
endif()
