AUTOMAKE_OPTIONS = 1.6
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(includedir) 
AM_CPPFLAGS += -DUSE_AUTOTOOLS -DUNIX -Dunix -D__unix__

if ZLIB
ZLIBDEF = 
else
ZLIBDEF = -DEXCLUDEZLIB
endif

AM_CPPFLAGS += $(ZLIBDEF)

# Global config directory
globalconfdir := @sysconfdir@

lib_LTLIBRARIES = libsword.la

libsword_la_SOURCES =
if ICU
ICULIBADD = $(ICU_LIBS) $(ICU_IOLIBS)
ICUDEF = -DSWICU_DATA=\"${pkglibdir}/${VERSION}_icu_${ICU_VER}\" 
else
ICULIBADD =
ICUDEF =
endif

AM_CPPFLAGS += $(ICUDEF)

#if WITHCURL
if INSTALLMGR
IMGRLIBADD = -lcurl $(CURL_LIBS)
else
IMGRLIBADD =
endif


libsword_la_LIBADD = $(ICULIBADD) $(IMGRLIBADD) $(CLUCENE_LIBS)

libsword_la_LDFLAGS = -release $(VERSION)

include ../src/keys/Makefile.am
include ../src/utilfuns/Makefile.am
include ../src/mgr/Makefile.am
include ../src/frontend/Makefile.am
include ../src/modules/Makefile.am
#include ../bindings/Makefile.am
bindingsdir = $(top_srcdir)/bindings
libsword_la_SOURCES += $(bindingsdir)/flatapi.cpp
include ../include/Makefile.am


