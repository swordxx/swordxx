#!/bin/bash
#
# This is a minimal script to build the library within the TravisCI
# tests. It is not meant to be demonstrative, used by end users, or
# any other set of such consumption. Although you can choose to take
# it as informative if you wish

set -evuo pipefail

mkdir -p build
cp travis-ci/config.cmake build/
cd build
/opt/cmake-${CMAKE_VERSION}-Linux-x86_64/bin/cmake -DCMAKE_INSTALL_PREFIX=install/ -DSWORDXX_BUILD_EXAMPLES=ON -DSWORDXX_BUILD_TESTS=ON ..
make VERBOSE=1
make VERBOSE=1 tests
make VERBOSE=1 install
