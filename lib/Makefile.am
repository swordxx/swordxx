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
ICULIBADD = $(ICU_LIBS) $(ICU_IOLIBS)
ICUDEF = -DSWICU_DATA=\"${pkglibdir}/${VERSION}_icu_${ICU_VER}\" 
else
ICULIBADD =
ICUDEF =
endif

INCLUDES += $(ICUDEF)

if INSTALLMGR
IMGRLIBADD = -lcurl $(CURL_LIBS)
else
IMGRLIBADD =
endif

if USELUCENE
endif
LUCLIBADD = -lclucene $(LUCENE_LIBS)

libsword_la_LIBADD = $(ICULIBADD) $(IMGRLIBADD) $(LUCLIBADD)

libsword_la_LDFLAGS = -version-info 3

include ../src/keys/Makefile.am
include ../src/utilfuns/Makefile.am
include ../src/mgr/Makefile.am
include ../src/frontend/Makefile.am
include ../src/modules/Makefile.am
include ../bindings/Makefile.am
include ../include/Makefile.am


