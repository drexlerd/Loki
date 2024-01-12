macro(configure_dependencies)
  # We installed the dependencies into the subdirectories under the install prefix.
    # Hence must append them to the single cmake_prefix_path.
    message("Configure dependencies of Loki:")
    message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
    list(LENGTH CMAKE_PREFIX_PATH PREFIX_PATH_COUNT)
    if(PREFIX_PATH_COUNT GREATER 1)
        message(FATAL_ERROR "Only one prefix path is allowed. Found multiple paths in CMAKE_PREFIX_PATH. Please add dependencies to the CMake Superbuild.")
    endif()

    list(APPEND MODIFIED_CMAKE_PREFIX_PATH
        "${CMAKE_PREFIX_PATH}/benchmark"
        "${CMAKE_PREFIX_PATH}/boost"
        "${CMAKE_PREFIX_PATH}/googletest")
    message(${MODIFIED_CMAKE_PREFIX_PATH})
    set(CMAKE_PREFIX_PATH ${MODIFIED_CMAKE_PREFIX_PATH})
    message(STATUS "MODIFIED_CMAKE_PREFIX_PATH:")
    foreach(CMAKE_PREFIX_PATH_ARG ${CMAKE_PREFIX_PATH})
        message(STATUS "-- ${CMAKE_PREFIX_PATH_ARG}")
    endforeach()
    # set(CMAKE_FIND_DEBUG_MODE ON)
endmacro()