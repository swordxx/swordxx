/******************************************************************************
 *
 *  defs.h -    Global defines, mostly platform-specific stuff
 *
 * Copyright 2000-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */
// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
#ifndef SWORDXX_DEFS_H
#define SWORDXX_DEFS_H

namespace swordxx {

// _declspec works in BC++ 5 and later, as well as VC++
#if defined(_MSC_VER)

#  ifdef SWMAKINGDLL
#    define SWDLLEXPORT _declspec( dllexport )
#  elif defined(SWUSINGDLL)
#    define SWDLLEXPORT _declspec( dllimport )
#  else
#    define SWDLLEXPORT
#  endif

#elif defined(__SWPM__)

#  ifdef SWMAKINGDLL
#    define SWDLLEXPORT _Export
#  elif defined(SWUSINGDLL)
#    define SWDLLEXPORT _Export
#  else
#    define SWDLLEXPORT
#  endif

#elif defined(__GNUWIN32__)

#  ifdef SWMAKINGDLL
#    define SWDLLEXPORT __declspec( dllexport )
#  elif defined(SWUSINGDLL)
#    define SWDLLEXPORT __declspec( dllimport )
#  else
#    define SWDLLEXPORT
#  endif

#elif defined(__GNUC__)

#  define SWDLLEXPORT

#else
#  define SWDLLEXPORT
#endif


enum SWTextDirection: char { DIRECTION_LTR = 0, DIRECTION_RTL, DIRECTION_BIDI };
enum SWTextMarkup: char {
    FMT_UNKNOWN = 0,
    FMT_PLAIN,
    FMT_THML,
    FMT_GBF,
    FMT_HTML,
    FMT_HTMLHREF,
    FMT_OSIS,
    FMT_WEBIF,
    FMT_TEI,
    FMT_XHTML
};

enum TextEncoding: char {
    ENC_UNKNOWN = 0,
    ENC_LATIN1,
    ENC_UTF8,
    ENC_SCSU,
    ENC_UTF16,
    ENC_HTML
};

enum BlockType: char {
    VERSEBLOCKS = 'v',
    CHAPTERBLOCKS = 'c',
    BOOKBLOCKS = 'b'
};

} /* namespace swordxx */

#endif /* SWORDXX_DEFS_H */
