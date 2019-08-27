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

#include <cassert>
#include <cstring>
#include "utilstr.h"


namespace swordxx {
namespace {

template <typename PrefixDoubleQuote,
          typename PrefixSingleQuote>
std::string & appendXmlAttributeValue(std::string & r,
                                      std::string_view attrValueView,
                                      PrefixDoubleQuote && prefixDoubleQuote,
                                      PrefixSingleQuote && prefixSingleQuote)
{
    using namespace std::literals::string_view_literals;

    static constexpr unsigned const HAS_SINGLE_QUOTE = 0b01;
    static constexpr unsigned const HAS_DOUBLE_QUOTE = 0b10;
    unsigned includesQuotes = 0u;
    for (auto pos = attrValueView.find_first_of("\"'"sv);
         pos != std::string::npos;
         pos = attrValueView.find_first_of("\"'"sv, pos + 1u))
        includesQuotes |= ((attrValueView[pos] == '"')
                           ? HAS_DOUBLE_QUOTE
                           : HAS_SINGLE_QUOTE);

    switch (includesQuotes) {
    case HAS_SINGLE_QUOTE | HAS_DOUBLE_QUOTE:
        r += std::forward<PrefixDoubleQuote>(prefixDoubleQuote);
        {
            auto pos = attrValueView.find('"');
            assert(pos != std::string::npos);
            do {
                r.append(attrValueView.data(), pos);
                r += "&quot;"sv;
                attrValueView.remove_prefix(pos + 1u);
                pos = attrValueView.find('"', 0u);
            } while (pos != std::string::npos);
            r += attrValueView;
        }
        r.push_back('"');
        break;
    case HAS_DOUBLE_QUOTE:
        r += std::forward<PrefixSingleQuote>(prefixSingleQuote);
        r += attrValueView;
        r.push_back('\'');
        break;
    case HAS_SINGLE_QUOTE:
    default:
        r += std::forward<PrefixDoubleQuote>(prefixDoubleQuote);
        r += attrValueView;
        r.push_back('"');
        break;
    }
    return r;
}

} // anonymous namespace

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

std::string XMLTag::attribute(std::string const & attribName) const
{
    auto const it(m_attributes.find(attribName));
    return (it != m_attributes.end()) ? it->second : std::string();
}

std::string XMLTag::toString() const {
    using namespace std::literals::string_view_literals;
    if (m_isEndTag) {
        std::string r;
        r.reserve(m_name.size() + 3u);
        r += "</"sv;
        r += m_name;
        r.push_back('>');
        return r;
    }

    if (m_attributes.empty()) {
        std::string r;
        if (isEmpty()) {
            r.reserve(m_name.size() + 3u);
            r.push_back('<');
            r += m_name;
            r += "/>"sv;
        } else {
            r.reserve(m_name.size() + 2u);
            r.push_back('<');
            r += m_name;
            r.push_back('>');
        }
        return r;
    }

    std::string r('<' + m_name);
    for (auto const & [attrName, attrValue] : m_attributes) {
        r.push_back(' ');
        r += attrName;
        appendXmlAttributeValue(r, attrValue, "=\""sv, "='"sv);
    }
    if (m_isEmpty) {
        r += "/>"sv;
    } else {
        r.push_back('>');
    }
    return r;
}

XMLTag & XMLTag::operator=(XMLTag &&) = default;
XMLTag & XMLTag::operator=(XMLTag const &) = default;

std::string formatXmlAttributeValue(std::string_view value) {
    std::string r;
    appendXmlAttributeValue(r, value, '"', '\'');
    return r;
}

} /* namespace swordxx */
