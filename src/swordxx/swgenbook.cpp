/******************************************************************************
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

SWGenBook::SWGenBook(std::shared_ptr<SWKey> key_,
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

SWGenBook::~SWGenBook() noexcept = default;

std::shared_ptr<TreeKey const>
SWGenBook::getTreeKey(std::shared_ptr<SWKey const> thiskey) const {
    if (!thiskey)
        thiskey = getKey();

    if (auto key_ = std::dynamic_pointer_cast<TreeKey const>(thiskey))
        return key_;

    if (auto const lkTest = std::dynamic_pointer_cast<ListKey const>(thiskey)) {
        if (auto key_ = std::dynamic_pointer_cast<TreeKey const >(lkTest->getElement()))
            return key_;
        if (auto const tkey =
                std::dynamic_pointer_cast<VerseTreeKey const>(lkTest->getElement()))
            if (auto key_ = tkey->getTreeKey())
                return key_;
    }

    if (auto const tkey = std::dynamic_pointer_cast<VerseTreeKey const>(thiskey))
        if (auto key_ = tkey->getTreeKey())
            return key_;

    std::shared_ptr<TreeKey> r(std::static_pointer_cast<TreeKey>(createKey()));
    r->positionFrom(*thiskey);
    return r;
}

} /* namespace swordxx */
