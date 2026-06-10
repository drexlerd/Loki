if(NOT PYPDDL_NATIVE_LIBDIR)
    set(PYPDDL_NATIVE_LIBDIR "pypddl/native/lib")
endif()
set(native_lib_dir "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/${PYPDDL_NATIVE_LIBDIR}")
if(NOT EXISTS "${native_lib_dir}")
    return()
endif()

find_program(PATCHELF_EXECUTABLE patchelf)
if(NOT PATCHELF_EXECUTABLE)
    message(WARNING "patchelf not found; pypddl native libraries keep their original runtime paths")
    return()
endif()

file(GLOB_RECURSE native_libraries LIST_DIRECTORIES false
    "${native_lib_dir}/*.so"
    "${native_lib_dir}/*.so.*")

foreach(native_library IN LISTS native_libraries)
    execute_process(
        COMMAND "${PATCHELF_EXECUTABLE}" --set-rpath "${PYPDDL_PYTHON_NATIVE_LIBRARY_RPATH}" "${native_library}"
        RESULT_VARIABLE rpath_result
        ERROR_VARIABLE rpath_error)
    if(NOT rpath_result EQUAL 0)
        message(WARNING "Could not set $ORIGIN rpath on ${native_library}: ${rpath_error}")
    endif()
endforeach()
