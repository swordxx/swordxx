/******************************************************************************
 *
 *  utf8html.cpp -    SWFilter descendant to convert a UTF-8 stream to
 *            HTML escapes
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

#include "utf8html.h"

#include <cstdio>
#include <string>


namespace swordxx {


UTF8HTML::UTF8HTML() {
}


char UTF8HTML::processText(std::string &text, const SWKey *key, const SWModule *module)
{
    (void) module;
        char digit[10];
        unsigned long ch;
     if ((unsigned long)key < 2)    // hack, we're en(1)/de(0)ciphering
        return (char)-1;

    std::string tmp(text);
    std::string out;

    // -------------------------------
    for (auto from = reinterpret_cast<unsigned char *>(tmp.data()); *from; from++) {
      ch = 0;
          if ((*from & 128) != 128) {
//              if (*from != ' ')
           out += *from;
               continue;
          }
          if ((*from & 128) && ((*from & 64) != 64)) {
        // error
               *from = 'x';
               continue;
          }
          *from <<= 1;
          int subsequent;
          for (subsequent = 1; (*from & 128); subsequent++) {
              *from <<= 1;
               from[subsequent] &= 63;
               ch <<= 6;
               ch |= from[subsequent];
          }
          subsequent--;
          *from <<=1;
          char significantFirstBits = 8 - (2+subsequent);

          ch |= (((short)*from) << (((6*subsequent)+significantFirstBits)-8));
          from += subsequent;
          out += '&';
          out += '#';
      sprintf(digit, "%lu", ch);
        for (char *dig = digit; *dig; dig++)
            out += *dig;
        out += ';';
    }
    text = std::move(out);
    return 0;
}


} /* namespace swordxx */

