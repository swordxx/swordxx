/***************************************************************************
 *
 *  thmlgbf.cpp -    ThML to GBF filter
 *
 * $Id$
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
    const char *from;
    char token[2048];
    int tokpos = 0;
    bool intoken     = false;
    bool ampersand = false;
    bool sechead = false;
    bool title = false;

    std::string orig = text;
    from = orig.c_str();

    for (text = ""; *from; from++) {
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

            if (startsWith(token, "nbsp"sv)) text += ' ';
            else if (startsWith(token, "quot"sv)) text += '"';
            else if (startsWith(token, "amp"sv)) text += '&';
            else if (startsWith(token, "lt"sv)) text += '<';
            else if (startsWith(token, "gt"sv)) text += '>';
            else if (startsWith(token, "brvbar"sv)) text += '\xA6';
            else if (startsWith(token, "sect"sv)) text += '\xA7';
            else if (startsWith(token, "copy"sv)) text += '\xA9';
            else if (startsWith(token, "laquo"sv)) text += '\xAB';
            else if (startsWith(token, "reg"sv)) text += '\xAE';
            else if (startsWith(token, "acute"sv)) text += '\xB4';
            else if (startsWith(token, "para"sv)) text += '\xB6';
            else if (startsWith(token, "raquo"sv)) text += '\xBB';

            else if (startsWith(token, "Aacute"sv)) text += '\xC1';
            else if (startsWith(token, "Agrave"sv)) text += '\xC0';
            else if (startsWith(token, "Acirc"sv)) text += '\xC2';
            else if (startsWith(token, "Auml"sv)) text += '\xC4';
            else if (startsWith(token, "Atilde"sv)) text += '\xC3';
            else if (startsWith(token, "Aring"sv)) text += '\xC5';
            else if (startsWith(token, "aacute"sv)) text += '\xE1';
            else if (startsWith(token, "agrave"sv)) text += '\xE0';
            else if (startsWith(token, "acirc"sv)) text += '\xE2';
            else if (startsWith(token, "auml"sv)) text += '\xE4';
            else if (startsWith(token, "atilde"sv)) text += '\xE3';
            else if (startsWith(token, "aring"sv)) text += '\xE5';
            else if (startsWith(token, "Eacute"sv)) text += '\xC9';
            else if (startsWith(token, "Egrave"sv)) text += '\xC8';
            else if (startsWith(token, "Ecirc"sv)) text += '\xCA';
            else if (startsWith(token, "Euml"sv)) text += '\xCB';
            else if (startsWith(token, "eacute"sv)) text += '\xE9';
            else if (startsWith(token, "egrave"sv)) text += '\xE8';
            else if (startsWith(token, "ecirc"sv)) text += '\xEA';
            else if (startsWith(token, "euml"sv)) text += '\xEB';
            else if (startsWith(token, "Iacute"sv)) text += '\xCD';
            else if (startsWith(token, "Igrave"sv)) text += '\xCC';
            else if (startsWith(token, "Icirc"sv)) text += '\xCE';
            else if (startsWith(token, "Iuml"sv)) text += '\xCF';
            else if (startsWith(token, "iacute"sv)) text += '\xED';
            else if (startsWith(token, "igrave"sv)) text += '\xEC';
            else if (startsWith(token, "icirc"sv)) text += '\xEE';
            else if (startsWith(token, "iuml"sv)) text += '\xEF';
            else if (startsWith(token, "Oacute"sv)) text += '\xD3';
            else if (startsWith(token, "Ograve"sv)) text += '\xD2';
            else if (startsWith(token, "Ocirc"sv)) text += '\xD4';
            else if (startsWith(token, "Ouml"sv)) text += '\xD6';
            else if (startsWith(token, "Otilde"sv)) text += '\xD5';
            else if (startsWith(token, "oacute"sv)) text += '\xF3';
            else if (startsWith(token, "ograve"sv)) text += '\xF2';
            else if (startsWith(token, "ocirc"sv)) text += '\xF4';
            else if (startsWith(token, "ouml"sv)) text += '\xF6';
            else if (startsWith(token, "otilde"sv)) text += '\xF5';
            else if (startsWith(token, "Uacute"sv)) text += '\xDA';
            else if (startsWith(token, "Ugrave"sv)) text += '\xD9';
            else if (startsWith(token, "Ucirc"sv)) text += '\xDB';
            else if (startsWith(token, "Uuml"sv)) text += '\xDC';
            else if (startsWith(token, "uacute"sv)) text += '\xFA';
            else if (startsWith(token, "ugrave"sv)) text += '\xF9';
            else if (startsWith(token, "ucirc"sv)) text += '\xFB';
            else if (startsWith(token, "uuml"sv)) text += '\xFC';
            else if (startsWith(token, "Yacute"sv)) text += '\xDD';
            else if (startsWith(token, "yacute"sv)) text += '\xFD';
            else if (startsWith(token, "yuml"sv)) text += '\xFF';

            else if (startsWith(token, "deg"sv)) text += '\xB0';
            else if (startsWith(token, "plusmn"sv)) text += '\xB1';
            else if (startsWith(token, "sup2"sv)) text += '\xB2';
            else if (startsWith(token, "sup3"sv)) text += '\xB3';
            else if (startsWith(token, "sup1"sv)) text += '\xB9';
            else if (startsWith(token, "nbsp"sv)) text += '\xBA';
            else if (startsWith(token, "pound"sv)) text += '\xA3';
            else if (startsWith(token, "cent"sv)) text += '\xA2';
            else if (startsWith(token, "frac14"sv)) text += '\xBC';
            else if (startsWith(token, "frac12"sv)) text += '\xBD';
            else if (startsWith(token, "frac34"sv)) text += '\xBE';
            else if (startsWith(token, "iquest"sv)) text += '\xBF';
            else if (startsWith(token, "iexcl"sv)) text += '\xA1';
            else if (startsWith(token, "ETH"sv)) text += '\xD0';
            else if (startsWith(token, "eth"sv)) text += '\xF0';
            else if (startsWith(token, "THORN"sv)) text += '\xDE';
            else if (startsWith(token, "thorn"sv)) text += '\xFE';
            else if (startsWith(token, "AElig"sv)) text += '\xC6';
            else if (startsWith(token, "aelig"sv)) text += '\xE6';
            else if (startsWith(token, "Oslash"sv)) text += '\xD8';
            else if (startsWith(token, "curren"sv)) text += '\xA4';
            else if (startsWith(token, "Ccedil"sv)) text += '\xC7';
            else if (startsWith(token, "ccedil"sv)) text += '\xE7';
            else if (startsWith(token, "szlig"sv)) text += '\xDF';
            else if (startsWith(token, "Ntilde"sv)) text += '\xD1';
            else if (startsWith(token, "ntilde"sv)) text += '\xF1';
            else if (startsWith(token, "yen"sv)) text += '\xA5';
            else if (startsWith(token, "not"sv)) text += '\xAC';
            else if (startsWith(token, "ordf"sv)) text += '\xAA';
            else if (startsWith(token, "uml"sv)) text += '\xA8';
            else if (startsWith(token, "shy"sv)) text += '\xAD';
            else if (startsWith(token, "macr"sv)) text += '\xAF';
            else if (startsWith(token, "micro"sv)) text += "\xB5";
            else if (startsWith(token, "middot"sv)) text +="\xB7";
            else if (startsWith(token, "cedil"sv)) text += "\xB8";
            else if (startsWith(token, "ordm"sv)) text +=  "\xBA";
            else if (startsWith(token, "times"sv)) text += "\xD7";
            else if (startsWith(token, "divide"sv)) text +="\xF7";
            else if (startsWith(token, "oslash"sv)) text +="\xF8";
            continue;

        }
        else if (*from == '>' && !ampersand) {
            intoken = false;
            // process desired tokens
            if (startsWith(token, "sync type=\"Strongs\" value=\""sv)) {
                text += "<W";
                for (unsigned int i = 27; token[i] != '\"'; i++)
                    text += token[i];
                text += '>';
                continue;
            }
            if (startsWith(token, "sync type=\"morph\" value=\""sv)) {
                text += "<WT";
                for (unsigned int i = 25; token[i] != '\"'; i++)
                    text += token[i];
                text += '>';
                continue;
            }
            else if (startsWith(token, "scripRef"sv)) {
                text += "<RX>";
                continue;
            }
             else if (startsWith(token, "/scripRef"sv)) {
                text += "<Rx>";
                continue;
            }
            else if (startsWith(token, "note"sv)) {
                text += "<RF>";
                continue;
            }
            else if (startsWith(token, "/note"sv)) {
                text += "<Rf>";
                continue;
            }
            else if (startsWith(token, "sup"sv)) {
                text += "<FS>";
            }
            else if (startsWith(token, "/sup"sv)) {
                text += "<Fs>";
            }
            else if (!strnicmp(token, "font color=#ff0000", 18)) {
                text += "<FR>";
                continue;
            }
            else if (!strnicmp(token, "/font", 5)) {
                text += "<Fr>";
                continue;
            }
            else if (startsWith(token, "div class=\"sechead\""sv)) {
                text += "<TS>";
                sechead = true;
                continue;
            }
            else if (sechead && startsWith(token, "/div"sv)) {
                text += "<Ts>";
                sechead = false;
                continue;
            }
            else if (startsWith(token, "div class=\"title\""sv)) {
                text += "<TT>";
                title = true;
                continue;
            }
            else if (title && startsWith(token, "/div"sv)) {
                text += "<Tt>";
                title = false;
                continue;
            }
            else if (!strnicmp(token, "br", 2)) {
                text += "<CL>";
                continue;
            }
            else switch(*token) {
            case 'I':            // font tags
            case 'i':
                text += "<FI>";
                continue;
            case 'B':        // bold start
            case 'b':
                text += "<FB>";
                continue;
            case '/':
                switch(token[1]) {
                case 'P':
                case 'p':
                    text += "<CM>";
                    continue;
                case 'I':
                case 'i':        // italic end
                    text += "<Fi>";
                    continue;
                case 'B':        // bold start
                case 'b':
                    text += "<Fb>";
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
        else    text += *from;
    }

    orig = text;
    from = orig.c_str();
    for (text = ""; *from; from++) {  //loop to remove extra spaces
                if ((std::strchr(" \t\n\r", *from))) {
                        while (*(from+1) && (std::strchr(" \t\n\r", *(from+1)))) {
                                from++;
                        }
                        text += " ";
                }
                else {
                        text += *from;
                }
        }

    return 0;
}

} /* namespace swordxx */
