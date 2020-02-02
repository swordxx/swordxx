/******************************************************************************
 *
 *  swlocale.h -    definition of Class SWLocale used for retrieval
 *                   of locale lookups
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

#ifndef SWORDXX_SWLOCALE_H
#define SWORDXX_SWLOCALE_H

#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include "defs.h"
#include "swconfig.h"


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

    /// \todo Convert to unordered_map with heterogenous lookup for C++2a.
    using LookupMap = std::map<std::string, std::string, std::less<>>;

    LookupMap m_textTranslations;
    LookupMap m_preferredAbbreviationTranslations;
    ConfigEntMap m_bookAbbrevs;

    std::string m_name;
    std::string m_description;

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

    std::string translateText(std::string_view text) const;
    std::string translatePrefAbbrev(std::string_view abbrev) const;

    void augment(SWLocale const & addFrom);

    ConfigEntMap const & bookAbbreviations() const noexcept
    { return m_bookAbbrevs; }

    static ConfigEntMap const & builtinBookAbbreviations();

    static const char *DEFAULT_LOCALE_NAME;

};

} /* namespace swordxx */

#endif /* SWORDXX_SWLOCALE_H */
