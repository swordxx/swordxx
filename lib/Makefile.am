AUTOMAKE_OPTIONS = 1.6
INCLUDES = -I$(top_srcdir)/include
DEFS += -DUSE_AUTOTOOLS

# Global config directory
globalconfdir := @sysconfdir@

lib_LTLIBRARIES = libsword.la

libsword_la_SOURCES =
if ICU
libsword_la_LIBADD = -licui18n -licuuc -lustdio
else
libsword_la_LIBADD =
endif
libsword_la_LDFLAGS = -version-info 1

include ../src/keys/Makefile.am
include ../src/utilfuns/Makefile.am
include ../src/mgr/Makefile.am
include ../src/frontend/Makefile.am
include ../src/modules/Makefile.am
include ../bindings/Makefile.am
include ../include/Makefile.am

