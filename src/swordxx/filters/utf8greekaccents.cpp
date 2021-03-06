/******************************************************************************
 *
 *  utf8greekaccents.cpp -    SWFilter descendant to remove UTF-8 Greek
 *                accents
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

#include "utf8greekaccents.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <unordered_map>
#include <vector>
#include "../unicode.h"


namespace swordxx {

namespace {

static const char oName[] = "Greek Accents";
static const char oTip[]  = "Toggles Greek Accents";

class Converters: public std::unordered_map<std::uint32_t, std::string> {

public: /* Methods: */

    Converters() {
        //first just remove combining characters
        emplace(0x2019, ""); // RIGHT SINGLE QUOTATION MARK
        emplace(0x1FBF, ""); // GREEK PSILI
        emplace(0x2CFF, ""); // COPTIC MORPHOLOGICAL DIVIDER
        emplace(0xFE24, ""); // COMBINING MACRON LEFT HALF
        emplace(0xFE25, ""); // COMBINING MACRON RIGHT HALF
        emplace(0xFE26, ""); // COMBINING CONJOINING MACRON
        emplace(0x0300, ""); // COMBINING GRAVE ACCENT
        emplace(0x0301, ""); // COMBINING ACUTE ACCENT
        emplace(0x0302, ""); // COMBINING CIRCUMFLEX ACCENT
        emplace(0x0308, ""); // COMBINING DIAERESIS
        emplace(0x0313, ""); // COMBINING COMMA ABOVE
        emplace(0x0314, ""); // COMBINING REVERSED COMMA ABOVE
        emplace(0x037A, ""); // GREEK YPOGEGRAMMENI
        emplace(0x0342, ""); // COMBINING GREEK PERISPOMENI
        emplace(0x1FBD, ""); // GREEK KORONIS
        emplace(0x0343, ""); // COMBINING GREEK KORONIS
        // Now converted pre-composed characters to their alphabetic bases, discarding the accents
        auto const utf8Emplace =
                [this](std::uint32_t value, std::uint32_t replacement)
                { return emplace(value, utf8FromValidCodepoint(replacement)); };
        // Greek
        // UPPER case
        utf8Emplace(0x0386, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH TONOS
        utf8Emplace(0x0388, 0x0395); // GREEK CAPITAL LETTER EPSILON WITH TONOS
        utf8Emplace(0x0389, 0x0397); // GREEK CAPITAL LETTER ETA WITH TONOS
        utf8Emplace(0x038A, 0x0399); // GREEK CAPITAL LETTER IOTA WITH TONOS
        utf8Emplace(0x03AA, 0x0399); // GREEK CAPITAL LETTER IOTA WITH DIALYTIKA
        utf8Emplace(0x038C, 0x039F); // GREEK CAPITAL LETTER OMICRON WITH TONOS
        utf8Emplace(0x038E, 0x03A5); // GREEK CAPITAL LETTER UPSILON WITH TONOS
        utf8Emplace(0x03AB, 0x03A5); // GREEK CAPITAL LETTER UPSILON WITH DIALYTIKA
        utf8Emplace(0x038F, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH TONOS

        // lower case
        utf8Emplace(0x03AC, 0x03B1); // GREEK SMALL LETTER ALPHA WITH TONOS
        utf8Emplace(0x03AD, 0x03B5); // GREEK SMALL LETTER EPSILON WITH TONOS
        utf8Emplace(0x03AE, 0x03B7); // GREEK SMALL LETTER ETA WITH TONOS
        utf8Emplace(0x03AF, 0x03B9); // GREEK SMALL LETTER IOTA WITH TONOS
        utf8Emplace(0x03CA, 0x03B9); // GREEK SMALL LETTER IOTA WITH DIALYTIKA
        utf8Emplace(0x03CC, 0x03BF); // GREEK SMALL LETTER OMICRON WITH TONOS
        utf8Emplace(0x03CD, 0x03C5); // GREEK SMALL LETTER UPSILON WITH TONOS
        utf8Emplace(0x03CB, 0x03C5); // GREEK SMALL LETTER UPSILON WITH DIALYTIKA
        utf8Emplace(0x03CE, 0x03C9); // GREEK SMALL LETTER OMEGA WITH TONOS

        // Extended Greek
        // UPPER case
        utf8Emplace(0x1F08, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH PSILI
        utf8Emplace(0x1F09, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH DASIA
        utf8Emplace(0x1F0A, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA
        utf8Emplace(0x1F0B, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH DASIA AND VARIA
        utf8Emplace(0x1F0C, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH PSILI AND OXIA
        utf8Emplace(0x1F0D, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH DASIA AND OXIA
        utf8Emplace(0x1F0E, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH PSILI AND PERISPOMENI
        utf8Emplace(0x1F0F, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH DASIA AND PERISPOMENI
        utf8Emplace(0x1F88, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH PSILI AND PROSGEGRAMMENI
        utf8Emplace(0x1F89, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH DASIA AND PROSGEGRAMMENI
        utf8Emplace(0x1F8A, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH PSILI AND VARIA AND PROSGEGRAMMENI
        utf8Emplace(0x1F8B, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH DASIA AND VARIA AND PROSGEGRAMMENI
        utf8Emplace(0x1F8C, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH PSILI AND OXIA AND PROSGEGRAMMENI
        utf8Emplace(0x1F8D, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH DASIA AND OXIA AND PROSGEGRAMMENI
        utf8Emplace(0x1F8E, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI
        utf8Emplace(0x1F8F, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI
        utf8Emplace(0x1FB8, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH VRACHY
        utf8Emplace(0x1FB9, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH MACRON
        utf8Emplace(0x1FBA, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH VARIA
        utf8Emplace(0x1FBB, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH OXIA
        utf8Emplace(0x1FBC, 0x0391); // GREEK CAPITAL LETTER ALPHA WITH PROSGEGRAMMENI

        utf8Emplace(0x1F18, 0x0395); // GREEK CAPITAL LETTER EPSILON WITH PSILI
        utf8Emplace(0x1F19, 0x0395); // GREEK CAPITAL LETTER EPSILON WITH DASIA
        utf8Emplace(0x1F1A, 0x0395); // GREEK CAPITAL LETTER EPSILON WITH PSILI AND VARIA
        utf8Emplace(0x1F1B, 0x0395); // GREEK CAPITAL LETTER EPSILON WITH DASIA AND VARIA
        utf8Emplace(0x1F1C, 0x0395); // GREEK CAPITAL LETTER EPSILON WITH PSILI AND OXIA
        utf8Emplace(0x1F1D, 0x0395); // GREEK CAPITAL LETTER EPSILON WITH DASIA AND OXIA
        utf8Emplace(0x1FC8, 0x0395); // GREEK CAPITAL LETTER EPSILON WITH VARIA
        utf8Emplace(0x1FC9, 0x0395); // GREEK CAPITAL LETTER EPSILON WITH OXIA

        utf8Emplace(0x1F28, 0x0397); // GREEK CAPITAL LETTER ETA WITH PSILI
        utf8Emplace(0x1F29, 0x0397); // GREEK CAPITAL LETTER ETA WITH DASIA
        utf8Emplace(0x1F2A, 0x0397); // GREEK CAPITAL LETTER ETA WITH PSILI AND VARIA
        utf8Emplace(0x1F2B, 0x0397); // GREEK CAPITAL LETTER ETA WITH DASIA AND VARIA
        utf8Emplace(0x1F2C, 0x0397); // GREEK CAPITAL LETTER ETA WITH PSILI AND OXIA
        utf8Emplace(0x1F2D, 0x0397); // GREEK CAPITAL LETTER ETA WITH DASIA AND OXIA
        utf8Emplace(0x1F2E, 0x0397); // GREEK CAPITAL LETTER ETA WITH PSILI AND PERISPOMENI
        utf8Emplace(0x1F2F, 0x0397); // GREEK CAPITAL LETTER ETA WITH DASIA AND PERISPOMENI
        utf8Emplace(0x1F98, 0x0397); // GREEK CAPITAL LETTER ETA WITH PSILI AND PROSGEGRAMMENI
        utf8Emplace(0x1F99, 0x0397); // GREEK CAPITAL LETTER ETA WITH DASIA AND PROSGEGRAMMENI
        utf8Emplace(0x1F9A, 0x0397); // GREEK CAPITAL LETTER ETA WITH PSILI AND VARIA AND PROSGEGRAMMENI
        utf8Emplace(0x1F9B, 0x0397); // GREEK CAPITAL LETTER ETA WITH DASIA AND VARIA AND PROSGEGRAMMENI
        utf8Emplace(0x1F9C, 0x0397); // GREEK CAPITAL LETTER ETA WITH PSILI AND OXIA AND PROSGEGRAMMENI
        utf8Emplace(0x1F9D, 0x0397); // GREEK CAPITAL LETTER ETA WITH DASIA AND OXIA AND PROSGEGRAMMENI
        utf8Emplace(0x1F9E, 0x0397); // GREEK CAPITAL LETTER ETA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI
        utf8Emplace(0x1F9F, 0x0397); // GREEK CAPITAL LETTER ETA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI
        utf8Emplace(0x1FCA, 0x0397); // GREEK CAPITAL LETTER ETA WITH VARIA
        utf8Emplace(0x1FCB, 0x0397); // GREEK CAPITAL LETTER ETA WITH OXIA
        utf8Emplace(0x1FCC, 0x0397); // GREEK CAPITAL LETTER ETA WITH PROSGEGRAMMENI

        utf8Emplace(0x1F38, 0x0399); // GREEK CAPITAL LETTER IOTA WITH PSILI
        utf8Emplace(0x1F39, 0x0399); // GREEK CAPITAL LETTER IOTA WITH DASIA
        utf8Emplace(0x1F3A, 0x0399); // GREEK CAPITAL LETTER IOTA WITH PSILI AND VARIA
        utf8Emplace(0x1F3B, 0x0399); // GREEK CAPITAL LETTER IOTA WITH DASIA AND VARIA
        utf8Emplace(0x1F3C, 0x0399); // GREEK CAPITAL LETTER IOTA WITH PSILI AND OXIA
        utf8Emplace(0x1F3D, 0x0399); // GREEK CAPITAL LETTER IOTA WITH DASIA AND OXIA
        utf8Emplace(0x1F3E, 0x0399); // GREEK CAPITAL LETTER IOTA WITH PSILI AND PERISPOMENI
        utf8Emplace(0x1F3F, 0x0399); // GREEK CAPITAL LETTER IOTA WITH DASIA AND PERISPOMENI
        utf8Emplace(0x1FD8, 0x0399); // GREEK CAPITAL LETTER IOTA WITH VRACHY
        utf8Emplace(0x1FD9, 0x0399); // GREEK CAPITAL LETTER IOTA WITH MACRON
        utf8Emplace(0x1FDA, 0x0399); // GREEK CAPITAL LETTER IOTA WITH VARIA
        utf8Emplace(0x1FDB, 0x0399); // GREEK CAPITAL LETTER IOTA WITH OXIA

        utf8Emplace(0x1F48, 0x039F); // GREEK CAPITAL LETTER OMICRON WITH PSILI
        utf8Emplace(0x1F49, 0x039F); // GREEK CAPITAL LETTER OMICRON WITH DASIA
        utf8Emplace(0x1F4A, 0x039F); // GREEK CAPITAL LETTER OMICRON WITH PSILI AND VARIA
        utf8Emplace(0x1F4B, 0x039F); // GREEK CAPITAL LETTER OMICRON WITH DASIA AND VARIA
        utf8Emplace(0x1F4C, 0x039F); // GREEK CAPITAL LETTER OMICRON WITH PSILI AND OXIA
        utf8Emplace(0x1F4D, 0x039F); // GREEK CAPITAL LETTER OMICRON WITH DASIA AND OXIA
        utf8Emplace(0x1FF8, 0x039F); // GREEK CAPITAL LETTER OMICRON WITH VARIA
        utf8Emplace(0x1FF9, 0x039F); // GREEK CAPITAL LETTER OMICRON WITH OXIA

        utf8Emplace(0x1F59, 0x03A5); // GREEK CAPITAL LETTER UPSILON WITH DASIA
        utf8Emplace(0x1F5A, 0x03A5); // GREEK CAPITAL LETTER UPSILON WITH PSILI AND VARIA
        utf8Emplace(0x1F5B, 0x03A5); // GREEK CAPITAL LETTER UPSILON WITH DASIA AND VARIA
        utf8Emplace(0x1F5C, 0x03A5); // GREEK CAPITAL LETTER UPSILON WITH PSILI AND OXIA
        utf8Emplace(0x1F5D, 0x03A5); // GREEK CAPITAL LETTER UPSILON WITH DASIA AND OXIA
        utf8Emplace(0x1F5E, 0x03A5); // GREEK CAPITAL LETTER UPSILON WITH PSILI AND PERISPOMENI
        utf8Emplace(0x1F5F, 0x03A5); // GREEK CAPITAL LETTER UPSILON WITH DASIA AND PERISPOMENI
        utf8Emplace(0x1FE8, 0x03A5); // GREEK CAPITAL LETTER UPSILON WITH VRACHY
        utf8Emplace(0x1FE9, 0x03A5); // GREEK CAPITAL LETTER UPSILON WITH MACRON
        utf8Emplace(0x1FEA, 0x03A5); // GREEK CAPITAL LETTER UPSILON WITH VARIA
        utf8Emplace(0x1FEB, 0x03A5); // GREEK CAPITAL LETTER UPSILON WITH OXIA

        utf8Emplace(0x1F68, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH PSILI
        utf8Emplace(0x1F69, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH DASIA
        utf8Emplace(0x1F6A, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH PSILI AND VARIA
        utf8Emplace(0x1F6B, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH DASIA AND VARIA
        utf8Emplace(0x1F6C, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH PSILI AND OXIA
        utf8Emplace(0x1F6D, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH DASIA AND OXIA
        utf8Emplace(0x1F6E, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH PSILI AND PERISPOMENI
        utf8Emplace(0x1F6F, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH DASIA AND PERISPOMENI
        utf8Emplace(0x1FA8, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH PSILI AND PROSGEGRAMMENI
        utf8Emplace(0x1FA9, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH DASIA AND PROSGEGRAMMENI
        utf8Emplace(0x1FAA, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH PSILI AND VARIA AND PROSGEGRAMMENI
        utf8Emplace(0x1FAB, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH DASIA AND VARIA AND PROSGEGRAMMENI
        utf8Emplace(0x1FAC, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH PSILI AND OXIA AND PROSGEGRAMMENI
        utf8Emplace(0x1FAD, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH DASIA AND OXIA AND PROSGEGRAMMENI
        utf8Emplace(0x1FAE, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH PSILI AND PERISPOMENI AND PROSGEGRAMMENI
        utf8Emplace(0x1FAF, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH DASIA AND PERISPOMENI AND PROSGEGRAMMENI
        utf8Emplace(0x1FFA, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH VARIA
        utf8Emplace(0x1FFB, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH OXIA
        utf8Emplace(0x1FFC, 0x03A9); // GREEK CAPITAL LETTER OMEGA WITH PROSGEGRAMMENI

        utf8Emplace(0x1FEC, 0x03A1); // GREEK CAPITAL LETTER RHO WITH DASIA

        // lower case
        //alpha
        utf8Emplace(0x1F00, 0x03B1); // GREEK SMALL LETTER ALPHA WITH PSILI
        utf8Emplace(0x1F01, 0x03B1); // GREEK SMALL LETTER ALPHA WITH DASIA
        utf8Emplace(0x1F02, 0x03B1); // GREEK SMALL LETTER ALPHA WITH PSILI AND VARIA
        utf8Emplace(0x1F03, 0x03B1); // GREEK SMALL LETTER ALPHA WITH DASIA AND VARIA
        utf8Emplace(0x1F04, 0x03B1); // GREEK SMALL LETTER ALPHA WITH PSILI AND OXIA
        utf8Emplace(0x1F05, 0x03B1); // GREEK SMALL LETTER ALPHA WITH DASIA AND OXIA
        utf8Emplace(0x1F06, 0x03B1); // GREEK SMALL LETTER ALPHA WITH PSILI AND PERISPOMENI
        utf8Emplace(0x1F07, 0x03B1); // GREEK SMALL LETTER ALPHA WITH DASIA AND PERISPOMENI
        utf8Emplace(0x1F80, 0x03B1); // GREEK SMALL LETTER ALPHA WITH PSILI AND YPOGEGRAMMENI
        utf8Emplace(0x1F81, 0x03B1); // GREEK SMALL LETTER ALPHA WITH DASIA AND YPOGEGRAMMENI
        utf8Emplace(0x1F82, 0x03B1); // GREEK SMALL LETTER ALPHA WITH PSILI AND VARIA AND YPOGEGRAMMENI
        utf8Emplace(0x1F83, 0x03B1); // GREEK SMALL LETTER ALPHA WITH DASIA AND VARIA AND YPOGEGRAMMENI
        utf8Emplace(0x1F84, 0x03B1); // GREEK SMALL LETTER ALPHA WITH PSILI AND OXIA AND YPOGEGRAMMENI
        utf8Emplace(0x1F85, 0x03B1); // GREEK SMALL LETTER ALPHA WITH DASIA AND OXIA AND YPOGEGRAMMENI
        utf8Emplace(0x1F86, 0x03B1); // GREEK SMALL LETTER ALPHA WITH PSILI AND PERISPOMENI AND YPOGEGRAMMENI
        utf8Emplace(0x1F87, 0x03B1); // GREEK SMALL LETTER ALPHA WITH DASIA AND PERISPOMENI AND YPOGEGRAMMENI
        utf8Emplace(0x1F70, 0x03B1); // GREEK SMALL LETTER ALPHA WITH VARIA
        utf8Emplace(0x1F71, 0x03B1); // GREEK SMALL LETTER ALPHA WITH OXIA
        utf8Emplace(0x1FB0, 0x03B1); // GREEK SMALL LETTER ALPHA WITH VRACHY
        utf8Emplace(0x1FB1, 0x03B1); // GREEK SMALL LETTER ALPHA WITH MACRON
        utf8Emplace(0x1FB2, 0x03B1); // GREEK SMALL LETTER ALPHA WITH VARIA AND YPOGEGRAMMENI
        utf8Emplace(0x1FB3, 0x03B1); // GREEK SMALL LETTER ALPHA WITH YPOGEGRAMMENI
        utf8Emplace(0x1FB4, 0x03B1); // GREEK SMALL LETTER ALPHA WITH OXIA AND YPOGEGRAMMENI
        utf8Emplace(0x1FB5, 0x03B1); // unused?
        utf8Emplace(0x1FB6, 0x03B1); // GREEK SMALL LETTER ALPHA WITH PERISPOMENI
        utf8Emplace(0x1FB7, 0x03B1); // GREEK SMALL LETTER ALPHA WITH PERISPOMENI AND YPOGEGRAMMENI

        utf8Emplace(0x1F10, 0x03B5); // GREEK SMALL LETTER EPSILON WITH PSILI
        utf8Emplace(0x1F11, 0x03B5); // GREEK SMALL LETTER EPSILON WITH DASIA
        utf8Emplace(0x1F12, 0x03B5); // GREEK SMALL LETTER EPSILON WITH PSILI AND VARIA
        utf8Emplace(0x1F13, 0x03B5); // GREEK SMALL LETTER EPSILON WITH DASIA AND VARIA
        utf8Emplace(0x1F14, 0x03B5); // GREEK SMALL LETTER EPSILON WITH PSILI AND OXIA
        utf8Emplace(0x1F15, 0x03B5); // GREEK SMALL LETTER EPSILON WITH DASIA AND OXIA
        utf8Emplace(0x1F72, 0x03B5); // GREEK SMALL LETTER EPSILON WITH VARIA
        utf8Emplace(0x1F73, 0x03B5); // GREEK SMALL LETTER EPSILON WITH OXIA

        utf8Emplace(0x1F90, 0x03B7); // GREEK SMALL LETTER ETA WITH PSILI AND YPOGEGRAMMENI
        utf8Emplace(0x1F91, 0x03B7); // GREEK SMALL LETTER ETA WITH DASIA AND YPOGEGRAMMENI
        utf8Emplace(0x1F92, 0x03B7); // GREEK SMALL LETTER ETA WITH PSILI AND VARIA AND YPOGEGRAMMENI
        utf8Emplace(0x1F93, 0x03B7); // GREEK SMALL LETTER ETA WITH DASIA AND VARIA AND YPOGEGRAMMENI
        utf8Emplace(0x1F94, 0x03B7); // GREEK SMALL LETTER ETA WITH PSILI AND OXIA AND YPOGEGRAMMENI
        utf8Emplace(0x1F95, 0x03B7); // GREEK SMALL LETTER ETA WITH DASIA AND OXIA AND YPOGEGRAMMENI
        utf8Emplace(0x1F96, 0x03B7); // GREEK SMALL LETTER ETA WITH PSILI AND PERISPOMENI AND YPOGEGRAMMENI
        utf8Emplace(0x1F97, 0x03B7); // GREEK SMALL LETTER ETA WITH DASIA AND PERISPOMENI AND YPOGEGRAMMENI
        utf8Emplace(0x1F20, 0x03B7); // GREEK SMALL LETTER ETA WITH PSILI
        utf8Emplace(0x1F21, 0x03B7); // GREEK SMALL LETTER ETA WITH DASIA
        utf8Emplace(0x1F22, 0x03B7); // GREEK SMALL LETTER ETA WITH PSILI AND VARIA
        utf8Emplace(0x1F23, 0x03B7); // GREEK SMALL LETTER ETA WITH DASIA AND VARIA
        utf8Emplace(0x1F24, 0x03B7); // GREEK SMALL LETTER ETA WITH PSILI AND OXIA
        utf8Emplace(0x1F25, 0x03B7); // GREEK SMALL LETTER ETA WITH DASIA AND OXIA
        utf8Emplace(0x1F26, 0x03B7); // GREEK SMALL LETTER ETA WITH PSILI AND PERISPOMENI
        utf8Emplace(0x1F27, 0x03B7); // GREEK SMALL LETTER ETA WITH DASIA AND PERISPOMENI
        utf8Emplace(0x1FC2, 0x03B7); // GREEK SMALL LETTER ETA WITH VARIA AND YPOGEGRAMMENI
        utf8Emplace(0x1FC3, 0x03B7); // GREEK SMALL LETTER ETA WITH YPOGEGRAMMENI
        utf8Emplace(0x1FC4, 0x03B7); // GREEK SMALL LETTER ETA WITH OXIA AND YPOGEGRAMMENI
        utf8Emplace(0x1FC5, 0x03B7); // unused?
        utf8Emplace(0x1FC6, 0x03B7); // GREEK SMALL LETTER ETA WITH PERISPOMENI
        utf8Emplace(0x1FC7, 0x03B7); // GREEK SMALL LETTER ETA WITH PERISPOMENI AND YPOGEGRAMMENI
        utf8Emplace(0x1F74, 0x03B7); // GREEK SMALL LETTER ETA WITH VARIA
        utf8Emplace(0x1F75, 0x03B7); // GREEK SMALL LETTER ETA WITH OXIA

        utf8Emplace(0x1F30, 0x03B9); // GREEK SMALL LETTER IOTA WITH PSILI
        utf8Emplace(0x1F31, 0x03B9); // GREEK SMALL LETTER IOTA WITH DASIA
        utf8Emplace(0x1F32, 0x03B9); // GREEK SMALL LETTER IOTA WITH PSILI AND VARIA
        utf8Emplace(0x1F33, 0x03B9); // GREEK SMALL LETTER IOTA WITH DASIA AND VARIA
        utf8Emplace(0x1F34, 0x03B9); // GREEK SMALL LETTER IOTA WITH PSILI AND OXIA
        utf8Emplace(0x1F35, 0x03B9); // GREEK SMALL LETTER IOTA WITH DASIA AND OXIA
        utf8Emplace(0x1F36, 0x03B9); // GREEK SMALL LETTER IOTA WITH PSILI AND PERISPOMENI
        utf8Emplace(0x1F37, 0x03B9); // GREEK SMALL LETTER IOTA WITH DASIA AND PERISPOMENI
        utf8Emplace(0x1F76, 0x03B9); // GREEK SMALL LETTER IOTA WITH VARIA
        utf8Emplace(0x1F77, 0x03B9); // GREEK SMALL LETTER IOTA WITH OXIA
        utf8Emplace(0x1FD0, 0x03B9); // GREEK SMALL LETTER IOTA WITH VRACHY
        utf8Emplace(0x1FD1, 0x03B9); // GREEK SMALL LETTER IOTA WITH MACRON
        utf8Emplace(0x1FD2, 0x03B9); // GREEK SMALL LETTER IOTA WITH DIALYTIKA AND VARIA
        utf8Emplace(0x1FD3, 0x03B9); // GREEK SMALL LETTER IOTA WITH DIALYTIKA AND OXIA
        utf8Emplace(0x1FD4, 0x03B9); // unused?
        utf8Emplace(0x1FD5, 0x03B9); // unused?
        utf8Emplace(0x1FD6, 0x03B9); // GREEK SMALL LETTER IOTA WITH PERISPOMENI
        utf8Emplace(0x1FD7, 0x03B9); // GREEK SMALL LETTER IOTA WITH DIALYTIKA AND PERISPOMENI

        utf8Emplace(0x1F40, 0x03BF); // GREEK SMALL LETTER OMICRON WITH PSILI
        utf8Emplace(0x1F41, 0x03BF); // GREEK SMALL LETTER OMICRON WITH DASIA
        utf8Emplace(0x1F42, 0x03BF); // GREEK SMALL LETTER OMICRON WITH PSILI AND VARIA
        utf8Emplace(0x1F43, 0x03BF); // GREEK SMALL LETTER OMICRON WITH DASIA AND VARIA
        utf8Emplace(0x1F44, 0x03BF); // GREEK SMALL LETTER OMICRON WITH PSILI AND OXIA
        utf8Emplace(0x1F45, 0x03BF); // GREEK SMALL LETTER OMICRON WITH DASIA AND OXIA
        utf8Emplace(0x1F78, 0x03BF); // GREEK SMALL LETTER OMICRON WITH VARIA
        utf8Emplace(0x1F79, 0x03BF); // GREEK SMALL LETTER OMICRON WITH OXIA

        utf8Emplace(0x1F50, 0x03C5); // GREEK SMALL LETTER UPSILON WITH PSILI
        utf8Emplace(0x1F51, 0x03C5); // GREEK SMALL LETTER UPSILON WITH DASIA
        utf8Emplace(0x1F52, 0x03C5); // GREEK SMALL LETTER UPSILON WITH PSILI AND VARIA
        utf8Emplace(0x1F53, 0x03C5); // GREEK SMALL LETTER UPSILON WITH DASIA AND VARIA
        utf8Emplace(0x1F54, 0x03C5); // GREEK SMALL LETTER UPSILON WITH PSILI AND OXIA
        utf8Emplace(0x1F55, 0x03C5); // GREEK SMALL LETTER UPSILON WITH DASIA AND OXIA
        utf8Emplace(0x1F56, 0x03C5); // GREEK SMALL LETTER UPSILON WITH PSILI AND PERISPOMENI
        utf8Emplace(0x1F57, 0x03C5); // GREEK SMALL LETTER UPSILON WITH DASIA AND PERISPOMENI
        utf8Emplace(0x1F7A, 0x03C5); // GREEK SMALL LETTER UPSILON WITH VARIA
        utf8Emplace(0x1F7B, 0x03C5); // GREEK SMALL LETTER UPSILON WITH OXIA
        utf8Emplace(0x1FE0, 0x03C5); // GREEK SMALL LETTER UPSILON WITH VRACHY
        utf8Emplace(0x1FE1, 0x03C5); // GREEK SMALL LETTER UPSILON WITH MACRON
        utf8Emplace(0x1FE2, 0x03C5); // GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND VARIA
        utf8Emplace(0x1FE3, 0x03C5); // GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND OXIA
        utf8Emplace(0x1FE6, 0x03C5); // GREEK SMALL LETTER UPSILON WITH PERISPOMENI
        utf8Emplace(0x1FE7, 0x03C5); // GREEK SMALL LETTER UPSILON WITH DIALYTIKA AND PERISPOMENI

        utf8Emplace(0x1F60, 0x03C9); // GREEK SMALL LETTER OMEGA WITH PSILI
        utf8Emplace(0x1F61, 0x03C9); // GREEK SMALL LETTER OMEGA WITH DASIA
        utf8Emplace(0x1F62, 0x03C9); // GREEK SMALL LETTER OMEGA WITH PSILI AND VARIA
        utf8Emplace(0x1F63, 0x03C9); // GREEK SMALL LETTER OMEGA WITH DASIA AND VARIA
        utf8Emplace(0x1F64, 0x03C9); // GREEK SMALL LETTER OMEGA WITH PSILI AND OXIA
        utf8Emplace(0x1F65, 0x03C9); // GREEK SMALL LETTER OMEGA WITH DASIA AND OXIA
        utf8Emplace(0x1F66, 0x03C9); // GREEK SMALL LETTER OMEGA WITH PSILI AND PERISPOMENI
        utf8Emplace(0x1F67, 0x03C9); // GREEK SMALL LETTER OMEGA WITH DASIA AND PERISPOMENI
        utf8Emplace(0x1F7C, 0x03C9); // GREEK SMALL LETTER OMEGA WITH VARIA
        utf8Emplace(0x1F7D, 0x03C9); // GREEK SMALL LETTER OMEGA WITH OXIA
        utf8Emplace(0x1FA0, 0x03C9); // GREEK SMALL LETTER OMEGA WITH PSILI AND YPOGEGRAMMENI
        utf8Emplace(0x1FA1, 0x03C9); // GREEK SMALL LETTER OMEGA WITH DASIA AND YPOGEGRAMMENI
        utf8Emplace(0x1FA2, 0x03C9); // GREEK SMALL LETTER OMEGA WITH PSILI AND VARIA AND YPOGEGRAMMENI
        utf8Emplace(0x1FA3, 0x03C9); // GREEK SMALL LETTER OMEGA WITH DASIA AND VARIA AND YPOGEGRAMMENI
        utf8Emplace(0x1FA4, 0x03C9); // GREEK SMALL LETTER OMEGA WITH PSILI AND OXIA AND YPOGEGRAMMENI
        utf8Emplace(0x1FA5, 0x03C9); // GREEK SMALL LETTER OMEGA WITH DASIA AND OXIA AND YPOGEGRAMMENI
        utf8Emplace(0x1FA6, 0x03C9); // GREEK SMALL LETTER OMEGA WITH PSILI AND PERISPOMENI AND YPOGEGRAMMENI
        utf8Emplace(0x1FA7, 0x03C9); // GREEK SMALL LETTER OMEGA WITH DASIA AND PERISPOMENI AND YPOGEGRAMMENI
        utf8Emplace(0x1FF2, 0x03C9); // GREEK SMALL LETTER OMEGA WITH VARIA AND YPOGEGRAMMENI
        utf8Emplace(0x1FF3, 0x03C9); // GREEK SMALL LETTER OMEGA WITH YPOGEGRAMMENI
        utf8Emplace(0x1FF4, 0x03C9); // GREEK SMALL LETTER OMEGA WITH OXIA AND YPOGEGRAMMENI
        utf8Emplace(0x1FF5, 0x03C9); // unused?
        utf8Emplace(0x1FF6, 0x03C9); // GREEK SMALL LETTER OMEGA WITH PERISPOMENI
        utf8Emplace(0x1FF7, 0x03C9); // GREEK SMALL LETTER OMEGA WITH PERISPOMENI AND YPOGEGRAMMENI

        utf8Emplace(0x1FE4, 0x03C1); // GREEK SMALL LETTER RHO WITH PSILI
        utf8Emplace(0x1FE5, 0x03C1); // GREEK SMALL LETTER RHO WITH DASIA
    }
}; /* class Converters { */

} // anonymous namespace

UTF8GreekAccents::UTF8GreekAccents()
    : OnOffOptionFilter(oName, oTip)
{}

UTF8GreekAccents::~UTF8GreekAccents() {};


char UTF8GreekAccents::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    (void) module;
    static Converters const converters;

    if (isOptionOn())
        return 0; // we don't want greek accents

    if (!text.empty()) {
        std::string_view sv(text);
        std::string result;
        do {
            auto const r(codepointFromValidUtf8(sv));
            if (r.second) {
                sv.remove_prefix(r.second);
                auto const it = converters.find(r.first);
                if (it == converters.end()) {
                    result.append(utf8FromValidCodepoint(r.first));
                } else {
                    result.append(it->second);
                }
            } else {
                assert(!r.first);
                result.append("\ufffd"); // Unicode replacement character
                sv.remove_prefix(1u);
            }
        } while (!sv.empty());
        text = std::move(result);
    }
    return 0;
}

} /* namespace swordxx */
