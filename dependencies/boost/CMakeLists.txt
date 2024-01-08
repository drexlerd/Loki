cmake_minimum_required(VERSION 3.0)
project(DownloadBoost)

include(ExternalProject)

message(STATUS "Preparing external project \"boost\":")

ExternalProject_Add(
    boost
    URL https://boostorg.jfrog.io/artifactory/main/release/1.84.0/source/boost_1_84_0.tar.gz
    # URL_HASH SHA256=a5800f405508f5df8114558ca9855d2640a2de8f0445f051fa1c7c3383045724
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_DISCONNECTED true
)

ExternalProject_Get_Property(Boost source_dir)
set(BOOST_INCLUDE_DIRS ${source_dir})
