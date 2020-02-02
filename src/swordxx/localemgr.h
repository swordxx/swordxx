/******************************************************************************
 *
 *  localemgr.h -    definition of class LocaleMgr used to interact with
 *            registered locales for a Sword++ installation
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

#ifndef SWORDXX_LOCALEMGR_H
#define SWORDXX_LOCALEMGR_H

#include <map>
#include <memory>
#include <list>
#include <string>
#include <string_view>
#include "defs.h"


namespace swordxx {

class SWLocale;

using LocaleMap = std::map<std::string, std::shared_ptr<SWLocale>, std::less<>>;

/**
* The LocaleMgr class handles all the different locales of Sword++.
* It provides functions to get a list of all available locales,
* to get the default locale name and to get it.
* The other functions are not interesting for frontend programmers.
*
* To get the default locale name use @see getDefaultLocaleName
* To set the default locale name use @see setDefaultLocaleName
* To get the locale for a language name use @see getLocale
* To get a list of availble locales use @see getAvailableLocales
*/
class SWDLLEXPORT LocaleMgr {

public: /* Methods: */

    LocaleMgr(LocaleMgr &&) = delete;
    LocaleMgr(LocaleMgr const &) = delete;
    LocaleMgr & operator=(LocaleMgr &&) = delete;
    LocaleMgr & operator=(LocaleMgr const &) = delete;

    /** Default constructor of  LocaleMgr
    * You do normally not need this constructor, use LocaleMgr::getSystemLocaleMgr() instead.
    */
    LocaleMgr(char const * iConfigPath = nullptr);

    virtual ~LocaleMgr() noexcept;

    /** Get the locale connected with the name "name".
    *
    * @param name The name of the locale you want to have. For example use getLocale("de") to get the locale for the German language.
    * @return Returns the locale object if the locale with the name given as parameter was found. If it wasn't found return nullptr.
    */
    virtual std::shared_ptr<SWLocale> getLocale(std::string_view name);

    /** Get the list of available locales.
    *
    * @return Returns a list of strings, which contains the names of the available locales.
    */
    virtual std::list<std::string> getAvailableLocales();

    /** Returns translated text.
    * This function uses both parameters to return the translated version of the given text.
    *
    * @param text The text to translate into the language given by the first parameter.
    * @param localeName The name of the locale Sword++ should use
    * @return Returns the translated text.
    */
    virtual std::string translate(std::string_view text,
                                  std::string_view localeName =
                                        std::string_view());

    /** Get the default locale name. To set it use @see setDefaultLocaleName
    *
    * @return Returns the default locale name
    */
    std::string const & getDefaultLocaleName() const noexcept
    { return m_defaultLocaleName; }

    /** Set the new standard locale of Sword++.
    *
    * @param name The name of the new default locale
    */
    virtual void setDefaultLocaleName(std::string_view defaultLocaleName);

    /** The LocaleMgr object used globally in the Sword++ world.
      * Do not create your own LocaleMgr, use this static object instead.
    */
    static std::shared_ptr<LocaleMgr> getSystemLocaleMgr();
    static void setSystemLocaleMgr(std::shared_ptr<LocaleMgr> newLocaleMgr);

    /** Augment this localmgr with all locale.conf files in a directory */
    virtual void loadConfigDir(char const * ipath);

protected: /* Methods: */

    inline LocaleMap & locales() noexcept { return m_locales; }
    inline LocaleMap const & locales() const noexcept { return m_locales; }

private: /* Fields: */

    std::string m_defaultLocaleName;
    LocaleMap m_locales;
    static std::shared_ptr<LocaleMgr> m_systemLocaleMgr;

}; /* class LocaleMgr */

} /* namespace swordxx */

#endif /* SWORDXX_LOCALEMGR_H */
