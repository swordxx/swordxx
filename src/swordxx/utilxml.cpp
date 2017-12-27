/******************************************************************************
 *
 *  utilxml.cpp -    Implementaion of utility classes to handle
 *            XML processing
 *
 * $Id$
 *
 * Copyright 2003-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "utilxml.h"

#include <algorithm>
#include "utilstr.h"


namespace swordxx {

namespace {

std::string getPart(std::string const & buf, int partNum, char const partSplit)
{
    auto tokenStart(buf.begin());
    auto const stringEnd(buf.end());
    for (; partNum; --partNum, ++tokenStart) {
        tokenStart = std::find(tokenStart, stringEnd, partSplit);
        if (tokenStart == stringEnd)
            return std::string();
    }

    auto const tokenEnd(std::find(tokenStart, stringEnd, partSplit));
    return std::string(tokenStart, tokenEnd);
}

} // anonymous namespace


void XMLTag::parse() const {
    attributes.clear();

    auto const bufStart(m_buf.begin());
    auto const bufEnd(m_buf.end());
    auto it(bufStart);
    while ((it != bufEnd) && !charIsAlpha(*it))
        ++it;
    for (; it != bufEnd; ++it) {
        if (strchr("\t\r\n ", *it)) {
                        // Convert newlines, carriage returns and tabs to spaces
            *it = ' ';

            while ((it != bufEnd) && !charIsAlpha(*it))
                ++it;
            if (it != bufEnd) {		// we have an attribute name
                std::string name;
                {
                    auto const start(it);
                    // Deprecated: check for following whitespacee
                    // Should be: for (; (buf[i] && buf[i] != '='; i++);
                    while ((it != bufEnd) && (!strchr(" =", *it)))
                        ++it;

                    if (it != start)
                        name.assign(start, it);
                }

                // The following does not allow for empty attributes
                //for (; ((buf[i]) && (strchr(" =\"\'", buf[i]))); i++);

                // skip space preceding the = sign
                // Deprecated: this is not part of the xml spec
                while (*it == ' ')
                    ++it;

                // skip the = sign
                if (it != bufEnd)
                    ++it;

                // skip space following the = sign
                // Deprecated: this is not part of the xml spec
                while (*it == ' ')
                    ++it;

                // remember and skip the quote sign
                if (it != bufEnd) {
                    auto const quoteChar(*it);
                    ++it;

                    if (it != bufEnd) {	// we have attribute value
                        auto const start(it);
                        // Skip until matching quote character
                        while ((it != bufEnd) && (*it != quoteChar))
                            ++it;

                        // Allow for empty quotes
                        attributes[name] = std::string(start, it);
                    }
                }
            }
        }

        // if there are no more characters left then quit
        if (it == bufEnd)
            break;

    }
    for (; it != bufStart; --it) {
        if (*it == '/')
            empty = true;
        if (!strchr(" \t\r\n>\t", *it))
            break;
    }

    parsed = true;
}


XMLTag::XMLTag(const char *tagString) {
    setText(tagString);
}


XMLTag::XMLTag(XMLTag const &) = default;


void XMLTag::setText(const char *tagString) {
    parsed = false;
    empty  = false;
    endTag = false;

    m_buf = "";

    if (!tagString)        // assert tagString before proceeding
        return;

    m_buf = tagString;

    int start = 0;
    int i;

    // skip beginning silliness
    for (i = 0; ((tagString[i]) && (!charIsAlpha(tagString[i]))); i++) {
        if (tagString[i] == '/')
            endTag = true;
    }
    start = i;
    for (; ((tagString[i]) && (!strchr("\t\r\n />", tagString[i]))); i++);
    if (i-start) {
        m_name.assign(tagString + start, i - start);
        if (tagString[i] == '/')
            empty = true;
    }
}

std::list<std::string> XMLTag::attributeNames() const {
    std::list<std::string> retVal;

    if (!parsed)
        parse();

    for (auto const & vp : attributes)
        retVal.push_back(vp.first);

    return retVal;
}

int XMLTag::attributePartCount(std::string const & attribName, char partSplit) const {
    auto buf(attribute(attribName));
    return std::count(buf.begin(), buf.end(), partSplit)+1;
}


std::string XMLTag::attribute(std::string const & attribName, int partNum, char partSplit) const {
    if (!parsed)
        parse();

    auto const it(attributes.find(attribName));
    if (it == attributes.end())
        return std::string();
    if (partNum > -1)
        return getPart(it->second, partNum, partSplit);
    return it->second;
}


void XMLTag::setAttribute(std::string const & attribName, const char *attribValue, int partNum, char partSplit) {
    if (!parsed)
        parse();

    std::string newVal = "";
    // set part of an attribute
    if (partNum > -1) {
        auto wholeAttr(attribute(attribName));
        int attrCount = attributePartCount(attribName, partSplit);
        for (int i = 0; i < attrCount; i++) {
            if (i == partNum) {
                if (attribValue) {
                    newVal += attribValue;
                    newVal += partSplit;
                }
                else {
                    // discard this part per null attribValue
                }
            }
            else {
                newVal += getPart(wholeAttr.c_str(), i, partSplit);
                newVal += partSplit;
            }
        }
        if (newVal.length()) newVal.pop_back();    // discard the last partSplit
        attribValue = (!attribValue && !newVal.length())
                      ? nullptr
                      : newVal.c_str();
    }

    // perform the actual set
    if (attribValue)
        attributes[attribName] = attribValue;
    else    attributes.erase(attribName);
}

void XMLTag::eraseAttribute(std::string const & attribName) noexcept
{ attributes.erase(attribName); }

std::string XMLTag::toString() const {
    std::string tag("<");
    if (!parsed)
        parse();

    if (isEndTag())
        tag.push_back('/');

    tag.append(name());
    for (auto const & vp : attributes) {
        tag.push_back(' ');
        tag.append(vp.first);
        tag.append((std::strchr(vp.second.c_str(), '\"')) ? "=\'" : "=\"");
        tag.append(vp.second);
        tag.push_back((std::strchr(vp.second.c_str(), '\"'))? '\'' : '\"');
    }

    if (isEmpty())
        tag.push_back('/');

    tag.push_back('>');

    return tag;
}


// if an eID is provided, then we check to be sure we have an attribute <tag eID="xxx"/> value xxx equiv to what is given us
// otherwise, we return if we're a simple XML end </tag>.
bool XMLTag::isEndTag(const char *eID) const
{ return eID ? attribute("eID") == eID : endTag; }


} /* namespace swordxx */

