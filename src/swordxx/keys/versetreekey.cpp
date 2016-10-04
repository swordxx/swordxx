/******************************************************************************
 *
 *  versetreekey.cpp -    code for class 'VerseTreeKey'- versekey using treekey
 *            for data retrieval
 *
 * $Id$
 *
 * Copyright 2006-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "versetreekey.h"

#include <cctype>
#include <cstdlib>
#include <cstring>


namespace swordxx {


/******************************************************************************
 * VerseTreeKey Constructor - initializes instance of VerseTreeKey
 *
 * ENT:    ikey - base key (will take various forms of 'BOOK CH:VS'.  See
 *        VerseTreeKey::parse for more detailed information)
 */

VerseTreeKey::VerseTreeKey(TreeKey *treeKey, const SWKey *ikey) : VerseKey(ikey)
{
    init(treeKey);
    if (ikey)
        parse();
}


/******************************************************************************
 * VerseTreeKey Constructor - initializes instance of VerseTreeKey
 *
 * ENT:    ikey - text key (will take various forms of 'BOOK CH:VS'.  See
 *        VerseTreeKey::parse for more detailed information)
 */

VerseTreeKey::VerseTreeKey(TreeKey *treeKey, const char *ikey) : VerseKey(ikey)
{
    init(treeKey);
    if (ikey)
        parse();
}


VerseTreeKey::VerseTreeKey(VerseTreeKey const &k) : VerseKey(k)
{
    init(k.treeKey);
}


VerseTreeKey::VerseTreeKey(TreeKey *treeKey, const char *min, const char *max) : VerseKey(min, max)
{
    init(treeKey);
}


void VerseTreeKey::init(TreeKey *treeKey)
{
    this->treeKey = (TreeKey *)treeKey->clone();
    this->treeKey->setPositionChangeListener(this);
    internalPosChange = false;
}


SWKey *VerseTreeKey::clone() const
{
    return new VerseTreeKey(*this);
}


int VerseTreeKey::getBookFromAbbrev(const char *iabbr) const
{
    int bookno = VerseKey::getBookFromAbbrev(iabbr);
    if (bookno < 0) {
/*
        vector<struct sbook>::iterator it = find(books, iabbr);
        if (it == books.end()) {
            TreeKey *tkey = this->treeKey;
            int saveError = tkey->popError();
            long bookmark = tkey->getOffset();
            std::string segment;
            internalPosChange = true;
            do {
                segment = (std::string)tkey->getLocalName();
            } while (tkey->parent());
            segment << 1;
            if (saveError) {
                error = saveError;
            }
            tkey->setOffset(bookmark);
        }
        books.push_back(sbook(iabbr));
*/
    }
    return bookno;
}

/******************************************************************************
 * VerseTreeKey Destructor - cleans up instance of VerseTreeKey
 *
 * ENT:    ikey - text key
 */

VerseTreeKey::~VerseTreeKey() {
    delete treeKey;
}


void VerseTreeKey::decrement(int /* steps */) {
    int treeError = 0;
    if (!error) lastGoodOffset = getTreeKey()->getOffset();
    do {
        treeKey->decrement();
        treeError = treeKey->popError();
    // iterate until 3 levels and no versekey parse errors
    } while (!treeError && ((treeKey->getLevel() < 3) || error));
    if (error && !treeError) {
        int saveError = error;
        increment();
        error = saveError;
    }
    if (treeError) {
        treeKey->setOffset(lastGoodOffset);
        error = treeError;
    }
    if (compare_(getUpperBound()) > 0) {
        positionFrom(getUpperBound());
        error = KEYERR_OUTOFBOUNDS;
    }
    if (compare_(getLowerBound()) < 0) {
        positionFrom(getLowerBound());
        error = KEYERR_OUTOFBOUNDS;
    }
}


void VerseTreeKey::increment(int /* steps */) {
    int treeError = 0;
    if (!error) lastGoodOffset = getTreeKey()->getOffset();
    do {
        treeKey->increment();
        treeError = treeKey->popError();
    // iterate until 3 levels and no versekey parse errors
    } while (!treeError && ((treeKey->getLevel() < 3) || error));
    if (error && !treeError) {
        int saveError = error;
        decrement();
        error = saveError;
    }
    if (treeError) {
        treeKey->setOffset(lastGoodOffset);
        error = treeError;
    }
    // bounds
    if (compare_(getUpperBound()) > 0) {
        positionFrom(getUpperBound());
        error = KEYERR_OUTOFBOUNDS;
    }
    if (compare_(getLowerBound()) < 0) {
        positionFrom(getLowerBound());
        error = KEYERR_OUTOFBOUNDS;
    }
}


void VerseTreeKey::positionChanged() {
    if (!internalPosChange) {
        TreeKey *tkey = this->TreeKey::PositionChangeListener::getTreeKey();
        int saveError = tkey->popError();
        long bookmark = tkey->getOffset();
        std::string seg[4];
        internalPosChange = true;
        int legs = 0;
        do {
            seg[legs] = tkey->getLocalName();
            legs++;
        } while (tkey->parent() && (legs < 4));

        legs--;

        if ((legs < 2) && (!seg[0].length() || seg[0] == "/")) {        //"[ Module Heading ]";
            m_testament = 0;
            m_book      = 0;
            m_chapter   = 0;
            setVerse(0);
        }
        else if ((legs < 2)
            && ((!strncmp(seg[0].c_str(), "[ Testament ", 12)) &&        //"[ Testament n Heading ]";
                (isdigit(seg[0][12])) &&
                (!strcmp(seg[0].c_str()+13, " Heading ]")))) {
            m_testament = (seg[0][12]-48);
            m_book      = 0;
            m_chapter   = 0;
            setVerse(0);
        }    //path = "[ Module Heading ]";
        else {
            setBookName(seg[--legs].c_str());
            m_chapter = (legs > 0) ? atoi(seg[--legs].c_str()) : 0;
            setVerse((legs > 0) ? atoi(seg[--legs].c_str()) : 0);
        }

//        setText(path);
        if (saveError) {
            error = saveError;
        }
        tkey->setOffset(bookmark);
        tkey->setError(saveError);
        internalPosChange = false;
    }
}


void VerseTreeKey::syncVerseToTree() {
    internalPosChange = true;
    std::string path;
    if (!getTestament()) path = "/"; // "[ Module Heading ]";
    else if (!getBook()) path = formatted("/[ Testament %d Heading ]", getTestament());
    else path = formatted("/%s/%d/%d", getOSISBookName(), getChapter(), getVerse());
    if (getSuffix()) path += getSuffix();
    long bookmark = treeKey->getOffset();
    treeKey->setText(path.c_str());

    // if our module has jacked inconsistencies, then let's put our tree back to where it was
    if (treeKey->popError()) {
        treeKey->setOffset(bookmark);
    }

    internalPosChange = false;
}


TreeKey *VerseTreeKey::getTreeKey() {
    syncVerseToTree();
    return treeKey;
}

// can autonormalize yet (ever?)
void VerseTreeKey::Normalize(char /* autocheck */) {
    error = 0;
}

long VerseTreeKey::NewIndex() const {
    return treeKey->getOffset();
}


void VerseTreeKey::positionToTop() {
    if (isBoundSet())
        return VerseKey::positionToTop();
    popError();
    treeKey->positionToTop();
    increment();
    decrement();
    popError();
}

void VerseTreeKey::positionToBottom() {
    if (isBoundSet())
        return VerseKey::positionToBottom();
    popError();
    treeKey->positionToBottom();
    decrement();
    increment();
    popError();
}

/******************************************************************************
 * VerseTreeKey::copyFrom - Equates this VerseTreeKey to another VerseTreeKey

void VerseTreeKey::copyFrom(const VerseTreeKey &ikey) {
    VerseKey::copyFrom(ikey);
}
 */

} /* namespace swordxx */
