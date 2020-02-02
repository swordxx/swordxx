/******************************************************************************
 *
 *  versekey.h -    code for class 'VerseKey'- a standard Biblical verse
 *            key
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


#ifndef SWORDXX_VERSEKEY_H
#define SWORDXX_VERSEKEY_H

#include "../swkey.h"

#include <array>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include "listkey.h"
#include "../defs.h"
#include "../versificationmgr.h"


namespace swordxx {

class SWLocale;

/**
 * Class VerseKey
 * The SWKey implementation used for verse based modules like Bibles or commentaries.
 */
class SWDLLEXPORT VerseKey : public SWKey {

    ListKey m_internalListKey;

    std::shared_ptr<VersificationMgr::System const> m_refSys;

    /** flag for auto normalization
    */
    bool m_autonorm = true;

    /** flag for intros on/off
    */
    bool m_intros = false;

    bool m_boundSet = false;

    // bounds caching is mutable, thus const
    void initBounds() const;

    // private with no bounds check
    void setFromOther(const VerseKey &vk);

    void checkBounds();

    // internal upper/lower bounds optimizations, if autonorms is on
    mutable long m_lowerBound = 0;
    mutable long m_upperBound = 0;
    mutable std::shared_ptr<VerseKey> m_tmpClone;

    struct VerseComponents {
        int test;
        int book;
        int chap;
        int verse;
        char suffix;
    };

    mutable VerseComponents m_lowerBoundComponents, m_upperBoundComponents;    // if autonorms is off, we can't optimize with index

    std::size_t m_userData = 0;

    std::shared_ptr<SWLocale> m_locale;

protected:

    /** The Testament: 0 - Module Heading; 1 - Old; 2 - New
    */
    signed char m_testament = 1;
    signed char m_book = 1;
    signed int m_chapter = 1;
    signed int m_verse = 1;
    signed char m_suffix = 0;

    /************************************************************************
     * VerseKey::bookFromAbbrev - Attempts to find a book no from a name or
     *                           abbreviation
     *
     * ENT:    @param abbr - key for which to search;
     * RET:    @return book number or empty
     */
    std::optional<std::size_t> bookFromAbbrev(std::string_view abbr) const;

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

    /**
      \param[in] v11n The versification system to use, if nullptr then KJV is
                      used.
    */
    VerseKey(std::shared_ptr<VersificationMgr::System const> v11n = nullptr);

    /**
    * VerseKey Constructor - initializes Instance of VerseKey
    *
    * @param ikey text key (will take various forms of 'BOOK CH:VS'.
      \param[in] v11n The versification system to use, if nullptr then KJV is
                      used.
    * See parse() for more detailed information)
    */
    VerseKey(char const * ikey,
             std::shared_ptr<VersificationMgr::System const> v11n = nullptr);

    /** VerseKey Constructor - initializes instance of VerseKey
    * with boundariess - see also lowerBoundKey()
    * and upperBoundKey()
    * @param min the lower boundary of the new    VerseKey
    * @param max the upper boundary of the new    VerseKey
      \param[in] v11n The versification system to use, if nullptr then KJV is
                      used.
    */
    VerseKey(char const * min,
             char const * max,
             std::shared_ptr<VersificationMgr::System const> v11n = nullptr);

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

    bool isBoundSet() const noexcept final override;

    /** sets the lower boundary for this VerseKey
    *
    * @param lb the new lower boundary for this VerseKey
    */
    void setLowerBoundKey(const VerseKey &lb);

    /** sets the upper boundary for this VerseKey
    * @param ub the new upper boundary for this VerseKey
    */
    void setUpperBoundKey(const VerseKey &ub);

    /** gets the lower boundary of this VerseKey
    * @return the lower boundary of this VerseKey
    */
    VerseKey lowerBoundKey() const;

    /** gets the upper boundary of this VerseKey
    * @return the upper boundary of this VerseKey
    */
    VerseKey upperBoundKey() const;

    /** clears the boundaries of this VerseKey
    */
    void clearBounds() noexcept;

    /** Creates a new SWKey based on the current VerseKey
    * see also the Copy Constructor
    */
    std::shared_ptr<SWKey> clone() const override;

    /** refreshes keytext before returning if cast to
    * a (char *) is requested
    */
    std::string getText() const override;
    std::string getShortText() const override;

    void setText(char const * ikey, bool checkNormalize)
    { SWKey::setText(ikey); parse(checkNormalize); }

    void setText(std::string newText) override;

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
    */
    void decrement(int steps = 1) override;

    /** Increments key a number of verses
    *
    * @param steps Number of verses to jump forward
    */
    void increment(int steps = 1) override;
    bool isTraversable() const override;

    /** Get/Set position of this key by Book Name
     */
    std::string getBookName() const;
    void setBookName(std::string_view bookName);

    std::string getBookAbbrev() const;

    /** \returns the number of the testament (or 0 for introduction/heading). */
    char getTestament() const noexcept { return m_testament; }
    int getTestamentMax() const { return 2; }

    /** \returns the number of the book (or 0 for introduction/heading). */
    char getBook() const noexcept { return m_book; }
    std::size_t getBookMax() const;

    /** \returns the number of the chapter (or 0 for introduction/heading). */
    int getChapter() const noexcept { return m_chapter; }
    int getChapterMax() const;

    /** \returns the number of the verse (or 0 for introduction/heading). */
    int getVerse() const noexcept { return m_verse; }
    int getVerseMax() const;

    /** \returns the number of verse suffix. */
    char getSuffix() const noexcept { return m_suffix; }

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
    void setSuffix(char isuffix) noexcept;

    /** checks limits and normalizes if necessary (e.g.
    * Matthew 29:47 = Mark 2:2).    If last verse is
    * exceeded, key is set to last Book CH:VS
    *
    */
    void normalize(bool autocheck = false);

    /** Sets flag that tells VerseKey to
    * automatically normalize itself when modified
    *
    * @param iautonorm value which to set autonorm
    */
    void setAutoNormalize(bool iautonorm);

    /** \returns whether this VerseKey is set to automatically normalize itself
                 when modified. */
    bool isAutoNormalize() const { return m_autonorm; }

    /** The Intros property determine whether or not to include
    * chapter/book/testament/module intros
    */
    void setIntros(bool val);

    /** \returns whether this VerseKey is set to include introductions for
                 chapters, books, testaments and modules. */
    bool isIntros() const { return m_intros; }


    /** Gets index based upon current verse
    *
    * @return offset
    */
    long getIndex() const override;


    /** Sets index based upon current verse
    *
    * @param iindex value to set index to
    */
    void setIndex(long iindex) override;


    /** Gets index into current testament based upon current verse
    *
    * @return offset
    */
    long getTestamentIndex() const;

    std::string getOSISRef() const;
    std::string const & getOSISBookName() const;

    /** Tries to parse a string and convert it into an OSIS reference
     * @param inRef reference string to try to parse
     * @param defaultKey for details @see ParseVerseList(..., defaultKey, ...)
     */
    static std::string convertToOSIS(std::string const & inRef,
                                     SWKey const * defaultKey);

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

    void setVersificationSystem(
            std::shared_ptr<VersificationMgr::System const> vs);

    std::shared_ptr<VersificationMgr::System const> versificationSystem()
            const noexcept
    { return m_refSys; }

    /** \returns a pointer to the locale used by this VerseKey. */
    std::shared_ptr<SWLocale> locale() const noexcept { return m_locale; }

    /**
      \brief Sets the locale to use for this VerseKey instance.
      \param[in] locale The locale to use, must not be nullptr.
    */
    void setLocale(std::shared_ptr<SWLocale> locale);


    // OPERATORS --------------------------------------------------------------------


    SWKEY_OPERATORS(VerseKey)

    virtual SWKey &operator =(const VerseKey &ikey) { positionFrom(ikey); return *this; }
};

} /* namespace swordxx */

#endif /* SWORDXX_VERSEKEY_H */
