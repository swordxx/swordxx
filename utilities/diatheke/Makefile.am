AUTOMAKE_OPTIONS = 1.5

INCLUDES = -I$(top_srcdir)/include

bin_PROGRAMS = diatheke
diatheke_SOURCES = diatheke.cpp corediatheke.cpp diathekemgr.cpp \
	diafiltmgr.cpp thmlcgi.cpp gbfcgi.cpp
diatheke_LDADD = -L$(top_builddir)/lib -lsword

