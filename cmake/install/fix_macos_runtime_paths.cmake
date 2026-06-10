if(NOT PYPDDL_NATIVE_LIBDIR)
    set(PYPDDL_NATIVE_LIBDIR "pypddl/native/lib")
endif()
set(native_lib_dir "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/${PYPDDL_NATIVE_LIBDIR}")
if(NOT EXISTS "${native_lib_dir}")
    return()
endif()

find_program(INSTALL_NAME_TOOL_EXECUTABLE install_name_tool)
if(NOT INSTALL_NAME_TOOL_EXECUTABLE)
    message(WARNING "install_name_tool not found; pypddl native libraries keep their original runtime paths")
    return()
endif()

file(GLOB_RECURSE native_libraries LIST_DIRECTORIES false
    "${native_lib_dir}/*.dylib"
    "${native_lib_dir}/*.dylib.*")

foreach(native_library IN LISTS native_libraries)
    get_filename_component(native_library_name "${native_library}" NAME)

    execute_process(
        COMMAND "${INSTALL_NAME_TOOL_EXECUTABLE}" -id "@rpath/${native_library_name}" "${native_library}"
        RESULT_VARIABLE install_name_result
        ERROR_VARIABLE install_name_error)
    if(NOT install_name_result EQUAL 0)
        message(WARNING "Could not update install name of ${native_library}: ${install_name_error}")
    endif()

    foreach(native_library_rpath IN LISTS PYPDDL_PYTHON_NATIVE_LIBRARY_RPATHS)
        execute_process(
            COMMAND "${INSTALL_NAME_TOOL_EXECUTABLE}" -delete_rpath "${native_library_rpath}" "${native_library}"
            OUTPUT_QUIET
            ERROR_QUIET)
        execute_process(
            COMMAND "${INSTALL_NAME_TOOL_EXECUTABLE}" -add_rpath "${native_library_rpath}" "${native_library}"
            RESULT_VARIABLE rpath_result
            ERROR_VARIABLE rpath_error)
        if(NOT rpath_result EQUAL 0)
            message(WARNING "Could not add ${native_library_rpath} rpath to ${native_library}: ${rpath_error}")
        endif()
    endforeach()
endforeach()
