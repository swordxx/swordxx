INCLUDES = -I$(top_srcdir)/include

# Global config directory
globalconfdir := @sysconfdir@

lib_LTLIBRARIES = libsword.la

libsword_la_SOURCES =
libsword_la_LIBADD =
libsword_la_LDFLAGS = -version-info 1

include ../src/keys/Makefile.am
include ../src/utilfuns/Makefile.am
include ../src/mgr/Makefile.am
include ../src/frontend/Makefile.am
include ../src/modules/Makefile.am
include ../include/Makefile.am

