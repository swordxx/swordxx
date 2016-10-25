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

#include <list>
#include <memory>
#include <string>
#include "defs.h"


#ifndef VERSIFICATIONMGR_H
#define VERSIFICATIONMGR_H


namespace swordxx {

typedef std::list <std::string>StringList;

struct sbook;
class TreeKey;


struct abbrev
{
    const char *ab;
    const char *osis;
};

struct sbook {
    /**Name of book
    */
    const char *name;

    /**OSIS name
    */
    const char *osis;

    /**Preferred Abbreviation
    */
    const char *prefAbbrev;

    /**Maximum chapters in book
    */
    unsigned char chapmax;
    /** Array[chapmax] of maximum verses in chapters
    */
    int *versemax;
};


class SWDLLEXPORT VersificationMgr {


public:
    class System;

private:

    class Private;
    Private *p;

    void init();

protected:
    static std::unique_ptr<VersificationMgr> systemVersificationMgr;

public:
    class SWDLLEXPORT Book {
        friend class System;
        friend struct BookOffsetLess;
        class Private;
        Private *m_p;

        /** book name */
        std::string m_longName;

        /** OSIS Abbreviation */
        std::string m_osisName;

        /** Preferred Abbreviation */
        std::string m_prefAbbrev;

        /** Maximum chapters in book */
        unsigned int m_chapMax;

        void init();

    public:
        Book() { init(); }
        Book(const Book &other);
        Book &operator =(const Book &other);
        Book(std::string longName,
             std::string osisName,
             std::string prefAbbrev,
             unsigned int chapMax)
            : m_longName(std::move(longName))
            , m_osisName(std::move(osisName))
            , m_prefAbbrev(std::move(prefAbbrev))
            , m_chapMax(chapMax)
        {
            init();
        }
        ~Book();
        std::string const & getLongName() const noexcept { return m_longName; }
        const char *getOSISName() const noexcept { return m_osisName.c_str(); }
        const char *getPreferredAbbreviation() const noexcept { return m_prefAbbrev.c_str(); }
        unsigned int getChapterMax() const noexcept { return m_chapMax; }
        int getVerseMax(int chapter) const;
    };

    class SWDLLEXPORT System {
        class Private;
        Private *p;
        std::string name;
        int BMAX[2];
        long ntStartOffset;
        void init();
    public:
        System() { this->name = ""; init(); }
        System(const System &other);
        System &operator =(const System &other);
        System(const char *name) { this->name = name; init(); }
        ~System();
        const char *getName() const { return name.c_str(); }
        const Book *getBookByName(const char *bookName) const;
        int getBookNumberByOSISName(const char *bookName) const;
        const Book *getBook(int number) const;
        int getBookCount() const;
        void loadFromSBook(sbook const * ot,
                           sbook const * nt,
                           int * chMax,
                           unsigned char const * mappings = nullptr);
        long getOffsetFromVerse(int book, int chapter, int verse) const;
        char getVerseFromOffset(long offset, int *book, int *chapter, int *verse) const;
        const int *getBMAX() const { return BMAX; };
        long getNTStartOffset() const { return ntStartOffset; }
        void translateVerse(const System *dstSys, const char **book, int *chapter, int *verse, int *verse_end) const;
    };
    VersificationMgr() { init(); }
    ~VersificationMgr();
    static VersificationMgr *getSystemVersificationMgr();
    static void setSystemVersificationMgr(VersificationMgr *newVersificationMgr);
    const StringList getVersificationSystems() const;
    const System *getVersificationSystem(const char *name) const;
    void registerVersificationSystem(char const * name,
                                     sbook const * ot,
                                     sbook const * nt,
                                     int * chMax,
                                     unsigned char const * mappings = nullptr);
    void registerVersificationSystem(const char *name, const TreeKey *);
};

SWDLLEXPORT extern const struct abbrev builtin_abbrevs[];

} /* namespace swordxx */
#endif
