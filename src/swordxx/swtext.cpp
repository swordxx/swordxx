/******************************************************************************
 *
 *  swtext.cpp -    code for base class 'SWText'- The basis for all text
 *            modules
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

#include "swtext.h"

#include "keys/listkey.h"
#include "localemgr.h"
#include "utilstr.h"


namespace swordxx {

namespace {

std::unique_ptr<VerseKey> staticCreateKey(char const * const versification) {
    auto vk(std::make_unique<VerseKey>());
    vk->setVersificationSystem(versification);
    return vk;
}

} // anonymous namespace


/******************************************************************************
 * SWText Constructor - Initializes data for instance of SWText
 *
 * ENT:    imodname - Internal name for module
 *    imoddesc - Name to display to user for module
 */

SWText::SWText(const char *imodname, const char *imoddesc, TextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang, const char *versification): SWModule(staticCreateKey(versification), imodname, imoddesc, "Biblical Texts", enc, dir, mark, ilang) {
    this->m_versification = versification;
    m_tmpVK1.setVersificationSystem(versification);
    m_tmpVK2.setVersificationSystem(versification);
        m_tmpSecond = false;
    skipConsecutiveLinks = false;
}


/******************************************************************************
 * SWText createKey - Create the correct key (VerseKey) for use with SWText
 */

std::unique_ptr<SWKey> SWText::createKey() const
{ return staticCreateKey(m_versification.c_str()); }

VerseKey &SWText::getVerseKey(const SWKey *keyToConvert) const {
    SWKey const * tmp = keyToConvert ? keyToConvert : this->key;
    /// \bug remove const_cast:
    SWKey * thisKey = const_cast<SWKey *>(tmp);

    if (VerseKey * const key = dynamic_cast<VerseKey *>(thisKey))
        return *key;
    if (ListKey * const lkTest = dynamic_cast<ListKey *>(thisKey))
        if (VerseKey * const key =
                dynamic_cast<VerseKey *>(lkTest->getElement()))
            return *key;

    VerseKey & retKey = (m_tmpSecond) ? m_tmpVK1 : m_tmpVK2;
    m_tmpSecond = !m_tmpSecond;
    retKey.setLocale(LocaleMgr::getSystemLocaleMgr()->getDefaultLocaleName());
    retKey.positionFrom(*thisKey);
    return retKey;
}


} /* namespace swordxx */
