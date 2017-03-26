/******************************************************************************
 *
 *  utilxml.h -    definition of class that deal with xml constructs
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

#ifndef UTILXML_H
#define UTILXML_H

#include <list>
#include <map>
#include <memory>
#include <string>
#include "defs.h"


namespace swordxx {

typedef std::map<std::string, std::string> StringPairMap;

/** Simple XML helper class.
*/
class SWDLLEXPORT XMLTag {
private:
    mutable std::unique_ptr<char[]> m_buf;
    std::string m_name;
    mutable bool parsed;
    mutable bool empty;
    mutable bool endTag;
    mutable StringPairMap attributes;
    mutable std::string junkBuf;

    void parse() const;
    const char *getPart(const char *buf, int partNum = 0, char partSplit = '|') const;

public:
    XMLTag(char const * tagString = nullptr);
    XMLTag(const XMLTag& tag);

    void setText(const char *tagString);
    inline std::string const & name() const { return m_name; }

    inline bool isEmpty() const {
        if (!parsed)
            parse();

        return empty;
    }
    inline void setEmpty(bool value) {
        if (!parsed)
            parse();
        empty = value;
        if (value)
            endTag = false;
    }

    /***
     * if an eID is provided, then we check to be sure we have an attribute <tag eID="xxx"/> value xxx equiv to what is given us
     * otherwise, we return if we're a simple XML end </tag>.
     */
    bool isEndTag(char const * eID = nullptr) const;

    std::list<std::string> attributeNames() const;
    int attributePartCount(const char *attribName, char partSplit = '|') const;

    // return values should not be considered to persist beyond the return of the function.
    std::string attribute(const char *attribName, int partNum = -1, char partSplit = '|') const;
    void setAttribute(const char *attribName, const char *attribValue, int partNum = -1, char partSplit = '|');
    std::string toString() const;
    inline XMLTag & operator =(const char *tagString) { setText(tagString); return *this; }
    inline XMLTag & operator =(const XMLTag &other) { setText(other.toString().c_str()); return *this; }
};

} /* namespace swordxx */
#endif

