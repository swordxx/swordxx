#!/bin/sh

./configure --prefix=/usr \
	--enable-debug --disable-shared \
	--without-conf --sysconfdir=/etc
