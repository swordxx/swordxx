AUTOMAKE_OPTIONS = 1.6
INCLUDES = -I $(top_srcdir)/include
LDADD = -L$(top_builddir)/lib -lsword 

noinst_PROGRAMS = cipherraw ciphertest ciphertest2 lexdump mkfastmod \
mod2vpl vpl2mod stepdump step2vpl mod2zmod gbfidx modwrite addvs addld emptyvss \
txt2sword addgb imp2gbs imp2ld imp2vs mod2imp thml2gbs mod2osis xml2gbs osis2mod

if ICU
ICUPROG = 
else
ICUPROG =
endif

noinst_PROGRAMS += $(ICUPROG)

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
mod2zmod_SOURCES = mod2zmod.cpp
gbfidx_SOURCES = gbfidx.cpp
modwrite_SOURCES = modwrite.cpp
addvs_SOURCES = addvs.cpp
addld_SOURCES = addld.cpp
emptyvss_SOURCES = emptyvss.cpp
txt2sword_SOURCES = txt2sword.cpp
addgb_SOURCES = addgb.cpp
imp2gbs_SOURCES = imp2gbs.cpp
imp2ld_SOURCES = imp2ld.cpp
imp2vs_SOURCES = imp2vs.cpp
mod2imp_SOURCES = mod2imp.cpp
thml2gbs_SOURCES = thml2gbs.cpp
mod2osis_SOURCES = mod2osis.cpp
xml2gbs_SOURCES = xml2gbs.cpp
osis2mod_SOURCES = osis2mod.cpp
