bindingsdir = $(top_srcdir)/bindings
#if BUILDSPLITLIB
#libsword_flat_la_SOURCES += $(bindingsdir)/flatapi.cpp
#else
libsword_la_SOURCES += $(bindingsdir)/flatapi.cpp
#endif

include $(bindingsdir)/bcppmake/Makefile.am
include $(bindingsdir)/gsoap/Makefile.am
