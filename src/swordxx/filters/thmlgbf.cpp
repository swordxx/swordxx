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

            if (!std::strncmp("nbsp", token, 4)) text += ' ';
            else if (!std::strncmp("quot", token, 4)) text += '"';
            else if (!std::strncmp("amp", token, 3)) text += '&';
            else if (!std::strncmp("lt", token, 2)) text += '<';
            else if (!std::strncmp("gt", token, 2)) text += '>';
            else if (!std::strncmp("brvbar", token, 6)) text += '\xA6';
            else if (!std::strncmp("sect", token, 4)) text += '\xA7';
            else if (!std::strncmp("copy", token, 4)) text += '\xA9';
            else if (!std::strncmp("laquo", token, 5)) text += '\xAB';
            else if (!std::strncmp("reg", token, 3)) text += '\xAE';
            else if (!std::strncmp("acute", token, 5)) text += '\xB4';
            else if (!std::strncmp("para", token, 4)) text += '\xB6';
            else if (!std::strncmp("raquo", token, 5)) text += '\xBB';

            else if (!std::strncmp("Aacute", token, 6)) text += '\xC1';
            else if (!std::strncmp("Agrave", token, 6)) text += '\xC0';
            else if (!std::strncmp("Acirc", token, 5)) text += '\xC2';
            else if (!std::strncmp("Auml", token, 4)) text += '\xC4';
            else if (!std::strncmp("Atilde", token, 6)) text += '\xC3';
            else if (!std::strncmp("Aring", token, 5)) text += '\xC5';
            else if (!std::strncmp("aacute", token, 6)) text += '\xE1';
            else if (!std::strncmp("agrave", token, 6)) text += '\xE0';
            else if (!std::strncmp("acirc", token, 5)) text += '\xE2';
            else if (!std::strncmp("auml", token, 4)) text += '\xE4';
            else if (!std::strncmp("atilde", token, 6)) text += '\xE3';
            else if (!std::strncmp("aring", token, 5)) text += '\xE5';
            else if (!std::strncmp("Eacute", token, 6)) text += '\xC9';
            else if (!std::strncmp("Egrave", token, 6)) text += '\xC8';
            else if (!std::strncmp("Ecirc", token, 5)) text += '\xCA';
            else if (!std::strncmp("Euml", token, 4)) text += '\xCB';
            else if (!std::strncmp("eacute", token, 6)) text += '\xE9';
            else if (!std::strncmp("egrave", token, 6)) text += '\xE8';
            else if (!std::strncmp("ecirc", token, 5)) text += '\xEA';
            else if (!std::strncmp("euml", token, 4)) text += '\xEB';
            else if (!std::strncmp("Iacute", token, 6)) text += '\xCD';
            else if (!std::strncmp("Igrave", token, 6)) text += '\xCC';
            else if (!std::strncmp("Icirc", token, 5)) text += '\xCE';
            else if (!std::strncmp("Iuml", token, 4)) text += '\xCF';
            else if (!std::strncmp("iacute", token, 6)) text += '\xED';
            else if (!std::strncmp("igrave", token, 6)) text += '\xEC';
            else if (!std::strncmp("icirc", token, 5)) text += '\xEE';
            else if (!std::strncmp("iuml", token, 4)) text += '\xEF';
            else if (!std::strncmp("Oacute", token, 6)) text += '\xD3';
            else if (!std::strncmp("Ograve", token, 6)) text += '\xD2';
            else if (!std::strncmp("Ocirc", token, 5)) text += '\xD4';
            else if (!std::strncmp("Ouml", token, 4)) text += '\xD6';
            else if (!std::strncmp("Otilde", token, 6)) text += '\xD5';
            else if (!std::strncmp("oacute", token, 6)) text += '\xF3';
            else if (!std::strncmp("ograve", token, 6)) text += '\xF2';
            else if (!std::strncmp("ocirc", token, 5)) text += '\xF4';
            else if (!std::strncmp("ouml", token, 4)) text += '\xF6';
            else if (!std::strncmp("otilde", token, 6)) text += '\xF5';
            else if (!std::strncmp("Uacute", token, 6)) text += '\xDA';
            else if (!std::strncmp("Ugrave", token, 6)) text += '\xD9';
            else if (!std::strncmp("Ucirc", token, 5)) text += '\xDB';
            else if (!std::strncmp("Uuml", token, 4)) text += '\xDC';
            else if (!std::strncmp("uacute", token, 6)) text += '\xFA';
            else if (!std::strncmp("ugrave", token, 6)) text += '\xF9';
            else if (!std::strncmp("ucirc", token, 5)) text += '\xFB';
            else if (!std::strncmp("uuml", token, 4)) text += '\xFC';
            else if (!std::strncmp("Yacute", token, 6)) text += '\xDD';
            else if (!std::strncmp("yacute", token, 6)) text += '\xFD';
            else if (!std::strncmp("yuml", token, 4)) text += '\xFF';

            else if (!std::strncmp("deg", token, 3)) text += '\xB0';
            else if (!std::strncmp("plusmn", token, 6)) text += '\xB1';
            else if (!std::strncmp("sup2", token, 4)) text += '\xB2';
            else if (!std::strncmp("sup3", token, 4)) text += '\xB3';
            else if (!std::strncmp("sup1", token, 4)) text += '\xB9';
            else if (!std::strncmp("nbsp", token, 4)) text += '\xBA';
            else if (!std::strncmp("pound", token, 5)) text += '\xA3';
            else if (!std::strncmp("cent", token, 4)) text += '\xA2';
            else if (!std::strncmp("frac14", token, 6)) text += '\xBC';
            else if (!std::strncmp("frac12", token, 6)) text += '\xBD';
            else if (!std::strncmp("frac34", token, 6)) text += '\xBE';
            else if (!std::strncmp("iquest", token, 6)) text += '\xBF';
            else if (!std::strncmp("iexcl", token, 5)) text += '\xA1';
            else if (!std::strncmp("ETH", token, 3)) text += '\xD0';
            else if (!std::strncmp("eth", token, 3)) text += '\xF0';
            else if (!std::strncmp("THORN", token, 5)) text += '\xDE';
            else if (!std::strncmp("thorn", token, 5)) text += '\xFE';
            else if (!std::strncmp("AElig", token, 5)) text += '\xC6';
            else if (!std::strncmp("aelig", token, 5)) text += '\xE6';
            else if (!std::strncmp("Oslash", token, 6)) text += '\xD8';
            else if (!std::strncmp("curren", token, 6)) text += '\xA4';
            else if (!std::strncmp("Ccedil", token, 6)) text += '\xC7';
            else if (!std::strncmp("ccedil", token, 6)) text += '\xE7';
            else if (!std::strncmp("szlig", token, 5)) text += '\xDF';
            else if (!std::strncmp("Ntilde", token, 6)) text += '\xD1';
            else if (!std::strncmp("ntilde", token, 6)) text += '\xF1';
            else if (!std::strncmp("yen", token, 3)) text += '\xA5';
            else if (!std::strncmp("not", token, 3)) text += '\xAC';
            else if (!std::strncmp("ordf", token, 4)) text += '\xAA';
            else if (!std::strncmp("uml", token, 3)) text += '\xA8';
            else if (!std::strncmp("shy", token, 3)) text += '\xAD';
            else if (!std::strncmp("macr", token, 4)) text += '\xAF';
            else if (!std::strncmp("micro", token, 5)) text += "\xB5";
            else if (!std::strncmp("middot", token, 6)) text +="\xB7";
            else if (!std::strncmp("cedil", token, 5)) text += "\xB8";
            else if (!std::strncmp("ordm", token, 4)) text +=  "\xBA";
            else if (!std::strncmp("times", token, 5)) text += "\xD7";
            else if (!std::strncmp("divide", token, 6)) text +="\xF7";
            else if (!std::strncmp("oslash", token, 6)) text +="\xF8";
            continue;

        }
        else if (*from == '>' && !ampersand) {
            intoken = false;
            // process desired tokens
            if (!std::strncmp(token, "sync type=\"Strongs\" value=\"", 27)) {
                text += "<W";
                for (unsigned int i = 27; token[i] != '\"'; i++)
                    text += token[i];
                text += '>';
                continue;
            }
            if (!std::strncmp(token, "sync type=\"morph\" value=\"", 25)) {
                text += "<WT";
                for (unsigned int i = 25; token[i] != '\"'; i++)
                    text += token[i];
                text += '>';
                continue;
            }
            else if (!std::strncmp(token, "scripRef", 8)) {
                text += "<RX>";
                continue;
            }
             else if (!std::strncmp(token, "/scripRef", 9)) {
                text += "<Rx>";
                continue;
            }
            else if (!std::strncmp(token, "note", 4)) {
                text += "<RF>";
                continue;
            }
            else if (!std::strncmp(token, "/note", 5)) {
                text += "<Rf>";
                continue;
            }
            else if (!std::strncmp(token, "sup", 3)) {
                text += "<FS>";
            }
            else if (!std::strncmp(token, "/sup", 4)) {
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
            else if (!std::strncmp(token, "div class=\"sechead\"", 19)) {
                text += "<TS>";
                sechead = true;
                continue;
            }
            else if (sechead && !std::strncmp(token, "/div", 19)) {
                text += "<Ts>";
                sechead = false;
                continue;
            }
            else if (!std::strncmp(token, "div class=\"title\"", 19)) {
                text += "<TT>";
                title = true;
                continue;
            }
            else if (title && !std::strncmp(token, "/div", 19)) {
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
