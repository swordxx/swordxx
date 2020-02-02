/******************************************************************************
 *
 *  versekey.cpp -    code for class 'VerseKey'- a standard Biblical
 *            verse key
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
#include <memory>
#include <sstream>
#include <type_traits>
#include "../localemgr.h"
#include "../max_v.h"
#include "../roman.h"
#include "../ShareRef.h"
#include "../swlocale.h"
#include "../swlog.h"
#include "../utilstr.h"
#include "../versificationmgr.h"


namespace swordxx {
namespace {

auto getDefaultLocale() {
    auto const localeMgr(LocaleMgr::getSystemLocaleMgr());
    return localeMgr->getLocale(localeMgr->getDefaultLocaleName());
}

} // anonymous namespace

VerseKey::VerseKey(std::shared_ptr<VersificationMgr::System const> v11n)
    : SWKey(nullptr)
    , m_refSys(v11n
               ? std::move(v11n)
               : VersificationMgr::systemVersificationMgr()->getVersificationSystem("KJV"))
    , m_locale(getDefaultLocale())
{ assert(m_refSys); }

/******************************************************************************
 * VerseKey Constructor - initializes instance of VerseKey
 *
 * ENT:    ikey - text key (will take various forms of 'BOOK CH:VS'.  See
 *        VerseKey::parse for more detailed information)
 */

VerseKey::VerseKey(char const * ikeyText,
                   std::shared_ptr<VersificationMgr::System const> v11n)
    : SWKey(ikeyText)
    , m_refSys(v11n
               ? std::move(v11n)
               : VersificationMgr::systemVersificationMgr()->getVersificationSystem("KJV"))
    , m_locale(getDefaultLocale())
{
    assert(m_refSys);
    if (ikeyText)
        parse();
}

VerseKey::VerseKey(char const * min,
                   char const * max,
                   std::shared_ptr<VersificationMgr::System const> v11n)
    : SWKey()
    , m_refSys(v11n
               ? std::move(v11n)
               : VersificationMgr::systemVersificationMgr()->getVersificationSystem("KJV"))
    , m_locale(getDefaultLocale())
{
    assert(m_refSys);
    ListKey tmpListKey = parseVerseList(min);
    if (tmpListKey.getCount())
        setLowerBoundKey(static_cast<VerseKey &>(*tmpListKey.getElement(0u)));
    tmpListKey = parseVerseList(max, min, true);
    if (tmpListKey.getCount()) {
        auto const newElement_(tmpListKey.getElement(0u));
        auto const & newElement = static_cast<VerseKey const &>(*newElement_);
        setUpperBoundKey(newElement.isBoundSet()
                         ? newElement.upperBoundKey()
                         : newElement);
    }
    positionToTop();
}

/******************************************************************************
 * VerseKey Constructor - initializes instance of VerseKey
 *
 * ENT:    ikey - base key (will take various forms of 'BOOK CH:VS'.  See
 *        VerseKey::parse for more detailed information)
 */

VerseKey::VerseKey(SWKey const & ikey)
    : SWKey(ikey)
    , m_refSys(VersificationMgr::systemVersificationMgr()->getVersificationSystem("KJV"))
    , m_locale(getDefaultLocale())
{ copyFrom(ikey); }

VerseKey::VerseKey(VerseKey const & copy)
    : SWKey(copy)
    , m_refSys(VersificationMgr::systemVersificationMgr()->getVersificationSystem("KJV"))
    , m_locale(getDefaultLocale())
{ copyFrom(copy); }


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

        ikey.m_refSys->translateVerse(*m_refSys, &map_book, &map_chapter, &map_verse, &map_range);
        //printf("verse: %s.%i.%i-%i\n",map_book,map_chapter,map_verse,map_range);

        if (auto const r = m_refSys->bookNumberByOSISName(map_book)) {
            m_book = r.value() + 1u;
            if (m_refSys->books()[m_book-1].getChapterMax() < map_chapter) {
                map_chapter = m_refSys->books()[m_book-1].getChapterMax();
                map_verse = m_refSys->books()[m_book-1].getVerseMax(map_chapter);
                m_error = KEYERR_OUTOFBOUNDS;
            } else if (map_chapter > 0 && m_refSys->books()[m_book-1].getVerseMax(map_chapter) < map_verse) {
                map_verse = m_refSys->books()[m_book-1].getVerseMax(map_chapter);
                m_error = KEYERR_OUTOFBOUNDS;
            }
        } else {
            m_book = 1;
            m_error = KEYERR_OUTOFBOUNDS;
        }

        // set values
        auto const bMax(m_refSys->getBMAX());
        if (m_book > bMax[0]) {
            m_book -= bMax[0];
            m_testament = 2;
        } else {
            m_testament = 1;
        }

        //if (map_verse == 0) Headings(1);

        m_chapter = map_chapter;
        m_verse = map_verse;
        m_suffix = ikey.getSuffix();

        if (map_verse < map_range) {
            if (map_range > m_refSys->books()[((m_testament>1)?bMax[0]:0)+m_book-1].getVerseMax(m_chapter))
                ++map_range;
            m_verse = map_range;
            setUpperBoundKey(*this);
            m_verse = map_verse;
            setLowerBoundKey(*this);
        }
    }
}


void VerseKey::positionFrom(const SWKey &ikey) {
     m_error = 0;
    auto fromKey(shareRef(ikey));
    if (auto const tryList = dynamic_cast<ListKey const *>(&ikey))
        if (auto const k = tryList->getElement())
            fromKey = k;
    if (auto const tryVerse = dynamic_cast<VerseKey const *>(fromKey.get())) {
        setFromOther(*tryVerse);
    }
    else {
        SWKey::positionFrom(*fromKey);
// extraneous parse which inadvertently clears error flag
// SWKey::positionFrom already calls copyFrom which calls setText, which VerseKey::setText already calls parse()
//        parse();
    }

     // should we always perform bounds checks?  Tried but seems to cause infinite recursion
    if (compare_(upperBoundKey()) > 0) {
        setFromOther(upperBoundKey());
        m_error = KEYERR_OUTOFBOUNDS;
    }
    if (compare_(lowerBoundKey()) < 0) {
        setFromOther(lowerBoundKey());
        m_error = KEYERR_OUTOFBOUNDS;
    }
}

void VerseKey::setText(std::string newText) {
    SWKey::setText(newText);
    parse();
}

/******************************************************************************
 * VerseKey::copyFrom - Equates this VerseKey to another VerseKey
 */

void VerseKey::copyFrom(const VerseKey &ikey) {
    m_autonorm = ikey.m_autonorm;
    m_intros = ikey.m_intros;
    m_userData = ikey.m_userData;
    m_testament = ikey.getTestament();
    m_book = ikey.getBook();
    m_chapter = ikey.getChapter();
    m_verse = ikey.getVerse();
    m_suffix = ikey.getSuffix();
    m_locale = ikey.m_locale;
    setVersificationSystem(ikey.versificationSystem());
    if (ikey.isBoundSet()) {
        setLowerBoundKey(ikey.lowerBoundKey());
        setUpperBoundKey(ikey.upperBoundKey());
    }
}


/******************************************************************************
 * VerseKey::copyFrom - Equates this VerseKey to another SWKey
 */

void VerseKey::copyFrom(const SWKey &ikey) {
    // check to see if we can do a more specific copy
    // plus some optimizations
    auto fromKey(shareRef(ikey));
    if (auto const tryList = dynamic_cast<ListKey const *>(&ikey))
        if (auto k = tryList->getElement())
            fromKey = k;
    if (auto const tryVerse = dynamic_cast<VerseKey const *>(fromKey.get())) {
        copyFrom(*tryVerse);
    }
    else {
        SWKey::copyFrom(*fromKey);
// extraneous parse which inadvertently clears error flag
// SWKey::copyFrom already calls setText, which VerseKey::setText already calls parse()
//        parse();
    }
}

std::shared_ptr<SWKey> VerseKey::clone() const
{ return std::make_shared<VerseKey>(*this); }


/******************************************************************************
 * VerseKey Destructor - cleans up instance of VerseKey
 *
 * ENT:    ikey - text key
 */

VerseKey::~VerseKey() {}

bool VerseKey::isBoundSet() const noexcept { return m_boundSet; }

void VerseKey::setVersificationSystem(
        std::shared_ptr<VersificationMgr::System const> vs)
{
    assert(vs);
    if (m_refSys != vs) {
        m_refSys = std::move(vs);

        // TODO: adjust bounds for versificaion system ???
        // TODO: when we have mapping done, rethink this
        //necessary as our bounds might not mean anything in the new v11n system
        clearBounds();
    }

}


/******************************************************************************
 * VerseKey::parse - parses keytext into testament|book|chapter|verse
 *
 * RET:    error status
 */

char VerseKey::parse(bool checkAutoNormalize)
{
    auto const bMax(m_refSys->getBMAX());
    m_testament = bMax[1]?2:1;
    m_book      = bMax[bMax[1]?1:0];
    m_chapter   = 1;
    m_verse     = 1;

    char error = 0;
    auto const keytext = SWKey::getText();
    if (!keytext.empty()) {
        // pass our own copy of keytext as keytext memory may be freshed during parse
        ListKey tmpListKey = parseVerseList(keytext.c_str());
        if (tmpListKey.getCount()) {
            this->positionFrom(*tmpListKey.getElement(0u));
            error = this->m_error;
        } else error = KEYERR_OUTOFBOUNDS;
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

    if (m_book < 1) {
        if (m_testament < 1)
            sprintf(buf, "[ Module Heading ]");
        else sprintf(buf, "[ Testament %d Heading ]", (int)m_testament);
    }
    else {
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

std::optional<std::size_t> VerseKey::bookFromAbbrev(std::string_view iabbr)
        const
{
    iabbr = trimmedView(iabbr);
    if (iabbr.empty())
        return {};


    /* The first iteration of the following loop tries to uppercase the string.
       If we still fail to match, we try matching the string without any toupper
       logic. This is useful for, say, Chinese user input on a system that
       doesn't properly support a true Unicode-toupper function. */
    auto const upperAbbr(utf8ToUpper(iabbr));
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
                if (auto r = m_refSys->bookNumberByOSISName(osis.c_str()))
                    return r;
            }
            return std::optional<std::size_t>();
        };
    std::string_view abbr(upperAbbr);
    for (int i = 0; i < 2; i++, abbr = iabbr) {
        if (auto r = scanAbbreviations(m_locale->bookAbbreviations(), abbr))
            return r;
        if (auto r = scanAbbreviations(SWLocale::builtinBookAbbreviations(), abbr))
            return r;
    }
    return {};
}

void VerseKey::setLocale(std::shared_ptr<SWLocale> locale) {
    assert(locale);
    m_locale = std::move(locale);
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
    using namespace std::literals::string_view_literals;

    static_assert(std::is_unsigned<decltype(VerseKey::m_userData)>::value, "");
    static_assert(max_v<decltype(VerseKey::m_userData)>
                  >= max_v<std::size_t>, "");

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
    std::optional<std::size_t> bookno;
    bool comma = false;
    bool dash = false;
    ListKey tmpListKey;
    char lastPartial = 0;
    bool inTerm = true;
    int notAllDigits = 0;
    bool doubleF = false;

    std::shared_ptr<VerseKey> const curKey(
                std::static_pointer_cast<VerseKey>(clone()));
    std::shared_ptr<VerseKey> const lastKey(
                std::static_pointer_cast<VerseKey>(clone()));
    lastKey->clearBounds();
    curKey->clearBounds();

    // some silly checks for corner cases
    if (!std::strcmp(buf, "[ Module Heading ]")) {
        curKey->setVerse(0);
        curKey->setChapter(0);
        curKey->setBook(0);
        curKey->setTestament(0);
        lastKey->setLowerBoundKey(*curKey);
        lastKey->setUpperBoundKey(*curKey);
        ListKey internalListKey;
        internalListKey << *lastKey;
        return internalListKey;
    }
    if (startsWith(buf, "[ Testament "sv) &&
        (charIsDigit(buf[12])) &&
        (!std::strcmp(buf+13, " Heading ]"))) {
        curKey->setVerse(0);
        curKey->setChapter(0);
        curKey->setBook(0);
        curKey->setTestament(buf[12]-48);
        lastKey->setLowerBoundKey(*curKey);
        lastKey->setUpperBoundKey(*curKey);
        ListKey internalListKey;
        internalListKey << *lastKey;
        return internalListKey;
    }

    curKey->setAutoNormalize(isAutoNormalize());
    lastKey->setAutoNormalize(false);
    if (defaultKey) lastKey->setText(defaultKey);

    auto const bMax(m_refSys->getBMAX());

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
                if (book.empty() || book.back() != ' ') {
                    book.push_back(' ');
                }
                break;
            }

        case '-':
            if (chap == -1) {
                book.push_back(*buf);
                book.push_back(*(buf + 1u));
                if (bookFromAbbrev(book)) {
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
            bookno.reset();
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
                        if (isRoman(std::string_view(book).substr(loop + 1u))
                            && !isRoman(std::string_view(book).substr(0u, loop)))
                        {
                            if (verse == -1) {
                                verse = chap;
                                auto const parseResult(
                                            parseRomanNumeral(
                                                std::string_view(book).substr(
                                                    loop + 1u)));
                                /// \todo Is the following assertion correct?
                                assert(parseResult.has_value());
                                chap = *parseResult;
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
                bookno = bookFromAbbrev(book);
                if (bookno && (suffix == 'f') && (book.back() == 'f')) {
                    suffix = 0;
                }
            }
            if ((bookno || book.empty()) && (!book.empty() || (chap >= 0) || (verse >= 0))) {
                char partial = 0;
                curKey->setVerse(1);
                curKey->setChapter(1);
                curKey->setBook(1);

                if (!bookno) {
                    curKey->setTestament(lastKey->getTestament());
                    curKey->setBook(lastKey->getBook());
                }
                else {
                    char t = 1;
                    if (bookno.value() >= bMax[0]) {
                        t++;
                        bookno.value() -= bMax[0];
                    }
                    curKey->setTestament(t);
                    curKey->setBook(bookno.value() + 1u);
                }


                if (((comma)||((verse < 0)&&!bookno))&&(!lastPartial)) {
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
                if ((buf[q] == '-') && (expandRange)) {    // if this is a dash save lowerBoundKey and wait for upper
                    buf+=q;
                    lastKey->setLowerBoundKey(*curKey);
                    lastKey->positionToTop();
                    tmpListKey << *lastKey;
                    auto const element(tmpListKey.getElement());
                    auto & vk = static_cast<VerseKey &>(*element);
                    vk.setAutoNormalize(isAutoNormalize());
                    vk.m_userData = static_cast<std::size_t>(buf - bufStart);
                }
                else {
                    if (!dash) {     // if last separator was not a dash just add
                        if (expandRange && partial) {
                            lastKey->setLowerBoundKey(*curKey);
                            if (partial > 1)
                                curKey->positionToMaxChapter();
                            if (partial > 0)
                                curKey->positionToMaxVerse();
                            lastKey->setUpperBoundKey(*curKey);
                            lastKey->positionToTop();
                            tmpListKey << *lastKey;
                            auto const element(tmpListKey.getElement());
                            auto & vk = static_cast<VerseKey &>(*element);
                            vk.setAutoNormalize(isAutoNormalize());
                            vk.m_userData =
                                    static_cast<std::size_t>(buf - bufStart);
                        }
                        else {
                            bool f = false;
                            if (curKey->getSuffix() == 'f') {
                                curKey->setSuffix(0);
                                f = true;
                            }
                            lastKey->setLowerBoundKey(*curKey);
                            if (f && doubleF) curKey->positionToMaxVerse();
                            else if (f) curKey->increment();
                            lastKey->setUpperBoundKey(*curKey);
                            lastKey->positionToTop();
                            tmpListKey << *lastKey;
                            auto const element(tmpListKey.getElement());
                            auto & vk = static_cast<VerseKey &>(*element);
                            vk.setAutoNormalize(isAutoNormalize());
                            vk.m_userData =
                                    static_cast<std::size_t>(buf - bufStart);
                        }
                    }
                    else    if (expandRange) {
                        if (auto const newElement =
                                std::dynamic_pointer_cast<VerseKey>(
                                    tmpListKey.getElement()))
                        {
                            if (partial > 1)
                                curKey->positionToMaxChapter();
                            if (partial > 0)
                                curKey->positionToMaxVerse();
                            newElement->setUpperBoundKey(*curKey);
                            *lastKey = *curKey;
                            newElement->positionToTop();
                            auto & vk = static_cast<VerseKey &>(
                                            *tmpListKey.getElement());
                            vk.m_userData =
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
            else if (chap == -1 && (book.empty() || book.back() != ' ')) {
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
                    if ((charIsLower(*buf) && (chap >=0 || bookno || lastKey->isBoundSet()))
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
                if (isRoman(std::string_view(book).substr(loop + 1u))
                    && !isRoman(std::string_view(book).substr(0, loop)))
                {
                    if (verse == -1) {
                        verse = chap;
                        auto const parseResult(
                                    parseRomanNumeral(
                                        std::string_view(book).substr(
                                            loop + 1u)));
                        /// \todo Is the following assertion correct?
                        assert(parseResult.has_value());
                        chap = *parseResult;
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
        bookno = bookFromAbbrev(book);
        if (bookno && (suffix == 'f') && (book.back() == 'f')) {
            suffix = 0;
        }
    }
    if ((bookno || book.empty()) && (!book.empty() || (chap >= 0) || (verse >= 0))) {
        char partial = 0;
        curKey->setVerse(1);
        curKey->setChapter(1);
        curKey->setBook(1);

        if (!bookno) {
            curKey->setTestament(lastKey->getTestament());
            curKey->setBook(lastKey->getBook());
        }
        else {
            char t = 1;
            if (bookno.value() >= bMax[0]) {
                t++;
                bookno.value() -= bMax[0];
            }
            curKey->setTestament(t);
            curKey->setBook(bookno.value() + 1u);
        }

        if (((comma)||((verse < 0)&&!bookno))&&(!lastPartial)) {
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

        if ((*buf == '-') && (expandRange)) {    // if this is a dash save lowerBoundKey and wait for upper
            lastKey->setLowerBoundKey(*curKey);
            lastKey->positionToTop();
            tmpListKey << *lastKey;
            static_cast<VerseKey &>(*tmpListKey.getElement()).m_userData =
                    static_cast<std::size_t>(buf - bufStart);
        }
        else {
            if (!dash) {     // if last separator was not a dash just add
                if (expandRange && partial) {
                    lastKey->setLowerBoundKey(*curKey);
                    if (partial > 1)
                        curKey->positionToMaxChapter();
                    if (partial > 0)
                        curKey->positionToMaxVerse();
                    lastKey->setUpperBoundKey(*curKey);
                    lastKey->positionToTop();
                    tmpListKey << *lastKey;
                    static_cast<VerseKey &>(*tmpListKey.getElement()).m_userData =
                            static_cast<std::size_t>(buf - bufStart);
                }
                else {
                    bool f = false;
                    if (curKey->getSuffix() == 'f') {
                        curKey->setSuffix(0);
                        f = true;
                    }
                    lastKey->setLowerBoundKey(*curKey);
                    if (f && doubleF) curKey->positionToMaxVerse();
                    else if (f) curKey->increment();
                    lastKey->setUpperBoundKey(*curKey);
                    lastKey->positionToTop();
                    tmpListKey << *lastKey;
                    static_cast<VerseKey &>(*tmpListKey.getElement()).m_userData =
                            static_cast<std::size_t>(buf - bufStart);
                }
            }
            else if (expandRange) {
                if (auto const newElement =
                        std::dynamic_pointer_cast<VerseKey>(
                        tmpListKey.getElement()))
                {
                    if (partial > 1)
                        curKey->positionToMaxChapter();
                    if (partial > 0)
                        curKey->positionToMaxVerse();
                    newElement->setUpperBoundKey(*curKey);
                    newElement->positionToTop();
                    static_cast<VerseKey &>(*tmpListKey.getElement()).m_userData =
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


void VerseKey::setLowerBoundKey(const VerseKey &lb)
{
    initBounds();

    m_lowerBound = lb.getIndex();
    m_lowerBoundComponents.test   = lb.getTestament();
    m_lowerBoundComponents.book   = lb.getBook();
    m_lowerBoundComponents.chap   = lb.getChapter();
    m_lowerBoundComponents.verse  = lb.getVerse();
    m_lowerBoundComponents.suffix = lb.getSuffix();

    // both this following check and upperBoundKey check force upperBoundKey to
    // change allowing lowerBoundKey then upperBoundKey logic to always flow
    // and set values without restrictions, as expected
    if (m_upperBound < m_lowerBound) m_upperBound = m_lowerBound;
    m_boundSet = true;
}


void VerseKey::setUpperBoundKey(const VerseKey &ub)
{
    initBounds();

    m_upperBound = ub.getIndex();
    m_upperBoundComponents.test   = ub.getTestament();
    m_upperBoundComponents.book   = ub.getBook();
    m_upperBoundComponents.chap   = ub.getChapter();
    m_upperBoundComponents.verse  = ub.getVerse();
    m_upperBoundComponents.suffix = ub.getSuffix();

    // see setLowerBoundKey comment, above
    if (m_upperBound < m_lowerBound) m_upperBound = m_lowerBound;
    m_boundSet = true;
}


VerseKey VerseKey::lowerBoundKey() const
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


VerseKey VerseKey::upperBoundKey() const
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

void VerseKey::clearBounds() noexcept
{
    m_tmpClone.reset();
    m_boundSet = false;
}


void VerseKey::initBounds() const
{
    if (!m_tmpClone) {
        m_tmpClone = std::static_pointer_cast<VerseKey>(clone());
        m_tmpClone->setAutoNormalize(false);
        m_tmpClone->setIntros(true);
        {
            auto const bMax(m_refSys->getBMAX());
            m_tmpClone->setTestament(bMax[1] ? 2 : 1);
            m_tmpClone->setBook(bMax[bMax[1] ? 1 : 0]);
        }
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
        m_tmpClone->setLocale(m_locale);
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
    auto const bMax(m_refSys->getBMAX());
    return m_locale->translateText(m_refSys->books()[((m_testament>1)?bMax[0]:0)+m_book-1].getLongName());
}


std::string const & VerseKey::getOSISBookName() const {
    auto const bMax(m_refSys->getBMAX());
    return m_refSys->books()[((m_testament>1)?bMax[0]:0)+m_book-1].getOSISName();
}


std::string VerseKey::getBookAbbrev() const {
    auto const bMax(m_refSys->getBMAX());
    return m_locale->translatePrefAbbrev(m_refSys->books()[((m_testament>1)?bMax[0]:0)+m_book-1].getPreferredAbbreviation());
}

std::size_t VerseKey::getBookMax() const
{ return m_refSys->getBMAX()[m_testament-1]; }

void VerseKey::positionToTop() {
    VerseKey const lb(lowerBoundKey());
    m_testament = (lb.getTestament() || m_intros) ? lb.getTestament() : 1;
    m_book      = (lb.getBook()      || m_intros) ? lb.getBook() : 1;
    m_chapter   = (lb.getChapter()   || m_intros) ? lb.getChapter() : 1;
    m_verse     = (lb.getVerse()     || m_intros) ? lb.getVerse() : 1;
    m_suffix    = lb.getSuffix();
    normalize(true);
    popError();    // clear error from normalize
}
void VerseKey::positionToBottom() {
    VerseKey const ub(upperBoundKey());
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
    auto const bookNum = ((m_testament > 1) ? m_refSys->getBMAX()[0] : 0) + m_book - 1;
    auto const & books = m_refSys->books();
    if (bookNum >= books.size())
        return -1;
    return books[bookNum].getChapterMax();
}

int VerseKey::getVerseMax() const {
    if (m_book < 1) return 0;
    auto const bookNum = ((m_testament > 1) ? m_refSys->getBMAX()[0] : 0) + m_book - 1;
    auto const & books = m_refSys->books();
    if (bookNum >= books.size())
        return -1;
    return books[bookNum].getVerseMax(m_chapter);
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

bool VerseKey::isTraversable() const { return true; }


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
        auto const bMax(m_refSys->getBMAX());

        while ((m_testament < 3) && (m_testament > 0)) {


            if (m_book > bMax[m_testament-1]) {
                m_book -= (bMax[m_testament-1] + (m_intros?1:0));
                m_testament++;
                continue;
            }
            if (m_book < (m_intros?0:1)) {
                if (--m_testament > 0) {
                    m_book += (bMax[m_testament-1] + (m_intros?1:0));
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
                        m_book += (bMax[m_testament-1] + (m_intros?1:0));
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
                            m_book += (bMax[m_testament-1] + (m_intros?1:0));
                        }
                    }
                    m_chapter += (getChapterMax() + (m_intros?1:0));
                }
                m_verse += (getVerseMax() + (m_intros?1:0));
                continue;
            }

            break;  // If we've made it this far (all failure checks continue) we're ok
        }

        if (m_testament > (bMax[1]?2:1)) {
            m_testament = bMax[1]?2:1;
            m_book      = bMax[m_testament-1];
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
        if (compare_(upperBoundKey()) > 0) {
            positionFrom(upperBoundKey());
            m_error = KEYERR_OUTOFBOUNDS;
        }
        if (compare_(lowerBoundKey()) < 0) {
            positionFrom(lowerBoundKey());
            m_error = KEYERR_OUTOFBOUNDS;
        }
    }
}


/******************************************************************************
 * VerseKey::setTestament - Sets/gets testament
 *
 * ENT:    itestament - value which to set testament
 *        [max_v<char>] - only get
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
    if (auto bnum = bookFromAbbrev(bookName)) {
        auto const bMax(m_refSys->getBMAX());
        if (bnum.value() >= bMax[0]) {
            bnum.value() -= bMax[0];
            m_testament = 2;
        }
        else    m_testament = 1;
        setBook(bnum.value() + 1u);
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
 *        [max_v<int>] - only get
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

void VerseKey::setSuffix(char suf) noexcept {
    m_suffix = suf;
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
        offset = m_refSys->getOffsetFromVerse((((m_testament>1)?m_refSys->getBMAX()[0]:0)+m_book-1), m_chapter, m_verse);
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
    {
        auto const bMax(m_refSys->getBMAX());
        if (m_book > bMax[0]) {
            m_book -= bMax[0];
            m_testament = 2;
        }
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
    if (auto const vkey = dynamic_cast<VerseKey const *>(&ikey))
        return compare_(*vkey);
    return SWKey::compare(ikey);
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


std::string VerseKey::getOSISRef() const {
    std::ostringstream oss;
    if (getVerse()) {
        oss << getOSISBookName() << '.' << getChapter() << '.' << getVerse();
    } else if (getChapter()) {
        oss << getOSISBookName() << '.' << getChapter();
    } else if (getBook()) {
        oss << getOSISBookName();
    }
    return oss.str();
}


/******************************************************************************
 * VerseKey::getRangeText - returns parsable range text for this key
 */

std::string VerseKey::getRangeText() const {
    if (isBoundSet() && m_lowerBound != m_upperBound) {
        return lowerBoundKey().getText() + '-' + upperBoundKey().getText();
    } else {
        return getText();
    }
}


/******************************************************************************
 * VerseKey::getOSISRefRangeText - returns parsable range text for this key
 */

std::string VerseKey::getOSISRefRangeText() const {
    if (isBoundSet() && (m_lowerBound != m_upperBound)) {
        std::string r(lowerBoundKey().getOSISRef());
        r.push_back('-');
        r.append(upperBoundKey().getOSISRef());
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
        auto const element(verses.getElement(i));
//        VerseKey *element = SWDYNAMIC_CAST(VerseKey, verses.GetElement(i));
        while ((startFragIndex != inRef.size())
               && inRef[startFragIndex]
               && (std::strchr(" {}:;,()[].", inRef[startFragIndex])))
        {
            oss << inRef[startFragIndex];
            ++startFragIndex;
        }
        auto const & vk = static_cast<VerseKey &>(*element);
        auto const len = vk.m_userData - startFragIndex + 1u;
        static_assert(max_v<decltype(vk.m_userData)>
                      <= max_v<std::size_t>, "");
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
