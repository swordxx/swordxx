#!/bin/sh
ACLOCAL="$AUTODIR""aclocal"
echo "*** Recreating aclocal.m4"
echo "$ACLOCAL"
	$ACLOCAL;

echo "*** Recreating configure"
AUTOCONF="$AUTODIR""autoconf"
AUTOHEAD="$AUTODIR""autoheader"
	$AUTOHEAD ;
	$AUTOCONF;
	
echo "*** Recreating the Makefile.in files"
AUTOMAKE="$AUTODIR""automake"
	$AUTOMAKE -ac --foreign;
