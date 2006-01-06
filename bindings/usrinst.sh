#!/bin/sh

OPTIONS="--prefix=/usr $OPTIONS"
OPTIONS="--enable-corba $OPTIONS"

CPPFLAGS="$CFLAGS $CPPFLAGS" ./configure $OPTIONS $*

