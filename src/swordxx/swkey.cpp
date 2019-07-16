/******************************************************************************
 *
 *  swkey.cpp -    code for base class 'SWKey'.  SWKey is the basis for all
 *        types of keys for indexing into modules (e.g. verse, word,
 *        place, etc.)
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

#include "swkey.h"

#include <cstring>
#include "localemgr.h"
#include "utilstr.h"


namespace swordxx {
namespace {

struct LocaleCache {
    std::string name;
    std::shared_ptr<SWLocale> locale;
} g_localeCache;

} // anonymous namespace


/******************************************************************************
 * SWKey Constructor - initializes instance of SWKey
 *
 * ENT:    ikey - text key
 */

SWKey::SWKey(std::string keyText)
    : m_keyText(std::move(keyText))
{
    setLocale(LocaleMgr::getSystemLocaleMgr()->getDefaultLocaleName());
    m_index     = 0;
    m_error     = 0;
    m_userData  = 0;
}

SWKey::SWKey(const char *ikey)
    : SWKey(std::string(ikey ? ikey : ""))
{}

SWKey::SWKey(SWKey const &k)
{
    setLocale(LocaleMgr::getSystemLocaleMgr()->getDefaultLocaleName());
    m_localeName = k.m_localeName;
    m_index     = k.m_index;
    m_userData  = k.m_userData;
    m_error     = k.m_error;
    setText(k.getText());
}

std::shared_ptr<SWKey> SWKey::clone() const
{ return std::make_shared<SWKey>(*this); }

/******************************************************************************
 * SWKey Destructor - cleans up instance of SWKey
 */

SWKey::~SWKey() {}


/******************************************************************************
 * SWKey::getPrivateLocale - Gets a real locale object from our name
 *
 * RET:    locale object associated with our name
 */

SWLocale & SWKey::getPrivateLocale() const {
    if (!m_locale) {
        if ((!g_localeCache.name.empty()) || (g_localeCache.name != m_localeName)) {
            g_localeCache.name = m_localeName;
            // this line is the entire bit of work we're trying to avoid with the cache
            // worth it?  compare time examples/cmdline/search KJV "God love world" to
            // same with this method reduced to:
            // if (!local) local = ... (call below); return locale;
            g_localeCache.locale = LocaleMgr::getSystemLocaleMgr()->getLocale(m_localeName.c_str());
        }
        m_locale = g_localeCache.locale;
    }
    return *m_locale;
}


/******************************************************************************
 * SWKey::setText Equates this SWKey to a character string
 *
 * ENT:    ikey - other swkey object
 */

void SWKey::setText(std::string newText) { m_keyText = std::move(newText); }


/******************************************************************************
 * SWKey::copyFrom Equates this SWKey to another SWKey object
 *
 * ENT:    ikey - other swkey object
 */

void SWKey::copyFrom(const SWKey &ikey) {
    setLocale(ikey.getLocale());
    setText(ikey.getText());
}


/******************************************************************************
 * SWKey::getText - returns text key if (const char *) cast is requested
 */

std::string SWKey::getText() const { return m_keyText; }


/******************************************************************************
 * SWKey::getRangeText - returns parsable range text for this key
 */

std::string SWKey::getRangeText() const { return m_keyText; }


/******************************************************************************
 * SWKey::getOSISRefRangeText - returns parsable range text for this key
 */

std::string SWKey::getOSISRefRangeText() const { return getRangeText(); }


/******************************************************************************
 * SWKey::compare    - Compares another VerseKey object
 *
 * ENT:    ikey - key to compare with this one
 *
 * RET:    > 0 if this key is greater than compare key
 *    < 0
 *      0
 */

int SWKey::compare(SWKey const & ikey) const noexcept {
    return m_keyText.compare(ikey.m_keyText);
}

void SWKey::positionToTop() {}
void SWKey::positionToBottom() {}


/******************************************************************************
 * SWKey::increment    - Increments key a number of entries
 *
 * ENT:    increment    - Number of entries to jump forward
 *
 * RET: *this
 */

void SWKey::increment(int) {
    m_error = KEYERR_OUTOFBOUNDS;
}


/******************************************************************************
 * SWKey::decrement    - Decrements key a number of entries
 *
 * ENT:    decrement    - Number of entries to jump backward
 *
 * RET: *this
 */

void SWKey::decrement(int) {
    m_error = KEYERR_OUTOFBOUNDS;
}

} /* namespace swordxx */
