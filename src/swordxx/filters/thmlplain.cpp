/******************************************************************************
 *
 *  thmlplain.cpp -    SWFilter descendant to strip out all ThML tags or
 *            convert to ASCII rendered symbols
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

#include "thmlplain.h"

#include <cstdlib>
#include <cstring>
#include <string>


namespace swordxx {

ThMLPlain::ThMLPlain() {
}

char ThMLPlain::processText(std::string &text, const SWKey *key, const SWModule *module)
{
    (void) key;
    (void) module;
    char token[2048];
    int tokpos = 0;
    bool intoken = false;
    bool ampersand = false;

    const char *from;
    std::string orig = text;
    from = orig.c_str();
    for (text = ""; *from; from++)
    {
        if (*from == 10 || *from == 13)
            from++;
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
            token[0] = 0;
            token[1] = 0;
            token[2] = 0;
            ampersand = true;
            continue;
        }
        if (*from == ';' && ampersand) {
            intoken = false;
            ampersand = false;

            if (!std::strncmp("nbsp", token, 4)) text += ' ';
            else if (!std::strncmp("quot", token, 4)) text += '"';
            else if (!std::strncmp("amp", token, 3)) text += '&';
            else if (!std::strncmp("lt", token, 2)) text += '<';
            else if (!std::strncmp("gt", token, 2)) text += '>';
            else if (!std::strncmp("brvbar", token, 6)) text += "¦";
            else if (!std::strncmp("sect", token, 4)) text += "§";
            else if (!std::strncmp("copy", token, 4)) text += "©";
            else if (!std::strncmp("laquo", token, 5)) text += "«";
            else if (!std::strncmp("reg", token, 3)) text += "®";
            else if (!std::strncmp("acute", token, 5)) text += "´";
            else if (!std::strncmp("para", token, 4)) text += "¶";
            else if (!std::strncmp("raquo", token, 5)) text += "»";

            else if (!std::strncmp("Aacute", token, 6)) text += "Á";
            else if (!std::strncmp("Agrave", token, 6)) text += "À";
            else if (!std::strncmp("Acirc", token, 5)) text += "Â";
            else if (!std::strncmp("Auml", token, 4)) text += "Ä";
            else if (!std::strncmp("Atilde", token, 6)) text += "Ã";
            else if (!std::strncmp("Aring", token, 5)) text += "Å";
            else if (!std::strncmp("aacute", token, 6)) text += "á";
            else if (!std::strncmp("agrave", token, 6)) text += "à";
            else if (!std::strncmp("acirc", token, 5)) text += "â";
            else if (!std::strncmp("auml", token, 4)) text += "ä";
            else if (!std::strncmp("atilde", token, 6)) text += "ã";
            else if (!std::strncmp("aring", token, 5)) text += "å";
            else if (!std::strncmp("Eacute", token, 6)) text += "É";
            else if (!std::strncmp("Egrave", token, 6)) text += "È";
            else if (!std::strncmp("Ecirc", token, 5)) text += "Ê";
            else if (!std::strncmp("Euml", token, 4)) text += "Ë";
            else if (!std::strncmp("eacute", token, 6)) text += "é";
            else if (!std::strncmp("egrave", token, 6)) text += "è";
            else if (!std::strncmp("ecirc", token, 5)) text += "ê";
            else if (!std::strncmp("euml", token, 4)) text += "ë";
            else if (!std::strncmp("Iacute", token, 6)) text += "Í";
            else if (!std::strncmp("Igrave", token, 6)) text += "Ì";
            else if (!std::strncmp("Icirc", token, 5)) text += "Î";
            else if (!std::strncmp("Iuml", token, 4)) text += "Ï";
            else if (!std::strncmp("iacute", token, 6)) text += "í";
            else if (!std::strncmp("igrave", token, 6)) text += "ì";
            else if (!std::strncmp("icirc", token, 5)) text += "î";
            else if (!std::strncmp("iuml", token, 4)) text += "ï";
            else if (!std::strncmp("Oacute", token, 6)) text += "Ó";
            else if (!std::strncmp("Ograve", token, 6)) text += "Ò";
            else if (!std::strncmp("Ocirc", token, 5)) text += "Ô";
            else if (!std::strncmp("Ouml", token, 4)) text += "Ö";
            else if (!std::strncmp("Otilde", token, 6)) text += "Õ";
            else if (!std::strncmp("oacute", token, 6)) text += "ó";
            else if (!std::strncmp("ograve", token, 6)) text += "ò";
            else if (!std::strncmp("ocirc", token, 5)) text += "ô";
            else if (!std::strncmp("ouml", token, 4)) text += "ö";
            else if (!std::strncmp("otilde", token, 6)) text += "õ";
            else if (!std::strncmp("Uacute", token, 6)) text += "Ú";
            else if (!std::strncmp("Ugrave", token, 6)) text += "Ù";
            else if (!std::strncmp("Ucirc", token, 5)) text += "Û";
            else if (!std::strncmp("Uuml", token, 4)) text += "Ü";
            else if (!std::strncmp("uacute", token, 6)) text += "ú";
            else if (!std::strncmp("ugrave", token, 6)) text += "ù";
            else if (!std::strncmp("ucirc", token, 5)) text += "û";
            else if (!std::strncmp("uuml", token, 4)) text += "ü";
            else if (!std::strncmp("Yacute", token, 6)) text += "Ý";
            else if (!std::strncmp("yacute", token, 6)) text += "ý";
            else if (!std::strncmp("yuml", token, 4)) text += "ÿ";

            else if (!std::strncmp("deg", token, 3)) text += "°";
            else if (!std::strncmp("plusmn", token, 6)) text += "±";
            else if (!std::strncmp("sup2", token, 4)) text += "²";
            else if (!std::strncmp("sup3", token, 4)) text += "³";
            else if (!std::strncmp("sup1", token, 4)) text += "¹";
            else if (!std::strncmp("nbsp", token, 4)) text += "º";
            else if (!std::strncmp("pound", token, 5)) text += "£";
            else if (!std::strncmp("cent", token, 4)) text += "¢";
            else if (!std::strncmp("frac14", token, 6)) text += "¼";
            else if (!std::strncmp("frac12", token, 6)) text += "½";
            else if (!std::strncmp("frac34", token, 6)) text += "¾";
            else if (!std::strncmp("iquest", token, 6)) text += "¿";
            else if (!std::strncmp("iexcl", token, 5)) text += "¡";
            else if (!std::strncmp("ETH", token, 3)) text += "Ð";
            else if (!std::strncmp("eth", token, 3)) text += "ð";
            else if (!std::strncmp("THORN", token, 5)) text += "Þ";
            else if (!std::strncmp("thorn", token, 5)) text += "þ";
            else if (!std::strncmp("AElig", token, 5)) text += "Æ";
            else if (!std::strncmp("aelig", token, 5)) text += "æ";
            else if (!std::strncmp("Oslash", token, 6)) text += "Ø";
            else if (!std::strncmp("curren", token, 6)) text += "¤";
            else if (!std::strncmp("Ccedil", token, 6)) text += "Ç";
            else if (!std::strncmp("ccedil", token, 6)) text += "ç";
            else if (!std::strncmp("szlig", token, 5)) text += "ß";
            else if (!std::strncmp("Ntilde", token, 6)) text += "Ñ";
            else if (!std::strncmp("ntilde", token, 6)) text += "ñ";
            else if (!std::strncmp("yen", token, 3)) text += "¥";
            else if (!std::strncmp("not", token, 3)) text += "¬";
            else if (!std::strncmp("ordf", token, 4)) text += "ª";
            else if (!std::strncmp("uml", token, 3)) text += "¨";
            else if (!std::strncmp("shy", token, 3)) text += "­";
            else if (!std::strncmp("macr", token, 4)) text += "¯";
            else if (!std::strncmp("micro", token, 5)) text += "µ";
            else if (!std::strncmp("middot", token, 6)) text += "·";
            else if (!std::strncmp("cedil", token, 5)) text += "¸";
            else if (!std::strncmp("ordm", token, 4)) text += "º";
            else if (!std::strncmp("times", token, 5)) text += "×";
            else if (!std::strncmp("divide", token, 6)) text += "÷";
            else if (!std::strncmp("oslash", token, 6)) text += "ø";
            continue;

        }
        else if (*from == '>' && !ampersand) {
            intoken = false;
            // process desired tokens
            if (!std::strncmp(token, "sync type=\"Strongs\" value=\"", 27)) {
                text += ' ';
                text += '<';
                for (unsigned int i = 27; token[i] != '\"'; i++)
                    text += token[i];
                text += '>';
                continue;
            }
            if (!std::strncmp(token, "sync type=\"morph\" value=\"", 25)) {
                text += ' ';
                text += '(';
                for (unsigned int i = 25; token[i] != '\"'; i++)
                    text += token[i];
                text += ')';
                continue;
            }
            if (!std::strncmp("note", token, 4)) {
                text += ' ';
                text += '[';
            }
            else if (!std::strncmp("br", token, 2))
                text += '\n';
            else if (!std::strncmp("/p", token, 2))
                text += '\n';
            else if (!std::strncmp("/note", token, 5)) {
                text += ']';
                text += ' ';
            }
            continue;
        }
        if (intoken) {
            if (tokpos < 2045) {
                token[tokpos++] = *from;
                //TODO: why is this + 2?  Are we trying to keep 2 or 3 nulls after the last valid char?
                // tokpos has been incremented past the last valid token. it should be pointing to null
                // +1 should give us 2 nulls, but we're +2 here, which actually keeps 3 nulls after the
                // last valid char.  Why are we doing any of this?  These were written before std::string and should
                // probably be switched to std::string, but perf tests before and after the switch should be run
                token[tokpos+2] = 0;
            }
        }
        else	text += *from;
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
