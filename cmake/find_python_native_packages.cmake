# Discovers the native prefix bundled in a pip-installed provider package
# (pyyggdrasil, pypddl, pytyr, ...) and makes it visible to find_package.
#
# loki_find_python_native_prefix(<python_package> <out_prefix_var>)
#   - If <out_prefix_var> is already set (manual override), only ensures it is
#     on CMAKE_PREFIX_PATH.
#   - Otherwise queries the package for its cmake_prefix() (native_prefix() on
#     older releases) and prepends it to CMAKE_PREFIX_PATH.
function(loki_find_python_native_prefix python_package out_prefix_var)
  if(${out_prefix_var})
    if(NOT "${${out_prefix_var}}" IN_LIST CMAKE_PREFIX_PATH)
      list(PREPEND CMAKE_PREFIX_PATH "${${out_prefix_var}}")
      set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" PARENT_SCOPE)
    endif()
    return()
  endif()

  if(NOT Python3_EXECUTABLE AND Python_EXECUTABLE)
    set(Python3_EXECUTABLE "${Python_EXECUTABLE}")
  endif()

  find_package(Python3 QUIET COMPONENTS Interpreter)
  if(NOT Python3_Interpreter_FOUND)
    return()
  endif()

  execute_process(
    COMMAND "${Python3_EXECUTABLE}" -c
            "import ${python_package} as m; print(getattr(m, 'cmake_prefix', m.native_prefix)())"
    RESULT_VARIABLE native_result
    OUTPUT_VARIABLE native_prefix
    ERROR_QUIET
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  if(native_result EQUAL 0 AND EXISTS "${native_prefix}")
    list(PREPEND CMAKE_PREFIX_PATH "${native_prefix}")
    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" PARENT_SCOPE)
    set(${out_prefix_var} "${native_prefix}" PARENT_SCOPE)
    message(STATUS "Found ${python_package} native prefix: ${native_prefix}")
  endif()
endfunction()
