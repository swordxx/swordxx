#!/bin/sh
echo "*** copying swig files"
cp -a ../*.i .

ACLOCAL="$AUTODIR""aclocal"
echo "*** Recreating aclocal.m4"
echo "$ACLOCAL"
	$ACLOCAL -I .;

echo "*** Recreating configure"
AUTOCONF="$AUTODIR""autoconf"
	$AUTOCONF;
	
echo "*** Recreating the Makefile.in files"
AUTOMAKE="$AUTODIR""automake"
	$AUTOMAKE -ac --foreign;
