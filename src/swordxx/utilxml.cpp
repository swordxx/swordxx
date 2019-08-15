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
#include <cstring>
#include "utilstr.h"


namespace swordxx {

XMLTag::XMLTag(XMLTag &&) = default;
XMLTag::XMLTag(XMLTag const &) = default;

XMLTag::XMLTag(char const * const tagString) {
    m_isEmpty  = false;
    m_isEndTag = false;

    if (!tagString)        // assert tagString before proceeding
        return;

    auto const bufStart(tagString);
    auto const bufEnd(bufStart + std::strlen(bufStart));

    {
        auto it(bufStart);

        // skip beginning silliness
        while ((it != bufEnd) && !charIsAlpha(*it)) {
            if (*it == '/')
                m_isEndTag = true;
            ++it;
        }
        auto const start(it);
        while ((it != bufEnd) && !std::strchr("\t\r\n />", *it))
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
        if (std::strchr("\t\r\n ", *it)) {
            do {
                ++it;
            } while ((it != bufEnd) && !charIsAlpha(*it));
            if (it != bufEnd) {		// we have an attribute name
                std::string name;
                {
                    auto const start(it);
                    // Deprecated: check for following whitespacee
                    // Should be: for (; (buf[i] && buf[i] != '='; i++);
                    while ((it != bufEnd) && (!std::strchr(" =", *it)))
                        ++it;

                    if (it != start)
                        name.assign(start, it);
                }

                // The following does not allow for empty attributes
                //for (; ((buf[i]) && (std::strchr(" =\"\'", buf[i]))); i++);

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
        if (!std::strchr(" \t\r\n>\t", *it))
            break;
    }
}

std::list<std::string> XMLTag::attributeNames() const {
    std::list<std::string> retVal;
    for (auto const & vp : m_attributes)
        retVal.push_back(vp.first);
    return retVal;
}

std::string XMLTag::attribute(std::string const & attribName) const
{
    auto const it(m_attributes.find(attribName));
    return (it != m_attributes.end()) ? it->second : std::string();
}

void XMLTag::setAttribute(std::string const & attribName,
                          std::string attribValue)
{ m_attributes[attribName] = std::move(attribValue); }

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
