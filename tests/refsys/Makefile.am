AUTOMAKE_OPTIONS = 1.6
INCLUDES = -I $(top_srcdir)/include
#if BUILDSPLITLIB
#LDADD = $(top_builddir)/lib/libsword_imp.la \
#	$(top_builddir)/lib/libsword.la \
#	$(XML_LIBS)
#else
LDADD = $(top_builddir)/lib/libsword.la \
	$(XML_LIBS)
#endif

noinst_PROGRAMS = mkrefsys

mkrefsys_SOURCES = mkrefsys.cpp

