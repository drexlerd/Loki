cmake_minimum_required(VERSION 3.21)

project(gtlPackage VERSION 1.2.0 LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

add_library(gtl INTERFACE)
add_library(gtl::gtl ALIAS gtl)

target_include_directories(gtl INTERFACE
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

install(
    TARGETS gtl
    EXPORT gtlTargets)

install(
    EXPORT gtlTargets
    NAMESPACE gtl::
    FILE gtlTargets.cmake
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gtl")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/gtlConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/gtlConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gtl")

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/gtlConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/gtlConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/gtlConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/gtl")
