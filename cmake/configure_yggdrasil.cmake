function(configure_yggdrasil)
  if(LOKI_DISABLE_YGGDRASIL_DISCOVERY)
    return()
  endif()

  find_package(Python3 QUIET COMPONENTS Interpreter)
  if(NOT Python3_Interpreter_FOUND)
    return()
  endif()

  execute_process(
    COMMAND "${Python3_EXECUTABLE}" -c "import pyyggdrasil; print(pyyggdrasil.native_prefix())"
    RESULT_VARIABLE yggdrasil_result
    OUTPUT_VARIABLE yggdrasil_prefix
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  if(yggdrasil_result EQUAL 0 AND EXISTS "${yggdrasil_prefix}")
    file(GLOB yggdrasil_cmake_dirs LIST_DIRECTORIES true "${yggdrasil_prefix}/lib*/cmake")
    foreach(yggdrasil_cmake_dir IN LISTS yggdrasil_cmake_dirs)
      get_filename_component(yggdrasil_lib_dir "${yggdrasil_cmake_dir}" DIRECTORY)
      list(PREPEND CMAKE_PREFIX_PATH "${yggdrasil_cmake_dir}" "${yggdrasil_lib_dir}")
    endforeach()
    list(PREPEND CMAKE_PREFIX_PATH "${yggdrasil_prefix}")
    set(YGGDRASIL_NATIVE_PREFIX "${yggdrasil_prefix}" PARENT_SCOPE)
    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" PARENT_SCOPE)
    message(STATUS "Found yggdrasil native prefix: ${yggdrasil_prefix}")
  endif()
endfunction()
