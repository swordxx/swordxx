/******************************************************************************
 *
 *  swlocale.h -    definition of Class SWLocale used for retrieval
 *                   of locale lookups
 *
 * $Id$
 *
 * Copyright 2000-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWLOCALE_H
#define SWLOCALE_H

#include <map>
#include <memory>
#include <string>
#include "defs.h"


namespace swordxx {

class SWConfig;
static const int ENDOFABBREVS = -2;

/** SWLocale is used for the localisation of the booknames
* The SWLocale is a class which holds the information of one language.
* Every language supported by Sword++ has one SWLocale object,
* get the name of the Language using @see getname of this class.
* Another functions useful for frontend developers is @see getDescription.
*/
class SWDLLEXPORT SWLocale {

    using LookupMap = std::map<std::string, std::string>;

    LookupMap m_lookupTable;
    LookupMap m_mergedAbbrevs;

    std::unique_ptr<SWConfig> m_localeSource;

    std::string m_name;
    std::string m_description;
    std::string m_encoding;
    std::unique_ptr<struct abbrev[]> m_bookAbbrevs;
    std::size_t m_abbrevsCnt;
    const char ** m_bookLongNames;
    const char ** m_bookPrefAbbrev;

public:
    SWLocale(const char *ifilename);
    virtual ~SWLocale() noexcept;

    /**
    * This function is used to get the name of the languages which this object is handling.
    * @return The name of the managed language. A possible example is "de".
    */
    std::string const & getName() const noexcept { return m_name; }

    /**
    * @return The description. A possible example is "German".
    */
    std::string const & getDescription() const noexcept { return m_description; }
    std::string const & getEncoding() const noexcept { return m_encoding; }
    std::string const & translate(const char *text);

    void augment(SWLocale & addFrom);

    struct abbrev const * getBookAbbrevs(std::size_t * retSize);
    static const char *DEFAULT_LOCALE_NAME;

};

} /* namespace swordxx */
#endif
