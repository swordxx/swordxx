AUTOMAKE_OPTIONS = 1.6

INCLUDES = -I$(top_srcdir)/include

bin_PROGRAMS = diatheke

diatheke_SOURCES = diatheke.cpp corediatheke.cpp diathekemgr.cpp \
	diafiltmgr.cpp thmlcgi.cpp gbfcgi.cpp

diatheke_LDADD = -L$(top_builddir)/lib -lsword

noinst_HEADERS = corediatheke.h diafiltmgr.h diathekemgr.h gbfcgi.h thmlcgi.h
