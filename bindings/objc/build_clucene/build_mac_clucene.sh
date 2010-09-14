#!/bin/sh -e
# script by Manfred Bergmann in 2006, updated in 2010
#

APP=clucene
COMP=core
VERS=0.9.21b
BDIR=`pwd`

SRCPACKAGE="$APP-$COMP-$VERS.tar.gz"

DEBUG=0
FAT=0
PPC=0
INTEL=0

# check commandline
for arg in "$@" 
do
	if [ "$arg" = "debug" ]; then
		DEBUG=1
		echo "building debug version"
	fi
	if [ "$arg" = "fat" ]; then
		FAT=1
		PPC=1
		INTEL=1
		echo "building fat version"
	fi
	if [ "$arg" = "ppc" ]; then
		PPC=1
		echo "building ppc version"
	else
		PPC=0
	fi
	if [ "$arg" = "intel" ]; then
		INTEL=1
		echo "building intel version"
	else
		INTEL=0
	fi
done

# using seperate build dirs and building in them doesn't work with sword
BUILD=$BDIR/build
PPCPREFIX=$BUILD/ppc_inst
INTELPREFIX=$BUILD/intel_inst
INTEL64PREFIX=$BUILD/intel64_inst
RESULTPREFIX=$BUILD/result_inst

# Create install dirs if they doesn't exist
if [ ! -d $BUILD ]; then
	mkdir -p $BUILD
fi
if [ ! -d $PPCPREFIX ]; then
	mkdir -p $PPCPREFIX
fi
if [ ! -d $INTELPREFIX ]; then
	mkdir -p $INTELPREFIX
fi
if [ ! -d $INTEL64PREFIX ]; then
	mkdir -p $INTEL64PREFIX
fi
if [ ! -d $RESULTPREFIX ]; then
	mkdir -p $RESULTPREFIX
	if [ ! -d $RESULTPREFIX/lib ]; then
		mkdir -p $RESULTPREFIX/lib
	fi
fi

# delete old source dir
/bin/rm -rf $SRCPACKAGE

# download first is not exist
# due to size limitation in the source repo this file wil be downloaded
# and buiding will need an internet connection
if [ ! -f $SRCPACKAGE ]; then
  curl -o $SRCPACKAGE http://www.crosswire.org/~mbergmann/$SRCPACKAGE
fi
# ungzip src
tar -xzf $SRCPACKAGE

CLUCENEPATH="$BDIR/$APP-$COMP-$VERS"

# build stuff
if [ $PPC -eq 1 ] || [ $FAT -eq 1 ]; then
    echo "building PPC version of library..."    
	cd $CLUCENEPATH
	export CC=gcc
	export CXX=g++
	export SDK=/Developer/SDKs/MacOSX10.5.sdk
	if [ $DEBUG -eq 1 ]; then
		export CFLAGS="-O0 -g -arch ppc -mmacosx-version-min=10.5 -isysroot $SDK -I$SDK/usr/include"
	else
	    export CFLAGS="-O2 -g0 -arch ppc -mmacosx-version-min=10.5 -isysroot $SDK -I$SDK/usr/include"
	fi
	export CXXFLAGS="$CFLAGS"
	export LDFLAGS="-isysroot $SDK -Wl,-syslibroot,$SDK"
	./configure --prefix=$PPCPREFIX
	make install
	make clean
	cd $BDIR
	# copy to result dir
	PPC_LIB_EXPORT="$RESULTPREFIX/lib/lib$APP-ppc.a"
	cp $PPCPREFIX/lib/lib$APP.a $PPC_LIB_EXPORT
    echo "building PPC version of library...done"
fi

if [ $INTEL -eq 1 ] || [ $FAT -eq 1 ]; then
    echo "building INTEL version of library..."
	cd $CLUCENEPATH
	export CC=gcc
	export CXX=g++
	export SDK=/Developer/SDKs/MacOSX10.5.sdk
	if [ $DEBUG -eq 1 ]; then
		export CFLAGS="-O0 -g -arch i686 -mmacosx-version-min=10.5 -isysroot $SDK -I$SDK/usr/include -I$ICUPATH/include"
	else
	    export CFLAGS="-O2 -g0 -arch i686 -mmacosx-version-min=10.5 -isysroot $SDK -I$SDK/usr/include -I$ICUPATH/include"
	fi
	export CXXFLAGS="$CFLAGS"
	export LDFLAGS="-isysroot $SDK -Wl,-syslibroot,$SDK"
	./configure --prefix=$INTELPREFIX
	make install
	make clean
	cd $BDIR
	# copy to result dir
	INTEL_LIB_EXPORT="$RESULTPREFIX/lib/lib$APP-intel.a"
	cp $INTELPREFIX/lib/lib$APP.a $INTEL_LIB_EXPORT
    echo "building INTEL version of library...done"

    echo "building INTEL64 version of library..."
	cd $CLUCENEPATH
	export CC=gcc
	export CXX=g++
	export SDK=/Developer/SDKs/MacOSX10.5.sdk
	if [ $DEBUG -eq 1 ]; then
		export CFLAGS="-O0 -g -arch x86_64 -mmacosx-version-min=10.5 -isysroot $SDK -I$SDK/usr/include"
	else
	    export CFLAGS="-O2 -g0 -arch x86_64 -mmacosx-version-min=10.5 -isysroot $SDK -I$SDK/usr/include"
	fi
	export CXXFLAGS="$CFLAGS"
	export LDFLAGS="-isysroot $SDK -Wl,-syslibroot,$SDK"
	./configure --prefix=$INTEL64PREFIX
	make install
	make clean
	cd $BDIR
	# copy to result dir
	INTEL64_LIB_EXPORT="$RESULTPREFIX/lib/lib$APP-intel64.a"
	cp $INTEL64PREFIX/lib/lib$APP.a $INTEL64_LIB_EXPORT
    echo "building INTEL64 version of library...done"
fi

# only for fat version
if [ $FAT -eq 1 ]; then
	# creating result
	# build fat binary with lipo
	lipo -create $PPC_LIB_EXPORT $INTEL_LIB_EXPORT $INTEL64_LIB_EXPORT -output $RESULTPREFIX/lib/lib$APP-ub.a
fi
