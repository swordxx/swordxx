/******************************************************************************
 *
 *  swkey.h -    code for base class 'swkey'.  swkey is the basis for all
 *        types of keys for indexing into modules (e.g. verse, word,
 *        place, etc.)
 *
 * $Id$
 *
 * Copyright 1998-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWKEY_H
#define SWKEY_H

#include <cstdint>
#include <memory>
#include <string>
#include "defs.h"
#include "utilstr.h"
#include "position.h"


namespace swordxx {

#define KEYERR_OUTOFBOUNDS 1

#define SWKEY_OPERATORS(cn) \
  cn & operator=(char const * ikey) { setText(ikey); return *this; } \
  cn & operator=(SWKey const & ikey) { positionFrom(ikey); return *this; } \
  cn & operator=(decltype(Position::Top)) { positionToTop(); return *this; } \
  cn & operator=(decltype(Position::Bottom)) { positionToBottom(); return *this; } \
  cn & operator-=(int steps) { decrement(steps); return *this; } \
  cn & operator+=(int steps) { increment(steps); return *this; } \
  cn & operator++() { increment(1); return *this; } \
  cn & operator--() { decrement(1); return *this; }

class SWLocale;

/** SWKey is used for positioning an SWModule to a specific entry.
 *    It always represents a possible location into a module and can additionally represent
 *    a domain of entries (e.g. "John 3:16" in the domain "John 1:1 - Mark 5:25")
 */
class SWDLLEXPORT SWKey {

    // for caching; don't use directly, call getPrivateLocale()
    mutable std::shared_ptr<SWLocale> m_locale;


    long m_index;
    void init();


protected:
    std::unique_ptr<std::string> m_keyText;
    bool m_boundSet;
    bool m_persist;
    char m_error;

    std::string m_localeName;
    SWLocale & getPrivateLocale() const;


public:

    // misc storage for whatever
    uint64_t m_userData;

    /** initializes instance of SWKey from a string
     * All keys can be reduced to a string representation which should be able
     *    to be used to again set the key to the same position
     * @param ikey string to use for initializing this new key
     */
    SWKey(char const * ikey = nullptr);

    /** Copy Constructor
     * @param k The SWKey object to copy.
     */
    SWKey(const SWKey &k);

    /** Destructor, cleans up this instance of SWKey
     */
    virtual ~SWKey();

    /** Returns a new exact clone of this SWKey object.  This allocates
     * a new SWKey which must be deleted by the caller
     * @return new clone of this key
     */
    virtual SWKey *clone() const;


    /** Gets whether this key should persist in any module to which it is set
     * otherwise just a copy will be used in the module.
     * @return 1 - persists in module; 0 - a copy is attempted
     */
    inline bool isPersist() const noexcept { return m_persist; }

    /** Sets whether this key should persist in any module to which it is set
     * otherwise just a copy will be used in the module.
     * @param ipersist value which to set persist;
     * @return 1 - persists in module; 0 - a copy is attempted
     */
    inline void setPersist(bool ipersist) noexcept { m_persist = ipersist; }

    /** Gets and clears error status
     * @return error status
     */
    inline char popError() noexcept {
        char const r = m_error;
        m_error = 0;
        return r;
    }

    inline void setError(char const err) noexcept { m_error = err; }

    /** Sets this SWKey with a character string
     * @param ikey string used to set this key
     */
    virtual void setText(const char * const ikey);

    /** Copies as much info (position, range, etc.) as possible from another SWKey object
     * @param ikey other SWKey object from which to copy
     */
    virtual void copyFrom(const SWKey &ikey);
    virtual void positionFrom(const SWKey &ikey) { copyFrom(ikey); }

    /** returns string representation of this key
     */
    virtual const char *getText() const;
    virtual std::string getShortText() const { return getText(); }
    virtual std::string getRangeText() const;
    virtual std::string getOSISRefRangeText() const;

    inline bool isBoundSet() const noexcept { return m_boundSet; }
    inline void clearBound() noexcept { m_boundSet = false; }

    /** Compares this key object to another SWKey object
     * @param ikey key to compare with this one
     * @return >0 if this key is greater than compare key;
     *    <0 if this key is smaller than compare key;
     *    0 if the keys are the same
     */
    virtual int compare(SWKey const & ikey) const noexcept;

    /** test equality of this SWKey object's position with another SWKey
     * @param ikey key to compare with this one
     * @return true if the key positions are equal
     */
    virtual bool equals(SWKey const & ikey) const noexcept
    { return !compare(ikey); }

    virtual void positionToTop();
    virtual void positionToBottom();

    /** Decrements key a number of entry positions
     * This is only valid if isTraversable is true
     * @param steps Number of entries to jump backward
     */
    virtual void decrement(int steps = 1);

    /** Increments key a number of entry positions
     * This is only valid if isTraversable is true
     * @param steps Number of entries to jump forward
     */
    virtual void increment(int steps = 1);

    /** Whether or not this key can be ++ -- incremented
     */
    virtual bool isTraversable() const { return false; }

    std::string const & getLocale() const noexcept
    { return m_localeName; }

    // this will force an on demand lookup of our locale:
    void setLocale(char const * name) {
        if (name) {
            m_localeName = name;
        } else {
            m_localeName.clear();
        }
        m_locale = nullptr;
    }

    /** Use this function to get an index position within a module.
     */
    virtual long getIndex() const { return m_index; }

    /** See documentation for @ref Index()
     */
    virtual void setIndex(long iindex) { m_index = iindex; }

    SWKEY_OPERATORS(SWKey)

}; /* class SWKey */

inline bool operator==(SWKey const & lhs, SWKey const & rhs) noexcept
{ return lhs.equals(rhs); }

inline bool operator!=(SWKey const & lhs, SWKey const & rhs) noexcept
{ return !lhs.equals(rhs); }

inline bool operator>(SWKey const & lhs, SWKey const & rhs) noexcept
{ return lhs.compare(rhs) > 0; }

inline bool operator<(SWKey const & lhs, SWKey const & rhs) noexcept
{ return lhs.compare(rhs) < 0; }

inline bool operator>=(SWKey const & lhs, SWKey const & rhs) noexcept
{ return !(lhs.compare(rhs) < 0); }

inline bool operator<=(SWKey const & lhs, SWKey const & rhs) noexcept
{ return !(lhs.compare(rhs) > 0); }

} /* namespace swordxx */
#endif
