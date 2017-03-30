/******************************************************************************
 *
 *  versekey.h -    code for class 'VerseKey'- a standard Biblical verse
 *            key
 *
 * $Id$
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ    85280-2528
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


#ifndef VERSEKEY_H
#define VERSEKEY_H

#include "../swkey.h"

#include <memory>
#include "listkey.h"
#include "../defs.h"
#include "../versificationmgr.h"


namespace swordxx {

#define VERSEKEY_OPERATORS(cn) \
    SWKEY_OPERATORS(cn) \
    cn & operator=(decltype(Position::MaxVerse)) \
    { positionToMaxVerse(); return *this; } \
    cn & operator=(decltype(Position::MaxChapter)) \
    { positionToMaxChapter(); return *this; }

/**
 * Class VerseKey
 * The SWKey implementation used for verse based modules like Bibles or commentaries.
 */
class SWDLLEXPORT VerseKey : public SWKey {

    ListKey m_internalListKey;

    VersificationMgr::System const * m_refSys = nullptr;

    /** flag for auto normalization
    */
    bool m_autonorm = true;

    /** flag for intros on/off
    */
    bool m_intros = false;

    // bounds caching is mutable, thus const
    void initBounds() const;

    // private with no bounds check
    void setFromOther(const VerseKey &vk);

    void checkBounds();

    // internal upper/lower bounds optimizations, if autonorms is on
    mutable long m_lowerBound = 0;
    mutable long m_upperBound = 0;
    mutable std::unique_ptr<VerseKey> m_tmpClone;

    typedef struct VerseComponents { int test; int book; int chap; int verse; char suffix; } VerseComponents;

    mutable VerseComponents m_lowerBoundComponents, m_upperBoundComponents;    // if autonorms is off, we can't optimize with index

protected:

    /** The Testament: 0 - Module Heading; 1 - Old; 2 - New
    */
    signed char m_testament = 1;
    signed char m_book = 1;
    signed int m_chapter = 1;
    signed int m_verse = 1;
    signed char m_suffix = 0;

    /************************************************************************
     * VerseKey::getBookFromAbbrev - Attempts to find a book no from a name or
     *                           abbreviation
     *
     * ENT:    @param abbr - key for which to search;
     * RET:    @return book number or < 0 = not valid
     */
    virtual int getBookFromAbbrev(const char *abbr) const;

    /** Refresh keytext based on testament|book|chapter|verse
    * default auto normalization to true
    * default display headings option is false
    */
    void freshtext() const;
    /**    Parse a character array into testament|book|chapter|verse
    *
    */
    char parse(bool checkNormalize = true);
public:
#if 0
    static long otbks[];
    static long otcps[];
    static long ntbks[];
    static long ntcps[];
#endif
    int m_BMAX[2];

    /**
    * VerseKey Constructor - initializes Instance of VerseKey
    *
    * @param ikey text key (will take various forms of 'BOOK CH:VS'.
    * See parse() for more detailed information)
    */
    VerseKey(char const * ikey = nullptr);

    /** VerseKey Constructor - initializes instance of VerseKey
    * with boundariess - see also LowerBound()
    * and UpperBound()
    * @param min the lower boundary of the new    VerseKey
    * @param max the upper boundary of the new    VerseKey
    */
    VerseKey(const char *min, const char *max, const char *v11n = "KJV");

    /**    VerseKey Copy Constructor - will create a new VerseKey
    * based on an existing SWKey
    *
    * @param k the    VerseKey to copy from
    */
    VerseKey(const SWKey &k);

    /**    VerseKey Copy Constructor - will create a new VerseKey
    * based on an existing one
    *
    * @param k the    VerseKey to copy from
    */
    VerseKey(const VerseKey &k);

    /**    VerseKey Destructor
    * Cleans up an instance of VerseKey
    */
    ~VerseKey() override;

    /** sets the lower boundary for this VerseKey
    *
    * @param lb the new lower boundary for this VerseKey
    */
    void setLowerBound(const VerseKey &lb);

    /** sets the upper boundary for this VerseKey
    * @param ub the new upper boundary for this VerseKey
    * @return the upper boundary the key was set to
    */
    void setUpperBound(const VerseKey &ub);

    /** gets the lower boundary of this VerseKey
    * @return the lower boundary of this VerseKey
    */
    VerseKey getLowerBound() const;

    /** gets the upper boundary of this VerseKey
    * @return the upper boundary of this VerseKey
    */
    VerseKey getUpperBound() const;

    /** clears the boundaries of this VerseKey
    */
    void clearBounds();

    /** Creates a new SWKey based on the current VerseKey
    * see also the Copy Constructor
    */
    SWKey * clone() const override;

    /** refreshes keytext before returning if cast to
    * a (char *) is requested
    */
    char const * getText() const override;
    std::string getShortText() const override;

    void setText(char const * ikey, bool checkNormalize)
    { SWKey::setText(ikey); parse(checkNormalize); }

    void setText(char const * ikey) override
    { SWKey::setText(ikey); parse(); }

    void copyFrom(SWKey const & ikey) override;

    /** Equates this VerseKey to another VerseKey
    */
    virtual void copyFrom(const VerseKey &ikey);

    /** Only repositions this VerseKey to another VerseKey
    */
    void positionFrom(SWKey const & ikey) override;

    void positionToTop() override;
    void positionToBottom() override;
    virtual void positionToMaxChapter();
    virtual void positionToMaxVerse();

    /** Decrements key a number of verses
    *
    * @param steps Number of verses to jump backward
    * @return *this
    */
    void decrement(int steps = 1) override;

    /** Increments key a number of verses
    *
    * @param steps Number of verses to jump forward
    * @return *this
    */
    void increment(int steps = 1) override;
    bool isTraversable() const override { return true; }

    /** Get/Set position of this key by Book Name
     */
    std::string const & getBookName() const;
    void setBookName(const char *bname);

    std::string const & getBookAbbrev() const;
    /** Gets testament
    *
    * @return value of testament
    */
    char getTestament() const;
    int getTestamentMax() const { return 2; }

    /** Gets book
    *
    * @return value of book
    */
    char getBook() const;
    int getBookMax() const { return m_BMAX[m_testament-1]; }

    /** Gets chapter
    *
    * @return value of chapter
    */
    int getChapter() const;
    int getChapterMax() const;

    /** Gets verse
    *
    * @return value of verse
    */
    int getVerse() const;
    int getVerseMax() const;

    /** Gets verse suffix
    *
    * @return value of verse suffix
    */
    char getSuffix() const;

    /** Sets testament
    *
    * @param itestament value which to set testament
    */
    void setTestament(char itestament);

    /** Sets book
    *
    * @param ibook value which to set book
    */
    void setBook(char ibook);

    /** Sets chapter
    *
    * @param ichapter value which to set chapter
    */
    void setChapter(int ichapter);

    /** Sets verse
    *
    * @param iverse value which to set verse
    */
    void setVerse(int iverse);

    /** Sets/gets verse suffix
    *
    * @param isuffix value which to set verse suffix
    */
    void setSuffix(char isuffix);

    /** checks limits and normalizes if necessary (e.g.
    * Matthew 29:47 = Mark 2:2).    If last verse is
    * exceeded, key is set to last Book CH:VS
    *
    * @return *this
    */
    void normalize(bool autocheck = false);

    /** Sets flag that tells VerseKey to
    * automatically normalize itself when modified
    *
    * @param iautonorm value which to set autonorm
    * [std::numeric_limits<char>::max()] - only get
    * @return if unchanged -> value of autonorm,
    * if changed -> previous value of autonorm
    */
    void setAutoNormalize(bool iautonorm);
    bool isAutoNormalize() const;

    /** The Intros property determine whether or not to include
    * chapter/book/testament/module intros
    */
    void setIntros(bool val);
    bool isIntros() const;


    /** Gets index based upon current verse
    *
    * @return offset
    */
    long getIndex() const override;


    /** Sets index based upon current verse
    *
    * @param iindex value to set index to
    * @return offset
    */
    void setIndex(long iindex) override;


    /** Gets index into current testament based upon current verse
    *
    * @return offset
    */
    long getTestamentIndex() const;

    const char *getOSISRef() const;
    std::string const & getOSISBookName() const;

    /** Tries to parse a string and convert it into an OSIS reference
     * @param inRef reference string to try to parse
     * @param defaultKey @see ParseVerseList(..., defaultKey, ...)
     */
    static std::string convertToOSIS(const char *inRef, const SWKey *defaultKey);

    /******************************************************************************
     * VerseKey::parseVerseList - Attempts to parse a buffer into separate
     *                verse entries returned in a ListKey
     *
     * ENT:    buf        - buffer to parse;
     *    defaultKey    - if verse, chap, book, or testament is left off,
     *                pull info from this key (ie. Gen 2:3; 4:5;
     *                Gen would be used when parsing the 4:5 section)
     *    expandRange    - whether or not to expand eg. John 1:10-12 or just
     *                save John 1:10
     *
     * RET:    ListKey reference filled with verse entries contained in buf
     *
     * COMMENT: This code works but wreaks.  Rewrite to make more maintainable.
     */
    ListKey parseVerseList(char const * buf,
                           char const * defaultKey = nullptr,
                           bool expandRange = false,
                           bool useChapterAsVerse = false);
    std::string getRangeText() const override;
    std::string getOSISRefRangeText() const override;
    /** Compares another    SWKey object
    *
    * @param ikey key to compare with this one
    * @return >0 if this    VerseKey is greater than compare    SWKey,
    * <0 if this    VerseKey is smaller than compare    SWKey,
    * 0 if the keys are the same
    */
    int compare(SWKey const & ikey) const noexcept override;

    /** Compares another    VerseKey object
    *
    * @param ikey key to compare with this one
    * @return >0 if this    VerseKey is greater than compare    VerseKey,
    * <0 if this    VerseKey is smaller than compare    VerseKey,
    * 0 if the keys are the same
    */
    int compare_(VerseKey const & ikey) const noexcept;

    void setVersificationSystem(const char *name);
    std::string const & getVersificationSystem() const;

    // DEBUG
    void validateCurrentLocale() const;


    // OPERATORS --------------------------------------------------------------------


    VERSEKEY_OPERATORS(VerseKey)

    virtual SWKey &operator =(const VerseKey &ikey) { positionFrom(ikey); return *this; }
};

} /* namespace swordxx */

#endif //VERSEKEY_H
