/******************************************************************************
 *
 *  swgenbook.cpp -    Implementation of SWGenBook class
 *
 * $Id$
 *
 * Copyright 2002-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "swgenbook.h"

#include "keys/versetreekey.h"


namespace swordxx {

/******************************************************************************
 * SWGenBook Constructor - Initializes data for instance of SWGenBook
 *
 * ENT:    imodname - Internal name for module
 *    imoddesc - Name to display to user for module
 */

SWGenBook::SWGenBook(SWKey * key_, const char *imodname, const char *imoddesc, SWTextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang) : SWModule(key_, imodname, imoddesc, (char *)"Generic Books", enc, dir, mark, ilang) {
    tmpTreeKey = nullptr;
}


/******************************************************************************
 * SWGenBook Destructor - Cleans up instance of SWGenBook
 */

SWGenBook::~SWGenBook() {
    delete tmpTreeKey;
}


TreeKey & SWGenBook::getTreeKey(SWKey * k) const {
    SWKey * const thiskey = k ? k : this->key;

    if (TreeKey * const key = dynamic_cast<TreeKey *>(thiskey))
        return *key;

    if (ListKey * const lkTest = dynamic_cast<ListKey *>(thiskey)) {
        if (TreeKey * const key = dynamic_cast<TreeKey *>(lkTest->getElement()))
            return *key;
        if (VerseTreeKey * const tkey =
                dynamic_cast<VerseTreeKey *>(lkTest->getElement()))
            if (TreeKey * const key = tkey->getTreeKey())
                return *key;
    }

    if (VerseTreeKey * const tkey = dynamic_cast<VerseTreeKey *>(thiskey))
        if (TreeKey * const key = tkey->getTreeKey())
            return *key;

    delete tmpTreeKey;
    tmpTreeKey = (TreeKey *)createKey();
    (*tmpTreeKey) = *(thiskey);
    return (*tmpTreeKey);
}

} /* namespace swordxx */
