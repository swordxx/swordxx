#!/bin/sh
echo "*** Changing to your build dir and"
echo "*** Running configure with your settings"
TEMPDIR=`pwd`

cd $BUILDDIR && $TEMPDIR/configure --prefix=/usr \
	--enable-debug --disable-shared
