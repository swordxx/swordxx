/******************************************************************************
 *
 *  entriesblk.cpp -    EntriesBlock facilitates compressed lex/dict modules
 *
 * $Id$
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "entriesblk.h"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>


namespace swordxx {

const int EntriesBlock::METAHEADERSIZE = 4;
    // count(4);
const int EntriesBlock::METAENTRYSIZE = 8;
    // offset(4); size(4);

EntriesBlock::EntriesBlock(const char *iBlock, unsigned long size)
    : m_block(size ? size : sizeof(std::uint32_t), '\0')
{ std::memcpy(m_block.data(), iBlock, size); }


EntriesBlock::EntriesBlock()
    : m_block(sizeof(std::uint32_t), '\0')
{}

EntriesBlock::~EntriesBlock() noexcept = default;


void EntriesBlock::setCount(int count) {
    std::uint32_t rawCount = archtosword32(count);
    assert(m_block.size() >= sizeof(rawCount));
    std::memcpy(m_block.data(), &rawCount, sizeof(rawCount));
}


int EntriesBlock::getCount() {
    std::uint32_t count = 0;
    std::memcpy(&count, m_block.data(), sizeof(count));
    count = swordtoarch32(count);
    return count;
}


void EntriesBlock::getMetaEntry(int index, unsigned long *offset, unsigned long *size) {
    std::uint32_t rawOffset = 0;
    std::uint32_t rawSize = 0;
    *offset = 0;
    *size = 0;
    if (index >= getCount())    // assert index < count
        return;

    // first 4 bytes is count, each 6 bytes after is each meta entry
    std::memcpy(&rawOffset, m_block.data() + METAHEADERSIZE + (index * METAENTRYSIZE), sizeof(rawOffset));
    std::memcpy(&rawSize, m_block.data() + METAHEADERSIZE + (index * METAENTRYSIZE) + sizeof(rawOffset), sizeof(rawSize));

    *offset = (unsigned long)swordtoarch32(rawOffset);
    *size   = (unsigned long)swordtoarch32(rawSize);
}


void EntriesBlock::setMetaEntry(int index, unsigned long offset, unsigned long size) {
    std::uint32_t rawOffset = archtosword32(offset);
    std::uint32_t rawSize = archtosword32(size);

    if (index >= getCount())    // assert index < count
        return;

    // first 4 bytes is count, each 6 bytes after is each meta entry
    std::memcpy(m_block.data() + METAHEADERSIZE + (index * METAENTRYSIZE), &rawOffset, sizeof(rawOffset));
    std::memcpy(m_block.data() + METAHEADERSIZE + (index * METAENTRYSIZE) + sizeof(rawOffset), &rawSize, sizeof(rawSize));
}


const char *EntriesBlock::getRawData(unsigned long *retSize) {
    unsigned long max = 4;
    int loop;
    unsigned long offset;
    unsigned long size;
    for (loop = 0; loop < getCount(); loop++) {
        getMetaEntry(loop, &offset, &size);
        max = ((offset + size) > max) ? (offset + size) : max;
    }
    *retSize = max;
    return m_block.data();
}


int EntriesBlock::addEntry(const char *entry) {
    unsigned long dataSize;
    getRawData(&dataSize);
    unsigned long  len = std::strlen(entry);
    unsigned long offset;
    unsigned long size;
    int count = getCount();
    unsigned long dataStart = METAHEADERSIZE + (count * METAENTRYSIZE);
    // new meta entry + new data size + 1 because null
    m_block.resize(dataSize + METAENTRYSIZE + len + 1);
    // shift right to make room for new meta entry
    std::memmove(m_block.data() + dataStart + METAENTRYSIZE,
                 m_block.data() + dataStart,
                 dataSize - dataStart);

    for (int loop = 0; loop < count; loop++) {
        getMetaEntry(loop, &offset, &size);
        if (offset) {    // if not a deleted entry
            offset += METAENTRYSIZE;
            setMetaEntry(loop, offset, size);
        }
    }

    offset = dataSize;    // original dataSize before realloc
    size = len + 1;
    // add our text to the end
    std::memcpy(m_block.data() + offset + METAENTRYSIZE, entry, size);
    // increment count
    setCount(count + 1);
    // add our meta entry
    setMetaEntry(count, offset + METAENTRYSIZE, size);
    // return index of our new entry
    return count;
}


const char *EntriesBlock::getEntry(int entryIndex) {
    unsigned long offset;
    unsigned long size;
    static const char empty[] = "";

    getMetaEntry(entryIndex, &offset, &size);
    return (offset) ? m_block.data() + offset : empty;
}


unsigned long EntriesBlock::getEntrySize(int entryIndex) {
    unsigned long offset;
    unsigned long size;

    getMetaEntry(entryIndex, &offset, &size);
    return (offset) ? size : 0;
}


void EntriesBlock::removeEntry(int entryIndex) {
    unsigned long offset;
    unsigned long size, size2;
    unsigned long dataSize;
    getRawData(&dataSize);
    getMetaEntry(entryIndex, &offset, &size);
    int count = getCount();

    if (!offset)    // already deleted
        return;

    // shift left to retrieve space used for old entry
    std::memmove(m_block.data() + offset,
                 m_block.data() + offset + size,
                 dataSize - (offset + size));

    // fix offset for all entries after our entry that were shifted left
    for (int loop = entryIndex + 1; loop < count; loop++) {
        getMetaEntry(loop, &offset, &size2);
        if (offset) {    // if not a deleted entry
            offset -= size;
            setMetaEntry(loop, offset, size2);
        }
    }

    // zero out our meta entry
    setMetaEntry(entryIndex, 0L, 0);
}


} /* namespace swordxx */
