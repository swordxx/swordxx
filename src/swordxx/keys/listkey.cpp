/******************************************************************************
 *
 *  listkey.cpp -    code for base class 'ListKey'.  ListKey is the basis
 *            for all types of keys that have lists of specified
 *            indexes (e.g. a list of verses, place, etc.)
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

#include "listkey.h"

#include <cstdlib>
#include <cstring>
#include "../utilstr.h"


namespace swordxx {

/******************************************************************************
 * ListKey Constructor - initializes instance of ListKey
 *
 * ENT:    ikey - text key
 */

ListKey::ListKey(const char *ikey): SWKey(ikey) {
    arraymax = 0;
    clear();
    init();
}


ListKey::ListKey(ListKey const &k) : SWKey(k.SWKey::getText()) {
    arraymax = k.arraymax;
    arraypos = k.arraypos;
    arraycnt = k.arraycnt;
    array = arraymax
            ? static_cast<SWKey **>(malloc(k.arraymax * sizeof(SWKey *)))
            : nullptr;
    for (int i = 0; i < arraycnt; i++)
        array[i] = k.array[i]->clone();
    init();
}


void ListKey::init() {
    // this is a listkey, bound is always set
    boundSet = true;
}


SWKey *ListKey::clone() const
{
    return new ListKey(*this);
}

/******************************************************************************
 * ListKey Destructor - cleans up instance of ListKey
 */

ListKey::~ListKey()
{
    clear();
}


/******************************************************************************
 * ListKey::clear    - Clears out elements of list
 */

void ListKey::clear()
{
    int loop;

    if (arraymax) {
        for (loop = 0; loop < arraycnt; loop++)
            delete array[loop];

        free(array);
        arraymax  = 0;
    }
    arraycnt  = 0;
    arraypos  = 0;
    array     = nullptr;
}


/******************************************************************************
 * ListKey::copyFrom Equates this ListKey to another ListKey object
 *
 * ENT:    ikey - other ListKey object
 */

void ListKey::copyFrom(const ListKey &ikey) {
    clear();

    arraymax = ikey.arraymax;
    arraypos = ikey.arraypos;
    arraycnt = ikey.arraycnt;
    array = arraymax
            ? static_cast<SWKey **>(malloc(ikey.arraymax * sizeof(SWKey *)))
            : nullptr;
    for (int i = 0; i < arraycnt; i++)
        array[i] = ikey.array[i]->clone();

    setToElement(0);
}


/******************************************************************************
 * ListKey::add - Adds an element to the list
 */

void ListKey::add(const SWKey &ikey) {
    if (++arraycnt > arraymax) {
        array = (SWKey **) ((array) ? realloc(array, (arraycnt + 32) * sizeof(SWKey *)) : calloc(arraycnt + 32, sizeof(SWKey *)));
        arraymax = arraycnt + 32;
    }
    array[arraycnt-1] = ikey.clone();
    setToElement(arraycnt-1);
}



/******************************************************************************
 * ListKey::setPosition(SW_POSITION)    - Positions this key
 *
 * ENT:    p    - position
 *
 * RET:    *this
 */

void ListKey::setPosition(SW_POSITION p) {
    switch (p) {
    case 1:    // GCC won't compile P_TOP
        setToElement(0, p);
        break;
    case 2:    // GCC won't compile P_BOTTOM
        setToElement(arraycnt-1, p);
        break;
    }
}


/******************************************************************************
 * ListKey::increment - Increments a number of elements
 */

void ListKey::increment(int step) {
    if (step < 0) {
        decrement(step*-1);
        return;
    }
    popError();        // clear error
    for(; step && !popError(); step--) {
        if (arraypos < arraycnt && arraycnt) {
            if (array[arraypos]->isBoundSet())
                ++(*(array[arraypos]));
            if ((array[arraypos]->popError()) || (!array[arraypos]->isBoundSet())) {
                setToElement(arraypos+1);
            }
            else SWKey::setText(array[arraypos]->getText());
        }
        else error = KEYERR_OUTOFBOUNDS;
    }
}


/******************************************************************************
 * ListKey::decrement - Decrements a number of elements
 */

void ListKey::decrement(int step) {
    if (step < 0) {
        increment(step*-1);
        return;
    }
    popError();        // clear error
    for(; step && !popError(); step--) {
        if (arraypos > -1 && arraycnt) {
            if (array[arraypos]->isBoundSet())
                --(*(array[arraypos]));
            if ((array[arraypos]->popError()) || (!array[arraypos]->isBoundSet())) {
                setToElement(arraypos-1, BOTTOM);
            }
            else SWKey::setText(array[arraypos]->getText());
        }
        else error = KEYERR_OUTOFBOUNDS;
    }
}


/******************************************************************************
 * ListKey::getCount    - Returns number of elements in list
 */

int ListKey::getCount() const {
    return arraycnt;
}


/******************************************************************************
 * ListKey::setToElement    - Sets key to element number
 *
 * ENT:    ielement    - element number to set to
 *     pos        - set the subkey element to position (TOP) or BOTTOM
 *
 * RET:    error status
 */

char ListKey::setToElement(int ielement, SW_POSITION pos) {
    arraypos = ielement;
    if (arraypos >= arraycnt) {
        arraypos = (arraycnt>0)?arraycnt - 1:0;
        error = KEYERR_OUTOFBOUNDS;
    }
    else {
        if (arraypos < 0) {
            arraypos = 0;
            error = KEYERR_OUTOFBOUNDS;
        }
        else {
            error = 0;
        }
    }

    if (arraycnt) {
        if (array[arraypos]->isBoundSet())
            (*array[arraypos]) = pos;
        SWKey::setText(array[arraypos]->getText());
    }
    else SWKey::setText("");

    return error;
}


/******************************************************************************
 * ListKey::getElement    - Gets a key element number
 *
 * ENT:    pos    - element number to get (or default current)
 *
 * RET:    Key or null on error
 */

const SWKey *ListKey::getElement(int pos) const {
    if (pos < 0)
        pos = arraypos;

    return (pos >= arraycnt) ? nullptr : array[pos];
}

SWKey *ListKey::getElement(int pos) {
    const ListKey &self = *this;
    return const_cast<SWKey *>(self.getElement(pos));
}



/******************************************************************************
 * ListKey::remove    - Removes current element from list
 */

void ListKey::remove() {
    if ((arraypos > -1) && (arraypos < arraycnt)) {
        delete array[arraypos];
        if (arraypos < arraycnt - 1)
            memmove(&array[arraypos], &array[arraypos+1], (arraycnt - arraypos - 1) * sizeof(SWKey *));
        arraycnt--;

        setToElement((arraypos)?arraypos-1:0);
    }
}


/******************************************************************************
 * ListKey::getRangeText - returns parsable range text for this key
 */

std::string ListKey::getRangeText() const {
    std::string r;
    for (int i = 0; i < arraycnt; i++) {
        r.append(array[i]->getRangeText());
        if (i < arraycnt - 1)
            r.append("; ");
    }
    return r;
}


/******************************************************************************
 * ListKey::getOSISRefRangeText - returns parsable range text for this key
 */

std::string ListKey::getOSISRefRangeText() const {
    std::string r;
    for (int i = 0; i < arraycnt; i++) {
        r.append(array[i]->getOSISRefRangeText());
        if (i < arraycnt - 1)
            r.append(1u, ';');
    }
    return r;
}


/******************************************************************************
 * ListKey::getText - returns text key if (const char *) cast is requested
 */

const char *ListKey::getText() const {
    int pos = arraypos;
    SWKey * key = (pos >= arraycnt || !arraycnt) ? nullptr : array[pos];
    return (key) ? key->getText() : SWKey::getText();
}

const char *ListKey::getShortText() const {
    int pos = arraypos;
    SWKey * key = (pos >= arraycnt || !arraycnt) ? nullptr : array[pos];
    return (key) ? key->getShortText() : SWKey::getText();
}


void ListKey::setText(const char *ikey) {
    // at least try to set the current element to this text
    for (arraypos = 0; arraypos < arraycnt; arraypos++) {
        SWKey *key = array[arraypos];
        if (key) {
            if (key->isTraversable() && key->isBoundSet()) {
                key->setText(ikey);
                if (!key->popError())
                    break;
            }
            else {
                if (!strcmp(key->getText(), ikey))
                    break;
            }
        }
    }
    if (arraypos >= arraycnt) {
        error = 1;
        arraypos = arraycnt-1;
    }

    SWKey::setText(ikey);
}

// This sort impl sucks.  Let's change it to a quicksort or some other efficient algol
void ListKey::sort() {
    for (int i = 0; i < arraycnt; i++) {
        for (int j = i; j < arraycnt; j++) {
            if (*array[j] < *array[i]) {
                SWKey *tmp = array[i];
                array[i] = array[j];
                array[j] = tmp;
            }
        }
    }
}

} /* namespace swordxx */

