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

class Private;

    Private * m_p;

    SWConfig * m_localeSource;

    std::string m_name;
    char * m_description;
    char * m_encoding;
    struct abbrev * m_bookAbbrevs;
    int m_abbrevsCnt;
    const char ** m_bookLongNames;
    const char ** m_bookPrefAbbrev;

public:
    SWLocale(const char *ifilename);
    virtual ~SWLocale();

    /**
    * This function is used to get the name of the languages which this object is handling.
    * @return The name of the managed language. A possible example is "de".
    */
    virtual std::string const & getName() const noexcept { return m_name; }
    /**
    * @return The description. A possible example is "German".
    */
    virtual const char *getDescription();
    virtual const char *getEncoding();
    virtual std::string const & translate(const char *text);
    virtual void augment(SWLocale &addFrom);
    virtual SWLocale & operator +=(SWLocale &addFrom) { augment(addFrom); return *this; }
    virtual const struct abbrev *getBookAbbrevs(int *retSize);
    static const char *DEFAULT_LOCALE_NAME;
};

} /* namespace swordxx */
#endif
