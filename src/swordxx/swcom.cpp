/******************************************************************************
 *
 *  swcom.cpp -    code for base class 'SWCom'- The basis for all commentary
 *        modules
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

#include "swcom.h"

#include "keys/versekey.h"
#include "localemgr.h"
#include "utilstr.h"


namespace swordxx {


/******************************************************************************
 * SWCom Constructor - Initializes data for instance of SWCom
 *
 * ENT:    imodname - Internal name for module
 *    imoddesc - Name to display to user for module
 */

SWCom::SWCom(const char *imodname, const char *imoddesc, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char *ilang, const char *versification): SWModule(imodname, imoddesc, "Commentaries", enc, dir, mark, ilang) {
    this->versification = 0;
    stdstr(&(this->versification), versification);
    delete key;
    key = (VerseKey *)createKey();
    tmpVK1 = (VerseKey *)createKey();
    tmpVK2 = (VerseKey *)createKey();
        tmpSecond = false;
}


/******************************************************************************
 * SWCom Destructor - Cleans up instance of SWCom
 */

SWCom::~SWCom() {
    delete tmpVK1;
    delete tmpVK2;
    delete [] versification;
}


SWKey *SWCom::createKey() const {
    VerseKey *vk = new VerseKey();

    vk->setVersificationSystem(versification);

    return vk;
}


long SWCom::getIndex() const {
    VerseKey *key = &getVerseKey();
    entryIndex = key->getIndex();
    return entryIndex;
}

void SWCom::setIndex(long iindex) {
    VerseKey *key = &getVerseKey();
    key->setTestament(1);
    key->setIndex(iindex);

    if (key != this->key) {
        this->key->copyFrom(*key);
    }
}


VerseKey &SWCom::getVerseKey(const SWKey *keyToConvert) const {
    const SWKey *thisKey = keyToConvert ? keyToConvert : this->key;

    VerseKey *key = 0;
    // see if we have a VerseKey * or decendant
    try {
        key = SWDYNAMIC_CAST(VerseKey, thisKey);
    }
    catch ( ... ) {    }
    if (!key) {
        ListKey *lkTest = 0;
        try {
            lkTest = SWDYNAMIC_CAST(ListKey, thisKey);
        }
        catch ( ... ) {    }
        if (lkTest) {
            try {
                key = SWDYNAMIC_CAST(VerseKey, lkTest->getElement());
            }
            catch ( ... ) {    }
        }
    }
    if (!key) {
                VerseKey *retKey = (tmpSecond) ? tmpVK1 : tmpVK2;
                tmpSecond = !tmpSecond;
        retKey->setLocale(LocaleMgr::getSystemLocaleMgr()->getDefaultLocaleName());
        (*retKey) = *(thisKey);
        return (*retKey);
    }
    else    return *key;
}


} /* namespace swordxx */
