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


void XMLTag::parse() const {
    int i;
    int start;
    std::string name;
    attributes.clear();

    if (!m_buf)
        return;
    for (i = 0; ((m_buf[i]) && (!charIsAlpha(m_buf[i]))); i++);
    for (; m_buf[i]; i++) {
        if (strchr("\t\r\n ", m_buf[i])) {
                        // Convert newlines, carriage returns and tabs to spaces
            m_buf[i] = ' ';

            for (; ((m_buf[i]) && (!charIsAlpha(m_buf[i]))); i++);
            if (m_buf[i]) {		// we have an attribute name
                start = i;
                // Deprecated: check for following whitespacee
                // Should be: for (; (buf[i] && buf[i] != '='; i++);
                for (; ((m_buf[i]) && (!strchr(" =", m_buf[i]))); i++);

                if (i-start)
                    name.assign(m_buf.get() + start, i - start);

                // The following does not allow for empty attributes
                //for (; ((buf[i]) && (strchr(" =\"\'", buf[i]))); i++);

                // skip space preceding the = sign
                // Deprecated: this is not part of the xml spec
                for (; m_buf[i] == ' '; i++) ;

                // skip the = sign
                if (m_buf[i])
                    i++;

                // skip space following the = sign
                // Deprecated: this is not part of the xml spec
                for (; m_buf[i] == ' '; i++) ;

                // remember and skip the quote sign
                char quoteChar = m_buf[i];
                if (quoteChar)
                    i++;

                if (m_buf[i]) {	// we have attribute value
                    start = i;
                    // Skip until matching quote character
                    while ((m_buf[i]) && (m_buf[i] != quoteChar))
                        ++i;

                    // Allow for empty quotes
                    attributes[name] = std::string(m_buf.get() + start, i - start);
                }
            }
        }

        // if there are no more characters left then quit
        if (!m_buf[i])
            break;

    }
    for (;i;i--) {
        if (m_buf[i] == '/')
            empty = true;
        if (!strchr(" \t\r\n>\t", m_buf[i]))
            break;
    }

    parsed = true;
}


XMLTag::XMLTag(const char *tagString) {
    setText(tagString);
}


XMLTag::XMLTag(const XMLTag& t)
    : m_name(t.m_name)
    , attributes(t.attributes)
{
    parsed = t.parsed;
    empty = t.empty;
    endTag = t.endTag;
    if (t.m_buf) {
        auto const len(std::strlen(t.m_buf.get()));
        m_buf = std::make_unique<char[]>(len + 1u);
        std::memcpy(m_buf.get(), t.m_buf.get(), len + 1);
    }
}


void XMLTag::setText(const char *tagString) {
    parsed = false;
    empty  = false;
    endTag = false;

    m_buf.reset();

    if (!tagString)        // assert tagString before proceeding
        return;

    auto const bufLength(std::strlen(tagString) + 1u);
    m_buf = std::make_unique<char[]>(bufLength);
    std::memcpy(m_buf.get(), tagString, bufLength);

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

    for (StringPairMap::const_iterator it = attributes.begin(); it != attributes.end(); it++)
        retVal.push_back(it->first.c_str());

    return retVal;
}


const char *XMLTag::getPart(const char *buf, int partNum, char partSplit) const {
    for (; (buf && partNum); partNum--) {
        buf = strchr(buf, partSplit);
        if (buf)
            buf++;
    }
    if (buf) {
        const char *end = strchr(buf, partSplit);
        junkBuf = buf;
        if (end)
            junkBuf.resize(end - buf, '\0');
        return junkBuf.c_str();
    }
    return nullptr;
}


int XMLTag::attributePartCount(std::string const & attribName, char partSplit) const {
    auto buf(attribute(attribName));
    return std::count(buf.begin(), buf.end(), partSplit)+1;
}


std::string XMLTag::attribute(std::string const & attribName, int partNum, char partSplit) const {
    if (!parsed)
        parse();

    StringPairMap::const_iterator it = attributes.find(attribName);
    if (it == attributes.end())
        return std::string();
    if (partNum > -1)
        return getPart(it->second.c_str(), partNum, partSplit);
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
    for (StringPairMap::iterator it = attributes.begin(); it != attributes.end(); it++) {
        //tag.appendFormatted(" %s=\"%s\"", it->first.c_str(), it->second.c_str());
        tag.push_back(' ');
        tag.append(it->first.c_str());
        tag.append((strchr(it->second.c_str(), '\"')) ? "=\'" : "=\"");
        tag.append(it->second.c_str());
        tag.push_back((strchr(it->second.c_str(), '\"'))? '\'' : '\"');
    }

    if (isEmpty())
        tag.push_back('/');

    tag.push_back('>');


    m_buf = std::make_unique<char[]>(tag.length() + 1);
    std::strcpy(m_buf.get(), tag.c_str());

    return tag;
}


// if an eID is provided, then we check to be sure we have an attribute <tag eID="xxx"/> value xxx equiv to what is given us
// otherwise, we return if we're a simple XML end </tag>.
bool XMLTag::isEndTag(const char *eID) const {
    if (eID) {
        return (std::string(eID) == attribute("eID"));
    }
    return endTag;
}


} /* namespace swordxx */

