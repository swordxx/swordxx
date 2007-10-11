commondir = $(top_srcdir)/src/modules/common

libsword_la_SOURCES += $(commondir)/rawstr.cpp
libsword_la_SOURCES += $(commondir)/rawstr4.cpp
libsword_la_SOURCES += $(commondir)/swcomprs.cpp
libsword_la_SOURCES += $(commondir)/lzsscomprs.cpp

if ZLIB
SWZLIB = $(commondir)/zipcomprs.cpp
else
SWZLIB =
endif
libsword_la_SOURCES += $(SWZLIB)
libsword_la_SOURCES += $(commondir)/rawverse.cpp
libsword_la_SOURCES += $(commondir)/rawverse4.cpp
libsword_la_SOURCES += $(commondir)/swcipher.cpp
libsword_la_SOURCES += $(commondir)/zverse.cpp
libsword_la_SOURCES += $(commondir)/zstr.cpp
libsword_la_SOURCES += $(commondir)/entriesblk.cpp
libsword_la_SOURCES += $(commondir)/sapphire.cpp

