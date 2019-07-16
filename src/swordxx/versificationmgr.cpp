/******************************************************************************
 *
 *  versificationmgr.cpp -    implementation of class VersificationMgr used
 *                for managing versification systems
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

#include "versificationmgr.h"

#include <algorithm>
#include <atomic>
#include <cstring>
#include <vector>
#include <map>
#include "canon.h"             // KJV internal versification system
#include "canon_calvin.h"      // French v11n system
#include "canon_catholic.h"    // Catholic v11n system (10 chapter Esther)
#include "canon_catholic2.h"   // Catholic2 v11n system (16 chapter Esther)
#include "canon_darbyfr.h"     // French v11n system based on John Darby's French translation
#include "canon_german.h"      // German v11n system
#include "canon_kjva.h"        // KJV + Apocrypha v11n system
#include "canon_leningrad.h"   // Leningrad Codex (WLC) v11n system
#include "canon_luther.h"      // Luther v11n system
#include "canon_lxx.h"         // General LXX v11n system (includes GNT, as used in Orthodox Bibles)
#include "canon_mt.h"          // Masoretic Text (MT) v11n system
#include "canon_nrsv.h"        // NRSV v11n system
#include "canon_nrsva.h"       // NRSV + Apocrypha v11n system
#include "canon_null.h"        // null v11n system
#include "canon_orthodox.h"    // Orthodox v11n system as used in Orthodox Bibles
#include "canon_segond.h"      // French v11n system as used by Segond Bibles and its derivatives
#include "canon_synodal.h"     // Russian Synodal v11n system
#include "canon_synodalprot.h" // Russian Synodal v11n system
#include "canon_vulg.h"        // Vulgate v11n system
#include "keys/treekey.h"
#include "swlog.h"


namespace swordxx {

std::shared_ptr<VersificationMgr const>
VersificationMgr::systemVersificationMgr() {
    if (auto r = std::atomic_load_explicit(&m_systemVersificationMgr,
                                           std::memory_order_acquire))
        return r;
    std::shared_ptr<VersificationMgr const> newMgr(
                []() {
                    auto r(std::make_shared<VersificationMgr>());
                    r->registerVersificationSystem("KJV", otbooks, ntbooks, vm);
                    r->registerVersificationSystem("Leningrad", otbooks_leningrad, ntbooks_null, vm_leningrad);
                    r->registerVersificationSystem("MT", otbooks_mt, ntbooks_null, vm_mt);
                    r->registerVersificationSystem("KJVA", otbooks_kjva, ntbooks, vm_kjva);
                    r->registerVersificationSystem("NRSV", otbooks, ntbooks, vm_nrsv, mappings_nrsv);
                    r->registerVersificationSystem("NRSVA", otbooks_nrsva, ntbooks, vm_nrsva);
                    r->registerVersificationSystem("Synodal", otbooks_synodal, ntbooks_synodal, vm_synodal, mappings_synodal);
                    r->registerVersificationSystem("SynodalProt", otbooks_synodalProt, ntbooks_synodal, vm_synodalProt);
                    r->registerVersificationSystem("Vulg", otbooks_vulg, ntbooks_vulg, vm_vulg, mappings_vulg);
                    r->registerVersificationSystem("German", otbooks_german, ntbooks, vm_german);
                    r->registerVersificationSystem("Luther", otbooks_luther, ntbooks_luther, vm_luther);
                    r->registerVersificationSystem("Catholic", otbooks_catholic, ntbooks, vm_catholic);
                    r->registerVersificationSystem("Catholic2", otbooks_catholic2, ntbooks, vm_catholic2);
                    r->registerVersificationSystem("LXX", otbooks_lxx, ntbooks, vm_lxx);
                    r->registerVersificationSystem("Orthodox", otbooks_orthodox, ntbooks, vm_orthodox);
                    r->registerVersificationSystem("Calvin", otbooks, ntbooks, vm_calvin, mappings_calvin);
                    r->registerVersificationSystem("DarbyFr", otbooks, ntbooks, vm_darbyfr, mappings_darbyfr);
                    r->registerVersificationSystem("Segond", otbooks, ntbooks, vm_segond, mappings_segond);
                    return r;
                }());

    std::shared_ptr<VersificationMgr const> expected;
    if (std::atomic_compare_exchange_strong_explicit(&m_systemVersificationMgr,
                                                     &expected,
                                                     newMgr,
                                                     std::memory_order_release,
                                                     std::memory_order_acquire))
        return newMgr;
    return expected;
}


class VersificationMgr::System::Private {
public:
    /** Array[chapmax] of maximum verses in chapters */
    std::vector<Book> m_books;
    std::map<std::string, int> m_osisLookup;
    /** General mapping rule is that first verse of every chapter corresponds first
        verse of another chapter in default intermediate canon(kjva), so mapping data
        contains expections. Intermediate canon could not contain corresponding data.

        pointers on uchar[7]: 1 value - book id 1-based, ot+nt, 2-4 map to, 5-7 map
        from (chap,verse from, verse to if greater then "verse from")

        TODO what if book name in one v11n differs from cannon
            special section in mapping for book transformation
    */
    using mapping = std::vector<unsigned char const *>;
    std::vector<mapping> m_mappings;
    std::vector<char const *> m_mappingsExtraBooks;

    Private() = default;
    Private(Private const & copy) {
        m_books = copy.m_books;
        m_osisLookup = copy.m_osisLookup;
    }
    Private & operator=(Private const & copy) {
        m_books = copy.m_books;
        m_osisLookup = copy.m_osisLookup;
        return *this;
    }
};


class VersificationMgr::Book::Private {
friend struct BookOffsetLess;
public:
    /** Array[chapmax] of maximum verses in chapters */
    std::vector<int> m_verseMax;
    std::vector<long> m_offsetPrecomputed;

    Private() = default;
    Private(Private const &) = default;
    Private & operator=(Private const &) = default;
};


struct BookOffsetLess {
    bool operator() (const VersificationMgr::Book &o1, const VersificationMgr::Book &o2) const { return o1.m_p->m_offsetPrecomputed[0] < o2.m_p->m_offsetPrecomputed[0]; }
    bool operator() (const long &o1, const VersificationMgr::Book &o2) const { return o1 < o2.m_p->m_offsetPrecomputed[0]; }
    bool operator() (const VersificationMgr::Book &o1, const long &o2) const { return o1.m_p->m_offsetPrecomputed[0] < o2; }
    bool operator() (const long &o1, const long &o2) const { return o1 < o2; }
};


void VersificationMgr::Book::init() {
    m_p = new Private();
}


void VersificationMgr::System::init() {
    m_p = new Private();
    m_BMAX[0] = 0;
    m_BMAX[1] = 0;
    m_ntStartOffset = 0;
}


VersificationMgr::System::System(const System &other) {
    init();
    m_name = other.m_name;
    m_BMAX[0] = other.m_BMAX[0];
    m_BMAX[1] = other.m_BMAX[1];
    (*m_p) = *(other.m_p);
    m_ntStartOffset = other.m_ntStartOffset;
}


VersificationMgr::System &VersificationMgr::System::operator =(const System &other) {
    m_name = other.m_name;
    m_BMAX[0] = other.m_BMAX[0];
    m_BMAX[1] = other.m_BMAX[1];
    (*m_p) = *(other.m_p);
    m_ntStartOffset = other.m_ntStartOffset;
    return *this;
}


VersificationMgr::System::~System() {
    delete m_p;
}


const VersificationMgr::Book *VersificationMgr::System::getBook(int number) const {
    return (number < (signed int)m_p->m_books.size())
            ? &(m_p->m_books[number])
            : nullptr;
}


int VersificationMgr::System::getBookNumberByOSISName(const char *bookName) const {
    auto const it(m_p->m_osisLookup.find(bookName));
    return (it != m_p->m_osisLookup.end()) ? it->second : -1;
}


void VersificationMgr::System::loadFromSBook(const sbook *ot, const sbook *nt, int const *chMax, const unsigned char *mappings) {
    int chap = 0;
    int book = 0;
    long offset = 0;    // module heading
    offset++;            // testament heading
    while (ot->chapmax) {
        m_p->m_books.push_back(Book(ot->name, ot->osis, ot->prefAbbrev, ot->chapmax));
        offset++;        // book heading
        Book &b = m_p->m_books[m_p->m_books.size()-1];
        m_p->m_osisLookup[b.getOSISName()] = m_p->m_books.size();
        for (int i = 0; i < ot->chapmax; i++) {
            b.m_p->m_verseMax.push_back(chMax[chap]);
            offset++;        // chapter heading
            b.m_p->m_offsetPrecomputed.push_back(offset);
            offset += chMax[chap++];
        }
        ot++;
        book++;
    }
    m_BMAX[0] = book;
    book = 0;
    m_ntStartOffset = offset;
    offset++;            // testament heading
    while (nt->chapmax) {
        m_p->m_books.push_back(Book(nt->name, nt->osis, nt->prefAbbrev, nt->chapmax));
        offset++;        // book heading
        Book &b = m_p->m_books[m_p->m_books.size()-1];
        m_p->m_osisLookup[b.getOSISName()] = m_p->m_books.size();
        for (int i = 0; i < nt->chapmax; i++) {
            b.m_p->m_verseMax.push_back(chMax[chap]);
            offset++;        // chapter heading
            b.m_p->m_offsetPrecomputed.push_back(offset);
            offset += chMax[chap++];
        }
        nt++;
        book++;
    }
    m_BMAX[1] = book;

    // TODO: build offset speed array

    // parse mappings
    if (mappings) {
        const unsigned char *m=mappings;
        for (; *m != 0; m += std::strlen((const char*)m)+1) {
            m_p->m_mappingsExtraBooks.push_back((const char*)m);
        }
        m_p->m_mappings.resize(m_p->m_books.size()+m_p->m_mappingsExtraBooks.size());

        for (++m; *m != 0; m += 7) {
            m_p->m_mappings[m[0]-1].push_back(m);
            if (*m > m_p->m_books.size()) {
                m_p->m_mappings[m[7]-1].push_back(m);
                m += 1;
            }
        }
    }
}


VersificationMgr::Book::Book(const Book &other) {
    m_longName = other.m_longName;
    m_osisName = other.m_osisName;
    m_prefAbbrev = other.m_prefAbbrev;
    m_chapMax = other.m_chapMax;
    init();
    (*m_p) = *(other.m_p);
}


VersificationMgr::Book& VersificationMgr::Book::operator =(const Book &other) {
    m_longName = other.m_longName;
    m_osisName = other.m_osisName;
    m_prefAbbrev = other.m_prefAbbrev;
    m_chapMax = other.m_chapMax;
    init();
    (*m_p) = *(other.m_p);
    return *this;
}


VersificationMgr::Book::~Book() {
    delete m_p;
}


int VersificationMgr::Book::getVerseMax(int chapter) const {
    chapter--;
    return (m_p && (chapter < (signed int)m_p->m_verseMax.size()) && (chapter > -1)) ? m_p->m_verseMax[chapter] : -1;
}


int VersificationMgr::System::getBookCount() const {
    return (m_p ? m_p->m_books.size() : 0);
}


long VersificationMgr::System::getOffsetFromVerse(int book, int chapter, int verse) const {
    long  offset = -1;
    chapter--;

    const Book *b = getBook(book);

    if (!b)                                        return -1;    // assert we have a valid book
    if ((chapter > -1) && (chapter >= (signed int)b->m_p->m_offsetPrecomputed.size())) return -1;    // assert we have a valid chapter

    offset = b->m_p->m_offsetPrecomputed[(chapter > -1)?chapter:0];
    if (chapter < 0) offset--;

/* old code
 *
    offset = offsets[testament-1][0][book];
    offset = offsets[testament-1][1][(int)offset + chapter];
    if (!(offset|verse)) // if we have a testament but nothing else.
        offset = 1;

*/

    return (offset + verse);
}


char VersificationMgr::System::getVerseFromOffset(long offset, int *book, int *chapter, int *verse) const {

    if (offset < 1) {    // just handle the module heading corner case up front (and error case)
        (*book) = -1;
        (*chapter) = 0;
        (*verse) = 0;
        return offset;    // < 0 = error
    }

    // binary search for book
    auto b = lower_bound(m_p->m_books.cbegin(), m_p->m_books.cend(), offset, BookOffsetLess());
    if (b == m_p->m_books.end()) b--;
    (*book)    = distance(m_p->m_books.cbegin(), b)+1;
    if (offset < (b->m_p->m_offsetPrecomputed.front())-((((!(*book)) || (*book)==m_BMAX[0]+1))?2:1)) { // -1 for chapter headings
        (*book)--;
        if (b != m_p->m_books.cbegin()) {
            b--;
        }
    }
    auto c = lower_bound(b->m_p->m_offsetPrecomputed.cbegin(), b->m_p->m_offsetPrecomputed.cend(), offset);

    // if we're a book heading, we are lessthan chapter precomputes, but greater book.  This catches corner case.
    if (c == b->m_p->m_offsetPrecomputed.end()) {
        c--;
    }
    if ((offset < *c) && (c == b->m_p->m_offsetPrecomputed.cbegin())) {
        (*chapter) = (offset - *c)+1;    // should be 0 or -1 (for testament heading)
        (*verse) = 0;
    }
    else {
        if (offset < *c) c--;
        (*chapter) = distance(b->m_p->m_offsetPrecomputed.cbegin(), c)+1;
        (*verse)   = (offset - *c);
    }
    return ((*chapter > 0) && (*verse > b->getVerseMax(*chapter))) ? KEYERR_OUTOFBOUNDS : 0;
}


/***************************************************
 * VersificationMgr
 */

class VersificationMgr::Private {
public:
    Private() = default;
    Private(Private const &) = default;
    Private & operator=(Private const &) = default;
    std::map<std::string, System> m_systems;
};
// ---------------- statics -----------------
std::shared_ptr<VersificationMgr const>
        VersificationMgr::m_systemVersificationMgr;

void VersificationMgr::init() {
    p = new Private();
}


VersificationMgr::~VersificationMgr() {
    delete p;
}


void VersificationMgr::setSystemVersificationMgr(
        std::shared_ptr<VersificationMgr const> newVersificationMgr)
{
    std::atomic_store_explicit(&m_systemVersificationMgr,
                               std::move(newVersificationMgr),
                               std::memory_order_release);
}


const VersificationMgr::System *VersificationMgr::getVersificationSystem(const char *name) const {
    auto const it(p->m_systems.find(name));
    return (it != p->m_systems.end()) ? &(it->second) : nullptr;
}


void VersificationMgr::registerVersificationSystem(const char *name, const sbook *ot, const sbook *nt, int const *chMax, const unsigned char *mappings) {
    p->m_systems[name] = name;
    System &s = p->m_systems[name];
    s.loadFromSBook(ot, nt, chMax, mappings);
}


const std::list<std::string> VersificationMgr::getVersificationSystems() const {
    std::list<std::string> retVal;
    for (auto it = p->m_systems.cbegin(); it != p->m_systems.cend(); ++it)
        retVal.push_back(it->first);
    return retVal;
}

void VersificationMgr::System::translateVerse(const System *dstSys, const char **book, int *chapter, int *verse, int *verse_end) const {
    auto const & myName(getName());
    auto const & dstName(dstSys->getName());

    SWLog::getSystemLog()->logDebug("translate verse from %s to %s: %s.%i.%i-%i\n",myName.c_str(), dstName.c_str(), *book, *chapter, *verse, *verse_end);

    if (myName == "KJVA" || myName == "KJV") {
        if (dstName == "KJVA" || dstName == "KJV")
            return;
        // reversed mapping
        SWLog::getSystemLog()->logDebug("Perform reversed mapping.\n");
        int b = dstSys->getBookNumberByOSISName(*book)-1;

        SWLog::getSystemLog()->logDebug("\tgetBookNumberByOSISName %i %s.\n", b, *book);

        if (b < 0) {
            SWLog::getSystemLog()->logDebug("\tmappingsExtraBooks.size() %i.\n", dstSys->m_p->m_mappingsExtraBooks.size());
            for (int i=0; i<(int)dstSys->m_p->m_mappingsExtraBooks.size(); ++i) {
                SWLog::getSystemLog()->logDebug("\t%s %s.\n", *book, dstSys->m_p->m_mappingsExtraBooks[i]);
                if (!std::strcmp(*book, dstSys->m_p->m_mappingsExtraBooks[i])) {
                    b = m_p->m_books.size()+i-2;
                    break;
                }
            }
        }

        SWLog::getSystemLog()->logDebug("\tb %i.\n", b);

        if (b >= (int)dstSys->m_p->m_mappings.size() || b < 0) {
            SWLog::getSystemLog()->logDebug("no modification");
            return;
        }

        unsigned char const * a = nullptr;

        // reversed mapping should use forward search for item
        for (unsigned int i=0; i<dstSys->m_p->m_mappings[b].size(); ++i) {
            const unsigned char *m = dstSys->m_p->m_mappings[b][i];
            if (m[4] == *chapter && m[5] <= *verse) {
                SWLog::getSystemLog()->logDebug("found mapping %i %i %i %i %i %i\n",m[1],m[2],m[3],m[4],m[5],m[6]);
                if (m[5] == *verse || (m[6] >= *verse && m[5] <= *verse)) {
                    // inside of any mapping range
                    *chapter = m[1];
                    *verse = m[2];
                    *verse_end = m[3];
                    if (*m >= dstSys->m_p->m_books.size()) {
                        SWLog::getSystemLog()->logWarning("map to extra books, possible bug source\n");
                        *book = dstSys->getBook(m[7]-1)->getOSISName().c_str();
                    }
                    return;
                }
                // destination mapping can have duplicate items, use the last (by using <=)
                if (!a || (a[5]>a[6]?a[5]:a[6]) <= (m[5]>m[6]?m[5]:m[6]))
                    a = m;
            }
        }
        if (!a) {
            SWLog::getSystemLog()->logDebug("set appropriate: %i %i %i %i %i %i\n",a[1],a[2],a[3],a[4],a[5],a[6]);
            (*chapter) = a[1];
            // shift verse
            const int d = (a[3]>a[2]?a[3]:a[2])-(a[6]>a[5]?a[6]:a[5]);
            if (*verse < *verse_end)
                *verse_end += d;
            else
                *verse_end = (*verse) + d;
            *verse += d;
            if (*a > dstSys->m_p->m_books.size()) {
                SWLog::getSystemLog()->logDebug("appropriate: %i %i %i %i %i %i %i %i\n",a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7]);
                SWLog::getSystemLog()->logDebug("book: %s\n", dstSys->getBook(a[7]-1)->getOSISName().c_str());
                *book = dstSys->getBook(a[7]-1)->getOSISName().c_str();
            }
            return;
        }
        SWLog::getSystemLog()->logDebug("There is no mapping.\n");
    }
    else if (dstName != "KJVA" && dstName != "KJV") {
        const System *kjva = systemVersificationMgr()->getVersificationSystem("KJVA");
        const int src_verse = *verse;

        translateVerse(kjva, book, chapter, verse, verse_end);

        int interm_verse = *verse, interm_range = *verse_end, interm_chapter = *chapter;
        const char *interm_book = *book;

        kjva->translateVerse(dstSys, book, chapter, verse, verse_end);

        // contraction->expansion fix
        if (verse < verse_end && !(interm_verse < interm_range)) {
            kjva->translateVerse(this, &interm_book, &interm_chapter, &interm_verse, &interm_range);
            if (interm_verse < interm_range) {
                *verse += src_verse - interm_verse;
                if (*verse > *verse_end)
                    *verse = *verse_end;
                else
                    *verse_end = *verse;
            }
        }
    }
    else {
        SWLog::getSystemLog()->logDebug("Perform forward mapping.\n");
        const int b = getBookNumberByOSISName(*book)-1;
        if (b >= (int)m_p->m_mappings.size())
            return;
        // forward mapping should use reversed search for item
        for (int i=m_p->m_mappings[b].size()-1; i>=0; --i) {
            const unsigned char *m = m_p->m_mappings[b][i];
            if (m[1] < *chapter) {
                SWLog::getSystemLog()->logWarning("There is no mapping for this chapter.\n");
                return;
            }
            if (m[1] == *chapter && m[2] <= *verse) {
                SWLog::getSystemLog()->logDebug("found mapping %i %i %i %i %i %i\n",m[1],m[2],m[3],m[4],m[5],m[6]);
                if (m[2] == *verse || (m[3] >= *verse && m[2] <= *verse)) {
                    *chapter = m[4];
                    *verse = m[5];
                    *verse_end = m[6];
                }
                else {
                    *chapter = m[4];
                    // shift verse
                    const int d = (m[6]>m[5]?m[6]:m[5])-(m[3]>m[2]?m[3]:m[2]);
                    if (*verse < *verse_end)
                        *verse_end += d;
                    else
                        *verse_end = (*verse) + d;
                    *verse += d;
                }
                if (*m > m_p->m_books.size())
                    *book = m_p->m_mappingsExtraBooks[m[0]-m_p->m_books.size()-1];
                return;
            }
        }
        SWLog::getSystemLog()->logDebug("No mapping.\n");
    }
}

} /* namespace swordxx */

