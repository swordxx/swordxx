/******************************************************************************
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

#include <map>
#include <string>
#include <string_view>
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

    std::map<std::string, std::string> & attributes() noexcept
    { return m_attributes; }

    std::map<std::string, std::string> const & attributes() const noexcept
    { return m_attributes; }

    std::string attribute(std::string const & attribName) const;

    std::string toString() const;

    XMLTag & operator=(XMLTag &&);
    XMLTag & operator=(XMLTag const &);

private: /* Fields: */

    std::map<std::string, std::string> m_attributes;
    std::string m_name;
    bool m_isEmpty;
    bool m_isEndTag;

};

std::string & appendXmlAttributeValue(std::string & r,
                                      std::string_view attrValueView);

std::string & appendEqualsXmlAttributeValue(std::string & r,
                                            std::string_view attrValueView);

std::string formatXmlAttributeValue(std::string_view value);

} /* namespace swordxx */

#endif /* SWORDXX_UTILXML_H */
