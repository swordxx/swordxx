AUTOMAKE_OPTIONS = 1.6
INCLUDES = -I $(top_srcdir)/include
#if BUILDSPLITLIB
#LDADD = $(top_builddir)/lib/libsword_imp.la \
#        $(top_builddir)/lib/libsword.la
#else
LDADD = $(top_builddir)/lib/libsword.la
#endif
if BUILDREFSYS
refsysdir = refsys
else
refsysdir =
endif

SUBDIRS = $(refsysdir)

noinst_PROGRAMS = keytest mgrtest parsekey listtest casttest modtest \
compnone complzss localetest introtest indextest configtest keycast \
romantest testblocks filtertest rawldidxtest lextest swaptest \
genbooktest treeidxtest swbuftest xmltest webiftest

#outputcps

if ICU
ICUPROG = icutest translittest tlitmgrtest
else
ICUPROG =
endif

noinst_PROGRAMS += $(ICUPROG)

if ICU
icutest_SOURCES = icutest.cpp
translittest_SOURCES = translittest.cpp
tlitmgrtest_SOURCES = tlitmgrtest.cpp
tlitmgrtest_LDADD = -lustdio
endif

if ZLIB
ZLIBPROG = compzip
else
ZLIBPROG =
endif

if BUILDNEWVKEY
VKPROG = keytest2 refsystest
else
VKPROG =
endif

noinst_PROGRAMS += $(ZLIBPROG) $(VKPROG)

if ZLIB
compzip_SOURCES = compzip.cpp
endif

if BUILDNEWVKEY
keytest2_SOURCES = keytest2.cpp
refsystest_SOURCES = refsystest.cpp
#You have to make some parts of versekey.h public to 
#get outputcps to work
#outputcps_SOURCES = outputcps.cpp
endif

keytest_SOURCES = keytest.cpp
mgrtest_SOURCES = mgrtest.cpp
parsekey_SOURCES = parsekey.cpp
listtest_SOURCES = listtest.cpp
casttest_SOURCES = casttest.cpp
modtest_SOURCES = modtest.cpp
compnone_SOURCES = compnone.cpp
complzss_SOURCES = complzss.cpp
localetest_SOURCES = localetest.cpp
keycast_SOURCES = keycast.cpp
introtest_SOURCES = introtest.cpp
indextest_SOURCES = indextest.cpp
configtest_SOURCES = configtest.cpp
romantest_SOURCES = romantest.cpp
testblocks_SOURCES = testblocks.cpp
filtertest_SOURCES = filtertest.cpp
lextest_SOURCES = lextest.cpp
rawldidxtest_SOURCES = rawldidxtest.cpp
swaptest_SOURCES = swaptest.cpp
treeidxtest_SOURCES = treeidxtest.cpp
genbooktest_SOURCES = genbooktest.cpp
swbuftest_SOURCES = swbuftest.cpp
webiftest_SOURCES = webiftest.cpp
xmltest_SOURCES = xmltest.cpp


EXTRA_DIST = 
include bcppmake/Makefile.am
include testsuite/Makefile.am
include tmp/Makefile.am
