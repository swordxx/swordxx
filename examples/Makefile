# Makefile.in generated automatically by automake 1.5 from Makefile.am.

# Copyright 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001
# Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.



SHELL = /bin/sh

srcdir = .
top_srcdir = ..

prefix = /usr
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = /etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include
pkgdatadir = $(datadir)/sword
pkglibdir = $(libdir)/sword
pkgincludedir = $(includedir)/sword
top_builddir = ..

ACLOCAL = ${SHELL} /space/home/scribe/src/sword/missing --run aclocal
AUTOCONF = ${SHELL} /space/home/scribe/src/sword/missing --run autoconf
AUTOMAKE = ${SHELL} /space/home/scribe/src/sword/missing --run automake
AUTOHEADER = ${SHELL} /space/home/scribe/src/sword/missing --run autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
INSTALL_HEADER = $(INSTALL_DATA)
transform = s,x,x,
NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
host_alias = 
host_triplet = i686-pc-linux-gnu
AMTAR = ${SHELL} /space/home/scribe/src/sword/missing --run tar
AS = as
AWK = gawk
CC = gcc
CXX = g++
DEPDIR = .deps
DLLTOOL = dlltool
ECHO = echo
EXEEXT = 
INSTALL_STRIP_PROGRAM = ${SHELL} $(install_sh) -c -s
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LN_S = ln -s
MAINT = #
OBJDUMP = objdump
OBJEXT = o
PACKAGE = sword
RANLIB = ranlib
STRIP = strip
VERSION = 1.5.3
am__include = include
am__quote = 
dir_confdef = no
enable_debug = no
enable_profile = no
enable_vcl = no
install_sh = /space/home/scribe/src/sword/install-sh
target_cpu = i686
target_mingw32 = no
target_os = linux-gnu
target_system = intel
target_vendor = pc
with_conf = no
with_icu = no
with_zlib = yes

AUTOMAKE_OPTIONS = 1.5
INCLUDES = -I $(top_srcdir)/include
LDADD = -L$(top_builddir)/lib -lsword 

noinst_PROGRAMS = lookup search threaded_search

lookup_SOURCES = cmdline/lookup.cpp
search_SOURCES = cmdline/search.cpp
threaded_search_SOURCES = cmdline/threaded_search.cpp
threaded_search_LDADD = $(LDADD) -lpthread
subdir = examples
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = $(top_builddir)/config.h
CONFIG_CLEAN_FILES =
noinst_PROGRAMS = lookup$(EXEEXT) search$(EXEEXT) \
	threaded_search$(EXEEXT)
PROGRAMS = $(noinst_PROGRAMS)

am_lookup_OBJECTS = lookup.$(OBJEXT)
lookup_OBJECTS = $(am_lookup_OBJECTS)
lookup_LDADD = $(LDADD)
lookup_DEPENDENCIES =
lookup_LDFLAGS =
am_search_OBJECTS = search.$(OBJEXT)
search_OBJECTS = $(am_search_OBJECTS)
search_LDADD = $(LDADD)
search_DEPENDENCIES =
search_LDFLAGS =
am_threaded_search_OBJECTS = threaded_search.$(OBJEXT)
threaded_search_OBJECTS = $(am_threaded_search_OBJECTS)
threaded_search_DEPENDENCIES =
threaded_search_LDFLAGS =

DEFS = -DHAVE_CONFIG_H
DEFAULT_INCLUDES =  -I. -I$(srcdir) -I$(top_builddir)
CPPFLAGS = 
LDFLAGS = 
LIBS = -lstdc++ -lz 
depcomp = $(SHELL) $(top_srcdir)/depcomp
DEP_FILES = $(DEPDIR)/lookup.Po $(DEPDIR)/search.Po \
	$(DEPDIR)/threaded_search.Po
CXXCOMPILE = $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) \
	$(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS)
LTCXXCOMPILE = $(LIBTOOL) --mode=compile $(CXX) $(DEFS) \
	$(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) \
	$(AM_CXXFLAGS) $(CXXFLAGS)
CXXLD = $(CXX)
CXXLINK = $(LIBTOOL) --mode=link $(CXXLD) $(AM_CXXFLAGS) $(CXXFLAGS) \
	$(AM_LDFLAGS) $(LDFLAGS) -o $@
CXXFLAGS = -g -O2
DIST_SOURCES = $(lookup_SOURCES) $(search_SOURCES) \
	$(threaded_search_SOURCES)
DIST_COMMON = Makefile.am Makefile.in
SOURCES = $(lookup_SOURCES) $(search_SOURCES) $(threaded_search_SOURCES)

all: all-am

.SUFFIXES:
.SUFFIXES: .cpp .lo .o .obj

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs

distclean-libtool:
	-rm -f libtool
$(srcdir)/Makefile.in: # Makefile.am  $(top_srcdir)/configure.ac $(ACLOCAL_M4)
	cd $(top_srcdir) && \
	  $(AUTOMAKE) --foreign  examples/Makefile
Makefile: # $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) && \
	  CONFIG_HEADERS= CONFIG_LINKS= \
	  CONFIG_FILES=$(subdir)/$@ $(SHELL) ./config.status

clean-noinstPROGRAMS:
	-test -z "$(noinst_PROGRAMS)" || rm -f $(noinst_PROGRAMS)
lookup.$(OBJEXT): cmdline/lookup.cpp
lookup$(EXEEXT): $(lookup_OBJECTS) $(lookup_DEPENDENCIES) 
	@rm -f lookup$(EXEEXT)
	$(CXXLINK) $(lookup_LDFLAGS) $(lookup_OBJECTS) $(lookup_LDADD) $(LIBS)
search.$(OBJEXT): cmdline/search.cpp
search$(EXEEXT): $(search_OBJECTS) $(search_DEPENDENCIES) 
	@rm -f search$(EXEEXT)
	$(CXXLINK) $(search_LDFLAGS) $(search_OBJECTS) $(search_LDADD) $(LIBS)
threaded_search.$(OBJEXT): cmdline/threaded_search.cpp
threaded_search$(EXEEXT): $(threaded_search_OBJECTS) $(threaded_search_DEPENDENCIES) 
	@rm -f threaded_search$(EXEEXT)
	$(CXXLINK) $(threaded_search_LDFLAGS) $(threaded_search_OBJECTS) $(threaded_search_LDADD) $(LIBS)

mostlyclean-compile:
	-rm -f *.$(OBJEXT) core *.core

distclean-compile:
	-rm -f *.tab.c

include $(DEPDIR)/lookup.Po
include $(DEPDIR)/search.Po
include $(DEPDIR)/threaded_search.Po

distclean-depend:
	-rm -rf $(DEPDIR)

.cpp.o:
	source='$<' object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Po' tmpdepfile='$(DEPDIR)/$*.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXXCOMPILE) -c -o $@ `test -f $< || echo '$(srcdir)/'`$<

.cpp.obj:
	source='$<' object='$@' libtool=no \
	depfile='$(DEPDIR)/$*.Po' tmpdepfile='$(DEPDIR)/$*.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXXCOMPILE) -c -o $@ `cygpath -w $<`

.cpp.lo:
	source='$<' object='$@' libtool=yes \
	depfile='$(DEPDIR)/$*.Plo' tmpdepfile='$(DEPDIR)/$*.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LTCXXCOMPILE) -c -o $@ `test -f $< || echo '$(srcdir)/'`$<

lookup.o: cmdline/lookup.cpp
	source='cmdline/lookup.cpp' object='lookup.o' libtool=no \
	depfile='$(DEPDIR)/lookup.Po' tmpdepfile='$(DEPDIR)/lookup.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o lookup.o `test -f cmdline/lookup.cpp || echo '$(srcdir)/'`cmdline/lookup.cpp

lookup.obj: cmdline/lookup.cpp
	source='cmdline/lookup.cpp' object='lookup.obj' libtool=no \
	depfile='$(DEPDIR)/lookup.Po' tmpdepfile='$(DEPDIR)/lookup.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o lookup.obj `cygpath -w cmdline/lookup.cpp`

lookup.lo: cmdline/lookup.cpp
	source='cmdline/lookup.cpp' object='lookup.lo' libtool=yes \
	depfile='$(DEPDIR)/lookup.Plo' tmpdepfile='$(DEPDIR)/lookup.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o lookup.lo `test -f cmdline/lookup.cpp || echo '$(srcdir)/'`cmdline/lookup.cpp

search.o: cmdline/search.cpp
	source='cmdline/search.cpp' object='search.o' libtool=no \
	depfile='$(DEPDIR)/search.Po' tmpdepfile='$(DEPDIR)/search.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o search.o `test -f cmdline/search.cpp || echo '$(srcdir)/'`cmdline/search.cpp

search.obj: cmdline/search.cpp
	source='cmdline/search.cpp' object='search.obj' libtool=no \
	depfile='$(DEPDIR)/search.Po' tmpdepfile='$(DEPDIR)/search.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o search.obj `cygpath -w cmdline/search.cpp`

search.lo: cmdline/search.cpp
	source='cmdline/search.cpp' object='search.lo' libtool=yes \
	depfile='$(DEPDIR)/search.Plo' tmpdepfile='$(DEPDIR)/search.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o search.lo `test -f cmdline/search.cpp || echo '$(srcdir)/'`cmdline/search.cpp

threaded_search.o: cmdline/threaded_search.cpp
	source='cmdline/threaded_search.cpp' object='threaded_search.o' libtool=no \
	depfile='$(DEPDIR)/threaded_search.Po' tmpdepfile='$(DEPDIR)/threaded_search.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o threaded_search.o `test -f cmdline/threaded_search.cpp || echo '$(srcdir)/'`cmdline/threaded_search.cpp

threaded_search.obj: cmdline/threaded_search.cpp
	source='cmdline/threaded_search.cpp' object='threaded_search.obj' libtool=no \
	depfile='$(DEPDIR)/threaded_search.Po' tmpdepfile='$(DEPDIR)/threaded_search.TPo' \
	$(CXXDEPMODE) $(depcomp) \
	$(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o threaded_search.obj `cygpath -w cmdline/threaded_search.cpp`

threaded_search.lo: cmdline/threaded_search.cpp
	source='cmdline/threaded_search.cpp' object='threaded_search.lo' libtool=yes \
	depfile='$(DEPDIR)/threaded_search.Plo' tmpdepfile='$(DEPDIR)/threaded_search.TPlo' \
	$(CXXDEPMODE) $(depcomp) \
	$(LIBTOOL) --mode=compile $(CXX) $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CXXFLAGS) $(CXXFLAGS) -c -o threaded_search.lo `test -f cmdline/threaded_search.cpp || echo '$(srcdir)/'`cmdline/threaded_search.cpp
CXXDEPMODE = depmode=gcc3
uninstall-info-am:

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP) $(TAGS_FILES)
	list='$(SOURCES) $(HEADERS) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	mkid -fID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) \
		$(TAGS_FILES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS) $(TAGS_FILES)'; \
	unique=`for i in $$list; do \
	    if test -f "$$i"; then echo $$i; else echo $(srcdir)/$$i; fi; \
	  done | \
	  $(AWK) '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || etags $(ETAGS_ARGS) $$tags  $$unique $(LISP)

GTAGS:
	here=`CDPATH=: && cd $(top_builddir) && pwd` \
	  && cd $(top_srcdir) \
	  && gtags -i $(GTAGS_ARGS) $$here

distclean-tags:
	-rm -f TAGS ID GTAGS GRTAGS GSYMS GPATH

DISTFILES = $(DIST_COMMON) $(DIST_SOURCES) $(TEXINFOS) $(EXTRA_DIST)

top_distdir = ..
distdir = $(top_distdir)/$(PACKAGE)-$(VERSION)

distdir: $(DISTFILES)
	@for file in $(DISTFILES); do \
	  if test -f $$file; then d=.; else d=$(srcdir); fi; \
	  dir=`echo "$$file" | sed -e 's,/[^/]*$$,,'`; \
	  if test "$$dir" != "$$file" && test "$$dir" != "."; then \
	    $(mkinstalldirs) "$(distdir)/$$dir"; \
	  fi; \
	  if test -d $$d/$$file; then \
	    cp -pR $$d/$$file $(distdir) \
	    || exit 1; \
	  else \
	    test -f $(distdir)/$$file \
	    || cp -p $$d/$$file $(distdir)/$$file \
	    || exit 1; \
	  fi; \
	done
check-am: all-am
check: check-am
all-am: Makefile $(PROGRAMS)

installdirs:

install: install-am
install-exec: install-exec-am
install-data: install-data-am
uninstall: uninstall-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am

installcheck: installcheck-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) INSTALL_PROGRAM="$(INSTALL_STRIP_PROGRAM)" \
	  `test -z '$(STRIP)' || \
	    echo "INSTALL_PROGRAM_ENV=STRIPPROG='$(STRIP)'"` install
mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES) stamp-h stamp-h[0-9]*

maintainer-clean-generic:
	@echo "This command is intended for maintainers to use"
	@echo "it deletes files that may require special tools to rebuild."
clean: clean-am

clean-am: clean-generic clean-libtool clean-noinstPROGRAMS \
	mostlyclean-am

distclean: distclean-am

distclean-am: clean-am distclean-compile distclean-depend \
	distclean-generic distclean-libtool distclean-tags

dvi: dvi-am

dvi-am:

info: info-am

info-am:

install-data-am:

install-exec-am:

install-info: install-info-am

install-man:

installcheck-am:

maintainer-clean: maintainer-clean-am

maintainer-clean-am: distclean-am maintainer-clean-generic

mostlyclean: mostlyclean-am

mostlyclean-am: mostlyclean-compile mostlyclean-generic \
	mostlyclean-libtool

uninstall-am: uninstall-info-am

.PHONY: GTAGS all all-am check check-am clean clean-generic \
	clean-libtool clean-noinstPROGRAMS distclean distclean-compile \
	distclean-depend distclean-generic distclean-libtool \
	distclean-tags distdir dvi dvi-am info info-am install \
	install-am install-data install-data-am install-exec \
	install-exec-am install-info install-info-am install-man \
	install-strip installcheck installcheck-am installdirs \
	maintainer-clean maintainer-clean-generic mostlyclean \
	mostlyclean-compile mostlyclean-generic mostlyclean-libtool \
	tags uninstall uninstall-am uninstall-info-am

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
