#!/bin/sh
ACLOCAL="$AUTODIR""aclocal"
echo "*** Recreating aclocal.m4"
echo "$ACLOCAL"
if $ACLOCAL --version | egrep "1\.6" >/dev/null 2>&1; then \
        echo "aclocal 1.6" ;
	$ACLOCAL;\
elif $ACLOCAL --version | egrep "1\.5" >/dev/null 2>&1; then \
        echo "aclocal 1.5" ;\
	echo "You must have aclocal 1.6 to use aclocal with this \
source";\
elif $ACLOCAL --version | egrep "1\.4" >/dev/null 2>&1; then \
        echo "aclocal 1.4" ;\
	echo "You must have aclocal 1.6 to use aclocal with this \
source";\
else \
        echo "aclocal unknown" ;\
	echo "You must have aclocal 1.6 to use aclocal with this \
source";\
fi
echo "*** Recreating configure"
AUTOCONF="$AUTODIR""autoconf"
AUTOHEAD="$AUTODIR""autoheader"
if $AUTOCONF --version | egrep "2\.53" >/dev/null 2>&1; then \
        echo "autoconf 2.53" ;
	if egrep "^AM_CONFIG_HEADER" configure.ac >/dev/null 2>&1; then \
        	echo "*** Creating config.h template" ;\
	        $AUTOHEAD ;\
	fi
	$AUTOCONF;\
elif $AUTOCONF --version | egrep "2\.52" >/dev/null 2>&1; then \
        echo "autoconf 2.52" ;
	if egrep "^AM_CONFIG_HEADER" configure.ac >/dev/null 2>&1; then \
        	echo "*** Creating config.h template" ;\
	        $AUTOHEAD ;\
	fi
	$AUTOCONF;\
elif $AUTOCONF --version | egrep "2\.13$" >/dev/null 2>&1; then \
        echo "autoconf 2.13" ;\
	echo "You must have autoconf 2.52+ to use autoconf with this \
source";\
else \
        echo "autoconf unknown" ;\
	echo "You must have autoconf 2.52+ to use autoconf with this \
source";\
fi
echo "*** Recreating the Makefile.in files"
AUTOMAKE="$AUTODIR""automake"
if $AUTOMAKE --version | egrep "1\.6$" >/dev/null 2>&1; then \
        echo "automake 1.6" ;
	$AUTOMAKE;\
elif $AUTOMAKE --version | egrep "1\.5" >/dev/null 2>&1; then \
        echo "automake 1.5" ;\
	echo "You must have automake 1.6 to use automake with this \
source";\
elif $AUTOMAKE --version | egrep "1\.4" >/dev/null 2>&1; then \
        echo "automake 1.4" ;\
	echo "You must have automake 1.6 to use automake with this \
source";\
else \
        echo "automake unknown" ;\
	echo "You must have automake 1.6 to use automake with this \
source";\
fi
