mgrdir = $(top_srcdir)/src/mgr

if CONFDEF
globdef = -DGLOBCONFPATH=\"${globalconfdir}/sword.conf\"
else
globdef =
endif

if ICU
icudatadir = -DICUDATA=\"${pkglibdir}\"
else
icudatadir =
endif


DEFS += $(globdef) $(icudatadir)

libsword_la_SOURCES += $(mgrdir)/swconfig.cpp
libsword_la_SOURCES += $(mgrdir)/swmgr.cpp
libsword_la_SOURCES += $(mgrdir)/swfiltermgr.cpp
libsword_la_SOURCES += $(mgrdir)/encfiltmgr.cpp
libsword_la_SOURCES += $(mgrdir)/markupfiltmgr.cpp
libsword_la_SOURCES += $(mgrdir)/filemgr.cpp
libsword_la_SOURCES += $(mgrdir)/swlocale.cpp
libsword_la_SOURCES += $(mgrdir)/localemgr.cpp
libsword_la_SOURCES += $(mgrdir)/swcacher.cpp
