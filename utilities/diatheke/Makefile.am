AUTOMAKE_OPTIONS = 1.6

INCLUDES = -I$(top_srcdir)/include

bin_PROGRAMS = diatheke

diatheke_SOURCES = diatheke.cpp corediatheke.cpp diathekemgr.cpp \
	diafiltmgr.cpp thmlcgi.cpp gbfcgi.cpp osiscgi.cpp

if ICU
iculibs = -licui18n -licuuc
endif

if USELUCENE
lucenelibs = -lclucene
endif

diatheke_LDADD = $(top_builddir)/lib/libsword.la \
	$(iculibs) $(lucenelibs)

noinst_HEADERS = corediatheke.h diafiltmgr.h diathekemgr.h gbfcgi.h thmlcgi.h \
	osiscgi.h

include cgi/Makefile.am
