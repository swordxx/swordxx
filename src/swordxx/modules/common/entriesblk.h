/******************************************************************************
 *
 *  entriesblk.h -    Implementation of EntriesBlock
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

#ifndef SWORDXX_ENTRIESBLK_H
#define SWORDXX_ENTRIESBLK_H

#include <vector>
#include "../../defs.h"
#include "../../sysdata.h"


namespace swordxx {

class SWDLLEXPORT EntriesBlock {

private:
    std::vector<char> m_block;
    void setCount(int count);
    void getMetaEntry(int index, unsigned long *offset, unsigned long *size);
    void setMetaEntry(int index, unsigned long offset, unsigned long size);

public:
    EntriesBlock();
    EntriesBlock(char const * iBlock, std::size_t size);
    ~EntriesBlock() noexcept;

    int getCount();
    int addEntry(const char *entry);
    const char *getEntry(int entryIndex);
    unsigned long getEntrySize(int entryIndex);
    void removeEntry(int entryIndex);
    const char *getRawData(unsigned long *size);
};


} /* namespace swordxx */

#endif /* SWORDXX_ENTRIESBLK_H */
