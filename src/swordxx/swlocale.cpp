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

#include "keys/versekey.h"
#include "swconfig.h"
#include "utilstr.h"
#include "versificationmgr.h"

namespace swordxx {

char const * SWLocale::DEFAULT_LOCALE_NAME = "en";

SWLocale::SWLocale(const char *ifilename) {
    {
        std::shared_ptr<ConfigEntMap> abbrevsMap;
        if (ifilename) {
            m_localeSource = std::make_unique<SWConfig>(ifilename);

            /* Build abbreviations map from locale,  */
            abbrevsMap = std::make_shared<ConfigEntMap>(
                             (*m_localeSource)["Book Abbrevs"]);
        } else {
            m_localeSource = std::make_unique<SWConfig>();
            auto & meta = (*m_localeSource)["Meta"];
            meta["Name"] = DEFAULT_LOCALE_NAME;
            meta["Description"] = "English (US)";

            // Use english abbrevs by default:
            abbrevsMap = std::make_shared<ConfigEntMap>();
        }

        // Append the built-in (English) abbreviations to the end:
        for (std::size_t j = 0; builtin_abbrevs[j].osis[0]; ++j)
            abbrevsMap->emplace(builtin_abbrevs[j].ab, builtin_abbrevs[j].osis);
        m_bookAbbrevs = std::move(abbrevsMap);
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

std::string const & SWLocale::translate(std::string_view text) {
    auto const entry(m_lookupTable.find(text));
    if (entry == m_lookupTable.end()) {
        auto const & textSection = (*m_localeSource)["Text"];
        auto const confEntry(textSection.find(text));
        if (confEntry == textSection.end())
            return m_lookupTable.emplace(text, text).first->second;
        return m_lookupTable.emplace(std::move(text),
                                     confEntry->second).first->second;
    }
    return entry->second;
}

void SWLocale::augment(SWLocale &addFrom) {
    m_localeSource->augment(*addFrom.m_localeSource);
    m_lookupTable.clear();
}

} /* namespace swordxx */
