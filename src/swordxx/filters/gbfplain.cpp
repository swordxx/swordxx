/******************************************************************************
 *
 *  gbfplain.cpp -    SWFilter descendant to strip out all GBF tags or
 *            convert to ASCII rendered symbols
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "gbfplain.h"

#include <cstdlib>
#include <string>


namespace swordxx {


GBFPlain::GBFPlain() {
}


char GBFPlain::processText (std::string &text, const SWKey *key, const SWModule *module)
{
    (void) key;
    (void) module;
    char token[2048];
    int tokpos = 0;
    bool intoken = false;
    std::string out;

    for (auto const * from = text.c_str(); *from; ++from) {
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
                case 'G':               // Greek
                case 'H':               // Hebrew
                case 'T':               // Tense
                    out.append(" <");
                    //for (char *tok = token + 2; *tok; tok++)
                    //    out += *tok;
                    out.append(token+2);
                    out.append("> ");
                    continue;
                }
                break;
            case 'R':
                switch(token[1]) {
                case 'F':               // footnote begin
                    out.append(" [");
                    continue;
                case 'f':               // footnote end
                    out.append("] ");
                    continue;
                }
                break;
            case 'C':
                switch(token[1]) {
                case 'A':               // ASCII value
                    out.push_back((char)std::atoi(&token[2]));
                    continue;
                case 'G':
                    out.push_back('>');
                    continue;
/*                                Bug in WEB
                case 'L':
                    *to++ = '<';
                    continue;
*/
                case 'L':	//        Bug in WEB.  Use above entry when fixed
                case 'N':               // new line
                    out.push_back('\n');
                    continue;
                case 'M':               // new paragraph
                    out.append("\n\n");
                    continue;
                }
                break;
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
        else	out.push_back(*from);
    }
    text = std::move(out);
    return 0;
}


} /* namespace swordxx */

