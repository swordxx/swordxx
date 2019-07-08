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
#include "../utilstr.h"


namespace swordxx {

ThMLPlain::ThMLPlain() {
}

char ThMLPlain::processText(std::string &text, const SWKey *key, const SWModule *module)
{
    using namespace std::literals::string_view_literals;

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

            if (startsWith(token, "nbsp"sv)) text += ' ';
            else if (startsWith(token, "quot"sv)) text += '"';
            else if (startsWith(token, "amp"sv)) text += '&';
            else if (startsWith(token, "lt"sv)) text += '<';
            else if (startsWith(token, "gt"sv)) text += '>';
            else if (startsWith(token, "brvbar"sv)) text += "¦";
            else if (startsWith(token, "sect"sv)) text += "§";
            else if (startsWith(token, "copy"sv)) text += "©";
            else if (startsWith(token, "laquo"sv)) text += "«";
            else if (startsWith(token, "reg"sv)) text += "®";
            else if (startsWith(token, "acute"sv)) text += "´";
            else if (startsWith(token, "para"sv)) text += "¶";
            else if (startsWith(token, "raquo"sv)) text += "»";

            else if (startsWith(token, "Aacute"sv)) text += "Á";
            else if (startsWith(token, "Agrave"sv)) text += "À";
            else if (startsWith(token, "Acirc"sv)) text += "Â";
            else if (startsWith(token, "Auml"sv)) text += "Ä";
            else if (startsWith(token, "Atilde"sv)) text += "Ã";
            else if (startsWith(token, "Aring"sv)) text += "Å";
            else if (startsWith(token, "aacute"sv)) text += "á";
            else if (startsWith(token, "agrave"sv)) text += "à";
            else if (startsWith(token, "acirc"sv)) text += "â";
            else if (startsWith(token, "auml"sv)) text += "ä";
            else if (startsWith(token, "atilde"sv)) text += "ã";
            else if (startsWith(token, "aring"sv)) text += "å";
            else if (startsWith(token, "Eacute"sv)) text += "É";
            else if (startsWith(token, "Egrave"sv)) text += "È";
            else if (startsWith(token, "Ecirc"sv)) text += "Ê";
            else if (startsWith(token, "Euml"sv)) text += "Ë";
            else if (startsWith(token, "eacute"sv)) text += "é";
            else if (startsWith(token, "egrave"sv)) text += "è";
            else if (startsWith(token, "ecirc"sv)) text += "ê";
            else if (startsWith(token, "euml"sv)) text += "ë";
            else if (startsWith(token, "Iacute"sv)) text += "Í";
            else if (startsWith(token, "Igrave"sv)) text += "Ì";
            else if (startsWith(token, "Icirc"sv)) text += "Î";
            else if (startsWith(token, "Iuml"sv)) text += "Ï";
            else if (startsWith(token, "iacute"sv)) text += "í";
            else if (startsWith(token, "igrave"sv)) text += "ì";
            else if (startsWith(token, "icirc"sv)) text += "î";
            else if (startsWith(token, "iuml"sv)) text += "ï";
            else if (startsWith(token, "Oacute"sv)) text += "Ó";
            else if (startsWith(token, "Ograve"sv)) text += "Ò";
            else if (startsWith(token, "Ocirc"sv)) text += "Ô";
            else if (startsWith(token, "Ouml"sv)) text += "Ö";
            else if (startsWith(token, "Otilde"sv)) text += "Õ";
            else if (startsWith(token, "oacute"sv)) text += "ó";
            else if (startsWith(token, "ograve"sv)) text += "ò";
            else if (startsWith(token, "ocirc"sv)) text += "ô";
            else if (startsWith(token, "ouml"sv)) text += "ö";
            else if (startsWith(token, "otilde"sv)) text += "õ";
            else if (startsWith(token, "Uacute"sv)) text += "Ú";
            else if (startsWith(token, "Ugrave"sv)) text += "Ù";
            else if (startsWith(token, "Ucirc"sv)) text += "Û";
            else if (startsWith(token, "Uuml"sv)) text += "Ü";
            else if (startsWith(token, "uacute"sv)) text += "ú";
            else if (startsWith(token, "ugrave"sv)) text += "ù";
            else if (startsWith(token, "ucirc"sv)) text += "û";
            else if (startsWith(token, "uuml"sv)) text += "ü";
            else if (startsWith(token, "Yacute"sv)) text += "Ý";
            else if (startsWith(token, "yacute"sv)) text += "ý";
            else if (startsWith(token, "yuml"sv)) text += "ÿ";

            else if (startsWith(token, "deg"sv)) text += "°";
            else if (startsWith(token, "plusmn"sv)) text += "±";
            else if (startsWith(token, "sup2"sv)) text += "²";
            else if (startsWith(token, "sup3"sv)) text += "³";
            else if (startsWith(token, "sup1"sv)) text += "¹";
            else if (startsWith(token, "nbsp"sv)) text += "º";
            else if (startsWith(token, "pound"sv)) text += "£";
            else if (startsWith(token, "cent"sv)) text += "¢";
            else if (startsWith(token, "frac14"sv)) text += "¼";
            else if (startsWith(token, "frac12"sv)) text += "½";
            else if (startsWith(token, "frac34"sv)) text += "¾";
            else if (startsWith(token, "iquest"sv)) text += "¿";
            else if (startsWith(token, "iexcl"sv)) text += "¡";
            else if (startsWith(token, "ETH"sv)) text += "Ð";
            else if (startsWith(token, "eth"sv)) text += "ð";
            else if (startsWith(token, "THORN"sv)) text += "Þ";
            else if (startsWith(token, "thorn"sv)) text += "þ";
            else if (startsWith(token, "AElig"sv)) text += "Æ";
            else if (startsWith(token, "aelig"sv)) text += "æ";
            else if (startsWith(token, "Oslash"sv)) text += "Ø";
            else if (startsWith(token, "curren"sv)) text += "¤";
            else if (startsWith(token, "Ccedil"sv)) text += "Ç";
            else if (startsWith(token, "ccedil"sv)) text += "ç";
            else if (startsWith(token, "szlig"sv)) text += "ß";
            else if (startsWith(token, "Ntilde"sv)) text += "Ñ";
            else if (startsWith(token, "ntilde"sv)) text += "ñ";
            else if (startsWith(token, "yen"sv)) text += "¥";
            else if (startsWith(token, "not"sv)) text += "¬";
            else if (startsWith(token, "ordf"sv)) text += "ª";
            else if (startsWith(token, "uml"sv)) text += "¨";
            else if (startsWith(token, "shy"sv)) text += "­";
            else if (startsWith(token, "macr"sv)) text += "¯";
            else if (startsWith(token, "micro"sv)) text += "µ";
            else if (startsWith(token, "middot"sv)) text += "·";
            else if (startsWith(token, "cedil"sv)) text += "¸";
            else if (startsWith(token, "ordm"sv)) text += "º";
            else if (startsWith(token, "times"sv)) text += "×";
            else if (startsWith(token, "divide"sv)) text += "÷";
            else if (startsWith(token, "oslash"sv)) text += "ø";
            continue;

        }
        else if (*from == '>' && !ampersand) {
            intoken = false;
            // process desired tokens
            if (startsWith(token, "sync type=\"Strongs\" value=\""sv)) {
                text += ' ';
                text += '<';
                for (unsigned int i = 27; token[i] != '\"'; i++)
                    text += token[i];
                text += '>';
                continue;
            }
            if (startsWith(token, "sync type=\"morph\" value=\""sv)) {
                text += ' ';
                text += '(';
                for (unsigned int i = 25; token[i] != '\"'; i++)
                    text += token[i];
                text += ')';
                continue;
            }
            if (startsWith(token, "note"sv)) {
                text += ' ';
                text += '[';
            }
            else if (startsWith(token, "br"sv))
                text += '\n';
            else if (startsWith(token, "/p"sv))
                text += '\n';
            else if (startsWith(token, "/note"sv)) {
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
