/******************************************************************************
 *
 *  swlocale.cpp -    implementation of Class SWLocale used for retrieval
 *            of locale lookups
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

#include "swlocale.h"

#include <map>
#include "keys/versekey.h"
#include "swconfig.h"
#include "utilstr.h"
#include "versificationmgr.h"


namespace swordxx {

char const * SWLocale::DEFAULT_LOCALE_NAME = "en";

SWLocale::SWLocale(const char *ifilename) {
    m_bookLongNames  = nullptr;
    m_bookPrefAbbrev = nullptr;
    if (ifilename) {
        m_localeSource = std::make_unique<SWConfig>(ifilename);

        // Assure all english abbrevs are present
        for (std::size_t j = 0; builtin_abbrevs[j].osis[0]; ++j)
            m_mergedAbbrevs[builtin_abbrevs[j].ab] = builtin_abbrevs[j].osis;

        for (auto const & ap : m_localeSource->sections()["Book Abbrevs"])
            m_mergedAbbrevs[ap.first.c_str()] = ap.second.c_str();

        auto const size = m_mergedAbbrevs.size();
        m_bookAbbrevs = std::make_unique<struct abbrev[]>(size + 1);
        std::size_t i = 0;
        for (auto const & ap : m_mergedAbbrevs) {
            auto & bookAbbrev = m_bookAbbrevs[i];
            bookAbbrev.ab = ap.first.c_str();
            bookAbbrev.osis = ap.second.c_str();
            ++i;
        }

        m_bookAbbrevs[i].ab = "";
        m_bookAbbrevs[i].osis = "";
        m_abbrevsCnt = size;
    } else {
        m_localeSource = std::make_unique<SWConfig>();
        auto & meta = (*m_localeSource)["Meta"];
        meta["Name"] = DEFAULT_LOCALE_NAME;
        meta["Description"] = "English (US)";
        m_abbrevsCnt = 0u;
        while (builtin_abbrevs[m_abbrevsCnt].osis[0])
            ++m_abbrevsCnt;
    }

    auto const & sections = m_localeSource->sections();
    auto const metaSectionIt(sections.find("Meta"));
    if (metaSectionIt != sections.end()) {
        auto const & metaSection = metaSectionIt->second;
        auto const maybeGetEntry =
                [&metaSection](std::string & field, char const * const key) {
                    auto const entry(metaSection.find(key));
                    if (entry != metaSection.end())
                        field = entry->second;
                };
        maybeGetEntry(m_name, "Name");
        maybeGetEntry(m_description, "Description");
        maybeGetEntry(m_encoding, "Encoding");
    }
}

SWLocale::~SWLocale() noexcept = default;

std::string const & SWLocale::translate(const char *text) {
    LookupMap::iterator entry;

    entry = m_lookupTable.find(text);

    if (entry == m_lookupTable.end()) {
        ConfigEntMap::iterator confEntry;
        confEntry = m_localeSource->sections()["Text"].find(text);
        if (confEntry == m_localeSource->sections()["Text"].end())
            m_lookupTable.insert(LookupMap::value_type(text, text));
        else {
            m_lookupTable.insert(
                        LookupMap::value_type(text,
                                              (*confEntry).second.c_str()));
        }
        entry = m_lookupTable.find(text);
    }
    return (*entry).second;
}

void SWLocale::augment(SWLocale &addFrom) {
    m_localeSource->augment(*addFrom.m_localeSource);
}

struct abbrev const * SWLocale::getBookAbbrevs(std::size_t * retSize) {
    if (retSize)
        (*retSize) = m_abbrevsCnt;
    return m_bookAbbrevs ? m_bookAbbrevs.get() : builtin_abbrevs;
}

} /* namespace swordxx */
