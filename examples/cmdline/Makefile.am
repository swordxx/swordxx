AUTOMAKE_OPTIONS = 1.6
INCLUDES = -I $(top_srcdir)/include
#if BUILDSPLITLIB
#LDADD = $(top_builddir)/lib/libsword_imp.la \
#        $(top_builddir)/lib/libsword.la
#else
LDADD = $(top_builddir)/lib/libsword.la
#endif
noinst_PROGRAMS = lookup search threaded_search

lookup_SOURCES = lookup.cpp
search_SOURCES = search.cpp
threaded_search_SOURCES = threaded_search.cpp
threaded_search_LDADD = $(LDADD) -lpthread
