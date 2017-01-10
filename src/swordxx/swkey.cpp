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

SWKey::LocaleCache   SWKey::m_localeCache;

/******************************************************************************
 * SWKey Constructor - initializes instance of SWKey
 *
 * ENT:    ikey - text key
 */

SWKey::SWKey(const char *ikey)
    : m_keyText(std::make_unique<std::string>(ikey ? ikey : ""))
{
    init();
    m_index     = 0;
    m_persist   = 0;
    m_error     = 0;
    m_userData  = 0;
}

SWKey::SWKey(SWKey const &k)
{
    init();
    stdstr(&m_localeName, k.m_localeName);
    m_index     = k.m_index;
    m_persist   = k.m_persist;
    m_userData  = k.m_userData;
    m_error     = k.m_error;
    setText(k.getText());
}

void SWKey::init() {
    m_boundSet = false;
    m_locale = nullptr;
    m_localeName = nullptr;
    setLocale(LocaleMgr::getSystemLocaleMgr()->getDefaultLocaleName());
}

SWKey *SWKey::clone() const
{
    return new SWKey(*this);
}

/******************************************************************************
 * SWKey Destructor - cleans up instance of SWKey
 */

SWKey::~SWKey() {
    delete [] m_localeName;
}


/******************************************************************************
 * SWKey::getPrivateLocale - Gets a real locale object from our name
 *
 * RET:    locale object associated with our name
 */

SWLocale & SWKey::getPrivateLocale() const {
    if (!m_locale) {
        if ((!m_localeCache.name) || (strcmp(m_localeCache.name, m_localeName))) {
            stdstr(&(m_localeCache.name), m_localeName);
            // this line is the entire bit of work we're trying to avoid with the cache
            // worth it?  compare time examples/cmdline/search KJV "God love world" to
            // same with this method reduced to:
            // if (!local) local = ... (call below); return locale;
            m_localeCache.locale = LocaleMgr::getSystemLocaleMgr()->getLocale(m_localeName);
        }
        m_locale = m_localeCache.locale;
    }
    return *m_locale;
}


/******************************************************************************
 * SWKey::setText Equates this SWKey to a character string
 *
 * ENT:    ikey - other swkey object
 */

void SWKey::setText(const char * const ikey) {
    if (m_keyText) {
        if (ikey) {
            (*m_keyText) = ikey;
        } else {
            m_keyText.reset();
        }
    } else if (ikey) {
        m_keyText = std::make_unique<std::string>(ikey);
    }
}


/******************************************************************************
 * SWKey::copyFrom Equates this SWKey to another SWKey object
 *
 * ENT:    ikey - other swkey object
 */

void SWKey::copyFrom(const SWKey &ikey) {
// not desirable    Persist(ikey.Persist());
    setLocale(ikey.getLocale());
    setText(ikey.getText());
}


/******************************************************************************
 * SWKey::getText - returns text key if (const char *) cast is requested
 */

const char * SWKey::getText() const
{ return m_keyText ? m_keyText->c_str() : nullptr; }


/******************************************************************************
 * SWKey::getRangeText - returns parsable range text for this key
 */

std::string SWKey::getRangeText() const { return m_keyText ? *m_keyText : ""; }


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
    /// \bug Potential null pointer dereference
    return strcmp(m_keyText->c_str(), ikey.m_keyText->c_str());
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
