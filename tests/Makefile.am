INCLUDES = -I $(top_srcdir)/include
LDADD = -L$(top_builddir)/lib -lsword 


noinst_PROGRAMS = keytest mgrtest parsekey listtest casttest modtest \
compnone complzss localetest keycast introtest indextest configtest \
romantest testblocks filtertest swaptest genbooktest treeidxtest

if ICU
ICUPROG = icutest
else
ICUPROG =
endif

noinst_PROGRAMS += $(ICUPROG)

if ICU
icutest_SOURCES = icutest.cpp
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
swaptest_SOURCES = swaptest.cpp
treeidxtest_SOURCES = treeidxtest.cpp
genbooktest_SOURCES = genbooktest.cpp
