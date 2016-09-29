/******************************************************************************
 *
 *  versetreekey.h -    code for class 'VerseTreeKey'- a standard Biblical
 *            verse key
 *
 * $Id$
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


#ifndef VERSETREEKEY_H
#define VERSETREEKEY_H

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

    TreeKey *treeKey;
//    vector<struct sbook> books;

    void init(TreeKey *treeKey);
    void syncVerseToTree();
    long lastGoodOffset;

protected:
    int getBookFromAbbrev(char const * abbr) const override;

public:

    /**
    * VerseKey Constructor - initializes Instance of VerseKey
    *
    * @param treeKey a TreeKey which will form the basis of this VerseTreeKey
    * @param ikey text key (will take various forms of 'BOOK CH:VS'.
    * See parse() for more detailed information)
    */
    VerseTreeKey(TreeKey * treeKey, char const * ikey = nullptr);

    /**
    * VerseKey Constructor - initializes instance of VerseKey
    *
    * @param treeKey a TreeKey which will form the basis of this VerseTreeKey
    * @param ikey base key (will take various forms of 'BOOK CH:VS'.
    *    See parse() for more detailed information)
    */
    VerseTreeKey(TreeKey *treeKey, const SWKey *ikey);

    /** VerseKey Constructor - initializes instance of VerseKey
    * with boundariess - see also LowerBound()
    * and UpperBound()
    * @param treeKey a TreeKey which will form the basis of this VerseTreeKey
    * @param min the lower boundary of the new    VerseKey
    * @param max the upper boundary of the new    VerseKey
    */
    VerseTreeKey(TreeKey *treeKey, const char *min, const char *max);

    /**    VerseKey Copy Constructor - will create a new    VerseKey
    * based on an existing one
    *
    * @param k the    VerseKey to copy from
    */
    VerseTreeKey(const VerseTreeKey &k);

    ~VerseTreeKey() override;

    SWKey * clone() const override;

    bool isTraversable() const override { return true; }

    virtual TreeKey *getTreeKey();

    void positionChanged() override;
    bool internalPosChange;

    void decrement(int steps = 1) override;
    void increment(int steps = 1) override;

    virtual void Normalize(char autocheck = 0);

    void setPosition(SW_POSITION newpos) override;
    virtual long NewIndex() const;

    SWKEY_OPERATORS(VerseTreeKey)

    SWKey & operator=(VerseKey const & ikey) override {
        copyFrom(ikey);
        return *this;
    }

};

} /* namespace swordxx */

#endif //VERSETREEKEY_H
