
utilfunsdir = $(top_srcdir)/src/utilfuns
libsword_la_SOURCES += $(utilfunsdir)/Greek2Greek.cpp
libsword_la_SOURCES += $(utilfunsdir)/utilstr.cpp
libsword_la_SOURCES += $(utilfunsdir)/unixstr.cpp
libsword_la_SOURCES += $(utilfunsdir)/swunicod.cpp
libsword_la_SOURCES += $(utilfunsdir)/swversion.cpp

if MINGW
SWREGEX = $(utilfunsdir)/regex.c
else
SWREGEX =
endif
libsword_la_SOURCES += $(SWREGEX)


libsword_la_SOURCES += $(utilfunsdir)/roman.cpp


