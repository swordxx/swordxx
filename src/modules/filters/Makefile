
root := ../../..
subdirs := 
pre-targets := 
targets := ${root}/lib/libsword.a
clean-targets := none
include ${root}/Makefile.cfg

cpp += swbasicfilter.cpp

cpp += gbfhtml.cpp
cpp += gbfhtmlhref.cpp
cpp += gbfplain.cpp
cpp += gbfrtf.cpp
cpp += plainhtml.cpp
cpp += rwphtml.cpp
cpp += rwprtf.cpp
cpp += cipherfil.cpp
cpp += rtfhtml.cpp

cpp += gbfstrongs.cpp
cpp += gbffootnotes.cpp
cpp += gbfheadings.cpp
cpp += gbfmorph.cpp
cpp += plainfootnotes.cpp

cpp += gbfthml.cpp
cpp += thmlgbf.cpp
cpp += thmlrtf.cpp
cpp += thmlhtml.cpp
cpp += thmlhtmlhref.cpp
cpp += thmlplain.cpp

cpp += unicodertf.cpp
cpp += latin1utf8.cpp
cpp += latin1utf16.cpp
cpp += utf8utf16.cpp
cpp += utf16utf8.cpp
cpp += scsuutf8.cpp
cpp += utf8html.cpp
cpp += utf8latin1.cpp

cpp += thmlolb.cpp

ifeq ($(icu),yes)
cpp += utf8transliterator.cpp
endif

cpp += utf8cantillation.cpp
cpp += utf8hebrewpoints.cpp
cpp += utf8greekaccents.cpp

include ${root}/Makefile.post
