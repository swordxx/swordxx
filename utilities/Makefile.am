AUTOMAKE_OPTIONS = 1.6
SUBDIRS = diatheke
INCLUDES = -I $(top_srcdir)/include
LDADD = $(top_builddir)/lib/libsword.la
noinst_PROGRAMS = cipherraw lexdump \
stepdump step2vpl gbfidx modwrite addvs emptyvss \
addgb genbookutil treeidxutil

bin_PROGRAMS = mod2imp mod2osis osis2mod tei2mod vs2osisref vs2osisreftxt mod2vpl \
	mkfastmod vpl2mod imp2vs installmgr xml2gbs imp2gbs

#if INSTALLMGR
#IMGRPROG =
#else
#IMGRPROG = 
#endif


#if ICU
#ICUPROG = 
#else
#ICUPROG =
#endif

if ZLIB
ZLIBPROG = mod2zmod addld imp2ld
else
ZLIBPROG =
endif



#noinst_PROGRAMS += $(ICUPROG)
bin_PROGRAMS += $(ZLIBPROG) 
#$(IMGRPROG)

if ICU
endif

cipherraw_SOURCES = cipherraw.cpp
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
addgb_SOURCES = addgb.cpp
imp2gbs_SOURCES = imp2gbs.cpp
imp2vs_SOURCES = imp2vs.cpp
mod2imp_SOURCES = mod2imp.cpp
mod2osis_SOURCES = mod2osis.cpp
xml2gbs_SOURCES = xml2gbs.cpp
osis2mod_SOURCES = osis2mod.cpp
tei2mod_SOURCES = tei2mod.cpp
vs2osisref_SOURCES = vs2osisref.cpp
vs2osisreftxt_SOURCES = vs2osisreftxt.cpp
genbookutil_SOURCES = genbookutil.cpp
treeidxutil_SOURCES = treeidxutil.cpp

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
