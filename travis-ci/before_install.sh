#!/bin/bash
set -evuo pipefail

[ -d /opt/cmake-${CMAKE_VERSION}-Linux-x86_64 ] || curl -sSL https://cmake.org/files/v${CMAKE_VERSION%.*}/cmake-${CMAKE_VERSION}-Linux-x86_64.tar.gz | sudo tar -xzC /opt
