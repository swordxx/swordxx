/******************************************************************************
 *
 *  utilxml.h -    definition of class that deal with xml constructs
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

#ifndef SWORDXX_UTILXML_H
#define SWORDXX_UTILXML_H

#include <list>
#include <map>
#include <memory>
#include <string>
#include "defs.h"


namespace swordxx {

/** Simple XML helper class. */
class SWDLLEXPORT XMLTag {

public: /* Methods: */

    XMLTag(char const * tagString = nullptr);

    XMLTag(XMLTag &&);
    XMLTag(XMLTag const &);

    std::string const & name() const noexcept { return m_name; }

    bool isEmpty() const noexcept { return m_isEmpty; }

    void setEmpty(bool value) {
        m_isEmpty = value;
        if (value)
            m_isEndTag = false;
    }

    /*
     * Return if we're a simple XML end </tag>.
     */
    bool isEndTag() const noexcept { return m_isEndTag; }

    /***
     * if an eID is provided, then we check to be sure we have an attribute
     * <tag eID="xxx"/> value xxx equiv to what is given us. Otherwise, we
     * return if we're a simple XML end </tag>.
     */
    bool isEndTag(char const * eID) const noexcept;

    std::map<std::string, std::string> & attributes() noexcept
    { return m_attributes; }

    std::map<std::string, std::string> const & attributes() const noexcept
    { return m_attributes; }

    std::string attribute(std::string const & attribName) const;

    void setAttribute(std::string const & attribName,
                      std::string attribValue);

    void eraseAttribute(std::string const & attribName) noexcept;

    std::string toString() const;

    XMLTag & operator=(XMLTag &&);
    XMLTag & operator=(XMLTag const &);

private: /* Fields: */

    std::map<std::string, std::string> m_attributes;
    std::string m_name;
    bool m_isEmpty;
    bool m_isEndTag;

};

} /* namespace swordxx */

#endif /* SWORDXX_UTILXML_H */
