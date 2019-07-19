/******************************************************************************
 *
 *  versification.h -    definition of class VersificationMgr used for managing
 *            versification systems
 *
 * $Id$
 *
 * Copyright 2008-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWORDXX_VERSIFICATIONMGR_H
#define SWORDXX_VERSIFICATIONMGR_H

#include <list>
#include <memory>
#include <string>
#include <utility>
#include "defs.h"


namespace swordxx {

struct BookOffsetLess;
class TreeKey;

struct abbrev {
    char const * ab;
    char const * osis;
};

struct sbook {

    /** Name of the book: */
    char const * name;

    /** OSIS name: */
    char const * osis;

    /** Preferred abbreviation: */
    char const * prefAbbrev;

    /** Maximum chapters in book: */
    unsigned char chapmax;

    /** Array[chapmax] of maximum verses in chapters: */
    int * versemax;

};


class SWDLLEXPORT VersificationMgr {

public: /* Types: */

    class System;

    class SWDLLEXPORT Book {

        friend class System;
        friend struct BookOffsetLess;

    private: /* Types: */

        class Private;

    public: /* Methods: */

        Book() { init(); }

        Book(Book const & copy);

        Book(std::string longName,
             std::string osisName,
             std::string prefAbbrev,
             unsigned int chapMax)
            : m_longName(std::move(longName))
            , m_osisName(std::move(osisName))
            , m_prefAbbrev(std::move(prefAbbrev))
            , m_chapMax(chapMax)
        { init(); }

        ~Book();

        Book & operator=(Book const & copy);

        std::string const & getLongName() const noexcept { return m_longName; }

        std::string const & getOSISName() const noexcept { return m_osisName; }

        std::string const & getPreferredAbbreviation() const noexcept
        { return m_prefAbbrev; }

        unsigned int getChapterMax() const noexcept { return m_chapMax; }

        int getVerseMax(int chapter) const noexcept;

    private: /* Methods: */

        void init();

    private: /* Fields: */

        Private *m_p;

        /** book name */
        std::string m_longName;

        /** OSIS Abbreviation */
        std::string m_osisName;

        /** Preferred Abbreviation */
        std::string m_prefAbbrev;

        /** Maximum chapters in book */
        unsigned int m_chapMax;

    };

    class SWDLLEXPORT System {

    private: /* Types: */

        class Private;

    public: /* Methods: */

        System() { init(); }

        System(System const & copy);

        System(char const * name) : m_name(name) { init(); }

        ~System();

        System & operator=(System const & copy);

        std::string const & getName() const noexcept { return m_name; }

        int getBookNumberByOSISName(char const * bookName) const noexcept;

        Book const * getBook(int number) const noexcept;

        int getBookCount() const noexcept;

        void loadFromSBook(sbook const * ot,
                           sbook const * nt,
                           int const * chMax,
                           unsigned char const * mappings = nullptr);

        long getOffsetFromVerse(int book, int chapter, int verse) const noexcept;

        char getVerseFromOffset(long offset,
                                int * book,
                                int * chapter,
                                int * verse) const noexcept;

        int const * getBMAX() const noexcept { return m_BMAX; }

        long getNTStartOffset() const noexcept { return m_ntStartOffset; }

        void translateVerse(System const * dstSys,
                            char const ** book,
                            int * chapter,
                            int * verse,
                            int * verse_end) const;

    private: /* Methods: */

        void init();

    private: /* Fields: */

        Private * m_p;
        std::string m_name;
        int m_BMAX[2];
        long m_ntStartOffset;

    };

private: /* Types: */

    class Private;

public: /* Methods: */

    VersificationMgr() { init(); }

    ~VersificationMgr();

    static std::shared_ptr<VersificationMgr const> systemVersificationMgr();

    static void setSystemVersificationMgr(
            std::shared_ptr<VersificationMgr const> newVersificationMgr);

    std::list<std::string> const getVersificationSystems() const;

    System const * getVersificationSystem(char const * name) const;

    void registerVersificationSystem(char const * name,
                                     sbook const * ot,
                                     sbook const * nt,
                                     int const * chMax,
                                     unsigned char const * mappings = nullptr);

private: /* Methods: */

    void init();

private: /* Fields: */

    static std::shared_ptr<VersificationMgr const> m_systemVersificationMgr;
    Private * p;

};

SWDLLEXPORT extern abbrev const builtin_abbrevs[];

} /* namespace swordxx */

#endif /* SWORDXX_VERSIFICATIONMGR_H */
