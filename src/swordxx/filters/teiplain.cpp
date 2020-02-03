/***************************************************************************
 *
 *  teiplain.cpp -    TEI to Plaintext filter
 *
 * Copyright 2006-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "teiplain.h"

#include <cstdlib>
#include <cstring>


namespace swordxx {

TEIPlain::MyUserData::~MyUserData() noexcept = default;

TEIPlain::TEIPlain() {
    setEscapeStringCaseSensitive(true);

    addEscapeStringSubstitute("amp", "&");
    addEscapeStringSubstitute("apos", "'");
    addEscapeStringSubstitute("lt", "<");
    addEscapeStringSubstitute("gt", ">");
    addEscapeStringSubstitute("quot", "\"");

    setTokenCaseSensitive(true);
}

std::unique_ptr<BasicFilterUserData> TEIPlain::createUserData(
        SWModule const * module,
        SWKey const * key)
{ return std::make_unique<MyUserData>(module, key); }

bool TEIPlain::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
  // manually process if it wasn't a simple substitution
    if (!substituteToken(buf, token)) {
        //MyUserData * u = static_cast<MyUserData *>(userData);
        XMLTag tag(token);

        // <p> paragraph tag
        if (tag.name() == "p") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {    // non-empty start tag
                buf += "\n";
            }
            else if (tag.isEndTag()) {    // end tag
                buf += "\n";
                userData->supressAdjacentWhitespace = true;
            }
            else {                    // empty paragraph break marker
                buf += "\n\n";
                userData->supressAdjacentWhitespace = true;
            }
        }

        // <entryFree>
        else if (tag.name() == "entryFree") {
            std::string n = tag.attribute("n");
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                    if (n != "") {
                    buf += n;
                    buf += ". ";
                }
            }
        }

        // <sense>
        else if (tag.name() == "sense") {
            std::string n = tag.attribute("n");
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                    if (n != "") {
                    buf += n;
                    buf += ". ";
                }
            }
            else if (tag.isEndTag()) {
                            buf += "\n";
            }
        }

        // <div>
        else if (tag.name() == "div") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf.append("\n\n\n");
            }
        }

        // <etym>
        else if (tag.name() == "etym") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "[";
            }
            else if (tag.isEndTag()) {
                    buf += "]";
            }
        }

        // <list> <item>  This implementation does not distinguish between forms of lists
        // it would be nice if a numbered list could be added
        else if (tag.name() == "list") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "\n";
            } else if (tag.isEndTag()) {
                buf += "\n";
            }
        } else if (tag.name() == "item") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "\t* ";
            } else if (tag.isEndTag()) {
                buf += "\n";
            }
        } else {
            return false;  // we still didn't handle token
        }
    }
    return true;
}


} /* namespace swordxx */
