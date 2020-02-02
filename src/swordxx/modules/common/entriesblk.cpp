/******************************************************************************
 *
 *  entriesblk.cpp -    EntriesBlock facilitates compressed lex/dict modules
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
#include <new>
#include <stdexcept>
#include "../../sysdata.h"
#include "../../max_v.h"


namespace swordxx {

namespace {

constexpr auto const METAHEADERSIZE = sizeof(EntriesBlock::SizeType);
constexpr auto const METAENTRYSIZE =
        sizeof(EntriesBlock::EntryOffsetType)
        + sizeof(EntriesBlock::EntrySizeType);

template <typename T>
inline constexpr bool sumFits(T const) noexcept { return true; }

template <typename T, typename ... Args>
inline constexpr bool sumFits(T a, T b, Args && ... args) noexcept {
    return (max_v<T> - a >= b)
           && sumFits<T>(a + b, std::forward<Args>(args)...);
}


} // anonymous namespace

EntriesBlock::EntriesBlock(void const * iBlock, std::size_t size)
    : m_serializedSize(size ? size : sizeof(SizeType))
{
    if (!size)
        return;
    auto readPtr(static_cast<char const *>(iBlock));
    auto sizeLeft(size);
    auto const read =
            [&readPtr, &sizeLeft](void * const data, std::size_t const dataSize)
            {
                if (sizeLeft < dataSize)
                    return false;
                std::memcpy(data, readPtr, dataSize);
                readPtr += dataSize;
                sizeLeft -= dataSize;
                return true;
            };
    auto const read32 =
            [&read](auto & v) {
                std::remove_cv_t<std::remove_reference_t<decltype(v)> > raw;
                static_assert(std::is_same<decltype(raw), std::uint32_t>::value,
                              "");
                if (!read(&raw, sizeof(raw)))
                    return false;
                v = swordtoarch32(raw);
                return true;
            };

    // Parse number of entries:
    SizeType numEntries_;
    if (!read32(numEntries_))
        throw std::runtime_error("Failed to deserialize number of entries!");
    m_entries.reserve(numEntries_);

    // Parse metadata:
    struct MetaInfo { EntryOffsetType offset; EntrySizeType size; };
    std::vector<MetaInfo> metaInfo(numEntries_);
    for (auto & metaEntry : metaInfo) {
        if (!read32(metaEntry.offset))
            throw std::runtime_error("Failed to deserialize entry offset!");
        if (!read32(metaEntry.size))
            throw std::runtime_error("Failed to deserialize entry size!");
    }

    { // Parse data:
        std::vector<char> entryBuffer;
        for (auto const & metaEntry : metaInfo) {
            // Drop removed entries:
            if (!metaEntry.offset) {
                m_serializedSize -= METAENTRYSIZE;
                continue;
            }

            if (metaEntry.size <= 0u)
                throw std::runtime_error("Unexpected empty entry!");

            if (metaEntry.offset != (size - sizeLeft))
                throw std::runtime_error(
                        "Out of bounds or incorrect entry data offset!");

            entryBuffer.resize(metaEntry.size);
            if (!read(entryBuffer.data(), entryBuffer.size()))
                throw std::runtime_error("Failed to read entry!");

            if (entryBuffer.back() != '\0')
                throw std::runtime_error("Entry not properly NULL-terminated!");
            entryBuffer.pop_back();

            m_entries.emplace_back(entryBuffer.cbegin(), entryBuffer.cend());
        }
    }
    if (sizeLeft)
        throw std::runtime_error("Unexpected data after entries!");
}


EntriesBlock::EntriesBlock() noexcept = default;

EntriesBlock::~EntriesBlock() noexcept = default;


EntriesBlock::SizeType EntriesBlock::numEntries() const {
    assert(m_entries.size() <= max_v<SizeType>);
    return static_cast<SizeType>(m_entries.size());
}

std::vector<char> EntriesBlock::serialized() const {
    std::vector<char> r(m_serializedSize);
    auto writePtr(r.data());
    auto sizeLeft(r.size());
    auto const write =
            [&writePtr, &sizeLeft](void const * data, std::size_t size) noexcept
            {
                assert(sizeLeft < size);
                std::memcpy(writePtr, &data, sizeof(size));
                writePtr += sizeof(size);
                sizeLeft -= sizeof(size);
            };
    auto const write32 =
            [&write](auto v) noexcept {
                static_assert(std::is_same<decltype(v), std::uint32_t>::value,
                              "");
                auto const raw(archtosword32(v));
                write(&raw, sizeof(raw));
            };

    // Write number of entries:
    assert(m_entries.size() <= max_v<SizeType>);
    write32(static_cast<SizeType>(m_entries.size()));

    if (!m_entries.empty()) {
        { // Write metadata entries:
            EntryOffsetType entryOffset =
                    static_cast<EntryOffsetType>(
                        METAHEADERSIZE + (m_entries.size() * METAENTRYSIZE));
            for (auto const & entry : m_entries) {
                write32(entryOffset);
                write32(static_cast<EntrySizeType>(entry.size()));
                entryOffset += entry.size();
            }
            assert(entryOffset == r.size() - sizeLeft);
        }

        // Write data:
        for (auto const & entry : m_entries)
            write(entry.c_str(), entry.size() + 1u);
        assert(sizeLeft == 0u);
    }
    return r;
}


EntriesBlock::SizeType EntriesBlock::addEntry(std::string_view entry) {
    if (entry.size() > max_v<EntrySizeType> - 1u)
        throw std::runtime_error("Entry too big!");

    auto const oldNumEntries(m_entries.size());
    if (oldNumEntries >= max_v<SizeType>)
        throw std::runtime_error("Too many entries!");

    if (!sumFits<std::size_t>(m_serializedSize, METAENTRYSIZE, entry.size(), 1u))
        throw std::bad_array_new_length();

    std::size_t const newSerializedSize(
                m_serializedSize + METAENTRYSIZE + entry.size() + 1u);
    if (newSerializedSize < max_v<EntryOffsetType>)
        throw std::bad_array_new_length();

    m_entries.emplace_back(std::move(entry));
    m_serializedSize = newSerializedSize;

    return static_cast<SizeType>(oldNumEntries); // index of our new entry
}

std::string const & EntriesBlock::operator[](SizeType index) const noexcept {
    static std::string const empty;
    return (index < m_entries.size()) ? m_entries[index] : empty;
}

} /* namespace swordxx */
