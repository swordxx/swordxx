/******************************************************************************
 *
 * Copyright 2016 Jaak Ristioja
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

#ifndef SWORDXX_SWCONFIG_H
#define SWORDXX_SWCONFIG_H

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include "defs.h"


namespace swordxx {
namespace detail {

/* A multimap that still lets you use [] to reference the first entry of a key
   if multiples exist. Default-constructs a value if no such key exists. */
template <typename Key, typename T, typename ... Args>
class MultiMapWithDefault: public std::multimap<Key, T, Args...> {

public: /* Methods: */

    template <typename Key_>
    T & operator[](Key_ && key) {
        auto const it(this->find(key));
        return (it != this->end())
               ? it->second
               : this->emplace(std::forward<Key_>(key), T())->second;
    }

    template <typename Key_, typename Value_>
    bool has(Key_ && key, Value_ && value) const noexcept {
        auto const end(this->upper_bound(key));
        return std::find_if(this->lower_bound(std::forward<Key_>(key)),
                            end,
                            [v = std::forward<Value_>(value)](auto const & vp)
                            { return vp.second == v; }) != end;
    }
};

} /* namespace detail { */

using ConfigEntMap =
        detail::MultiMapWithDefault<std::string, std::string, std::less<>>;
using SectionMap = std::map<std::string, ConfigEntMap, std::less<>>;

/**
   \brief A class to read and save settings from and to a file.
   \note Alternatively, if an empty file-name is given, the object acts as an
         in-memory representation of a configuration.
*/
class SWDLLEXPORT SWConfig {

public: /* Methods: */

    /**
      \brief Constructs a memory-backed configuration object.
    */
    SWConfig();

    /**
      \brief Constructs a file-backed configuration object.
      \param[in] filename the file to use for the configuration. Empty for
                          memory-backed configuration.
    */
    SWConfig(std::string filename);
    virtual ~SWConfig();

    inline std::string const & filename() const noexcept { return m_filename; }
    inline SectionMap const & sections() const noexcept { return m_sections; }
    inline SectionMap & sections() noexcept { return m_sections; }

    /**
       \brief Reloads configuration settings from disk.
       \pre This is not a memory-backed SWConfig object.
       \returns whether loading the configuration from the file succeeded.
    */
    bool reload();

    /**
       \brief Flushes configuration settings to disk.
       \pre This is not a memory-backed SWConfig object.
       \returns whether saving the configuration to the file succeeded.
    */
    bool save();

    /**
       \brief Merges the settings from other to this object.
       \param[in] other The other object to merge settings from.
       \warning Any values already present in this object will be overwritten.
    */
    void augment(SWConfig const & other);

    /// \brief Shorthand for sections()[sectionIndex]
    template <typename SectionIndex>
    ConfigEntMap & operator[](SectionIndex && sectionIndex)
    { return m_sections[std::forward<SectionIndex>(sectionIndex)]; }

private: /* Fields: */

    std::string const m_filename; /**< The filename of the configuration file.*/
    SectionMap m_sections; /**< Mapping of available sections. */

}; /* class SWConfig */

} /* namespace swordxx */

#endif /* SWORDXX_SWCONFIG_H */
