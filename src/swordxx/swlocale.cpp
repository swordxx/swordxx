/******************************************************************************
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
namespace {

template <typename LookupMap>
std::string translate(LookupMap const & lookupMap, std::string_view str) {
    auto const it(lookupMap.find(str));
    return (it != lookupMap.end()) ? it->second : std::string(str);
}

} // anonymous namespace

char const * SWLocale::DEFAULT_LOCALE_NAME = "en";

SWLocale::SWLocale(const char *ifilename) {
    if (!ifilename) {
        m_name = DEFAULT_LOCALE_NAME;
        m_description = "English (US)";
        return; // Nothing else to do
    }

    // Load configuration file:
    SWConfig localeSource(ifilename);

    // Read name and description:
    auto const & sections = localeSource.sections();
    auto const metaSectionIt(sections.find("Meta"));
    if (metaSectionIt != sections.end()) {
        auto const & metaSection = metaSectionIt->second;
        { // Read name:
            auto const entry(metaSection.find("Name"));
            if (entry != metaSection.end())
                m_name = entry->second;
        }{ // Read description:
            auto const entry(metaSection.find("Description"));
            if (entry != metaSection.end())
                m_description = entry->second;
        }
    }

    /* Read all book abbreviation entries: */
    m_bookAbbrevs = localeSource["Book Abbrevs"];

    // Add all text entries:
    for (auto & ep : localeSource["Text"])
        m_textTranslations.emplace(ep.first, ep.second);

    // Add all preferred abbreviation entries:
    for (auto & vp : localeSource["Pref Abbrevs"])
        m_preferredAbbreviationTranslations.emplace(vp.first, vp.second);
}

SWLocale::~SWLocale() noexcept = default;

std::string SWLocale::translateText(std::string_view text) const
{ return translate(m_textTranslations, std::move(text)); }

std::string SWLocale::translatePrefAbbrev(std::string_view abbrev) const
{ return translate(m_preferredAbbreviationTranslations, std::move(abbrev)); }

void SWLocale::augment(SWLocale const & addFrom) {
    /* Because the other locale augments this one, it makes sense to use the
       augmented name and description as well, which might contain information
       about the augmentation. */
    m_name = addFrom.m_name;
    m_description = addFrom.m_description;

    // Texts in the locale augmenting this one override the texts of this locale
    for (auto const & vp : addFrom.m_textTranslations)
        m_textTranslations[vp.first] = vp.second;

    // Book abbrevations are appended except the ones which would be duplicates:
    auto const bookAbbrevExists =
            [this](auto const & valuePair) {
                for (auto range(m_bookAbbrevs.equal_range(valuePair.first));
                     range.first != range.second;
                     ++range.first)
                    if (range.first->second == valuePair.second)
                        return true;
                return false;
            };
    for (auto const & vp : addFrom.m_bookAbbrevs)
        if (!bookAbbrevExists(vp))
            m_bookAbbrevs.emplace(vp);
}

ConfigEntMap const & SWLocale::builtinBookAbbreviations() {
    static ConfigEntMap const builtinAbbrevs(
                [](){
                    ConfigEntMap abbrevsMap;
                    for (std::size_t i = 0u; builtin_abbrevs[i].osis[0u]; ++i)
                        abbrevsMap.emplace(builtin_abbrevs[i].ab,
                                           builtin_abbrevs[i].osis);
                    return abbrevsMap;
                }());
    return builtinAbbrevs;

}

} /* namespace swordxx */
