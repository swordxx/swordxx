# This file started on 18 January 2010 by Gregory Hellings
# It is ceded to The SWORD Library developers and CrossWire under the terms
# of their own GPLv2 license and all copyright is transferred to them for
# all posterity and eternity, wherever such transfer is possible.  Where it is
# not, then this file is released under the GPLv2 by myself.
#
#

SET(sword_base_frontend_SOURCES
    src/frontend/swlog.cpp
)
SOURCE_GROUP("src\\frontend" FILES ${sword_base_frontend_SOURCES})

SET(sword_base_keys_SOURCES
    src/keys/swkey.cpp
    src/keys/listkey.cpp
    src/keys/strkey.cpp
    src/keys/treekey.cpp
    src/keys/treekeyidx.cpp
    src/keys/versekey.cpp
    src/keys/versetreekey.cpp
)
SOURCE_GROUP("src\\keys" FILES ${sword_base_keys_SOURCES})

SET(sword_base_mgr_SOURCES
    src/mgr/swconfig.cpp
    src/mgr/swmgr.cpp
    src/mgr/swfiltermgr.cpp
    src/mgr/encfiltmgr.cpp
    src/mgr/markupfiltmgr.cpp
    src/mgr/filemgr.cpp
    src/mgr/versificationmgr.cpp
    src/mgr/remotetrans.cpp
    src/mgr/swlocale.cpp
    src/mgr/localemgr.cpp
    src/mgr/swsearchable.cpp
    src/mgr/installmgr.cpp
    src/mgr/stringmgr.cpp
)
SOURCE_GROUP("src\\mgr" FILES ${sword_base_mgr_SOURCES})

SET(sword_base_module_SOURCES
    src/modules/swmodule.cpp
    src/modules/comments/swcom.cpp
    src/modules/comments/hrefcom/hrefcom.cpp
    src/modules/comments/rawcom/rawcom.cpp
    src/modules/comments/rawcom4/rawcom4.cpp
    src/modules/comments/rawfiles/rawfiles.cpp
    src/modules/comments/zcom/zcom.cpp
    src/modules/comments/zcom4/zcom4.cpp
    src/modules/common/rawstr.cpp
    src/modules/common/rawstr4.cpp
    src/modules/common/swcomprs.cpp
    src/modules/common/lzsscomprs.cpp
    src/modules/common/rawverse.cpp
    src/modules/common/rawverse4.cpp
    src/modules/common/swcipher.cpp
    src/modules/common/zverse.cpp
    src/modules/common/zverse4.cpp
    src/modules/common/zstr.cpp
    src/modules/common/entriesblk.cpp
    src/modules/common/sapphire.cpp
    src/modules/filters/swbasicfilter.cpp
    src/modules/filters/swoptfilter.cpp

    src/modules/filters/gbfhtml.cpp
    src/modules/filters/gbfxhtml.cpp
    src/modules/filters/gbfhtmlhref.cpp
    src/modules/filters/gbfwebif.cpp
    src/modules/filters/gbfplain.cpp
    src/modules/filters/gbfrtf.cpp
    src/modules/filters/gbfstrongs.cpp
    src/modules/filters/gbffootnotes.cpp
    src/modules/filters/gbfheadings.cpp
    src/modules/filters/gbfredletterwords.cpp
    src/modules/filters/gbfmorph.cpp
    src/modules/filters/gbfwordjs.cpp
    src/modules/filters/gbflatex.cpp

    src/modules/filters/thmlstrongs.cpp
    src/modules/filters/thmlfootnotes.cpp
    src/modules/filters/thmlheadings.cpp
    src/modules/filters/thmlmorph.cpp
    src/modules/filters/thmllemma.cpp
    src/modules/filters/thmlscripref.cpp
    src/modules/filters/thmlvariants.cpp
    src/modules/filters/thmlgbf.cpp
    src/modules/filters/thmlrtf.cpp
    src/modules/filters/thmlhtml.cpp
    src/modules/filters/thmlxhtml.cpp
    src/modules/filters/thmlhtmlhref.cpp
    src/modules/filters/thmlwebif.cpp
    src/modules/filters/thmlwordjs.cpp
    src/modules/filters/thmllatex.cpp

    src/modules/filters/teiplain.cpp
    src/modules/filters/teirtf.cpp
    src/modules/filters/teixhtml.cpp
    src/modules/filters/teihtmlhref.cpp
    src/modules/filters/teilatex.cpp

    src/modules/filters/gbfthml.cpp
    src/modules/filters/gbfosis.cpp
    src/modules/filters/thmlosis.cpp
    src/modules/filters/thmlplain.cpp
    src/modules/filters/osisosis.cpp

    src/modules/filters/osisenum.cpp
    src/modules/filters/osisglosses.cpp
    src/modules/filters/osisxlit.cpp
    src/modules/filters/osisheadings.cpp
    src/modules/filters/osisfootnotes.cpp
    src/modules/filters/osishtmlhref.cpp
    src/modules/filters/osisxhtml.cpp
    src/modules/filters/osiswebif.cpp
    src/modules/filters/osismorph.cpp
    src/modules/filters/osisstrongs.cpp
    src/modules/filters/osisplain.cpp
    src/modules/filters/osisrtf.cpp
    src/modules/filters/osislemma.cpp
    src/modules/filters/osisredletterwords.cpp
    src/modules/filters/osisscripref.cpp
    src/modules/filters/osisvariants.cpp
    src/modules/filters/osiswordjs.cpp
    src/modules/filters/osismorphsegmentation.cpp
    src/modules/filters/osisreferencelinks.cpp
    src/modules/filters/osislatex.cpp

    src/modules/filters/latin1utf8.cpp
    src/modules/filters/latin1utf16.cpp
    src/modules/filters/utf8utf16.cpp
    src/modules/filters/utf16utf8.cpp
    src/modules/filters/utf8html.cpp
    src/modules/filters/utf8latin1.cpp
    src/modules/filters/unicodertf.cpp
    src/modules/filters/scsuutf8.cpp
    src/modules/filters/utf8scsu.cpp

    src/modules/filters/utf8cantillation.cpp
    src/modules/filters/utf8hebrewpoints.cpp
    src/modules/filters/utf8arabicpoints.cpp
    src/modules/filters/utf8greekaccents.cpp

    src/modules/filters/cipherfil.cpp

    src/modules/filters/rtfhtml.cpp
    src/modules/filters/greeklexattribs.cpp
    src/modules/filters/papyriplain.cpp

    src/modules/genbook/swgenbook.cpp
    src/modules/genbook/rawgenbook/rawgenbook.cpp

    src/modules/lexdict/swld.cpp
    src/modules/lexdict/rawld/rawld.cpp
    src/modules/lexdict/rawld4/rawld4.cpp
    src/modules/lexdict/zld/zld.cpp

    src/modules/texts/swtext.cpp
    src/modules/texts/rawtext/rawtext.cpp
    src/modules/texts/rawtext4/rawtext4.cpp
    src/modules/texts/ztext/ztext.cpp
    src/modules/texts/ztext4/ztext4.cpp
)
SOURCE_GROUP("src\\modules" FILES ${sword_base_module_SOURCES})

SET(sword_base_utilfns_SOURCES
    src/utilfuns/swobject.cpp
    src/utilfuns/utilstr.cpp
    src/utilfuns/utilxml.cpp
    src/utilfuns/ftpparse.c
    src/utilfuns/url.cpp
    src/utilfuns/roman.cpp
    src/utilfuns/version.cpp
)
SOURCE_GROUP("src\\utilfns" FILES ${sword_base_utilfns_SOURCES})

# Universal sources
SET(sword_base_SOURCES
    ${sword_base_frontend_SOURCES}
    ${sword_base_keys_SOURCES}
    ${sword_base_mgr_SOURCES}
    ${sword_base_module_SOURCES}
    ${sword_base_utilfns_SOURCES}
)

# Sources relying on ZLib
SET(sword_zlib_used_SOURCES
    src/modules/common/zipcomprs.cpp
    src/utilfuns/zlib/untgz.c
)

# Sources relying on bzip2 (libbz2)
SET(sword_bzip2_used_SOURCES
    src/modules/common/bz2comprs.cpp
)

# Sources relying on xz (liblzma)
SET(sword_xz_used_SOURCES
    src/modules/common/xzcomprs.cpp
)

# Sources relying on cURL
SET(sword_curl_found_SOURCES
    src/mgr/curlftpt.cpp
    src/mgr/curlhttpt.cpp
)

# Sources relying on CLucene
SET(sword_clucene_found_SOURCES)
SET(sword_clucene_nofound_SOURCES)

# Sources based on the ICU status
SET(sword_icu_found_SOURCES
    src/modules/filters/utf8transliterator.cpp
    src/modules/filters/utf8nfc.cpp
    src/modules/filters/utf8nfkd.cpp
    src/modules/filters/utf8arshaping.cpp
    src/modules/filters/utf8bidireorder.cpp
)

# Headers
FILE(GLOB_RECURSE SWORD_INSTALL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
LIST(APPEND SWORD_INSTALL_HEADERS
    "${CMAKE_CURRENT_BINARY_DIR}/include/version.h")
