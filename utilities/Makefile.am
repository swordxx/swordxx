AUTOMAKE_OPTIONS = 1.6
SUBDIRS = diatheke
INCLUDES = -I $(top_srcdir)/include
if BUILDSPLITLIB
LDADD = $(top_builddir)/lib/libsword_imp.la \
        $(top_builddir)/lib/libsword.la
else
LDADD = $(top_builddir)/lib/libsword.la
endif
noinst_PROGRAMS = cipherraw ciphertest ciphertest2 lexdump mkfastmod \
stepdump step2vpl gbfidx modwrite addvs emptyvss \
txt2sword addgb imp2gbs mod2imp thml2gbs xml2gbs

bin_PROGRAMS = mod2imp mod2osis osis2mod vs2osisref mod2vpl \
	vpl2mod imp2vs installmgr

if INSTALLMGR
IMGRPROG =
else
IMGRPROG = 
endif


if ICU
ICUPROG = 
else
ICUPROG =
endif

if ZLIB
ZLIBPROG = mod2zmod addld imp2ld
else
ZLIBPROG =
endif

noinst_PROGRAMS += $(ICUPROG)
bin_PROGRAMS += $(ZLIBPROG) $(IMGRPROG)

if ICU
endif

cipherraw_SOURCES = cipherraw.cpp
ciphertest_SOURCES = ciphertest.cpp
ciphertest2_SOURCES = ciphertest2.cpp
lexdump_SOURCES = lexdump.c
lexdump_LDADD = -lstdc++
mkfastmod_SOURCES = mkfastmod.cpp
mod2vpl_SOURCES = mod2vpl.cpp
vpl2mod_SOURCES = vpl2mod.cpp
stepdump_SOURCES = stepdump.cpp
step2vpl_SOURCES = step2vpl.cpp
gbfidx_SOURCES = gbfidx.cpp
modwrite_SOURCES = modwrite.cpp
addvs_SOURCES = addvs.cpp
emptyvss_SOURCES = emptyvss.cpp
txt2sword_SOURCES = txt2sword.cpp
addgb_SOURCES = addgb.cpp
imp2gbs_SOURCES = imp2gbs.cpp
imp2vs_SOURCES = imp2vs.cpp
mod2imp_SOURCES = mod2imp.cpp
thml2gbs_SOURCES = thml2gbs.cpp
mod2osis_SOURCES = mod2osis.cpp
xml2gbs_SOURCES = xml2gbs.cpp
osis2mod_SOURCES = osis2mod.cpp
vs2osisref_SOURCES = vs2osisref.cpp

installmgr_SOURCES = installmgr.cpp
#installmgr_LDADD = -lcurl $(CURL_LIBS) $(LDADD)

if ZLIB
mod2zmod_SOURCES = mod2zmod.cpp
addld_SOURCES = addld.cpp
imp2ld_SOURCES = imp2ld.cpp
endif

EXTRA_DIST = 
include bcppmake/Makefile.am
include kylixmake/Makefile.am
include perl/Makefile.am
