#!/bin/sh

# this script downloads icu dependencies and unpacks them
# ICU is needed for building SWORD

curl -o icu_build-ub.tar.gz http://www.crosswire.org/~mbergmann/icu_build-ub.tar.gz
tar -xzf icu_build-ub.tar.gz

