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

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include "../utilstr.h"


namespace swordxx {

/******************************************************************************
 * ListKey Constructor - initializes instance of ListKey
 *
 * ENT:    ikey - text key
 */

ListKey::ListKey(char const * const ikey)
    : SWKey(ikey)
{
    boundSet = true;
}


ListKey::ListKey(ListKey const & copy)
    : SWKey(copy.SWKey::getText())
    , m_arrayPos(copy.m_arrayPos)
    , m_array(copy.m_array.size())
{
    try {
        std::transform(copy.m_array.cbegin(),
                       copy.m_array.cend(),
                       m_array.begin(),
                       [](auto const & key)
                       { return std::unique_ptr<SWKey>(key->clone()); });
    } catch (...) {
        clear();
        throw;
    }
    boundSet = true;
}

SWKey * ListKey::clone() const { return new ListKey(*this); }

/******************************************************************************
 * ListKey Destructor - cleans up instance of ListKey
 */

ListKey::~ListKey() { clear(); }


/******************************************************************************
 * ListKey::clear    - Clears out elements of list
 */

void ListKey::clear() { m_array.clear(); }


/******************************************************************************
 * ListKey::copyFrom Equates this ListKey to another ListKey object
 *
 * ENT:    ikey - other ListKey object
 */

void ListKey::copyFrom(ListKey const & rhs) {
    decltype(m_array) newArray(rhs.m_array.size());
    std::transform(rhs.m_array.cbegin(),
                   rhs.m_array.cend(),
                   newArray.begin(),
                   [](auto const & key)
                   { return std::unique_ptr<SWKey>(key->clone()); });
    setToElement(0);
    clear();
    m_array = std::move(newArray);
}


/******************************************************************************
 * ListKey::add - Adds an element to the list
 */

void ListKey::add(SWKey const & ikey) {
    m_array.emplace_back(ikey.clone());
    setToElement(m_array.size() - 1u);
}



/******************************************************************************
 * ListKey::setPosition(SW_POSITION)    - Positions this key
 *
 * ENT:    p    - position
 *
 * RET:    *this
 */

void ListKey::positionToTop() { setToElementAndTop(0); }

void ListKey::positionToBottom() { setToElementAndBottom(m_array.size() - 1u); }


/******************************************************************************
 * ListKey::increment - Increments a number of elements
 */

void ListKey::increment(int step) {
    if (step < 0) {
        decrement(step * -1);
        return;
    }
    popError();        // clear error
    for(; step && !popError(); step--) {
        if (!m_array.empty() && m_arrayPos < m_array.size()) {
            auto const & key = m_array[m_arrayPos];
            if (key->isBoundSet())
                key->increment();
            if (key->popError() || !key->isBoundSet()) {
                setToElementAndTop(m_arrayPos + 1);
            } else {
                SWKey::setText(key->getText());
            }
        } else {
            error = KEYERR_OUTOFBOUNDS;
        }
    }
}


/******************************************************************************
 * ListKey::decrement - Decrements a number of elements
 */

void ListKey::decrement(int step) {
    if (step < 0) {
        increment(step * -1);
        return;
    }
    popError();        // clear error
    for(; step && !popError(); step--) {
        if (!m_array.empty() && m_arrayPos > -1) {
            auto const & key = m_array[m_arrayPos];
            if (key->isBoundSet())
                key->decrement();
            if (key->popError() || !key->isBoundSet()) {
                setToElementAndBottom(m_arrayPos - 1);
            } else {
                SWKey::setText(key->getText());
            }
        } else {
            error = KEYERR_OUTOFBOUNDS;
        }
    }
}


/******************************************************************************
 * ListKey::getCount    - Returns number of elements in list
 */

int ListKey::getCount() const { return m_array.size(); }

char ListKey::setToElementAndTop(int ielement) {
    m_arrayPos = ielement;
    auto const arraySize(m_array.size());
    if (m_arrayPos >= arraySize) {
        m_arrayPos = (arraySize > 0u) ? arraySize - 1u : 0u;
        error = KEYERR_OUTOFBOUNDS;
    } else {
        if (m_arrayPos < 0) {
            m_arrayPos = 0;
            error = KEYERR_OUTOFBOUNDS;
        } else {
            error = 0;
        }
    }

    if (arraySize) {
        auto const & key = m_array[m_arrayPos];
        if (key->isBoundSet())
            key->positionToTop();
        SWKey::setText(key->getText());
    } else {
        SWKey::setText("");
    }

    return error;
}

char ListKey::setToElementAndBottom(int ielement) {
    m_arrayPos = ielement;
    auto const arraySize(m_array.size());
    if (m_arrayPos >= arraySize) {
        m_arrayPos = (arraySize > 0u) ? arraySize - 1u : 0u;
        error = KEYERR_OUTOFBOUNDS;
    } else {
        if (m_arrayPos < 0) {
            m_arrayPos = 0;
            error = KEYERR_OUTOFBOUNDS;
        } else {
            error = 0;
        }
    }

    if (arraySize) {
        auto const & key = m_array[m_arrayPos];
        if (key->isBoundSet())
            key->positionToBottom();
        SWKey::setText(key->getText());
    } else {
        SWKey::setText("");
    }

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
        pos = m_arrayPos;

    return (pos >= m_array.size()) ? nullptr : m_array[pos].get();
}

SWKey *ListKey::getElement(int pos) {
    const ListKey &self = *this;
    return const_cast<SWKey *>(self.getElement(pos));
}



/******************************************************************************
 * ListKey::remove    - Removes current element from list
 */

void ListKey::remove() {
    if ((m_arrayPos > -1) && (m_arrayPos < m_array.size())) {
        m_array.erase(m_array.begin() + m_arrayPos);
        setToElement(m_arrayPos ? m_arrayPos - 1 : 0);
    }
}


/******************************************************************************
 * ListKey::getRangeText - returns parsable range text for this key
 */

std::string ListKey::getRangeText() const {
    std::string r;
    for (auto it = m_array.cbegin(); it != m_array.cend(); ++it) {
        r.append((*it)->getRangeText());
        if (it != m_array.rbegin().base())
            r.append("; ");
    }
    return r;
}


/******************************************************************************
 * ListKey::getOSISRefRangeText - returns parsable range text for this key
 */

std::string ListKey::getOSISRefRangeText() const {
    std::string r;
    for (auto it = m_array.cbegin(); it != m_array.cend(); ++it) {
        r.append((*it)->getOSISRefRangeText());
        if (it != m_array.rbegin().base())
            r.append(1u, ';');
    }
    return r;
}


/******************************************************************************
 * ListKey::getText - returns text key if (const char *) cast is requested
 */

const char *ListKey::getText() const {
    if (m_array.empty() || m_arrayPos >= m_array.size())
        return SWKey::getText();
    return m_array[m_arrayPos]->getText();
}

const char *ListKey::getShortText() const {
    if (m_array.empty() || m_arrayPos >= m_array.size())
        return SWKey::getText();
    return m_array[m_arrayPos]->getShortText();
}


void ListKey::setText(const char *ikey) {
    // at least try to set the current element to this text
    auto const arraySize(m_array.size());
    for (m_arrayPos = 0; m_arrayPos < arraySize; m_arrayPos++) {
        if (auto const & key = m_array[m_arrayPos]) {
            if (key->isTraversable() && key->isBoundSet()) {
                key->setText(ikey);
                if (!key->popError())
                    break;
            } else {
                if (!strcmp(key->getText(), ikey))
                    break;
            }
        }
    }
    if (m_arrayPos >= arraySize) {
        error = 1;
        m_arrayPos = arraySize - 1u;
    }

    SWKey::setText(ikey);
}

void ListKey::sort() {
    std::sort(m_array.begin(),
              m_array.end(),
              [](auto const & lhs, auto const & rhs) noexcept
              { return (*lhs) < (*rhs); });
}

} /* namespace swordxx */

