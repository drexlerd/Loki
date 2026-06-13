# Locates the native prefix bundled in the installed pyyggdrasil wheel and
# prepends it to CMAKE_PREFIX_PATH, so find_package(yggdrasil) (which provides
# the yggdrasil_* python-native helper functions) can resolve.
#
# A preset YGGDRASIL_NATIVE_PREFIX wins (manual override, or passed by the
# wheel build backends via -DYGGDRASIL_NATIVE_PREFIX); the python query is the
# fallback for manual cmake configures.
if(NOT YGGDRASIL_NATIVE_PREFIX)
    if(NOT Python3_EXECUTABLE AND Python_EXECUTABLE)
        set(Python3_EXECUTABLE "${Python_EXECUTABLE}")
    endif()
    find_package(Python3 QUIET COMPONENTS Interpreter)
    if(Python3_Interpreter_FOUND)
        execute_process(
            COMMAND "${Python3_EXECUTABLE}" -c
                    "import pyyggdrasil as m; print(getattr(m, 'cmake_prefix', m.native_prefix)())"
            RESULT_VARIABLE _pyyggdrasil_result
            OUTPUT_VARIABLE _pyyggdrasil_prefix
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        if(_pyyggdrasil_result EQUAL 0 AND EXISTS "${_pyyggdrasil_prefix}")
            set(YGGDRASIL_NATIVE_PREFIX "${_pyyggdrasil_prefix}")
            message(STATUS "Found pyyggdrasil native prefix: ${YGGDRASIL_NATIVE_PREFIX}")
        endif()
    endif()
endif()
if(YGGDRASIL_NATIVE_PREFIX AND NOT "${YGGDRASIL_NATIVE_PREFIX}" IN_LIST CMAKE_PREFIX_PATH)
    list(PREPEND CMAKE_PREFIX_PATH "${YGGDRASIL_NATIVE_PREFIX}")
endif()
