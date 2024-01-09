cmake_minimum_required(VERSION 3.13)
project(DownloadBoost)

include(ExternalProject)

message(STATUS "Preparing external project \"boost\":")

# ExternalProject_Add(
#     boost
#     # URL https://boostorg.jfrog.io/artifactory/main/release/1.84.0/source/boost_1_84_0.tar.gz  ## the official link is broken
#     URL https://archives.boost.io/release/1.84.0/source/boost_1_84_0.tar.gz
#     CONFIGURE_COMMAND ""
#     BUILD_COMMAND ""
#     INSTALL_COMMAND ""
#     UPDATE_DISCONNECTED true
# )

ExternalProject_Add(BoostHeaders
    URL https://archives.boost.io/release/1.84.0/source/boost_1_84_0.tar.gz
    PREFIX ${CMAKE_BINARY_DIR}/external/boost
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/boost ${CMAKE_INSTALL_PREFIX}/include/boost
            COMMAND ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR>/tools/cmake ${CMAKE_INSTALL_PREFIX}/lib/cmake
    UPDATE_DISCONNECTED 1
)

# ExternalProject_Get_Property(boost source_dir)
#  set(BOOST_INCLUDE_DIRS ${source_dir})
set(BOOST_INCLUDE_DIRS  ${CMAKE_INSTALL_PREFIX})
