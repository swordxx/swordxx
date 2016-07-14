/***************************************************************************
 *
 *  rtfhtml.cpp -    filter to convert RTF to HTML
 *
 * $Id$
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

#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <string>
#include "../sysdata.h"
#include "../utilstr.h"


namespace swordxx {

RTFHTML::RTFHTML()
{
}


char RTFHTML::processText(std::string &text, const SWKey *key, const SWModule *module)
{
    bool center = false;

    const char *from;
    std::string orig = text;
    from = orig.c_str();
    for (text = ""; *from; from++)
    {
        if (*from == '\\') // a RTF command
        {
            // \u12345?
            if ( *(from+1) == 'u' && (*(from+2) == '-' || isdigit(*(from+2)))) {
                from += 2;
                const char *end = from;
                while (isdigit(*++end));
                std::string num;
                num.append(from, end-from);
                int16_t n = atoi(num.c_str());
                uint32_t u = (uint16_t)n;
                text.append(getUTF8FromUniChar(u));
                from += (end-from);
                continue;
            }
            if ( !strncmp(from+1, "pard", 4) )
                //(from[1] == 'p') && (from[2] == 'a') && (from[3] == 'r') && (from[4] == 'd'))
            { // switch all modifiers off
                if (center)
                {
                    text += "</center>";
                    center = false;
                }
                from += 4;
                continue;
            }
            if ( !strncmp(from+1, "par", 3) )
                //(from[1] == 'p') && (from[2] == 'a') && (from[3] == 'r'))
            {
                text += "<p/>\n";
                from += 3;
                continue;
            }
            if (from[1] == ' ')
            {
                from += 1;
                continue;
            }
            if ( !strncmp(from+1, "qc", 2) )
            //(from[1] == 'q') && (from[2] == 'c')) // center on
            {
                if (!center)
                {
                    text += "<center>";
                    center = true;
                }
                from += 2;
                continue;
            }
        }

        text += *from;
    }
    return 0;
}

} /* namespace swordxx */
