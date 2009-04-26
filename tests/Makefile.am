AUTOMAKE_OPTIONS = 1.6
INCLUDES = -I $(top_srcdir)/include
LDADD = $(top_builddir)/lib/libsword.la

SUBDIRS = cppunit

noinst_PROGRAMS = ciphertest keytest mgrtest parsekey versekeytest vtreekeytest versemgrtest listtest casttest \
modtest compnone complzss localetest introtest indextest configtest keycast \
romantest testblocks filtertest rawldidxtest lextest swaptest \
 swbuftest xmltest webiftest

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
#tlitmgrtest_LDADD = -lustdio
endif

if ZLIB
ZLIBPROG = compzip
else
ZLIBPROG =
endif

noinst_PROGRAMS += $(ZLIBPROG)

if ZLIB
compzip_SOURCES = compzip.cpp
endif

keytest_SOURCES = keytest.cpp
mgrtest_SOURCES = mgrtest.cpp
ciphertest_SOURCES = ciphertest.cpp
parsekey_SOURCES = parsekey.cpp
versekeytest_SOURCES = versekeytest.cpp
vtreekeytest_SOURCES = vtreekeytest.cpp
versemgrtest_SOURCES = versemgrtest.cpp
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
swbuftest_SOURCES = swbuftest.cpp
webiftest_SOURCES = webiftest.cpp
xmltest_SOURCES = xmltest.cpp


EXTRA_DIST = 
include bcppmake/Makefile.am
include testsuite/Makefile.am
include tmp/Makefile.am
