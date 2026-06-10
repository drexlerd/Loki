add_subdirectory(unit)

if(NOT LOKI_BUILD_PYPDDL)
    add_test(
        NAME package_consumer
        COMMAND
            ${CMAKE_COMMAND}
                "-DLOKI_BINARY_DIR=${CMAKE_BINARY_DIR}"
                "-DLOKI_YGGDRASIL_PREFIX=${YGGDRASIL_NATIVE_PREFIX}"
                "-DLOKI_INSTALL_LIBDIR=${CMAKE_INSTALL_LIBDIR}"
                "-DLOKI_GENERATOR=${CMAKE_GENERATOR}"
                "-DLOKI_CONFIG=$<CONFIG>"
                "-P" "${CMAKE_CURRENT_SOURCE_DIR}/package_consumer.cmake"
    )
endif()
