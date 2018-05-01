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

SWGenBook::SWGenBook(std::unique_ptr<SWKey> key_,
                     char const * imodname,
                     char const * imoddesc,
                     TextEncoding enc,
                     SWTextDirection dir,
                     SWTextMarkup mark,
                     char const * ilang)
    : SWModule(std::move(key_),
               imodname,
               imoddesc,
               "Generic Books",
               enc,
               dir,
               mark,
               ilang)
{}


TreeKey & SWGenBook::getTreeKey(SWKey * k) const {
    SWKey * const thiskey = k ? k : this->key;

    if (TreeKey * const key_ = dynamic_cast<TreeKey *>(thiskey))
        return *key_;

    if (ListKey * const lkTest = dynamic_cast<ListKey *>(thiskey)) {
        if (TreeKey * const key_ = dynamic_cast<TreeKey *>(lkTest->getElement()))
            return *key_;
        if (VerseTreeKey * const tkey =
                dynamic_cast<VerseTreeKey *>(lkTest->getElement()))
            if (TreeKey * const key_ = tkey->getTreeKey())
                return *key_;
    }

    if (VerseTreeKey * const tkey = dynamic_cast<VerseTreeKey *>(thiskey))
        if (TreeKey * const key_ = tkey->getTreeKey())
            return *key_;

    m_tmpTreeKey.reset(static_cast<TreeKey *>(createKey().release()));
    m_tmpTreeKey->positionFrom(*thiskey);
    return (*m_tmpTreeKey);
}

} /* namespace swordxx */
