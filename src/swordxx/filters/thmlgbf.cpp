/***************************************************************************
 *
 *  thmlgbf.cpp -    ThML to GBF filter
 *
 * Copyright 1999-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "thmlgbf.h"

#include <cstdlib>
#include <cstring>
#include <string>
#include "../utilstr.h"


namespace swordxx {

ThMLGBF::ThMLGBF()
{
}


char ThMLGBF::processText(std::string &text, const SWKey *key, const SWModule *module) {
    using namespace std::literals::string_view_literals;

    (void) key;
    (void) module;
    char token[2048];
    int tokpos = 0;
    bool intoken     = false;
    bool ampersand = false;
    bool sechead = false;
    bool title = false;

    std::string out;

    for (auto const * from = text.c_str(); *from; from++) {
        if (*from == '<') {
            intoken = true;
            tokpos = 0;
            token[0] = 0;
            token[1] = 0;
            token[2] = 0;
            ampersand = false;
            continue;
        }
        else if (*from == '&') {
            intoken = true;
            tokpos = 0;
            std::memset(token, 0, 2048);
            ampersand = true;
            continue;
        }
        if (*from == ';' && ampersand) {
            intoken = false;

            if (startsWith(token, "nbsp"sv)) out += ' ';
            else if (startsWith(token, "quot"sv)) out += '"';
            else if (startsWith(token, "amp"sv)) out += '&';
            else if (startsWith(token, "lt"sv)) out += '<';
            else if (startsWith(token, "gt"sv)) out += '>';
            else if (startsWith(token, "brvbar"sv)) out += '\xA6';
            else if (startsWith(token, "sect"sv)) out += '\xA7';
            else if (startsWith(token, "copy"sv)) out += '\xA9';
            else if (startsWith(token, "laquo"sv)) out += '\xAB';
            else if (startsWith(token, "reg"sv)) out += '\xAE';
            else if (startsWith(token, "acute"sv)) out += '\xB4';
            else if (startsWith(token, "para"sv)) out += '\xB6';
            else if (startsWith(token, "raquo"sv)) out += '\xBB';

            else if (startsWith(token, "Aacute"sv)) out += '\xC1';
            else if (startsWith(token, "Agrave"sv)) out += '\xC0';
            else if (startsWith(token, "Acirc"sv)) out += '\xC2';
            else if (startsWith(token, "Auml"sv)) out += '\xC4';
            else if (startsWith(token, "Atilde"sv)) out += '\xC3';
            else if (startsWith(token, "Aring"sv)) out += '\xC5';
            else if (startsWith(token, "aacute"sv)) out += '\xE1';
            else if (startsWith(token, "agrave"sv)) out += '\xE0';
            else if (startsWith(token, "acirc"sv)) out += '\xE2';
            else if (startsWith(token, "auml"sv)) out += '\xE4';
            else if (startsWith(token, "atilde"sv)) out += '\xE3';
            else if (startsWith(token, "aring"sv)) out += '\xE5';
            else if (startsWith(token, "Eacute"sv)) out += '\xC9';
            else if (startsWith(token, "Egrave"sv)) out += '\xC8';
            else if (startsWith(token, "Ecirc"sv)) out += '\xCA';
            else if (startsWith(token, "Euml"sv)) out += '\xCB';
            else if (startsWith(token, "eacute"sv)) out += '\xE9';
            else if (startsWith(token, "egrave"sv)) out += '\xE8';
            else if (startsWith(token, "ecirc"sv)) out += '\xEA';
            else if (startsWith(token, "euml"sv)) out += '\xEB';
            else if (startsWith(token, "Iacute"sv)) out += '\xCD';
            else if (startsWith(token, "Igrave"sv)) out += '\xCC';
            else if (startsWith(token, "Icirc"sv)) out += '\xCE';
            else if (startsWith(token, "Iuml"sv)) out += '\xCF';
            else if (startsWith(token, "iacute"sv)) out += '\xED';
            else if (startsWith(token, "igrave"sv)) out += '\xEC';
            else if (startsWith(token, "icirc"sv)) out += '\xEE';
            else if (startsWith(token, "iuml"sv)) out += '\xEF';
            else if (startsWith(token, "Oacute"sv)) out += '\xD3';
            else if (startsWith(token, "Ograve"sv)) out += '\xD2';
            else if (startsWith(token, "Ocirc"sv)) out += '\xD4';
            else if (startsWith(token, "Ouml"sv)) out += '\xD6';
            else if (startsWith(token, "Otilde"sv)) out += '\xD5';
            else if (startsWith(token, "oacute"sv)) out += '\xF3';
            else if (startsWith(token, "ograve"sv)) out += '\xF2';
            else if (startsWith(token, "ocirc"sv)) out += '\xF4';
            else if (startsWith(token, "ouml"sv)) out += '\xF6';
            else if (startsWith(token, "otilde"sv)) out += '\xF5';
            else if (startsWith(token, "Uacute"sv)) out += '\xDA';
            else if (startsWith(token, "Ugrave"sv)) out += '\xD9';
            else if (startsWith(token, "Ucirc"sv)) out += '\xDB';
            else if (startsWith(token, "Uuml"sv)) out += '\xDC';
            else if (startsWith(token, "uacute"sv)) out += '\xFA';
            else if (startsWith(token, "ugrave"sv)) out += '\xF9';
            else if (startsWith(token, "ucirc"sv)) out += '\xFB';
            else if (startsWith(token, "uuml"sv)) out += '\xFC';
            else if (startsWith(token, "Yacute"sv)) out += '\xDD';
            else if (startsWith(token, "yacute"sv)) out += '\xFD';
            else if (startsWith(token, "yuml"sv)) out += '\xFF';

            else if (startsWith(token, "deg"sv)) out += '\xB0';
            else if (startsWith(token, "plusmn"sv)) out += '\xB1';
            else if (startsWith(token, "sup2"sv)) out += '\xB2';
            else if (startsWith(token, "sup3"sv)) out += '\xB3';
            else if (startsWith(token, "sup1"sv)) out += '\xB9';
            else if (startsWith(token, "nbsp"sv)) out += '\xBA';
            else if (startsWith(token, "pound"sv)) out += '\xA3';
            else if (startsWith(token, "cent"sv)) out += '\xA2';
            else if (startsWith(token, "frac14"sv)) out += '\xBC';
            else if (startsWith(token, "frac12"sv)) out += '\xBD';
            else if (startsWith(token, "frac34"sv)) out += '\xBE';
            else if (startsWith(token, "iquest"sv)) out += '\xBF';
            else if (startsWith(token, "iexcl"sv)) out += '\xA1';
            else if (startsWith(token, "ETH"sv)) out += '\xD0';
            else if (startsWith(token, "eth"sv)) out += '\xF0';
            else if (startsWith(token, "THORN"sv)) out += '\xDE';
            else if (startsWith(token, "thorn"sv)) out += '\xFE';
            else if (startsWith(token, "AElig"sv)) out += '\xC6';
            else if (startsWith(token, "aelig"sv)) out += '\xE6';
            else if (startsWith(token, "Oslash"sv)) out += '\xD8';
            else if (startsWith(token, "curren"sv)) out += '\xA4';
            else if (startsWith(token, "Ccedil"sv)) out += '\xC7';
            else if (startsWith(token, "ccedil"sv)) out += '\xE7';
            else if (startsWith(token, "szlig"sv)) out += '\xDF';
            else if (startsWith(token, "Ntilde"sv)) out += '\xD1';
            else if (startsWith(token, "ntilde"sv)) out += '\xF1';
            else if (startsWith(token, "yen"sv)) out += '\xA5';
            else if (startsWith(token, "not"sv)) out += '\xAC';
            else if (startsWith(token, "ordf"sv)) out += '\xAA';
            else if (startsWith(token, "uml"sv)) out += '\xA8';
            else if (startsWith(token, "shy"sv)) out += '\xAD';
            else if (startsWith(token, "macr"sv)) out += '\xAF';
            else if (startsWith(token, "micro"sv)) out += "\xB5";
            else if (startsWith(token, "middot"sv)) out +="\xB7";
            else if (startsWith(token, "cedil"sv)) out += "\xB8";
            else if (startsWith(token, "ordm"sv)) out +=  "\xBA";
            else if (startsWith(token, "times"sv)) out += "\xD7";
            else if (startsWith(token, "divide"sv)) out +="\xF7";
            else if (startsWith(token, "oslash"sv)) out +="\xF8";
            continue;

        }
        else if (*from == '>' && !ampersand) {
            intoken = false;
            // process desired tokens
            if (startsWith(token, "sync type=\"Strongs\" value=\""sv)) {
                out += "<W";
                for (unsigned int i = 27; token[i] != '\"'; i++)
                    out += token[i];
                out += '>';
                continue;
            }
            if (startsWith(token, "sync type=\"morph\" value=\""sv)) {
                out += "<WT";
                for (unsigned int i = 25; token[i] != '\"'; i++)
                    out += token[i];
                out += '>';
                continue;
            }
            else if (startsWith(token, "scripRef"sv)) {
                out += "<RX>";
                continue;
            }
             else if (startsWith(token, "/scripRef"sv)) {
                out += "<Rx>";
                continue;
            }
            else if (startsWith(token, "note"sv)) {
                out += "<RF>";
                continue;
            }
            else if (startsWith(token, "/note"sv)) {
                out += "<Rf>";
                continue;
            }
            else if (startsWith(token, "sup"sv)) {
                out += "<FS>";
            }
            else if (startsWith(token, "/sup"sv)) {
                out += "<Fs>";
            }
            else if (!strnicmp(token, "font color=#ff0000", 18)) {
                out += "<FR>";
                continue;
            }
            else if (!strnicmp(token, "/font", 5)) {
                out += "<Fr>";
                continue;
            }
            else if (startsWith(token, "div class=\"sechead\""sv)) {
                out += "<TS>";
                sechead = true;
                continue;
            }
            else if (sechead && startsWith(token, "/div"sv)) {
                out += "<Ts>";
                sechead = false;
                continue;
            }
            else if (startsWith(token, "div class=\"title\""sv)) {
                out += "<TT>";
                title = true;
                continue;
            }
            else if (title && startsWith(token, "/div"sv)) {
                out += "<Tt>";
                title = false;
                continue;
            }
            else if (!strnicmp(token, "br", 2)) {
                out += "<CL>";
                continue;
            }
            else switch(*token) {
            case 'I':            // font tags
            case 'i':
                out += "<FI>";
                continue;
            case 'B':        // bold start
            case 'b':
                out += "<FB>";
                continue;
            case '/':
                switch(token[1]) {
                case 'P':
                case 'p':
                    out += "<CM>";
                    continue;
                case 'I':
                case 'i':        // italic end
                    out += "<Fi>";
                    continue;
                case 'B':        // bold start
                case 'b':
                    out += "<Fb>";
                    continue;
                }
            }
            continue;
        }
        if (intoken) {
            if (tokpos < 2045) {
                token[tokpos++] = *from;
                // TODO: why is this + 2 ?
                token[tokpos+2] = 0;
            }
        }
        else    out += *from;
    }

    std::string out2;
    for (auto const * from = out.c_str(); *from; from++) {  //loop to remove extra spaces
        if ((std::strchr(" \t\n\r", *from))) {
            while (*(from+1) && (std::strchr(" \t\n\r", *(from+1)))) {
                from++;
            }
            out2 += " ";
        } else {
            out2 += *from;
        }
    }
    text = std::move(out2);
    return 0;
}

} /* namespace swordxx */
