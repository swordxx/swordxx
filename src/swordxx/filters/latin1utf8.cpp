/******************************************************************************
 *
 *  latin1utf8.cpp -    SWFilter descendant Latin1UTF8 to convert a Latin-1
 *            character to UTF-8
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

#include "latin1utf8.h"

#include <cstdlib>
#include <cstdio>
#include "../swmodule.h"


namespace swordxx {


Latin1UTF8::Latin1UTF8() {
}


char Latin1UTF8::processText(std::string &text, const SWKey *key, const SWModule *module)
{
    (void) module;

    if ((unsigned long)key < 2)    // hack, we're en(1)/de(0)ciphering
        return (char)-1;

    std::string out;

    for (auto const * from = reinterpret_cast<unsigned char const *>(text.c_str()); *from; from++) {
      if (*from < 0x80) {
        out += *from;
      }
      else if (*from < 0xc0) {
                switch(*from) {
            case 0x80: // '€'
                out += 0xe2; // 'â'
                out += 0x82; // '‚'
                out += 0xac; // '¬'
                break;
            case 0x82: // '‚'
                out += 0xe2; // 'â'
                out += 0x80; // '€'
                out += 0x9a; // 'š'
                break;
            case 0x83: // 'ƒ'
                out += 0xc6; // 'Æ'
                out += 0x92; // '’'
                break;
            case 0x84: // '„'
                out += 0xe2; // 'â'
                out += 0x80; // '€'
                out += 0x9e; // 'ž'
                break;
            case 0x85: // '…'
                out += 0xe2; // 'â'
                out += 0x80; // '€'
                out += 0xa6; // '¦'
                break;
            case 0x86: // '†'
                out += 0xe2; // 'â'
                out += 0x80; // '€'
                out += 0xa0; // ' '
                break;
            case 0x87: // '‡'
                out += 0xe2; // 'â'
                out += 0x80; // '€'
                out += 0xa1; // '¡'
                break;
            case 0x88: // 'ˆ'
                out += 0xcb; // 'Ë'
                out += 0x86; // '†'
                break;
            case 0x89: // '‰'
                out += 0xe2; // 'â'
                out += 0x80; // '€'
                out += 0xb0; // '°'
                break;
            case 0x8A: // 'Š'
                out += 0xc5; // 'Å'
                out += 0xa0; // ' '
                break;
            case 0x8B: // '‹'
                out += 0xe2; // 'â'
                out += 0x80; // '€'
                out += 0xb9; // '¹'
                break;
            case 0x8C: // 'Œ'
                out += 0xc5; // 'Å'
                out += 0x92; // '’'
                break;
            case 0x8E: // 'Ž'
                out += 0xc5; // 'Å'
                out += 0xbd; // '½'
                break;
            case 0x91: // '‘'
                out += 0xe2; // 'â'
                out += 0x80; // '€'
                out += 0x98; // '˜'
                break;
            case 0x92: // '’'
                out += 0xe2; // 'â'
                out += 0x80; // '€'
                out += 0x99; // '™'
                break;
            case 0x93: // '“'
                out += 0xe2; // 'â'
                out += 0x80; // '€'
                out += 0x9c; // 'œ'
                break;
            case 0x94: // '”'
                out += 0xe2; // 'â'
                out += 0x80; // '€'
                out += 0x9d; // ''
                break;
            case 0x95: // '•'
                out += 0xe2; // 'â'
                out += 0x80; // '€'
                out += 0xa2; // '¢'
                break;
            case 0x96: // '–'
                out += 0xe2; // 'â'
                out += 0x80; // '€'
                out += 0x93; // '“'
                break;
            case 0x97: // '—'
                out += 0xe2; // 'â'
                out += 0x80; // '€'
                out += 0x94; // '”'
                break;
            case 0x98: // '˜'
                out += 0xcb; // 'Ë'
                out += 0x9c; // 'œ'
                break;
            case 0x99: // '™'
                out += 0xe2; // 'â'
                out += 0x84; // '„'
                out += 0xa2; // '¢'
                break;
            case 0x9A: // 'š'
                out += 0xc5; // 'Å'
                out += 0xa1; // '¡'
                break;
            case 0x9B: // '›'
                out += 0xe2; // 'â'
                out += 0x80; // '€'
                out += 0xba; // 'º'
                break;
            case 0x9C: // 'œ'
                out += 0xc5; // 'Å'
                out += 0x93; // '“'
                break;
            case 0x9E: // 'ž'
                out += 0xc5; // 'Å'
                out += 0xbe; // '¾'
                break;
            case 0x9F: // 'Ÿ'
                out += 0xc5; // 'Å'
                out += 0xb8; // '¸'
                break;
                default:
                        out += 0xC2;
                        out += *from;
                }
      }
      else {
        out += 0xC3;
        out += (*from - 0x40);
      }
    }
    text = std::move(out);
    return 0;
}


} /* namespace swordxx */
