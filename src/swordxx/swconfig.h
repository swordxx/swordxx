/******************************************************************************
 *
 *  swconfig.h -    definition of Class SWConfig used for saving and
 *            retrieval of configuration information
 *
 * $Id$
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWCONFIG_H
#define SWCONFIG_H

#include <algorithm>
#include <map>
#include <string>
#include "defs.h"


namespace swordxx {
namespace detail {

/* A multimap that still lets you use [] to reference the first entry of a key
   if multiples exist. Default-constructs a value if no such key exists. */
template <typename Key, typename T, typename ... Args>
class MultiMapWithDefault: public std::multimap<Key, T, Args...> {

public: /* Methods: */

    T & operator[](Key const & key) {
        auto const it(this->find(key));
        return (it != this->end())
               ? it->second
               : this->emplace(key, T())->second;
    }

    bool has(Key const & key, T const & value) const noexcept {
        auto const end(this->upper_bound(key));
        return std::find_if(this->lower_bound(key),
                            end,
                            [&value](auto const & vp)
                            { return vp.second == value; }) != end;
    }
};

} /* namespace detail { */

using ConfigEntMap = detail::MultiMapWithDefault<std::string, std::string>;
using SectionMap = std::map<std::string, ConfigEntMap>;

/** The class to read and save settings using a file on disk.
*
*/
class SWDLLEXPORT SWConfig {
public:
    /** The filename used by this SWConfig object
    *
    */
    std::string filename;
    /** Map of available sections
    * The map of available sections.
    */
    SectionMap Sections;

    /** Constructor of SWConfig
    * @param ifilename The file, which should be used for this config.
    */
    SWConfig(const char *ifilename);
    SWConfig();
    virtual ~SWConfig();

    /** Load from disk
    * Load the content from disk.
    */
    virtual void Load();

    /** Save to disk
    * Save the content of this config object to disk.
    */
    virtual void Save();

    /** Merges the values of addFrom
    * @param addFrom The config which values should be merged to this config object. Already existing values will be overwritten.
    */
    virtual void augment(SWConfig &addFrom);
    virtual SWConfig & operator +=(SWConfig &addFrom) { augment(addFrom); return *this; }

    /** Get a section
    * This is an easy way to get and store config values.
    * The following will work:\n
    *
    * @code
    * SWConfig config("/home/user/.setttings");
    * config["Colors"]["Background"] = "red";
    * @endcode
    */
    virtual ConfigEntMap & operator [](const char *section);
    };
} /* namespace swordxx */
#endif
