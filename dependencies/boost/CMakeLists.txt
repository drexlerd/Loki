cmake_minimum_required(VERSION 3.21)
project(InstallBoost)

include(ExternalProject)

set(LOKI_BOOST_VERSION 1.84.0 CACHE STRING "Boost version used to build boost.")
string(REPLACE "." "_" LOKI_BOOST_ARCHIVE_VERSION ${LOKI_BOOST_VERSION})

message(STATUS "Preparing external project \"boost\":")

if(LOKI_BUILD_SHARED_DEPENDENCIES)
    set(BOOST_LINK shared)
    set(BOOST_RUNTIME_LINK shared)
else()
    set(BOOST_LINK static)
    set(BOOST_RUNTIME_LINK static)
endif()

# We need to build boost for cmake support
ExternalProject_Add(
    boost
    URL https://archives.boost.io/release/${LOKI_BOOST_VERSION}/source/boost_${LOKI_BOOST_ARCHIVE_VERSION}.tar.gz
    PREFIX ${CMAKE_BINARY_DIR}/boost
    STAMP_DIR ${CMAKE_BINARY_DIR}/boost/stamp/${LOKI_BOOST_VERSION}
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -E chdir <SOURCE_DIR> ./bootstrap.sh --prefix=<INSTALL_DIR>
    BUILD_COMMAND ${CMAKE_COMMAND} -E chdir <SOURCE_DIR> ./b2 headers
    INSTALL_COMMAND ${CMAKE_COMMAND} -E chdir <SOURCE_DIR> ./b2 install --without-python link=${BOOST_LINK} runtime-link=${BOOST_RUNTIME_LINK}
    INSTALL_DIR ${CMAKE_INSTALL_PREFIX}
)
