#!/bin/sh -e
# script by Manfred Bergmann in 2006, updated in 2010
#

APP=sword
VERS=1.6.2
BDIR=`pwd`

SWORDPATH="$BDIR/../../../"
ICUPATH="$BDIR/../dependencies/icu"
PPCCLUCENEPATH="$BDIR/../build_clucene/build/ppc_inst"
INTELCLUCENEPATH="$BDIR/../build_clucene/build/intel_inst"
INTEL64CLUCENEPATH="$BDIR/../build_clucene/build/intel64_inst"

usage() {
cat << EOF
usage: $0 options

OPTIONS:
   -d      Buildtype debug version
   -a      Architecture [intel|intel64|ppc|fat]
   -b      Buildpath, default = "build"
   -c      With clucene
   -s      Path to SDK i.e. "/Developer/SDKs/MacOSX10.5.sdk"
   -t      Deplyoment target i.e. "10.5"
EOF
}

DEBUG=0
FAT=0
PPC=0
INTEL=0
INTEL64=0
USECLUCENE=0
BUILDDIR=build
SDKDIR=/Developer/SDKs/MacOSX10.5.sdk
DTARGET=10.5

while getopts “da:b:s:t:c?” OPTION
do
     case $OPTION in
         d)
         DEBUG=1
         echo "building debug version"
         ;;
         a)
         if [ "$OPTARG" = "fat" ]; then
           FAT=1
           PPC=1
           INTEL=1
           INTEL64=1
           echo "building fat version"
         fi
         if [ "$OPTARG" = "ppc" ]; then
           PPC=1
           echo "building ppc version"
         fi
         if [ "$OPTARG" = "intel" ]; then
           INTEL=1
           echo "building intel version"
         fi
         if [ "$OPTARG" = "intel64" ]; then
           INTEL64=1
           echo "building intel64 version"
         fi
         ;;
         b)
         BUILDDIR="$OPTARG"
         echo "using builddir $BUILDDIR"
         ;;
         s)
         SDKDIR="$OPTARG"
         echo "using SDK at $SDKDIR"
         ;;
         t)
         DTARGET="$OPTARG"
         echo "using deployment target $DTARGET"
         ;;
         c)
         USECLUCENE=1
         echo "building with clucene"
         ;;
         ?)
         usage
         exit
         ;;
     esac
done

# using seperate build dirs and building in them doesn't work with sword
BUILD=$BDIR/"$BUILDDIR"
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

# add icu tools to path
export PATH="$PATH:$ICUPATH/bin"
export DYLD_LIBRARY_PATH="$DYLD_LIBRARY_PATH:$ICUPATH/lib"

PPC_LIB_EXPORT=
INTEL_LIB_EXPORT=
INTEL64_LIB_EXPORT=

# build stuff
if [ $PPC -eq 1 ] || [ $FAT -eq 1 ]; then
    echo "building PPC version of library..."    
	cd $SWORDPATH
	echo 'autogen.sh ...'
	./autogen.sh
	echo 'autogen.sh ... done'
	export CC=gcc
	export CXX=g++
	export SDK=$SDKDIR
	if [ $DEBUG -eq 1 ]; then
		export CFLAGS="-O0 -g -arch ppc -mmacosx-version-min=$DTARGET -isysroot $SDK -I$SDK/usr/include -I$ICUPATH/include"
	else
	  export CFLAGS="-O2 -g0 -arch ppc -mmacosx-version-min=$DTARGET -isysroot $SDK -I$SDK/usr/include -I$ICUPATH/include"
	fi
	export CXXFLAGS="$CFLAGS"
	export LDFLAGS="-isysroot $SDK -Wl,-syslibroot,$SDK"
	if [ $USECLUCENE -eq 1 ]; then
  	./configure --prefix=$PPCPREFIX --with-clucene=$PPCCLUCENEPATH --with-zlib --with-conf --with-icu --with-curl --disable-tests --disable-shared
	else
  	./configure --prefix=$PPCPREFIX --without-clucene --with-zlib --with-conf --with-icu --with-curl --disable-tests --disable-shared --enable-utilities
  fi
	make all install
	make clean
	cd $BDIR
	# copy to result dir
	PPC_LIB_EXPORT="$RESULTPREFIX/lib/lib$APP-ppc.a"
	cp $PPCPREFIX/lib/lib$APP.a $PPC_LIB_EXPORT
  echo "building PPC version of library...done"
fi

if [ $INTEL -eq 1 ] || [ $FAT -eq 1 ]; then
  echo "building INTEL version of library..."
	cd $SWORDPATH
	echo 'autogen.sh ...'
	./autogen.sh
	echo 'autogen.sh ... done'
	export CC=gcc
	export CXX=g++
	export SDK=$SDKDIR
	if [ $DEBUG -eq 1 ]; then
		export CFLAGS="-O0 -g -arch i686 -mmacosx-version-min=$DTARGET -isysroot $SDK -I$SDK/usr/include -I$ICUPATH/include"
	else
	  export CFLAGS="-O2 -g0 -arch i686 -mmacosx-version-min=$DTARGET -isysroot $SDK -I$SDK/usr/include -I$ICUPATH/include"
	fi
	export CXXFLAGS="$CFLAGS"
	export LDFLAGS="-isysroot $SDK -Wl,-syslibroot,$SDK"
	if [ $USECLUCENE -eq 1 ]; then
  	./configure --prefix=$INTELPREFIX --with-clucene=$INTELCLUCENEPATH --with-zlib --with-conf --with-icu --with-curl --enable-tests --disable-shared
  else
  	./configure --prefix=$INTELPREFIX --without-clucene --with-zlib --with-conf --with-icu --with-curl --enable-tests --disable-shared --enable-utilities
  fi
	make all install
	make clean
	cd $BDIR
	# copy to result dir
	INTEL_LIB_EXPORT="$RESULTPREFIX/lib/lib$APP-intel.a"
	cp $INTELPREFIX/lib/lib$APP.a $INTEL_LIB_EXPORT
  echo "building INTEL version of library...done"
fi

if [ $INTEL64 -eq 1 ] || [ $FAT -eq 1 ]; then
  echo "building INTEL64 version of library..."
	cd $SWORDPATH
	echo 'autogen.sh ...'
	./autogen.sh
	echo 'autogen.sh ... done'
	export CC=gcc
	export CXX=g++
	export SDK=$SDKDIR
	if [ $DEBUG -eq 1 ]; then
		export CFLAGS="-O0 -g -arch x86_64 -mmacosx-version-min=$DTARGET -isysroot $SDK -I$SDK/usr/include -I$ICUPATH/include"
	else
	  export CFLAGS="-O2 -g0 -arch x86_64 -mmacosx-version-min=$DTARGET -isysroot $SDK -I$SDK/usr/include -I$ICUPATH/include"
	fi
	export CXXFLAGS="$CFLAGS"
	export LDFLAGS="-isysroot $SDK -Wl,-syslibroot,$SDK"
	if [ $USECLUCENE -eq 1 ]; then
  	./configure --prefix=$INTEL64PREFIX --with-clucene=$INTEL64CLUCENEPATH --with-zlib --with-conf --with-icu --with-curl --enable-tests --disable-shared
  else
  	./configure --prefix=$INTEL64PREFIX --without-clucene --with-zlib --with-conf --with-icu --with-curl --enable-tests --disable-shared --enable-utilities
  fi
	make all install
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


# run runlib to update the library content
#ranlib $RESULTPREFIX/lib/*
