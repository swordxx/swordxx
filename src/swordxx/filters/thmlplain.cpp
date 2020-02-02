/******************************************************************************
 *
 *  thmlplain.cpp -    SWFilter descendant to strip out all ThML tags or
 *            convert to ASCII rendered symbols
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

    std::string out;
    for (auto const * from = text.c_str(); *from; from++)
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

            if (startsWith(token, "nbsp"sv)) out += ' ';
            else if (startsWith(token, "quot"sv)) out += '"';
            else if (startsWith(token, "amp"sv)) out += '&';
            else if (startsWith(token, "lt"sv)) out += '<';
            else if (startsWith(token, "gt"sv)) out += '>';
            else if (startsWith(token, "brvbar"sv)) out += "¦";
            else if (startsWith(token, "sect"sv)) out += "§";
            else if (startsWith(token, "copy"sv)) out += "©";
            else if (startsWith(token, "laquo"sv)) out += "«";
            else if (startsWith(token, "reg"sv)) out += "®";
            else if (startsWith(token, "acute"sv)) out += "´";
            else if (startsWith(token, "para"sv)) out += "¶";
            else if (startsWith(token, "raquo"sv)) out += "»";

            else if (startsWith(token, "Aacute"sv)) out += "Á";
            else if (startsWith(token, "Agrave"sv)) out += "À";
            else if (startsWith(token, "Acirc"sv)) out += "Â";
            else if (startsWith(token, "Auml"sv)) out += "Ä";
            else if (startsWith(token, "Atilde"sv)) out += "Ã";
            else if (startsWith(token, "Aring"sv)) out += "Å";
            else if (startsWith(token, "aacute"sv)) out += "á";
            else if (startsWith(token, "agrave"sv)) out += "à";
            else if (startsWith(token, "acirc"sv)) out += "â";
            else if (startsWith(token, "auml"sv)) out += "ä";
            else if (startsWith(token, "atilde"sv)) out += "ã";
            else if (startsWith(token, "aring"sv)) out += "å";
            else if (startsWith(token, "Eacute"sv)) out += "É";
            else if (startsWith(token, "Egrave"sv)) out += "È";
            else if (startsWith(token, "Ecirc"sv)) out += "Ê";
            else if (startsWith(token, "Euml"sv)) out += "Ë";
            else if (startsWith(token, "eacute"sv)) out += "é";
            else if (startsWith(token, "egrave"sv)) out += "è";
            else if (startsWith(token, "ecirc"sv)) out += "ê";
            else if (startsWith(token, "euml"sv)) out += "ë";
            else if (startsWith(token, "Iacute"sv)) out += "Í";
            else if (startsWith(token, "Igrave"sv)) out += "Ì";
            else if (startsWith(token, "Icirc"sv)) out += "Î";
            else if (startsWith(token, "Iuml"sv)) out += "Ï";
            else if (startsWith(token, "iacute"sv)) out += "í";
            else if (startsWith(token, "igrave"sv)) out += "ì";
            else if (startsWith(token, "icirc"sv)) out += "î";
            else if (startsWith(token, "iuml"sv)) out += "ï";
            else if (startsWith(token, "Oacute"sv)) out += "Ó";
            else if (startsWith(token, "Ograve"sv)) out += "Ò";
            else if (startsWith(token, "Ocirc"sv)) out += "Ô";
            else if (startsWith(token, "Ouml"sv)) out += "Ö";
            else if (startsWith(token, "Otilde"sv)) out += "Õ";
            else if (startsWith(token, "oacute"sv)) out += "ó";
            else if (startsWith(token, "ograve"sv)) out += "ò";
            else if (startsWith(token, "ocirc"sv)) out += "ô";
            else if (startsWith(token, "ouml"sv)) out += "ö";
            else if (startsWith(token, "otilde"sv)) out += "õ";
            else if (startsWith(token, "Uacute"sv)) out += "Ú";
            else if (startsWith(token, "Ugrave"sv)) out += "Ù";
            else if (startsWith(token, "Ucirc"sv)) out += "Û";
            else if (startsWith(token, "Uuml"sv)) out += "Ü";
            else if (startsWith(token, "uacute"sv)) out += "ú";
            else if (startsWith(token, "ugrave"sv)) out += "ù";
            else if (startsWith(token, "ucirc"sv)) out += "û";
            else if (startsWith(token, "uuml"sv)) out += "ü";
            else if (startsWith(token, "Yacute"sv)) out += "Ý";
            else if (startsWith(token, "yacute"sv)) out += "ý";
            else if (startsWith(token, "yuml"sv)) out += "ÿ";

            else if (startsWith(token, "deg"sv)) out += "°";
            else if (startsWith(token, "plusmn"sv)) out += "±";
            else if (startsWith(token, "sup2"sv)) out += "²";
            else if (startsWith(token, "sup3"sv)) out += "³";
            else if (startsWith(token, "sup1"sv)) out += "¹";
            else if (startsWith(token, "nbsp"sv)) out += "º";
            else if (startsWith(token, "pound"sv)) out += "£";
            else if (startsWith(token, "cent"sv)) out += "¢";
            else if (startsWith(token, "frac14"sv)) out += "¼";
            else if (startsWith(token, "frac12"sv)) out += "½";
            else if (startsWith(token, "frac34"sv)) out += "¾";
            else if (startsWith(token, "iquest"sv)) out += "¿";
            else if (startsWith(token, "iexcl"sv)) out += "¡";
            else if (startsWith(token, "ETH"sv)) out += "Ð";
            else if (startsWith(token, "eth"sv)) out += "ð";
            else if (startsWith(token, "THORN"sv)) out += "Þ";
            else if (startsWith(token, "thorn"sv)) out += "þ";
            else if (startsWith(token, "AElig"sv)) out += "Æ";
            else if (startsWith(token, "aelig"sv)) out += "æ";
            else if (startsWith(token, "Oslash"sv)) out += "Ø";
            else if (startsWith(token, "curren"sv)) out += "¤";
            else if (startsWith(token, "Ccedil"sv)) out += "Ç";
            else if (startsWith(token, "ccedil"sv)) out += "ç";
            else if (startsWith(token, "szlig"sv)) out += "ß";
            else if (startsWith(token, "Ntilde"sv)) out += "Ñ";
            else if (startsWith(token, "ntilde"sv)) out += "ñ";
            else if (startsWith(token, "yen"sv)) out += "¥";
            else if (startsWith(token, "not"sv)) out += "¬";
            else if (startsWith(token, "ordf"sv)) out += "ª";
            else if (startsWith(token, "uml"sv)) out += "¨";
            else if (startsWith(token, "shy"sv)) out += "­";
            else if (startsWith(token, "macr"sv)) out += "¯";
            else if (startsWith(token, "micro"sv)) out += "µ";
            else if (startsWith(token, "middot"sv)) out += "·";
            else if (startsWith(token, "cedil"sv)) out += "¸";
            else if (startsWith(token, "ordm"sv)) out += "º";
            else if (startsWith(token, "times"sv)) out += "×";
            else if (startsWith(token, "divide"sv)) out += "÷";
            else if (startsWith(token, "oslash"sv)) out += "ø";
            continue;

        }
        else if (*from == '>' && !ampersand) {
            intoken = false;
            // process desired tokens
            if (startsWith(token, "sync type=\"Strongs\" value=\""sv)) {
                out += ' ';
                out += '<';
                for (unsigned int i = 27; token[i] != '\"'; i++)
                    out += token[i];
                out += '>';
                continue;
            }
            if (startsWith(token, "sync type=\"morph\" value=\""sv)) {
                out += ' ';
                out += '(';
                for (unsigned int i = 25; token[i] != '\"'; i++)
                    out += token[i];
                out += ')';
                continue;
            }
            if (startsWith(token, "note"sv)) {
                out += ' ';
                out += '[';
            }
            else if (startsWith(token, "br"sv))
                out += '\n';
            else if (startsWith(token, "/p"sv))
                out += '\n';
            else if (startsWith(token, "/note"sv)) {
                out += ']';
                out += ' ';
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
        else	out += *from;
    }

    std::string out2;
    for (auto const * from = out.c_str(); *from; from++) {  //loop to remove extra spaces
        if ((std::strchr(" \t\n\r", *from))) {
            while (*(from+1) && (std::strchr(" \t\n\r", *(from+1))))
                from++;
            out2 += " ";
        } else {
            out2 += *from;
        }
    }
    text = std::move(out2);
    return 0;
}


} /* namespace swordxx */
