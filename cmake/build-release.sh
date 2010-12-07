#!/bin/bash
#
# A sample script showing how to build most of the options available to this system.
# Invoke it from the top directory of SWORD by calling $ cmake/build-release.sh
#

mkdir -p build
cd build
# Configure with Python and Perl bindings, examples, tests and a debug build into
# a shared library, plus static option as well.  They will be installed to the
# /opt/sword directory
cmake -DSWORD_BINDINGS="Python Perl" \
	-DSWORD_BUILD_EXAMPLES="Yes" \
	-DSWORD_BUILD_TESTS="Yes" \
	-DLIBSWORD_LIBRARY_TYPE="Shared Static" \
	-DCMAKE_BUILD_TYPE="Release" \
	-DCMAKE_INSTALL_PREFIX="/opt/sword" ..
make -j5
cd bindings/swig/perl
make
cd ../python
python setup.py build

echo "Now the library has been built, along with the Perl and Python bindings. \
	If you would like to install, you may now do so by changing into the build \
	directory and typing \"sudo make install\".  Python bindings may be installed \
	by then changing into tye build/bindings/swig/python diretory and typing \"sudo python \
	setup.py install\" and Perl bindings may be installed by changing into the \
	build/bindings/swig/perl directory and issuing the command \"sudo make install\"."
