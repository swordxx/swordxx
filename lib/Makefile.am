AUTOMAKE_OPTIONS = 1.6
INCLUDES = -I$(top_srcdir)/include
INCLUDES += -DUSE_AUTOTOOLS -DUNIX

if ZLIB
ZLIBDEF = 
else
ZLIBDEF = -DEXCLUDEZLIB
endif

INCLUDES += $(ZLIBDEF)

# Global config directory
globalconfdir := @sysconfdir@

lib_LTLIBRARIES = libsword.la

libsword_la_SOURCES =
if ICU
ICULIBADD = -licui18n -licuuc -lustdio
else
ICULIBADD =
endif

if INSTALLMGR
IMGRLIBADD = $(CURL_LIBS)
else
IMGRLIBADD =
endif

libsword_la_LIBADD = $(ICULIBADD) $(IMGRLIBADD)

libsword_la_LDFLAGS = -version-info 1

include ../src/keys/Makefile.am
include ../src/utilfuns/Makefile.am
include ../src/mgr/Makefile.am
include ../src/frontend/Makefile.am
include ../src/modules/Makefile.am
include ../bindings/Makefile.am
include ../include/Makefile.am


