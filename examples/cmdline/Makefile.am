AUTOMAKE_OPTIONS = 1.5
INCLUDES = -I $(top_srcdir)/include
LDADD = -L$(top_builddir)/lib -lsword 

noinst_PROGRAMS = lookup search threaded_search

lookup_SOURCES = cmdline/lookup.cpp
search_SOURCES = cmdline/search.cpp
threaded_search_SOURCES = cmdline/threaded_search.cpp
threaded_search_LDADD = $(LDADD) -lpthread
