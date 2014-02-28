AUTOMAKE_OPTIONS = 1.6
AM_CPPFLAGS = -I $(top_srcdir)/include
if USE_INTERNAL_REGEX
AM_CPPFLAGS += -I$(top_srcdir)/include/internal/regex
endif
LDADD = $(top_builddir)/lib/libsword.la

noinst_PROGRAMS = parallelbibles

parallelbibles_SOURCES = parallelbibles.cpp

