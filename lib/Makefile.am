AUTOMAKE_OPTIONS = 1.6
AM_CPPFLAGS = -I$(top_srcdir)/include
AM_CPPFLAGS += -DUSE_AUTOTOOLS -DUNIX -Dunix

if ZLIB
ZLIBDEF = 
else
ZLIBDEF = -DEXCLUDEZLIB
endif

AM_CPPFLAGS += $(ZLIBDEF)

# Global config directory
globalconfdir := @sysconfdir@

if BUILDSPLITLIB
extralibs = libsword_imp.la libsword_flat.la
flatlib = libsword_flat.a
else
extralibs =
flatlib =
endif

lib_LTLIBRARIES = libsword.la $(extralibs)
#lib_LIBRARIES = $(flatlib)

libsword_la_SOURCES =
if ICU
ICULIBADD = $(ICU_LIBS) $(ICU_IOLIBS)
ICUDEF = -DSWICU_DATA=\"${pkglibdir}/${VERSION}_icu_${ICU_VER}\" 
else
ICULIBADD =
ICUDEF =
endif

AM_CPPFLAGS += $(ICUDEF)

if INSTALLMGR
IMGRLIBADD = -lcurl $(CURL_LIBS)
else
IMGRLIBADD =
endif


libsword_la_LIBADD = $(ICULIBADD) $(IMGRLIBADD) 

libsword_la_LDFLAGS = -version-info 5

if BUILDSPLITLIB
libsword_flat_la_LDFLAGS = -version-info 5
libsword_imp_la_LDFLAGS = -version-info 5
libsword_flat_la_LIBADD = $(ICULIBADD) $(IMGRLIBADD) libsword.la
libsword_imp_la_LIBADD = $(ICULIBADD) $(IMGRLIBADD) libsword.la
libsword_flat_la_SOURCES =
libsword_imp_la_SOURCES =
endif

include ../src/keys/Makefile.am
include ../src/utilfuns/Makefile.am
include ../src/mgr/Makefile.am
include ../src/frontend/Makefile.am
include ../src/modules/Makefile.am
include ../bindings/Makefile.am
include ../include/Makefile.am


