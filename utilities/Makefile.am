INCLUDES = -I $(top_srcdir)/include
#LIBS = -L$(top_builddir)/../lib -lsword 
LDADD = -L$(top_builddir)/lib -lsword 
#LDADD = $(top_builddir)/../lib/libsword.a
#if ZLIB
#ZLIBLIBS = -lz
#else
#ZLIBLIBS =
#endif
#LDADD += $(ZLIBLIBS)


noinst_PROGRAMS = cipherraw ciphertest ciphertest2 lexdump mkfastmod \
mod2vpl vpl2mod stepdump step2vpl mod2zmod modwrite addvs addld emptyvss \
txt2sword

if ICU
ICUPROG = normcode
else
ICUPROG =
endif

noinst_PROGRAMS += $(ICUPROG)

if ICU
normcode_SOURCES = normcode.cpp
endif

cipherraw_SOURCES = cipherraw.cpp
ciphertest_SOURCES = ciphertest.cpp
ciphertest2_SOURCES = ciphertest2.cpp
lexdump_SOURCES = lexdump.c
mkfastmod_SOURCES = mkfastmod.cpp
mod2vpl_SOURCES = mod2vpl.cpp
vpl2mod_SOURCES = vpl2mod.cpp
stepdump_SOURCES = stepdump.cpp
step2vpl_SOURCES = step2vpl.cpp
mod2zmod_SOURCES = mod2zmod.cpp
modwrite_SOURCES = modwrite.cpp
addvs_SOURCES = addvs.cpp
addld_SOURCES = addld.cpp
emptyvss_SOURCES = emptyvss.cpp
txt2sword_SOURCES = txt2sword.cpp

