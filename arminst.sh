#!/bin/sh

OPTIONS="--host=arm-linux --prefix=/usr"
OPTIONS="--disable-shared $OPTIONS"
OPTIONS="--without-conf $OPTIONS"
OPTIONS="--sysconfdir=/etc $OPTIONS"
#OPTIONS="--with-vcl $OPTIONS"
#OPTIONS="--enable-debug $OPTIONS"
#OPTIONS="--enable-profile $OPTIONS"
OPTIONS="--without-lucene $OPTIONS"
OPTIONS="--without-curl $OPTIONS"

export PATH=$PATH:/usr/local/arm/2.95.3/bin/
#export CXXFLAGS="-I/usr/include/g++-3 -O3"
export CXXFLAGS="-O3"
export CFLAGS="-O3"

CWD=`pwd`
SRCBASE=`dirname $CWD`
export QPEDIR=$SRCBASE/buildroot-oz/build/opie
export QTDIR=$SRCBASE/buildroot-oz/build/qt-2.3.7
export PATH=$QTDIR/bin:/usr/local/arm/bin:$PATH
export TMAKEPATH=/opt/Qtopia/tmake/lib/qws/linux-ipaq-g++
export CPPFLAGS="-I/usr/include/g++-3/ -I$SRCBASE/buildroot-oz/output/staging/include"
export CFLAGS=$CPPFLAGS
export CXXFLAGS=$CPPFLAGS
export LDFLAGS=-L$SRCBASE/buildroot-oz/output/staging/target/lib

CPPFLAGS="$CFLAGS $CPPFLAGS -DUSBINARY" ./configure $OPTIONS $*


echo ""
echo ""
echo ""
echo "Configured to NOT write a global /etc/sword.conf on 'make install'."
echo "If this is the first time you've installed sword, be sure to run"
echo "'make install_config' if you would like a basic configuration installed"
echo ""
echo "Next you might try something like: "
echo ""
echo "make"
echo "su"
echo "make install"
echo "(and optionally)"
echo "make install_config"
echo "make register"
echo ""
echo ""
echo ""
