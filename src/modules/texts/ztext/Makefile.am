ztextdir = $(top_srcdir)/src/modules/texts/ztext

#if BUILDSPLITLIB
#libsword_imp_la_SOURCES += $(ztextdir)/ztext.cpp
#libsword_la_SOURCES += $(ztextdir)/ztext2.cpp
#else
if BUILDNEWVKEY
libsword_la_SOURCES += $(ztextdir)/ztext2.cpp
endif
libsword_la_SOURCES += $(ztextdir)/ztext.cpp
#endif
