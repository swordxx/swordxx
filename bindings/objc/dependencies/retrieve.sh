#!/bin/sh

# this script downloads icu dependencies and unpacks them
# ICU is needed for building SWORD

curl -o icu_build-ub.tar.gz http://www.crosswire.org/~mbergmann/icu_build-ub.tar.gz
tar -xzf icu_build-ub.tar.gz

curl -o clucene_src.tar.gz http://www.crosswire.org/~mbergmann/clucene_src.tar.gz
tar -xzf clucene_src.tar.gz

cd icu/lib
ln -s libicudata.dylib libicudata.dylib.36
ln -s libicui18n.dylib libicui18n.dylib.36
ln -s libicuio.dylib libicuio.dylib.36
ln -s libicule.dylib libicule.dylib.36
ln -s libiculx.dylib libiculx.dylib.36
ln -s libicutu.dylib libicutu.dylib.36
ln -s libicuuc.dylib libicuuc.dylib.36
