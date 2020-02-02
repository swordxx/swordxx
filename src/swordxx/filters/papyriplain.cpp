/******************************************************************************
 *
 *  papyriplain.cpp -    SWFilter descendant to strip out all Papyri tags
 *
 * Copyright 2005-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "papyriplain.h"

#include <cstdlib>
#include <cstring>


namespace swordxx {


PapyriPlain::PapyriPlain()
    : SWOptionFilter(
          "Strip Papyri tags",
          "Strips all Papyri tags",
          {"On"})
{}


char PapyriPlain::processText (std::string &text, const SWKey *key, const SWModule *module)
{
    (void) key;
    (void) module;
    std::string out;

    for (auto const * from = text.c_str(); *from; ++from) {

        // remove hyphen and whitespace if that is all that separates words
        // also be sure we're not a double hyphen '--'
        if ((*from == '-') && (out.size() > 0) && (out.back() != '-')) {
            char remove = 0;
            const char *c;
            for (c = from+1; *c; c++) {
                if ((*c == 10) || (*c == 13)) {
                    remove = 1;
                }
                if (!std::strchr(" \t\n", *c)) {
                    if (remove) remove++;
                    break;
                }
            }
            if (remove > 1) {
                from = c-1;
                continue;
            }
        }

        // remove all newlines
        if ((*from == 10) || (*from == 13)) {
            if ((out.length()>1) && (out[out.size()-2] != ' ') && (*(from+1) != ' '))
                out.push_back(' ');
            continue;
        }


        // strip odd characters
        switch (*from) {
        case '(':
        case ')':
        case '[':
        case ']':
        case '{':
        case '}':
        case '<':
        case '>':
            continue;
        }

        // if we've made it this far
        out.push_back(*from);

    }
    text = std::move(out);
    return 0;
}

} /* namespace swordxx */
