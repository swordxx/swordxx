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
#include <cctype>
#include "utilstr.h"


namespace swordxx {


void XMLTag::parse() const {
    int i;
    int start;
    char * name = nullptr;
    char * value = nullptr;
    attributes.clear();

    if (!buf)
        return;
    for (i = 0; ((buf[i]) && (!isalpha(buf[i]))); i++);
    for (; buf[i]; i++) {
        if (strchr("\t\r\n ", buf[i])) {
                        // Convert newlines, carriage returns and tabs to spaces
            buf[i] = ' ';

            for (; ((buf[i]) && (!isalpha(buf[i]))); i++);
            if (buf[i]) {		// we have an attribute name
                start = i;
                // Deprecated: check for following whitespacee
                // Should be: for (; (buf[i] && buf[i] != '='; i++);
                for (; ((buf[i]) && (!strchr(" =", buf[i]))); i++);

                if (i-start) {
                    delete[] name;
                    name = new char [ (i-start) + 1 ];
                    strncpy(name, buf+start, i-start);
                    name[i-start] = 0;
                }

                // The following does not allow for empty attributes
                //for (; ((buf[i]) && (strchr(" =\"\'", buf[i]))); i++);

                // skip space preceding the = sign
                // Deprecated: this is not part of the xml spec
                for (; buf[i] == ' '; i++) ;

                // skip the = sign
                if (buf[i])
                    i++;

                // skip space following the = sign
                // Deprecated: this is not part of the xml spec
                for (; buf[i] == ' '; i++) ;

                // remember and skip the quote sign
                char quoteChar = buf[i];
                if (quoteChar)
                    i++;

                if (buf[i]) {	// we have attribute value
                    start = i;
                    // Skip until matching quote character
                    while ((buf[i]) && (buf[i] != quoteChar))
                        ++i;

                    // Allow for empty quotes
                    delete[] value;
                    value = new char [ (i-start) + 1 ];
                    if (i-start) {
                        strncpy(value, buf+start, i-start);
                    }
                    value[i-start] = 0;
                    attributes[name] = value;
                }
            }
        }

        // if there are no more characters left then quit
        if (!buf[i])
            break;

    }
    for (;i;i--) {
        if (buf[i] == '/')
            empty = true;
        if (!strchr(" \t\r\n>\t", buf[i]))
            break;
    }

    parsed = true;
    delete[] name;
    delete[] value;
}


XMLTag::XMLTag(const char *tagString) {

    name   = nullptr;
    buf    = nullptr;
    setText(tagString);
}


XMLTag::XMLTag(const XMLTag& t) : attributes(t.attributes)  {
    parsed = t.parsed;
    empty = t.empty;
    endTag = t.endTag;
    if (t.buf) {
        int len = strlen(t.buf);
        buf = new char[len + 1];
        memcpy(buf, t.buf, len + 1);
    }
    if (t.name) {
        int len = strlen(t.name);
        name = new char[len + 1];
        memcpy(name, t.name, len + 1);
    }
}


void XMLTag::setText(const char *tagString) {
    parsed = false;
    empty  = false;
    endTag = false;

    delete[] buf;
    buf = nullptr;

    if (!tagString)        // assert tagString before proceeding
        return;

    stdstr(&buf, tagString);

    int start = 0;
    int i;

    // skip beginning silliness
    for (i = 0; ((tagString[i]) && (!isalpha(tagString[i]))); i++) {
        if (tagString[i] == '/')
            endTag = true;
    }
    start = i;
    for (; ((tagString[i]) && (!strchr("\t\r\n />", tagString[i]))); i++);
    if (i-start) {
        delete[] name;
        name = new char [ (i-start) + 1 ];
        strncpy(name, tagString+start, i-start);
        name[i-start] = 0;
        if (tagString[i] == '/')
            empty = true;
    }
}


XMLTag::~XMLTag() {
    delete[] buf;
    delete[] name;
}


const StringList XMLTag::getAttributeNames() const {
    StringList retVal;

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


int XMLTag::getAttributePartCount(const char *attribName, char partSplit) const {
    auto buf(getAttribute(attribName));
    return std::count(buf.begin(), buf.end(), partSplit)+1;
}


std::string XMLTag::getAttribute(const char *attribName, int partNum, char partSplit) const {
    if (!parsed)
        parse();

    StringPairMap::const_iterator it = attributes.find(attribName);
    if (it == attributes.end())
        return std::string();
    if (partNum > -1)
        return getPart(it->second.c_str(), partNum, partSplit);
    return it->second;
}


void XMLTag::setAttribute(const char *attribName, const char *attribValue, int partNum, char partSplit) {
    if (!parsed)
        parse();

    std::string newVal = "";
    // set part of an attribute
    if (partNum > -1) {
        auto wholeAttr(getAttribute(attribName));
        int attrCount = getAttributePartCount(attribName, partSplit);
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


const char *XMLTag::toString() const {
    std::string tag = "<";
    if (!parsed)
        parse();

    if (isEndTag())
        tag.push_back('/');

    tag.append(getName());
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


    delete[] buf;
    buf = new char [ tag.length() + 1 ];
    strcpy(buf, tag.c_str());

    return buf;
}


// if an eID is provided, then we check to be sure we have an attribute <tag eID="xxx"/> value xxx equiv to what is given us
// otherwise, we return if we're a simple XML end </tag>.
bool XMLTag::isEndTag(const char *eID) const {
    if (eID) {
        return (std::string(eID) == getAttribute("eID"));
    }
    return endTag;
}


} /* namespace swordxx */

