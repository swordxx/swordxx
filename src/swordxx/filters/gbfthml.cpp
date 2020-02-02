/***************************************************************************
 *
 *  gbfthml.cpp -    GBF to ThML filter
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

#include "gbfthml.h"

#include <cstdlib>
#include <string>


namespace swordxx {


GBFThML::GBFThML()
{
}


char GBFThML::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    (void) module;
    char token[2048];
    int tokpos = 0;
    bool intoken     = false;
    const char *tok;

    std::string out;

    for (auto const * from = text.c_str(); *from; from++) {
        if (*from == '<') {
            intoken = true;
            tokpos = 0;
            token[0] = 0;
            token[1] = 0;
            token[2] = 0;
            continue;
        }
        if (*from == '>') {
            intoken = false;
            // process desired tokens
            switch (*token) {
            case 'W':    // Strongs
              switch(token[1]) {
              case 'G':
              case 'H':
                out += "<sync type=\"Strongs\" value=\"";
                for (tok = token + 1; *tok; tok++)
                 out += *tok;
                out += "\" />";
                continue;

              case 'T':               // Tense
                out += "<sync type=\"Morph\" value=\"";
                for (tok = token + 2; *tok; tok++)
                 out += *tok;
                out += "\" />";
                continue;
                }
              break;
            case 'R':
              switch(token[1])
                {
                case 'X':
                 out += "<a href=\"";
                 for (tok = token + 3; *tok; tok++) {
                if(*tok != '<' && *tok+1 != 'R' && *tok+2 != 'x') {
                  out += *tok;
                }
                else {
                  break;
                }
                 }
                 out += "\">";
                 continue;
                case 'x':
                 out += "</a>";
                 continue;
                case 'F':               // footnote begin
                 out += "<note>";
                 continue;
                case 'f':               // footnote end
                 out += "</note>";
                 continue;
                }
              break;
            case 'F':            // font tags
              switch(token[1])
                {
                case 'N':
                 out += "<font face=\"";
                 for (tok = token + 2; *tok; tok++)
                    out += *tok;
                out += "\">";
                 continue;
                case 'n':
                 out += "</font>";
                 continue;
                case 'I':        // italic start
                 out += "<i>";
                 continue;
                case 'i':        // italic end
                 out += "</i>";
                 continue;
                case 'B':        // bold start
                 out += "<b>";
                 continue;
                case 'b':        // bold end
                 out += "</b>";
                 continue;

                case 'R':        // words of Jesus begin
                 out += "<font color=\"#ff0000\">";
                 continue;
                case 'r':        // words of Jesus end
                 out += "</font>";
                 continue;
                case 'U':        // Underline start
                 out += "<u>";
                 continue;
                case 'u':        // Underline end
                 out += "</u>";
                 continue;
                case 'O':        // Old Testament quote begin
                 out += "<cite>";
                 continue;
                case 'o':        // Old Testament quote end
                 out += "</cite>";
                 continue;
                case 'S':        // Superscript begin
                 out += "<sup>";
                 continue;
                case 's':        // Superscript end
                 out += "</sup>";
                 continue;
                case 'V':        // Subscript begin
                 out += "<sub>";
                 continue;
                case 'v':        // Subscript end
                 out += "</sub>";
                 continue;
                }
              break;
            case 'C':            // special character tags
              switch(token[1])
                {
                case 'A':               // ASCII value
                  out += (char)std::atoi(&token[2]);
                  continue;
                case 'G':
                  //*to++ = ' ';
                  continue;
                case 'L':               // line break
                 out += "<br /> ";
                  continue;
                case 'M':               // new paragraph
                 out += "<p />";
                  continue;
                case 'T':
                  //*to++ = ' ';
                  continue;
                }
              break;
            case 'T':            // title formatting
              switch(token[1])
                {
                case 'T':               // Book title begin
                 out += "<big>";
                 continue;
                case 't':
                 out += "</big>";
                 continue;
                case 'S':
                 out += "<div class=\"sechead\">";
                 continue;
                case 's':
                 out += "</div>";
                 continue;
                }
              break;

            case 'P':            // special formatting
              switch(token[1]) {
                case 'P':               // Poetry begin
                 out += "<verse>";
                 continue;
                case 'p':
                 out += "</verse>";
                 continue;
                }
              break;
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
        else    out += *from;
    }
    text = std::move(out);
    return 0;
}


} /* namespace swordxx */
