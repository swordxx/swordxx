AUTOMAKE_OPTIONS = 1.6
INCLUDES = -I $(top_srcdir)/include
LDADD = $(top_builddir)/lib/libsword.la

noinst_PROGRAMS = lookup search threaded_search

lookup_SOURCES = lookup.cpp
search_SOURCES = search.cpp
threaded_search_SOURCES = threaded_search.cpp
threaded_search_LDADD = $(LDADD) -lpthread
