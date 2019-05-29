/******************************************************************************
 *
 *  versekey.cpp -    code for class 'VerseKey'- a standard Biblical
 *            verse key
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

#include "versekey.h"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <limits>
#include <memory>
#include <sstream>
#include <type_traits>
#include "../roman.h"
#include "../stringmgr.h"
#include "../swlocale.h"
#include "../swlog.h"
#include "../utilstr.h"
#include "../versificationmgr.h"


namespace swordxx {

/******************************************************************************
 * VerseKey Constructor - initializes instance of VerseKey
 *
 * ENT:    ikey - base key (will take various forms of 'BOOK CH:VS'.  See
 *        VerseKey::parse for more detailed information)
 */

VerseKey::VerseKey(const SWKey &ikey) : SWKey(ikey)
{
    setVersificationSystem("KJV");
    copyFrom(ikey);
}

/******************************************************************************
 * VerseKey Constructor - initializes instance of VerseKey
 *
 * ENT:    ikey - text key (will take various forms of 'BOOK CH:VS'.  See
 *        VerseKey::parse for more detailed information)
 */

VerseKey::VerseKey(const char *ikeyText) : SWKey(ikeyText)
{
    setVersificationSystem("KJV");
    if (ikeyText)
        parse();
}


VerseKey::VerseKey(VerseKey const &k) : SWKey(k)
{
    copyFrom(k);
}


/******************************************************************************
 * VerseKey::setFromOther - Positions this VerseKey to another VerseKey
 */

void VerseKey::setFromOther(const VerseKey &ikey) {
    if (m_refSys == ikey.m_refSys) {
        m_testament = ikey.getTestament();
        m_book = ikey.getBook();
        m_chapter = ikey.getChapter();
        m_verse = ikey.getVerse();
        m_suffix = ikey.getSuffix();
    }
    else {
        // map verse between systems
        const char* map_book = ikey.getOSISBookName().c_str();
        int map_chapter = ikey.getChapter();
        int map_verse = ikey.getVerse();
        int map_range = map_verse;

        ikey.m_refSys->translateVerse(m_refSys, &map_book, &map_chapter, &map_verse, &map_range);
        //printf("verse: %s.%i.%i-%i\n",map_book,map_chapter,map_verse,map_range);

        m_book = m_refSys->getBookNumberByOSISName(map_book);

        // check existence
        if (m_book == -1) {
            m_book = 1;
            m_error = KEYERR_OUTOFBOUNDS;
        }
        else if (m_refSys->getBook(m_book-1)->getChapterMax() < map_chapter) {
            map_chapter = m_refSys->getBook(m_book-1)->getChapterMax();
            map_verse = m_refSys->getBook(m_book-1)->getVerseMax(map_chapter);
            m_error = KEYERR_OUTOFBOUNDS;
        }
        else if (map_chapter > 0 && m_refSys->getBook(m_book-1)->getVerseMax(map_chapter) < map_verse) {
            map_verse = m_refSys->getBook(m_book-1)->getVerseMax(map_chapter);
            m_error = KEYERR_OUTOFBOUNDS;
        }

        // set values
        if (m_book > m_BMAX[0]) {
            m_book -= m_BMAX[0];
            m_testament = 2;
        } else {
            m_testament = 1;
        }

        //if (map_verse == 0) Headings(1);

        m_chapter = map_chapter;
        m_verse = map_verse;
        m_suffix = ikey.getSuffix();

        if (map_verse < map_range) {
            if (map_range > m_refSys->getBook(((m_testament>1)?m_BMAX[0]:0)+m_book-1)->getVerseMax(m_chapter))
                ++map_range;
            m_verse = map_range;
            setUpperBound(*this);
            m_verse = map_verse;
            setLowerBound(*this);
        }
    }
}


void VerseKey::positionFrom(const SWKey &ikey) {
     m_error = 0;
        const SWKey *fromKey = &ikey;
    ListKey const * tryList = dynamic_cast<ListKey const *>(fromKey);
    if (tryList) {
        SWKey const *k = tryList->getElement();
        if (k) fromKey = k;
    }
    VerseKey const * tryVerse = dynamic_cast<VerseKey const *>(fromKey);
    if (tryVerse) {
        setFromOther(*tryVerse);
    }
    else {
        SWKey::positionFrom(*fromKey);
// extraneous parse which inadvertently clears error flag
// SWKey::positionFrom already calls copyFrom which calls setText, which VerseKey::setText already calls parse()
//        parse();
    }

     // should we always perform bounds checks?  Tried but seems to cause infinite recursion
    if (compare_(getUpperBound()) > 0) {
        setFromOther(getUpperBound());
        m_error = KEYERR_OUTOFBOUNDS;
    }
    if (compare_(getLowerBound()) < 0) {
        setFromOther(getLowerBound());
        m_error = KEYERR_OUTOFBOUNDS;
    }
}


/******************************************************************************
 * VerseKey::copyFrom - Equates this VerseKey to another VerseKey
 */

void VerseKey::copyFrom(const VerseKey &ikey) {
    m_autonorm = ikey.m_autonorm;
    m_intros = ikey.m_intros;
    m_testament = ikey.getTestament();
    m_book = ikey.getBook();
    m_chapter = ikey.getChapter();
    m_verse = ikey.getVerse();
    m_suffix = ikey.getSuffix();
    setLocale(ikey.getLocale());
    setVersificationSystem(ikey.getVersificationSystem().c_str());
    if (ikey.isBoundSet()) {
        setLowerBound(ikey.getLowerBound());
        setUpperBound(ikey.getUpperBound());
    }
}


/******************************************************************************
 * VerseKey::copyFrom - Equates this VerseKey to another SWKey
 */

void VerseKey::copyFrom(const SWKey &ikey) {
    // check to see if we can do a more specific copy
    // plus some optimizations
    const SWKey *fromKey = &ikey;
    ListKey const * tryList = dynamic_cast<ListKey const *>(fromKey);
    if (tryList) {
        if (SWKey const * k = tryList->getElement())
            fromKey = k;
    }
    VerseKey const * tryVerse = dynamic_cast<VerseKey const *>(fromKey);
    if (tryVerse) {
        copyFrom(*tryVerse);
    }
    else {
        SWKey::copyFrom(*fromKey);
// extraneous parse which inadvertently clears error flag
// SWKey::copyFrom already calls setText, which VerseKey::setText already calls parse()
//        parse();
    }
}


VerseKey::VerseKey(const char *min, const char *max, const char *v11n) : SWKey()
{
    setVersificationSystem(v11n);
    ListKey tmpListKey = parseVerseList(min);
    if (tmpListKey.getCount())
        setLowerBound(*static_cast<VerseKey *>(tmpListKey.getElement(0u)));
    tmpListKey = parseVerseList(max, min, true);
    if (tmpListKey.getCount()) {
        VerseKey * const newElement =
                static_cast<VerseKey *>(tmpListKey.getElement(0u));
        setUpperBound((newElement->isBoundSet())?newElement->getUpperBound():*newElement);
    }
    positionToTop();
}


std::unique_ptr<SWKey> VerseKey::clone() const
{ return std::make_unique<VerseKey>(*this); }


/******************************************************************************
 * VerseKey Destructor - cleans up instance of VerseKey
 *
 * ENT:    ikey - text key
 */

VerseKey::~VerseKey() {}


void VerseKey::setVersificationSystem(const char *name) {
    const VersificationMgr::System *newRefSys = VersificationMgr::systemVersificationMgr()->getVersificationSystem(name);
    // TODO: cheese, but what should we do if requested v11n system isn't found?
    if (!newRefSys)   newRefSys = VersificationMgr::systemVersificationMgr()->getVersificationSystem("KJV");
    if (m_refSys != newRefSys) {
        m_refSys = newRefSys;
        m_BMAX[0] = m_refSys->getBMAX()[0];
        m_BMAX[1] = m_refSys->getBMAX()[1];

        // TODO: adjust bounds for versificaion system ???
        // TODO: when we have mapping done, rethink this
        //necessary as our bounds might not mean anything in the new v11n system
        clearBounds();
    }

}


std::string const & VerseKey::getVersificationSystem() const
{ return m_refSys->getName(); }



/******************************************************************************
 * VerseKey::parse - parses keytext into testament|book|chapter|verse
 *
 * RET:    error status
 */

char VerseKey::parse(bool checkAutoNormalize)
{
    m_testament = m_BMAX[1]?2:1;
    m_book      = m_BMAX[m_BMAX[1]?1:0];
    m_chapter   = 1;
    m_verse     = 1;

    int error = 0;
    auto const keytext = SWKey::getText();
    if (!keytext.empty()) {
        // pass our own copy of keytext as keytext memory may be freshed during parse
        ListKey tmpListKey = parseVerseList(keytext.c_str());
        if (tmpListKey.getCount()) {
            this->positionFrom(*tmpListKey.getElement(0u));
            error = this->m_error;
        } else error = 1;
    }
    if (checkAutoNormalize) {
        normalize(true);
    }
    freshtext();

    return (this->m_error) ? this->m_error : (this->m_error = error);
}


/******************************************************************************
 * VerseKey::freshtext - refreshes keytext based on
 *                testament|book|chapter|verse
 */

void VerseKey::freshtext() const
{
    char buf[2024];
    int realTest = m_testament;
    int realbook = m_book;

    if (m_book < 1) {
        if (m_testament < 1)
            sprintf(buf, "[ Module Heading ]");
        else sprintf(buf, "[ Testament %d Heading ]", (int)m_testament);
    }
    else {
        if (realbook > m_BMAX[realTest-1]) {
            realbook -= m_BMAX[realTest-1];
            if (realTest < 2)
                realTest++;
            if (realbook > m_BMAX[realTest-1])
                realbook = m_BMAX[realTest-1];
        }
        sprintf(buf, "%s %d:%d", getBookName().c_str(), m_chapter, m_verse);
        if (m_suffix) {
            buf[std::strlen(buf)+1] = 0;
            buf[std::strlen(buf)] = m_suffix;
        }
    }

    /// \todo Fix this ugly hack:
    const_cast<VerseKey *>(this)->SWKey::setText(buf);
}



/************************************************************************
 * VerseKey::getBookAbbrev - Attempts to find a book no from a name or
 *                           abbreviation
 *
 * ENT:    abbr - key for which to search;
 * RET:    book number or < 0 = not valid
 */

int VerseKey::getBookFromAbbrev(std::string_view iabbr) const {
    iabbr = trimmedView(iabbr);
    if (iabbr.empty())
        return -1;

    StringMgr* stringMgr = StringMgr::getSystemStringMgr();

    /* The first iteration of the following loop tries to uppercase the string.
       If we still fail to match, we try matching the string without any toupper
       logic. This is useful for, say, Chinese user input on a system that
       doesn't properly support a true Unicode-toupper function. */
    std::string upperAbbr(iabbr);
    stringMgr->upperUTF8(upperAbbr);
    auto const scanAbbreviations =
        [this](ConfigEntMap const & abbrevs, std::string_view abbr) {
            assert(!abbr.empty());
            for (auto it = abbrevs.lower_bound(abbr); it != abbrevs.end(); ++it)
            {
                auto const & foundAbbr = it->first;
                if (foundAbbr.size() < abbr.size())
                    break;
                if (std::memcmp(abbr.data(), foundAbbr.c_str(), abbr.size()))
                    break;
                auto const & osis = it->second;
                auto const retVal =
                        m_refSys->getBookNumberByOSISName(osis.c_str());
                if (retVal >= 0)
                    return retVal;
            }
            return -1;
        };
    std::string_view abbr(upperAbbr);
    for (int i = 0; i < 2; i++, abbr = iabbr) {
        int r = scanAbbreviations(getPrivateLocale().bookAbbreviations(), abbr);
        if (r >= 0)
            return r;
        r = scanAbbreviations(SWLocale::builtinBookAbbreviations(), abbr);
        if (r >= 0)
            return r;
    }
    return -1;
}


/******************************************************************************
 * VerseKey::validateCurrentLocale - be sure a locale book abbrevs set is complete
 *
 */
void VerseKey::validateCurrentLocale() const {
    if (SWLog::getSystemLog()->getLogLevel() >= SWLog::LOG_DEBUG) { //make sure log is wanted, this loop stuff costs a lot of time
        for (int i = 0; i < m_refSys->getBookCount(); i++) {
            std::string abbr(getPrivateLocale().translate(m_refSys->getBook(i)->getLongName()));
            trimString(abbr);
            const int bn = getBookFromAbbrev(abbr);
            if (bn != i+1) {
                SWLog::getSystemLog()->logDebug("VerseKey::Book: %s does not have a matching toupper abbrevs entry! book number returned was: %d, should be %d. Required entry to add to locale:", abbr.c_str(), bn, i);

                StringMgr* stringMgr = StringMgr::getSystemStringMgr();
                stringMgr->upperUTF8(abbr);
                SWLog::getSystemLog()->logDebug("%s=%s\n", abbr.c_str(), m_refSys->getBook(i)->getOSISName().c_str());
            }
        }
    }
}


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

ListKey VerseKey::parseVerseList(const char *buf, const char *defaultKey, bool expandRange, bool useChapterAsVerse) {

    static_assert(std::is_unsigned<decltype(VerseKey::m_userData)>::value, "");
    static_assert(std::numeric_limits<decltype(VerseKey::m_userData)>::max()
                  >= std::numeric_limits<std::size_t>::max(), "");

    // assert we have a buffer
    if (!buf)
        return ListKey();

    // hold on to our own copy of params, as threads/recursion may change outside values
    std::string const iBuf(buf);
    buf = iBuf.c_str();
    auto const bufStart = buf;

    std::string book;
    book.reserve(100u);
    std::string number;
    number.reserve(10u);
    char suffix = 0;
    int chap = -1, verse = -1;
    int bookno = 0;
    bool comma = false;
    bool dash = false;
    ListKey tmpListKey;
    char lastPartial = 0;
    bool inTerm = true;
    int notAllDigits = 0;
    bool doubleF = false;

    std::unique_ptr<VerseKey> const curKey(
                static_cast<VerseKey *>(clone().release()));
    std::unique_ptr<VerseKey> const lastKey(
                static_cast<VerseKey *>(clone().release()));
    lastKey->clearBounds();
    curKey->clearBounds();

    // some silly checks for corner cases
    if (!std::strcmp(buf, "[ Module Heading ]")) {
        curKey->setVerse(0);
        curKey->setChapter(0);
        curKey->setBook(0);
        curKey->setTestament(0);
        lastKey->setLowerBound(*curKey);
        lastKey->setUpperBound(*curKey);
        ListKey internalListKey;
        internalListKey << *lastKey;
        return internalListKey;
    }
    if ((!std::strncmp(buf, "[ Testament ", 12)) &&
        (charIsDigit(buf[12])) &&
        (!std::strcmp(buf+13, " Heading ]"))) {
        curKey->setVerse(0);
        curKey->setChapter(0);
        curKey->setBook(0);
        curKey->setTestament(buf[12]-48);
        lastKey->setLowerBound(*curKey);
        lastKey->setUpperBound(*curKey);
        ListKey internalListKey;
        internalListKey << *lastKey;
        return internalListKey;
    }

    curKey->setAutoNormalize(isAutoNormalize());
    lastKey->setAutoNormalize(false);
    if (defaultKey) lastKey->setText(defaultKey);

    while (*buf) {
        switch (*buf) {
        case ':':
            if (buf[1] != ' ') {        // for silly "Mat 1:1: this verse...."
                if (!number.empty()) {
                    if (chap >= 0)
                        verse = std::atoi(number.c_str());
                    else    chap  = std::atoi(number.c_str());
                    number.clear();
                }
                comma = false;
                break;
            }
            goto terminate_range;
        case ' ':
            inTerm = true;
            while (true) {
                if (number.empty() || (chap < 0))
                    break;
                decltype(buf-buf) q = 1;
                while ((buf[q]) && (buf[q] != ' '))
                    ++q;
                if (buf[q] == ':')
                    break;
                inTerm = false;
                break;
            }
            if (inTerm) {
                if (book.empty() || (*book.rbegin()) != ' ') {
                    book.push_back(' ');
                }
                break;
            }

        case '-':
            if (chap == -1) {
                book.push_back(*buf);
                book.push_back(*(buf + 1u));
                if (getBookFromAbbrev(book) > -1) {
                    book.pop_back();
                    buf++;
                    break;
                } else {
                    book.pop_back();
                    book.pop_back();
                }
            }
        case ',': // on number new verse
        case ';': // on number new chapter
terminate_range:
            if (!number.empty()) {
                if (chap >= 0)
                    verse = std::atoi(number.c_str());
                else    chap = std::atoi(number.c_str());
                number.clear();
            }
            bookno = -1;
            if (!book.empty()) {
                auto loop = book.size() - 1u;

                for (; loop+1; loop--) { if (book[loop] == ' ') book.resize(loop); else break; }

                if (loop > 0 && charIsDigit(book[loop-1]) && charIsLower(book[loop])) {
                    book.resize(loop--);
                }
                for (; loop+1; loop--) {
                    if ((charIsDigit(book[loop])) || (book[loop] == ' ')) {
                        book.resize(loop);
                        continue;
                    }
                    else {
                        if ((asciiCharToUpper(book[loop])=='F')&&(loop)) {
                            if ((charIsDigit(book[loop-1])) || (book[loop-1] == ' ') || (asciiCharToUpper(book[loop-1]) == 'F')) {
                                book.resize(loop);
                                continue;
                            }
                        }
                    }
                    break;
                }

                for (loop = book.size() - 1u; loop+1; loop--) {
                    if (book[loop] == ' ') {
                        // "PS C" is ok, but "II C" is not ok
                        if (isroman(book.c_str() + loop + 1u)
                            && !isroman(book.c_str(), loop))
                        {
                            if (verse == -1) {
                                verse = chap;
                                chap = from_rom(book.c_str() + loop + 1u);
                                book.resize(loop);
                            }
                        }
                            break;
                    }
                }

                // check for special inscriptio and subscriptio which are saved as book intro and chap 1 intro (for INTF)
                for (loop = book.size() - 1u; loop+1; loop--) {
                    if (book[loop] == ' ') {
                        if (!strnicmp(book.c_str() + loop + 1u, "inscriptio", book.size() - loop - 1u)) {
                            book.resize(loop);
                            verse = 0;
                            chap = 0;
                        }
                        else if (!strnicmp(book.c_str() + loop + 1u, "subscriptio", book.size() - loop - 1u)) {
                            book.resize(loop);
                            verse = 0;
                            chap = 1;
                        }
                            break;
                    }
                }

                if (caseInsensitiveEquals(book, "V") || caseInsensitiveEquals(book, "VER")) {    // Verse abbrev
                    if (verse == -1) {
                        verse = chap;
                        chap = lastKey->getChapter();
                        book.clear();
                    }
                }
                if (caseInsensitiveEquals(book, "ch") || caseInsensitiveEquals(book, "chap")) {    // Verse abbrev
                    book = lastKey->getBookName();
                }
                bookno = getBookFromAbbrev(book);
                if ((bookno > -1) && (suffix == 'f') && (book[book.size() - 1u] == 'f')) {
                    suffix = 0;
                }
            }
            if (((bookno > -1) || book.empty()) && (!book.empty() || (chap >= 0) || (verse >= 0))) {
                char partial = 0;
                curKey->setVerse(1);
                curKey->setChapter(1);
                curKey->setBook(1);

                if (bookno < 0) {
                    curKey->setTestament(lastKey->getTestament());
                    curKey->setBook(lastKey->getBook());
                }
                else {
                    char t = 1;
                    if (bookno > m_BMAX[0]) {
                        t++;
                        bookno -= m_BMAX[0];
                    }
                    curKey->setTestament(t);
                    curKey->setBook(bookno);
                }


                if (((comma)||((verse < 0)&&(bookno < 0)))&&(!lastPartial)) {
//                if (comma) {
                    curKey->setChapter(lastKey->getChapter());
                    curKey->setVerse(chap);  // chap because this is the first number captured
                    if (suffix) {
                        curKey->setSuffix(suffix);
                    }
                }
                else {
                    if (useChapterAsVerse && verse < 0 && chap > 0 && curKey->getChapterMax() == 1) {
                        verse = chap;
                        chap = 1;
                    }


                    if (chap >= 0) {
                        curKey->setChapter(chap);
                    }
                    else {
                        partial++;
                        curKey->setChapter(1);
                    }
                    if (verse >= 0) {
                        curKey->setVerse(verse);
                        if (suffix) {
                            curKey->setSuffix(suffix);
                        }
                    }
                    else {
                        partial++;
                        curKey->setVerse(1);
                    }
                }

                // check for '-'
                decltype(buf-buf) q = 0;
                while ((buf[q]) && (buf[q] == ' '))
                    ++q;
                if ((buf[q] == '-') && (expandRange)) {    // if this is a dash save lowerBound and wait for upper
                    buf+=q;
                    lastKey->setLowerBound(*curKey);
                    lastKey->positionToTop();
                    tmpListKey << *lastKey;
                    ((VerseKey *)tmpListKey.getElement())->setAutoNormalize(isAutoNormalize());
                    tmpListKey.getElement()->m_userData =
                            static_cast<std::size_t>(buf - bufStart);
                }
                else {
                    if (!dash) {     // if last separator was not a dash just add
                        if (expandRange && partial) {
                            lastKey->setLowerBound(*curKey);
                            if (partial > 1)
                                curKey->positionToMaxChapter();
                            if (partial > 0)
                                curKey->positionToMaxVerse();
                            lastKey->setUpperBound(*curKey);
                            lastKey->positionToTop();
                            tmpListKey << *lastKey;
                            ((VerseKey *)tmpListKey.getElement())->setAutoNormalize(isAutoNormalize());
                            tmpListKey.getElement()->m_userData =
                                    static_cast<std::size_t>(buf - bufStart);
                        }
                        else {
                            bool f = false;
                            if (curKey->getSuffix() == 'f') {
                                curKey->setSuffix(0);
                                f = true;
                            }
                            lastKey->setLowerBound(*curKey);
                            if (f && doubleF) curKey->positionToMaxVerse();
                            else if (f) curKey->increment();
                            lastKey->setUpperBound(*curKey);
                            lastKey->positionToTop();
                            tmpListKey << *lastKey;
                            ((VerseKey *)tmpListKey.getElement())->setAutoNormalize(isAutoNormalize());
                            tmpListKey.getElement()->m_userData =
                                    static_cast<std::size_t>(buf - bufStart);
                        }
                    }
                    else    if (expandRange) {
                        VerseKey * const newElement =
                                dynamic_cast<VerseKey *>(
                                    tmpListKey.getElement());
                        if (newElement) {
                            if (partial > 1)
                                curKey->positionToMaxChapter();
                            if (partial > 0)
                                curKey->positionToMaxVerse();
                            newElement->setUpperBound(*curKey);
                            *lastKey = *curKey;
                            newElement->positionToTop();
                            tmpListKey.getElement()->m_userData =
                                    static_cast<std::size_t>(buf - bufStart);
                        }
                    }
                }
                lastPartial = partial;
            }
            book.clear();
            chap = -1;
            verse = -1;
            suffix = 0;
            if (*buf == ',')
                comma = true;
            else    comma = false;
            if (*buf == '-')
                dash = true;
            else    dash = false;
            break;
        case 10:    // ignore these
        case 13:
        case '[':
        case ']':
        case '(':
        case ')':
        case '{':
        case '}':
            break;
        case '.':
            if (buf > bufStart) {            // ignore (break) if preceeding char is not a digit
                for (notAllDigits = book.size(); notAllDigits; notAllDigits--) {
                    if ((!charIsDigit(book[notAllDigits-1])) && (!std::strchr(" .", book[notAllDigits-1])))
                        break;
                }
                if (!notAllDigits && !charIsDigit(buf[1]))
                    break;
            }

            if (!number.empty()) {
                if (chap >= 0)
                    verse = std::atoi(number.c_str());
                else    chap  = std::atoi(number.c_str());
                number.clear();
            }
            else if (chap == -1 && (book.empty() || (*book.rbegin()) != ' ')) {
                book.push_back(' ');
            }

            break;

        default:
            if (charIsDigit(*buf)) {
                number.push_back(*buf);
                suffix = 0;
                doubleF = 0;
            }
            else {
                switch (*buf) {
                case ' ':    // ignore these and don't reset number
                case 'F':
                    break;
                default:
                    // suffixes (and oddly 'f'-- ff.)
                    if ((charIsLower(*buf) && (chap >=0 || bookno > -1 || lastKey->isBoundSet()))
                            || *buf == 'f') {
                        // if suffix is already an 'f', then we need to mark if we're doubleF.
                        doubleF = (*buf == 'f' && suffix == 'f');
                        if (suffix && !doubleF)
                            number.clear();
                        suffix = *buf;
                    } else {
                        number.clear();
                    }
                    break;
                }
            }
            if (chap == -1)
                book.push_back(*buf);
        }
        buf++;
    }
    if (!number.empty()) {
        if (chap >= 0)
            verse = std::atoi(number.c_str());
        else    chap  = std::atoi(number.c_str());
        number.clear();
    }
    if (!book.empty()) {
        auto loop = book.size() - 1u;

        // strip trailing spaces
        for (; loop+1; loop--) { if (book[loop] == ' ') book.resize(loop); else break; }

        // check if endsWith([0-9][a-z]) and kill the last letter, e.g., ...12a, and chop off the 'a'
        // why?  What's this for? wouldn't this mess up 2t?
        if (loop > 0 && charIsDigit(book[loop-1]) && charIsLower(book[loop])) {
            book.resize(loop--);
        }

        // skip trailing spaces and numbers
        for (; loop+1; loop--) {
            if ((charIsDigit(book[loop])) || (book[loop] == ' ')) {
                book.resize(loop);
                continue;
            }
            else {
                if ((asciiCharToUpper(book[loop])=='F')&&(loop)) {
                    if ((charIsDigit(book[loop-1])) || (book[loop-1] == ' ') || (asciiCharToUpper(book[loop-1]) == 'F')) {
                        book.resize(loop);
                        continue;
                    }
                }
            }
            break;
        }

        // check for roman numeral chapter
        for (loop = book.size() - 1u; loop+1; loop--) {
            if (book[loop] == ' ') {
                // "PS C" is ok, but "II C" is not ok
                if (isroman(book.c_str() + loop + 1u) && !isroman(book.c_str(), loop)) {
                    if (verse == -1) {
                        verse = chap;
                        chap = from_rom(book.c_str() + loop + 1u);
                        book.resize(loop);
                    }
                }
                break;
            }
        }
        // check for special inscriptio and subscriptio which are saved as book intro and chap 1 intro (for INTF)
        for (loop = book.size() - 1u; loop+1; loop--) {
            if (book[loop] == ' ') {
                if (!strnicmp(book.c_str() + loop + 1u, "inscriptio", book.size() - loop - 1u)) {
                    book.resize(loop);
                    verse = 0;
                    chap = 0;
                    suffix = 0;
                }
                else if (!strnicmp(book.c_str() + loop + 1u, "subscriptio", book.size() - loop - 1u)) {
                    book.resize(loop);
                    verse = 0;
                    chap = 1;
                        suffix = 0;
                }
                break;
            }
        }

        if (caseInsensitiveEquals(book, "V") || caseInsensitiveEquals(book, "VER")) {    // Verse abbrev.
            if (verse == -1) {
                verse = chap;
                chap = lastKey->getChapter();
                book.clear();
            }
        }

        if (caseInsensitiveEquals(book, "ch") || caseInsensitiveEquals(book, "chap")) {    // Verse abbrev
            book = lastKey->getBookName();
        }
        bookno = getBookFromAbbrev(book);
        if ((bookno > -1) && (suffix == 'f') && (book[book.size()-1] == 'f')) {
            suffix = 0;
        }
    }
    if (((bookno > -1) || book.empty()) && (!book.empty() || (chap >= 0) || (verse >= 0))) {
        char partial = 0;
        curKey->setVerse(1);
        curKey->setChapter(1);
        curKey->setBook(1);

        if (bookno < 0) {
            curKey->setTestament(lastKey->getTestament());
            curKey->setBook(lastKey->getBook());
        }
        else {
            char t = 1;
            if (bookno > m_BMAX[0]) {
                t++;
                bookno -= m_BMAX[0];
            }
            curKey->setTestament(t);
            curKey->setBook(bookno);
        }

        if (((comma)||((verse < 0)&&(bookno < 0)))&&(!lastPartial)) {
            curKey->setChapter(lastKey->getChapter());
            curKey->setVerse(chap);  // chap because this is the first number captured
            if (suffix) {
                curKey->setSuffix(suffix);
            }
        }
        else {
            if (useChapterAsVerse && verse < 0 && chap > 0 && curKey->getChapterMax() == 1) {
                verse = chap;
                chap = 1;
            }


            if (chap >= 0) {
                curKey->setChapter(chap);
            }
            else {
                partial++;
                curKey->setChapter(1);
            }
            if (verse >= 0) {
                curKey->setVerse(verse);
                if (suffix) {
                    curKey->setSuffix(suffix);
                }
            }
            else {
                partial++;
                curKey->setVerse(1);
            }
        }

        if ((*buf == '-') && (expandRange)) {    // if this is a dash save lowerBound and wait for upper
            lastKey->setLowerBound(*curKey);
            lastKey->positionToTop();
            tmpListKey << *lastKey;
            tmpListKey.getElement()->m_userData =
                    static_cast<std::size_t>(buf - bufStart);
        }
        else {
            if (!dash) {     // if last separator was not a dash just add
                if (expandRange && partial) {
                    lastKey->setLowerBound(*curKey);
                    if (partial > 1)
                        curKey->positionToMaxChapter();
                    if (partial > 0)
                        curKey->positionToMaxVerse();
                    lastKey->setUpperBound(*curKey);
                    lastKey->positionToTop();
                    tmpListKey << *lastKey;
                    tmpListKey.getElement()->m_userData =
                            static_cast<std::size_t>(buf - bufStart);
                }
                else {
                    bool f = false;
                    if (curKey->getSuffix() == 'f') {
                        curKey->setSuffix(0);
                        f = true;
                    }
                    lastKey->setLowerBound(*curKey);
                    if (f && doubleF) curKey->positionToMaxVerse();
                    else if (f) curKey->increment();
                    lastKey->setUpperBound(*curKey);
                    lastKey->positionToTop();
                    tmpListKey << *lastKey;
                    tmpListKey.getElement()->m_userData =
                            static_cast<std::size_t>(buf - bufStart);
                }
            }
            else if (expandRange) {
                VerseKey * const newElement =
                        dynamic_cast<VerseKey *>(tmpListKey.getElement());
                if (newElement) {
                    if (partial > 1)
                        curKey->positionToMaxChapter();
                    if (partial > 0)
                        curKey->positionToMaxVerse();
                    newElement->setUpperBound(*curKey);
                    newElement->positionToTop();
                    tmpListKey.getElement()->m_userData =
                            static_cast<std::size_t>(buf - bufStart);
                }
            }
        }
    }
    tmpListKey.positionToTop();
    ListKey internalListKey;
    internalListKey = tmpListKey;
    internalListKey.positionToTop();    // Align internalListKey to first element before passing back;

    return internalListKey;
}


/******************************************************************************
 * VerseKey::setLowerBound    - sets / gets the lower boundary for this key
 */

void VerseKey::setLowerBound(const VerseKey &lb)
{
    initBounds();

    m_lowerBound = lb.getIndex();
    m_lowerBoundComponents.test   = lb.getTestament();
    m_lowerBoundComponents.book   = lb.getBook();
    m_lowerBoundComponents.chap   = lb.getChapter();
    m_lowerBoundComponents.verse  = lb.getVerse();
    m_lowerBoundComponents.suffix = lb.getSuffix();

    // both this following check and UpperBound check force upperBound to
    // change allowing LowerBound then UpperBound logic to always flow
    // and set values without restrictions, as expected
    if (m_upperBound < m_lowerBound) m_upperBound = m_lowerBound;
    m_boundSet = true;
}


/******************************************************************************
 * VerseKey::setUpperBound    - sets / gets the upper boundary for this key
 */

void VerseKey::setUpperBound(const VerseKey &ub)
{
    initBounds();

    m_upperBound = ub.getIndex();
    m_upperBoundComponents.test   = ub.getTestament();
    m_upperBoundComponents.book   = ub.getBook();
    m_upperBoundComponents.chap   = ub.getChapter();
    m_upperBoundComponents.verse  = ub.getVerse();
    m_upperBoundComponents.suffix = ub.getSuffix();

    // see setLowerBound comment, above
    if (m_upperBound < m_lowerBound) m_upperBound = m_lowerBound;
    m_boundSet = true;
}


/******************************************************************************
 * VerseKey::getLowerBound    - gets the lower boundary for this key
 */

VerseKey VerseKey::getLowerBound() const
{
    initBounds();
    if (!isAutoNormalize()) {
        m_tmpClone->m_testament = m_lowerBoundComponents.test;
        m_tmpClone->m_book      = m_lowerBoundComponents.book;
        m_tmpClone->m_chapter   = m_lowerBoundComponents.chap;
        m_tmpClone->setVerse   (m_lowerBoundComponents.verse);
        m_tmpClone->setSuffix  (m_lowerBoundComponents.suffix);
    }
    else {
        m_tmpClone->setIndex(m_lowerBound);
        m_tmpClone->setSuffix  (m_lowerBoundComponents.suffix);
    }

    return (*m_tmpClone);
}


/******************************************************************************
 * VerseKey::getUpperBound    - sets / gets the upper boundary for this key
 */

VerseKey VerseKey::getUpperBound() const
{
    initBounds();
    if (!isAutoNormalize()) {
        m_tmpClone->m_testament = m_upperBoundComponents.test;
        m_tmpClone->m_book      = m_upperBoundComponents.book;
        m_tmpClone->m_chapter   = m_upperBoundComponents.chap;
        m_tmpClone->setVerse   (m_upperBoundComponents.verse);
        m_tmpClone->setSuffix  (m_upperBoundComponents.suffix);
    }
    else {
        m_tmpClone->setIndex(m_upperBound);
        m_tmpClone->setSuffix  (m_upperBoundComponents.suffix);
    }

    return (*m_tmpClone);
}


/******************************************************************************
 * VerseKey::clearBounds    - clears bounds for this VerseKey
 */

void VerseKey::clearBounds()
{
    m_tmpClone.reset();
    m_boundSet = false;
}


void VerseKey::initBounds() const
{
    if (!m_tmpClone) {
        m_tmpClone.reset(static_cast<VerseKey *>(clone().release()));
        m_tmpClone->setAutoNormalize(false);
        m_tmpClone->setIntros(true);
        m_tmpClone->setTestament(m_BMAX[1] ? 2 : 1);
        m_tmpClone->setBook(m_BMAX[m_BMAX[1] ? 1 : 0]);
        m_tmpClone->setChapter(m_tmpClone->getChapterMax());
        m_tmpClone->setVerse(m_tmpClone->getVerseMax());
        m_upperBound = m_tmpClone->getIndex();
        m_upperBoundComponents.test   = m_tmpClone->getTestament();
        m_upperBoundComponents.book   = m_tmpClone->getBook();
        m_upperBoundComponents.chap   = m_tmpClone->getChapter();
        m_upperBoundComponents.verse  = m_tmpClone->getVerse();
        m_upperBoundComponents.suffix = m_tmpClone->getSuffix();

        m_lowerBound = 0;
        m_lowerBoundComponents.test   = 0;
        m_lowerBoundComponents.book   = 0;
        m_lowerBoundComponents.chap   = 0;
        m_lowerBoundComponents.verse  = 0;
        m_lowerBoundComponents.suffix = 0;
    } else {
        m_tmpClone->setLocale(getLocale());
    }
}


/******************************************************************************
 * VerseKey::getText - refreshes keytext before returning if cast to
 *                a (char *) is requested
 */

std::string VerseKey::getText() const {
    freshtext();
    return SWKey::getText();
}


std::string VerseKey::getShortText() const {
    freshtext();
    if (m_book < 1) {
        if (m_testament < 1) {
            return "[ Module Heading ]";
        } else {
            std::ostringstream oss;
            oss << "[ Testament " << static_cast<int>(m_testament)
                << " Heading ]";
            return oss.str();
        }
    } else {
        std::ostringstream oss;
        oss << getBookAbbrev() << ' ' << m_chapter << ':' << m_verse;
        return oss.str();
    }
}


std::string VerseKey::getBookName() const {
    return getPrivateLocale().translate(m_refSys->getBook(((m_testament>1)?m_BMAX[0]:0)+m_book-1)->getLongName());
}


std::string const & VerseKey::getOSISBookName() const {
    return m_refSys->getBook(((m_testament>1)?m_BMAX[0]:0)+m_book-1)->getOSISName();
}


std::string VerseKey::getBookAbbrev() const {
    return getPrivateLocale().translate(m_refSys->getBook(((m_testament>1)?m_BMAX[0]:0)+m_book-1)->getPreferredAbbreviation());
}

void VerseKey::positionToTop() {
    VerseKey const lb(getLowerBound());
    m_testament = (lb.getTestament() || m_intros) ? lb.getTestament() : 1;
    m_book      = (lb.getBook()      || m_intros) ? lb.getBook() : 1;
    m_chapter   = (lb.getChapter()   || m_intros) ? lb.getChapter() : 1;
    m_verse     = (lb.getVerse()     || m_intros) ? lb.getVerse() : 1;
    m_suffix    = lb.getSuffix();
    normalize(true);
    popError();    // clear error from normalize
}
void VerseKey::positionToBottom() {
    VerseKey const ub(getUpperBound());
    m_testament = (ub.getTestament() || m_intros) ? ub.getTestament() : 1;
    m_book      = (ub.getBook()      || m_intros) ? ub.getBook() : 1;
    m_chapter   = (ub.getChapter()   || m_intros) ? ub.getChapter() : 1;
    m_verse     = (ub.getVerse()     || m_intros) ? ub.getVerse() : 1;
    m_suffix    = ub.getSuffix();
    normalize(true);
    popError();    // clear error from normalize
}

void VerseKey::positionToMaxChapter() {
    m_suffix    = 0;
    m_verse     = 1;
    m_chapter   = 1;
    normalize();
    m_chapter   = getChapterMax();
    normalize(true);
    popError();    // clear error from normalize
}

void VerseKey::positionToMaxVerse() {
    m_suffix    = 0;
    m_verse     = 1;
    normalize();
    m_verse     = getVerseMax();
    m_suffix    = 0;
    normalize(true);
    popError();    // clear error from normalize
}

int VerseKey::getChapterMax() const {
    if (m_book < 1) return 0;
    const VersificationMgr::Book *b = m_refSys->getBook(((m_testament>1)?m_BMAX[0]:0)+m_book-1);
    return (b) ? b->getChapterMax() : -1;
}

int VerseKey::getVerseMax() const {
    if (m_book < 1) return 0;
    const VersificationMgr::Book *b = m_refSys->getBook(((m_testament>1)?m_BMAX[0]:0)+m_book-1);
    return (b) ? b->getVerseMax(m_chapter) : -1;
}


/******************************************************************************
 * VerseKey::increment    - Increments key a number of verses
 *
 * ENT:    step    - Number of verses to jump forward
 *
 * RET: *this
 */

void VerseKey::increment(int step) {
    // if we're not autonormalizing and we're already not normalized
    if (!m_autonorm && m_chapter > 0 && m_verse > getVerseMax()) {
        m_verse += step;
        checkBounds();
        return;
    }
    char ierror = 0;
    setIndex(getIndex() + step);
    while ((!m_verse) && (!m_intros) && (!ierror)) {
        setIndex(getIndex() + 1);
        ierror = popError();
    }

    m_error = (ierror) ? ierror : m_error;
}


/******************************************************************************
 * VerseKey::decrement    - Decrements key a number of verses
 *
 * ENT:    step    - Number of verses to jump backward
 *
 * RET: *this
 */

void VerseKey::decrement(int step) {
    // if we're not autonormalizing and we're already not normalized
    if (!m_autonorm && m_chapter > 0 && m_verse > getVerseMax()) {
        m_verse -= step;
        checkBounds();
        return;
    }
    char ierror = 0;
    setIndex(getIndex() - step);
    while ((!m_verse) && (!m_intros) && (!ierror)) {
        setIndex(getIndex() - 1);
        ierror = popError();
    }
    if ((ierror) && (!m_intros))
        increment();

    m_error = (ierror) ? ierror : m_error;
}


/******************************************************************************
 * VerseKey::normalize    - checks limits and normalizes if necessary (e.g.
 *                Matthew 29:47 = Mark 2:2).  If last verse is
 *                exceeded, key is set to last Book CH:VS
 * RET: *this
 */

void VerseKey::normalize(bool autocheck)
{

    if ((!autocheck || m_autonorm)    // only normalize if we were explicitely called or if autonorm is turned on
    ) {
        m_error = 0;

        while ((m_testament < 3) && (m_testament > 0)) {


            if (m_book > m_BMAX[m_testament-1]) {
                m_book -= (m_BMAX[m_testament-1] + (m_intros?1:0));
                m_testament++;
                continue;
            }
            if (m_book < (m_intros?0:1)) {
                if (--m_testament > 0) {
                    m_book += (m_BMAX[m_testament-1] + (m_intros?1:0));
                }
                continue;
            }


            if (m_chapter > getChapterMax()) {
                m_chapter -= (getChapterMax() + (m_intros?1:0));
                m_book++;
                continue;
            }
            if (m_chapter < (m_intros?0:1)) {
                --m_book;
                if (m_book < (m_intros?0:1)) {
                    if (--m_testament > 0) {
                        m_book += (m_BMAX[m_testament-1] + (m_intros?1:0));
                    }
                }
                m_chapter += (getChapterMax() + (m_intros?1:0));
                continue;
            }


            if (m_chapter > 0 && m_verse > getVerseMax()) {
                m_verse -= (getVerseMax() + (m_intros?1:0));
                m_chapter++;
                continue;
            }
            if (m_verse < (m_intros?0:1)) {
                if (--m_chapter < (m_intros?0:1)) {
                    --m_book;
                    if (m_book < (m_intros?0:1)) {
                        if (--m_testament > 0) {
                            m_book += (m_BMAX[m_testament-1] + (m_intros?1:0));
                        }
                    }
                    m_chapter += (getChapterMax() + (m_intros?1:0));
                }
                m_verse += (getVerseMax() + (m_intros?1:0));
                continue;
            }

            break;  // If we've made it this far (all failure checks continue) we're ok
        }

        if (m_testament > (m_BMAX[1]?2:1)) {
            m_testament = m_BMAX[1]?2:1;
            m_book      = m_BMAX[m_testament-1];
            m_chapter   = getChapterMax();
            m_verse     = getVerseMax();
            m_error     = KEYERR_OUTOFBOUNDS;
        }

        if (m_testament < 1) {
            m_error     = ((!m_intros) || (m_testament < 0) || (m_book < 0)) ? KEYERR_OUTOFBOUNDS : 0;
            m_testament = ((m_intros) ? 0 : 1);
            m_book      = ((m_intros) ? 0 : 1);
            m_chapter   = ((m_intros) ? 0 : 1);
            m_verse     = ((m_intros) ? 0 : 1);
        }

            // should we always perform bounds checks?  Tried but seems to cause infinite recursion
        if (compare_(getUpperBound()) > 0) {
            positionFrom(getUpperBound());
            m_error = KEYERR_OUTOFBOUNDS;
        }
        if (compare_(getLowerBound()) < 0) {
            positionFrom(getLowerBound());
            m_error = KEYERR_OUTOFBOUNDS;
        }
    }
}


/******************************************************************************
 * VerseKey::getTestament - Gets testament
 *
 * RET:    value of testament
 */

char VerseKey::getTestament() const
{
    return m_testament;
}


/******************************************************************************
 * VerseKey::getBook - Gets book
 *
 * RET:    value of book
 */

char VerseKey::getBook() const
{
    return m_book;
}


/******************************************************************************
 * VerseKey::getChapter - Gets chapter
 *
 * RET:    value of chapter
 */

int VerseKey::getChapter() const
{
    return m_chapter;
}


/******************************************************************************
 * VerseKey::getVerse - Gets verse
 *
 * RET:    value of verse
 */

int VerseKey::getVerse() const
{
    return m_verse;
}


/******************************************************************************
 * VerseKey::setTestament - Sets/gets testament
 *
 * ENT:    itestament - value which to set testament
 *        [std::numeric_limits<char>::max()] - only get
 *
 */

void VerseKey::setTestament(char itestament)
{
    m_suffix  = 0;
    m_verse   = (m_intros) ? 0 : 1;
    m_chapter = (m_intros) ? 0 : 1;
    m_book    = (m_intros) ? 0 : 1;
    m_testament = itestament;
    normalize(true);
}


/******************************************************************************
 * VerseKey::setBook - Sets/gets book
 *
 * ENT:    ibook - value which to set book
 */

void VerseKey::setBook(char ibook)
{
    m_suffix  = 0;
    m_verse   = (m_intros) ? 0 : 1;
    m_chapter = (m_intros) ? 0 : 1;
    m_book    = ibook;
    normalize(true);
}



/******************************************************************************
 * VerseKey::setBookName - Sets/gets book by name
 *
 * ENT:    bname - book name/abbrev
 */

void VerseKey::setBookName(std::string_view bookName) {
    int bnum = getBookFromAbbrev(bookName);
    if (bnum > -1) {
        if (bnum > m_BMAX[0]) {
            bnum -= m_BMAX[0];
            m_testament = 2;
        }
        else    m_testament = 1;
        setBook(bnum);
    }
    else m_error = KEYERR_OUTOFBOUNDS;
}


/******************************************************************************
 * VerseKey::setChapter - Sets/gets chapter
 *
 * ENT:    ichapter - value which to set chapter
 */

void VerseKey::setChapter(int ichapter)
{
    m_suffix  = 0;
    m_verse   = (m_intros) ? 0 : 1;
    m_chapter = ichapter;
    normalize(true);
}


/******************************************************************************
 * VerseKey::setVerse - Sets/gets verse
 *
 * ENT:    iverse - value which to set verse
 *        [std::numeric_limits<int>::max()] - only get
 *
 * RET:    if unchanged ->          value of verse
 *    if   changed -> previous value of verse
 */

void VerseKey::setVerse(int iverse)
{
    m_suffix  = 0;
    m_verse   = iverse;
    normalize(true);
}


char VerseKey::getSuffix() const {
    return m_suffix;
}

void VerseKey::setSuffix(char suf) {
    m_suffix = suf;
}

/******************************************************************************
 * VerseKey::isAutoNormalize - gets flag that tells VerseKey to auto-
 *                matically normalize itself when modified
 */

bool VerseKey::isAutoNormalize() const
{
    return m_autonorm;
}

void VerseKey::setAutoNormalize(bool iautonorm)
{
    m_autonorm = iautonorm;
    normalize(true);
}


/******************************************************************************
 * VerseKey::setIntros - Sets flag that tells VerseKey to include
 *                    chap/book/testmnt/module introductions
 *
 * ENT:    val - value which to set intros
 *
 */

void VerseKey::setIntros(bool val)
{
    m_intros = val;
    normalize(true);
}

bool VerseKey::isIntros() const
{
    return m_intros;
}


/******************************************************************************
 * VerseKey::getIndex - Gets index based upon current verse
 *
 * RET:    offset
 */

long VerseKey::getIndex() const
{
    long  offset;

    if (!m_testament) { // if we want module heading
        offset = 0;
    }
    else if (!m_book) {    // we want testament heading
        offset = ((m_testament == 2) ? m_refSys->getNTStartOffset():0) + 1;
    }
    else {
        offset = m_refSys->getOffsetFromVerse((((m_testament>1)?m_BMAX[0]:0)+m_book-1), m_chapter, m_verse);
    }
    return offset;
}


/******************************************************************************
 * VerseKey::getTestamentIndex - Gets index based upon current verse
 *
 * RET:    offset
 */

long VerseKey::getTestamentIndex() const
{
    long offset = getIndex();
    return (m_testament > 1) ? offset - m_refSys->getNTStartOffset() : offset;
}


/******************************************************************************
 * VerseKey::setIndex - Sets index based upon current verse
 *
 * ENT:    iindex - value to set index to
 *
 */

void VerseKey::setIndex(long iindex)
{
    // assert we're sane
    if (iindex < 0) {
        m_error = KEYERR_OUTOFBOUNDS;
        return;
    }

    int b;
    m_error = m_refSys->getVerseFromOffset(iindex, &b, &m_chapter, &m_verse);
    m_book = (unsigned char)b;
    m_testament = 1;
    if (m_book > m_BMAX[0]) {
        m_book -= m_BMAX[0];
        m_testament = 2;
    }
    // special case for Module and Testament heading
    if (m_book < 0) { m_testament = 0; m_book = 0; }
    if (m_chapter < 0) { m_book = 0; m_chapter = 0; }

    checkBounds();
}

void VerseKey::checkBounds() {

    long i = getIndex();

    initBounds();
    if (i > m_upperBound) {
        setIndex(m_upperBound);
        i = getIndex();
        m_error = KEYERR_OUTOFBOUNDS;
    }
    if (i < m_lowerBound) {
        setIndex(m_lowerBound);
        m_error = KEYERR_OUTOFBOUNDS;
    }
}


/******************************************************************************
 * VerseKey::compare    - Compares another SWKey object
 *
 * ENT:    ikey - key to compare with this one
 *
 * RET:    >0 if this versekey is greater than compare versekey
 *    <0 <
 *     0 =
 */

int VerseKey::compare(SWKey const & ikey) const noexcept {
    const SWKey *testKey = &ikey;
    if (const VerseKey * const vkey = dynamic_cast<VerseKey const *>(testKey))
        return compare_(*vkey);
    VerseKey const ivkey(ikey.getText());
    return compare_(ivkey);
}


/******************************************************************************
 * VerseKey::compare_    - Compares another VerseKey object
 *
 * ENT:    ikey - key to compare with this one
 *
 * RET:    >0 if this versekey is greater than compare versekey
 *    <0 <
 *     0 =
 */

int VerseKey::compare_(VerseKey const & ivkey) const noexcept {
    if (getTestament() != ivkey.getTestament())
        return (getTestament() < ivkey.getTestament()) ? -1 : 1;

    if (getBook() != ivkey.getBook())
        return (getBook() < ivkey.getBook()) ? -1 : 1;

    if (getChapter() != ivkey.getChapter())
        return (getChapter() < ivkey.getChapter()) ? -1 : 1;

    if (getVerse() != ivkey.getVerse())
        return (getVerse() < ivkey.getVerse()) ? -1 : 1;

    if (getSuffix() != ivkey.getSuffix())
        return (getSuffix() < ivkey.getSuffix()) ? -1 : 1;

    return 0;
}


const char *VerseKey::getOSISRef() const {
    static char buf[5][254];
    static int loop = 0;

    if (loop > 4)
        loop = 0;

    if (getVerse())
        sprintf(buf[loop], "%s.%d.%d", getOSISBookName().c_str(), getChapter(), getVerse());
    else if (getChapter())
        sprintf(buf[loop], "%s.%d", getOSISBookName().c_str(), getChapter());
    else if (getBook())
        sprintf(buf[loop], "%s", getOSISBookName().c_str());
    else    buf[loop][0] = 0;
    return buf[loop++];
}


/******************************************************************************
 * VerseKey::getRangeText - returns parsable range text for this key
 */

std::string VerseKey::getRangeText() const {
    if (isBoundSet() && m_lowerBound != m_upperBound) {
        return getLowerBound().getText() + '-' + getUpperBound().getText();
    } else {
        return getText();
    }
}


/******************************************************************************
 * VerseKey::getOSISRefRangeText - returns parsable range text for this key
 */

std::string VerseKey::getOSISRefRangeText() const {
    if (isBoundSet() && (m_lowerBound != m_upperBound)) {
        std::string r(getLowerBound().getOSISRef());
        r.push_back('-');
        r.append(getUpperBound().getOSISRef());
        return r;
    }
    return getOSISRef();
}


// TODO:  this is static so we have no context.  We can only parse KJV v11n now
//         possibly add a const char *versification = KJV param?
std::string VerseKey::convertToOSIS(std::string const & inRef,
                                    SWKey const * lastKnownKey)
{
    std::ostringstream oss;

    VerseKey defLanguage;
    ListKey verses = defLanguage.parseVerseList(inRef.c_str(),
                                                lastKnownKey->getText().c_str(),
                                                true);
    std::size_t startFragIndex = 0u;
    for (std::size_t i = 0u; i < verses.getCount(); ++i) {
        SWKey *element = verses.getElement(i);
//        VerseKey *element = SWDYNAMIC_CAST(VerseKey, verses.GetElement(i));
        while ((startFragIndex != inRef.size())
               && inRef[startFragIndex]
               && (std::strchr(" {}:;,()[].", inRef[startFragIndex])))
        {
            oss << inRef[startFragIndex];
            ++startFragIndex;
        }
        auto const len = element->m_userData - startFragIndex + 1u;
        static_assert(std::numeric_limits<decltype(element->m_userData)>::max()
                      <= std::numeric_limits<std::size_t>::max(), "");
        std::string frag(&inRef[startFragIndex], len);
        while (!frag.empty() && frag.back() == '\0')
            frag.pop_back();
        std::string postJunk;
        auto j = frag.find_last_not_of(" {}:;,()[].");
        if (j == std::string::npos) {
            std::swap(frag, postJunk);
        } else {
            postJunk.append(frag, j + 1u, std::string::npos);
            frag.resize(j + 1u);
        }
        startFragIndex += len;
        oss << "<reference osisRef=\"" << element->getOSISRefRangeText()
            << "\">" << frag << "</reference>" << postJunk;
    }
    if (startFragIndex < std::strlen(inRef.c_str()))
        oss << &inRef[startFragIndex];
    return oss.str();
}
} /* namespace swordxx */
