/******************************************************************************
 *
 *  listkey.h -    code for base class 'listkey'.  listkey is the basis for all
 *        types of keys for indexing into modules
 *        (e.g. verse, word, place, etc.)
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

#ifndef SWLSTKEY_H
#define SWLSTKEY_H

#include "../swkey.h"

#include "../defs.h"


namespace swordxx {

  /** ListKey is the basis for all
  * types of keys that have lists of specified indexes
  * (e.g. a list of verses, place, etc.)
  */
class SWDLLEXPORT ListKey : public SWKey {

    void init();

protected:
    int arraypos;
    int arraymax;
    int arraycnt;
    SWKey **array;

public:

    /** initializes instance of ListKey
     *
     * @param ikey text key
     */
    ListKey(char const * ikey = nullptr);
    ListKey(ListKey const &k);

    /** cleans up instance of ListKey
     */
    ~ListKey() override;

    SWKey * clone() const override;

    /** Clears out elements of list
     */
    virtual void clear();


    /** Returns number of key elements in list
     * @return number of key elements in list
     */
    virtual int getCount() const;

    /** Removes current element from list
     */
    virtual void remove();


    /** Sets container to subkey element number and repositions that subkey to either top or bottom
     *
     * @param ielement - element number to set to
     * @param pos - set the subkey element to position (TOP) or BOTTOM
     * @return error status
     */
    virtual char setToElement(int ielement, SW_POSITION = TOP);



    /** Gets a key element number
     *
     * @param pos element number to get (or default current)
     * @return Key or null on error
     */
    virtual SWKey *getElement(int pos = -1);
    virtual const SWKey *getElement(int pos = -1) const;

    /** Adds an element to the list
     * @param ikey the element to add
     */
    ListKey & operator <<(const SWKey &ikey) { add(ikey); return *this; }
    virtual void add(const SWKey &ikey);

    /** Equates this ListKey to another ListKey object
     *
     * @param ikey other ListKey object
     */
    virtual void copyFrom(const ListKey & ikey);
    void copyFrom(SWKey const & ikey) override { SWKey::copyFrom(ikey); }

    /** Positions this key
     *
     * @param pos position
     * @return *this
     */
    void setPosition(SW_POSITION pos) override;

    /** Decrements a number of elements
     */
    void decrement(int step = 1) override;

    /** Increments a number of elements
     */
    void increment(int step = 1) override;

    bool isTraversable() const override { return true; }
    long getIndex() const override { return arraypos; }
    std::string getRangeText() const override;
    std::string getOSISRefRangeText() const override;
    char const * getShortText() const override;

    /**
     * Returns the index for the new one given as as parameter.
     * The first parameter is the new index.
     */
    void setIndex(long index)  override{ setToElement(index); }
    char const * getText() const override;
    void setText(char const * ikey) override;
    virtual void sort();

    SWKEY_OPERATORS(ListKey)
    ListKey & operator =(const ListKey &key) { copyFrom(key); return *this; }
};

} /* namespace swordxx */
#endif
