#!/bin/sh

OPTIONS="--prefix=/usr $OPTIONS"
OPTIONS="--enable-corba $OPTIONS"

CPPFLAGS="$CFLAGS $CPPFLAGS" JAVAC=javac ./configure $OPTIONS $*

