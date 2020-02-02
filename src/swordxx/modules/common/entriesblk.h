/******************************************************************************
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

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include "../../defs.h"


namespace swordxx {

class SWDLLEXPORT EntriesBlock {

public: /* Types: */

    using SizeType = std::uint32_t;
    using EntrySizeType = std::uint32_t;
    using EntryOffsetType = std::uint32_t;

public: /* Methods: */

    EntriesBlock() noexcept;
    EntriesBlock(void const * iBlock, std::size_t size);
    ~EntriesBlock() noexcept;

    SizeType numEntries() const;
    SizeType addEntry(std::string_view entry);
    std::string const & operator[](SizeType entryIndex) const noexcept;

    std::vector<char> serialized() const;
    std::size_t serializedSize() const noexcept { return m_serializedSize; }

private: /* Fields: */

    std::vector<std::string> m_entries;
    std::size_t m_serializedSize = sizeof(SizeType);

};


} /* namespace swordxx */

#endif /* SWORDXX_ENTRIESBLK_H */
