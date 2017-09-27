#!/bin/bash
#
# This is a minimal script to build the library within the TravisCI
# tests. It is not meant to be demonstrative, used by end users, or
# any other set of such consumption. Although you can choose to take
# it as informative if you wish
mkdir -p build
cd build
/opt/cmake-${CMAKE_VERSION}-Linux-x86_64/bin/cmake -DCMAKE_INSTALL_PREFIX=install/ .. || exit 1
make VERBOSE=1 || exit 1
make VERBOSE=1 tests || exit 1
make VERBOSE=1 install || exit 1
