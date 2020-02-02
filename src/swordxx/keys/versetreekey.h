/******************************************************************************
 *
 *  versetreekey.h -    code for class 'VerseTreeKey'- a standard Biblical
 *            verse key
 *
 * Copyright 2006-2013 CrossWire Bible Society (http://www.crosswire.org)
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.    See the GNU
 * General Public License for more details.
 *
 */


#ifndef SWORDXX_VERSETREEKEY_H
#define SWORDXX_VERSETREEKEY_H

#include "treekey.h"
#include "versekey.h"

#include "../defs.h"
#include "listkey.h"


namespace swordxx {

/**
 * Class VerseKey
 * The SWKey implementation used for verse based modules like Bibles or commentaries.
 */
class SWDLLEXPORT VerseTreeKey : public VerseKey, public TreeKey::PositionChangeListener {

    std::shared_ptr<TreeKey> m_treeKey;
//    vector<struct sbook> books;

    void init(TreeKey const & treeKey);
    void syncVerseToTree();
    long m_lastGoodOffset;
    bool m_internalPosChange = false;

public:

    /**
    * VerseKey Constructor - initializes Instance of VerseKey
    *
    * @param treeKey a TreeKey which will form the basis of this VerseTreeKey
    * @param ikey text key (will take various forms of 'BOOK CH:VS'.
    * See parse() for more detailed information)
    */
    VerseTreeKey(TreeKey const & treeKey, char const * const ikey = nullptr);

    /**
    * VerseKey Constructor - initializes instance of VerseKey
    *
    * @param treeKey a TreeKey which will form the basis of this VerseTreeKey
    * @param ikey base key (will take various forms of 'BOOK CH:VS'.
    *    See parse() for more detailed information)
    */
    VerseTreeKey(TreeKey const & treeKey, SWKey const & ikey);

    /** VerseKey Constructor - initializes instance of VerseKey
    * with boundariess - see also lowerBoundKey()
    * and upperBoundKey()
    * @param treeKey a TreeKey which will form the basis of this VerseTreeKey
    * @param min the lower boundary of the new    VerseKey
    * @param max the upper boundary of the new    VerseKey
    */
    VerseTreeKey(TreeKey const & treeKey,
                 char const * const min,
                 char const * const max);

    /**    VerseKey Copy Constructor - will create a new    VerseKey
    * based on an existing one
    *
    * @param k the    VerseKey to copy from
    */
    VerseTreeKey(VerseTreeKey const & k);

    ~VerseTreeKey() override;

    std::shared_ptr<SWKey> clone() const override;

    bool isTraversable() const override;

    std::shared_ptr<TreeKey const> getTreeKey() const;

    void positionChanged() override;

    void decrement(int steps = 1) override;
    void increment(int steps = 1) override;

    virtual void Normalize(char autocheck = 0);

    void positionToTop() override;
    void positionToBottom() override;
    virtual long NewIndex() const;

    SWKEY_OPERATORS(VerseTreeKey)

    SWKey & operator=(VerseKey const & ikey) override;

};

} /* namespace swordxx */

#endif /* SWORDXX_VERSETREEKEY_H */
