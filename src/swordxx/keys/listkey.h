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

#include <limits>
#include <memory>
#include <type_traits>
#include <vector>
#include "../defs.h"


namespace swordxx {

  /** ListKey is the basis for all
  * types of keys that have lists of specified indexes
  * (e.g. a list of verses, place, etc.)
  */
class SWDLLEXPORT ListKey : public SWKey {

private:

    std::size_t m_arrayPos = 0;
    std::vector<std::unique_ptr<SWKey> > m_array;

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

    std::unique_ptr<SWKey> clone() const override;

    /** Clears out elements of list
     */
    virtual void clear();


    /** Returns number of key elements in list
     * @return number of key elements in list
     */
    virtual std::size_t getCount() const;

    /** Removes current element from list
     */
    virtual void remove();


    /** \returns setToElementAndTop(ielement); */
    char setToElement(int ielement)
    { return setToElementAndTop(ielement); }

    /** Sets container to subkey element number and repositions that subkey to top
     *
     * @param ielement - element number to set to
     * @return error status
     */
    virtual char setToElementAndTop(int ielement);

    /** Sets container to subkey element number and repositions that subkey to bottom
     *
     * @param ielement - element number to set to
     * @return error status
     */
    virtual char setToElementAndBottom(int ielement);



    /** Gets a key element number
     *
     * @param pos element number to get (or default current)
     * @return Key or null on error
     */
    SWKey * getElement() { return getElement(m_arrayPos); }
    SWKey const * getElement() const { return getElement(m_arrayPos); }
    SWKey * getElement(std::size_t const pos);
    SWKey const * getElement(std::size_t const pos) const;

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

    void positionToTop() override;
    void positionToBottom() override;

    /** Decrements a number of elements
     */
    void decrement(int step = 1) override;

    /** Increments a number of elements
     */
    void increment(int step = 1) override;

    bool isTraversable() const override { return true; }
    long getIndex() const override;
    std::string getRangeText() const override;
    std::string getOSISRefRangeText() const override;
    std::string getShortText() const override;

    /**
     * Returns the index for the new one given as as parameter.
     * The first parameter is the new index.
     */
    void setIndex(long index) override
    { setToElementAndTop_(setToElementCheckBounds(index)); }

    std::string getText() const override;
    void setText(std::string newText) override;
    virtual void sort();

    SWKEY_OPERATORS(ListKey)
    ListKey & operator =(const ListKey &key) { copyFrom(key); return *this; }

private: /* Methods: */

    char setToElementAndTop_(std::size_t const ielement);
    char setToElementAndBottom_(std::size_t const ielement);

    template <typename T>
    std::size_t setToElementCheckBounds(T const ielement) noexcept {
        static_assert(std::is_signed<T>::value, "");
        if (ielement < 0) {
            m_error = KEYERR_OUTOFBOUNDS;
            return 0u;
        }
        static_assert(std::numeric_limits<T>::max()
                      <= std::numeric_limits<std::size_t>::max(), "");
        return setToElementCheckBounds_(static_cast<std::size_t>(ielement));
    }

    std::size_t setToElementCheckBounds_(std::size_t const ielement) noexcept;

};

} /* namespace swordxx */
#endif
