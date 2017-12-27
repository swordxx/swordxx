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

XMLTag::XMLTag(XMLTag &&) = default;
XMLTag::XMLTag(XMLTag const &) = default;

XMLTag::XMLTag(char const * const tagString) {
    m_isEmpty  = false;
    m_isEndTag = false;

    if (!tagString)        // assert tagString before proceeding
        return;

    std::string m_buf(tagString);

    auto const bufStart(m_buf.begin());
    auto const bufEnd(m_buf.end());

    {
        auto it(m_buf.begin());

        // skip beginning silliness
        while ((it != bufEnd) && !charIsAlpha(*it)) {
            if (*it == '/')
                m_isEndTag = true;
            ++it;
        }
        auto const start(it);
        while ((it != bufEnd) && !strchr("\t\r\n />", *it))
            ++it;
        m_name.assign(start, it);
        if (*it == '/')
            m_isEmpty = true;
    }

    m_attributes.clear();

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
                        m_attributes[name] = std::string(start, it);
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
            m_isEmpty = true;
        if (!strchr(" \t\r\n>\t", *it))
            break;
    }
}

std::list<std::string> XMLTag::attributeNames() const {
    std::list<std::string> retVal;
    for (auto const & vp : m_attributes)
        retVal.push_back(vp.first);
    return retVal;
}

int XMLTag::attributePartCount(std::string const & attribName,
                               char partSplit) const
{
    auto buf(attribute(attribName));
    return std::count(buf.begin(), buf.end(), partSplit)+1;
}

std::string XMLTag::attribute(std::string const & attribName,
                              int partNum,
                              char partSplit) const
{
    auto const it(m_attributes.find(attribName));
    if (it == m_attributes.end())
        return std::string();
    if (partNum > -1)
        return getPart(it->second, partNum, partSplit);
    return it->second;
}

void XMLTag::setAttribute(std::string const & attribName,
                          char const * attribValue,
                          int partNum,
                          char partSplit)
{
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
        m_attributes[attribName] = attribValue;
    else    m_attributes.erase(attribName);
}

void XMLTag::eraseAttribute(std::string const & attribName) noexcept
{ m_attributes.erase(attribName); }

std::string XMLTag::toString() const {
    std::string tag("<");
    if (isEndTag())
        tag.push_back('/');

    tag.append(name());
    for (auto const & vp : m_attributes) {
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

bool XMLTag::isEndTag(char const * const eID) const noexcept {
    if (!eID)
        return m_isEndTag;
    auto const it(m_attributes.find("eID"));
    if (it == m_attributes.end())
        return !*eID;
    return it->second == eID;
}

XMLTag & XMLTag::operator=(XMLTag &&) = default;
XMLTag & XMLTag::operator=(XMLTag const &) = default;

} /* namespace swordxx */
