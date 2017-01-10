#!/bin/bash
#
# This is a minimal script to build the library within the TravisCI
# tests. It is not meant to be demonstrative, used by end users, or
# any other set of such consumption. Although you can choose to take
# it as informative if you wish
mkdir -p build
cd build
/opt/cmake-${CMAKE_VERSION}-Linux-x86_64/bin/cmake -DCMAKE_INSTALL_PREFIX=install/ .. || exit 1
make || exit 1
make tests || exit 1
make install || exit 1
