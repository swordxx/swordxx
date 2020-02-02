/***************************************************************************
 *
 *  rtfhtml.cpp -    filter to convert RTF to HTML
 *
 * Copyright 1999 The team of Bibletime (info@bibletime.de)
 * Copyright 2000-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "rtfhtml.h"

#include <cstdint>
#include <cstdlib>
#include <string>
#include "../sysdata.h"
#include "../unicode.h"
#include "../utilstr.h"


namespace swordxx {

RTFHTML::RTFHTML()
{
}


char RTFHTML::processText(std::string &text, const SWKey *key, const SWModule *module)
{
    using namespace std::literals::string_view_literals;

    (void) key;
    (void) module;
    bool center = false;

    std::string newText;
    for (const char * from = text.c_str(); *from; ++from) {
        if (*from == '\\') { // a RTF command
            // \u12345?
            if ( *(from+1) == 'u' && (*(from+2) == '-' || charIsDigit(*(from+2)))) {
                from += 2;
                const char *end = from;
                while (charIsDigit(*++end));
                std::string num;
                num.append(from, end-from);
                int16_t n = std::atoi(num.c_str());
                uint32_t u = (uint16_t)n;
                newText.append(utf8FromCodepoint(u));
                from += (end-from);
                continue;
            }
            if (startsWith(from+1, "pard"sv) )
                //(from[1] == 'p') && (from[2] == 'a') && (from[3] == 'r') && (from[4] == 'd'))
            { // switch all modifiers off
                if (center)
                {
                    newText += "</center>";
                    center = false;
                }
                from += 4;
                continue;
            }
            if (startsWith(from+1, "par"sv) )
                //(from[1] == 'p') && (from[2] == 'a') && (from[3] == 'r'))
            {
                newText += "<p/>\n";
                from += 3;
                continue;
            }
            if (startsWith(from+1, "qc"sv) )
            //(from[1] == 'q') && (from[2] == 'c')) // center on
            {
                if (!center)
                {
                    newText += "<center>";
                    center = true;
                }
                from += 2;
                continue;
            }
        } else if (*from == '&') {
            newText.append("&amp;");
            continue;
        } else if (*from == '<') {
            newText.append("&lt;");
            continue;
        }

        newText += *from;
    }
    text = std::move(newText);
    return 0;
}

} /* namespace swordxx */
