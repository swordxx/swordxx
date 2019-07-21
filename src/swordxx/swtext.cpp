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

std::shared_ptr<VerseKey> staticCreateKey(char const * const versification) {
    auto vk(std::make_shared<VerseKey>());
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
    setSkipConsecutiveLinks(false);
}


/******************************************************************************
 * SWText createKey - Create the correct key (VerseKey) for use with SWText
 */

std::shared_ptr<SWKey> SWText::createKey() const
{ return staticCreateKey(m_versification.c_str()); }

std::shared_ptr<VerseKey
const> SWText::getVerseKey(std::shared_ptr<SWKey const> thisKey) const {
    if (!thisKey)
        thisKey = getKey();

    if (auto key_ = std::dynamic_pointer_cast<VerseKey const>(thisKey))
        return key_;
    if (auto const lkTest = std::dynamic_pointer_cast<ListKey const>(thisKey))
        if (auto const key_ =
                std::dynamic_pointer_cast<VerseKey const>(lkTest->getElement()))
            return key_;

    auto r(std::make_shared<VerseKey>());
    r->setVersificationSystem(m_versification.c_str());
    r->positionFrom(*thisKey);
    return r;
}


} /* namespace swordxx */
