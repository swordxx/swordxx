/******************************************************************************
 *
 * Copyright 2016 Jaak Ristioja
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

#include "url.h"

#include <cstdio>
#include "utilstr.h"


namespace swordxx {
namespace URL {

std::string encode(std::string const & url) {
    std::string r;
    for (auto const & c : url) {
        switch (c) {
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':

        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':

        case '-': case '_': case '.': case '!': case '~':
        case '*': case '\'': case '(': case ')':
            r += c;
            break;
        case ' ':
            r += '+';
            break;
        default:
            r += formatted("%%%-.2X", c);
            break;
        }
    }
    return r;
}

} /* namespace URL { */
} /* namespace swordxx { */
