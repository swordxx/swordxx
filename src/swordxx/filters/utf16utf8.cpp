/******************************************************************************
 *
 * utf16utf8.cpp -    SWFilter descendant to convert UTF-16 to UTF-8
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "utf16utf8.h"

#include <string>


namespace swordxx {


UTF16UTF8::UTF16UTF8() {
}


char UTF16UTF8::processText(std::string &text, const SWKey *key, const SWModule *module)
{
  (void) key;
  (void) module;

  int len;
  unsigned long uchar;
  unsigned short schar;
  len = 0;
  for (auto const * from = reinterpret_cast<unsigned short const *>(text.c_str()); *from; from++) {
        len += 2;
  }

    std::string out;


  // -------------------------------

  for (auto const * from = reinterpret_cast<unsigned short const *>(text.c_str()); *from; from++) {
    uchar = 0;

    if (*from < 0xD800 || *from > 0xDFFF) {
      uchar = *from;
    }
    else if (*from >= 0xD800 && *from <= 0xDBFF) {
      uchar = *from;
      schar = *(from+1);
      if (uchar < 0xDC00 || uchar > 0xDFFF) {
    //error, do nothing
    continue;
      }
      uchar &= 0x03ff;
      schar &= 0x03ff;
      uchar <<= 10;
      uchar |= schar;
      uchar += 0x10000;
      from++;
    }
    else {
      //error, do nothing
      continue;
    }

    if (uchar < 0x80) {
      out += uchar;
    }
    else if (uchar < 0x800) {
      out += 0xc0 | (uchar >> 6);
      out += 0x80 | (uchar & 0x3f);
    }
    else if (uchar < 0x10000) {
      out += 0xe0 | (uchar >> 12);
      out += 0x80 | ((uchar >> 6) & 0x3f);
      out += 0x80 | (uchar & 0x3f);
    }
    else if (uchar < 0x200000) {
      out += 0xF0 | (uchar >> 18);
      out += 0x80 | ((uchar >> 12) & 0x3F);
      out += 0x80 | ((uchar >> 6) & 0x3F);
      out += 0x80 | (uchar & 0x3F);
    }
  }
  text = std::move(out);
  return 0;
}


} /* namespace swordxx */
