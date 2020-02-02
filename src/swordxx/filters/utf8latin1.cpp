/******************************************************************************
 *
 *  utf8latin1.cpp -    SWFilter descendant to convert UTF-8 to Latin-1
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

#include "utf8latin1.h"

#include <string>


namespace swordxx {


UTF8Latin1::UTF8Latin1(char rchar) : replacementChar(rchar) {
}


char UTF8Latin1::processText(std::string &text, const SWKey *key, const SWModule *module)
{
  (void) module;

  unsigned long uchar;
  unsigned char significantFirstBits, subsequent;

  if ((unsigned long)key < 2) {// hack, we're en(1)/de(0)ciphering
    return (char)-1;
  }

  std::string tmp(text);
  std::string out;
  // -------------------------------

  for (auto from = reinterpret_cast<unsigned char *>(tmp.data()); *from; from++) {
    uchar = 0;
    if ((*from & 128) != 128) {
      //              if (*from != ' ')
      uchar = *from;
    }
    else if ((*from & 128) && ((*from & 64) != 64)) {
      // error, do nothing
      continue;
    }
    else {
      *from <<= 1;
      for (subsequent = 1; (*from & 128); subsequent++) {
    *from <<= 1;
    from[subsequent] &= 63;
    uchar <<= 6;
    uchar |= from[subsequent];
      }
      subsequent--;
      *from <<=1;
      significantFirstBits = 8 - (2+subsequent);

      uchar |= (((short)*from) << (((6*subsequent)+significantFirstBits)-8));
      from += subsequent;
    }

    if (uchar < 0xff) {
        out += (unsigned char)uchar;
    }
    else {
        out += replacementChar;
    }
  }
  text = std::move(out);
  return 0;
}


} /* namespace swordxx */

