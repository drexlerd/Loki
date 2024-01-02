#!/bin/bash

wget --no-check-certificate 'https://archives.boost.io/release/1.84.0/source/boost_1_84_0.tar.gz'
tar xf boost_1_84_0.tar.gz
cd boost_1_84_0
./bootstrap.sh

# Compile with fPIC flag
export CXXFLAGS="-fPIC"
./b2 cxxflags="$CXXFLAGS" link=static
