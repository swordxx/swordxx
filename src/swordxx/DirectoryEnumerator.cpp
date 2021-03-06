/******************************************************************************
 *
 * Copyright 2019 Jaak Ristioja
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

#include "DirectoryEnumerator.h"

#include <cerrno>
#include <dirent.h>
#include <type_traits>


namespace swordxx {
namespace {

inline ::DIR * dirCast(void * dir) { return static_cast<::DIR *>(dir); }

} // anonymous namespace

DirectoryEnumerator::DirectoryEnumerator(char const * const path)
    : m_dir(::opendir(path))
{}

DirectoryEnumerator::DirectoryEnumerator(std::string const & path)
    : DirectoryEnumerator(path.c_str())
{}

DirectoryEnumerator::DirectoryEnumerator(DirectoryEnumerator && move) noexcept
    : m_dir(move.m_dir)
{ move.m_dir = nullptr; }

DirectoryEnumerator::~DirectoryEnumerator() noexcept {
    if (m_dir)
        ::closedir(dirCast(m_dir));
}

DirectoryEnumerator & DirectoryEnumerator::operator=(
        DirectoryEnumerator && move) noexcept
{
    if (m_dir)
        ::closedir(dirCast(m_dir));
    m_dir = move.m_dir;
    move.m_dir = nullptr;
    return *this;
}

char const * DirectoryEnumerator::readEntry() noexcept {
    // assert(m_dir) not needed: if !m_dir, readdir will just fail.
    errno = 0;
    for (;;) {
        auto const entry(::readdir(dirCast(m_dir)));
        static_assert(std::is_pointer<decltype(entry)>::value, "");
        if (!entry)
            return nullptr;
        auto const r = entry->d_name;
        if (*r == '.') {
            switch (*(r + 1)) {
            case '\0':
                continue; // skip "."
            case '.':
                if (*(r + 2) == '\0')
                    continue; // skip ".."
                break;
            default:
                break;
            }
        }
        return r;
    }
}

void DirectoryEnumerator::close() noexcept {
    if (m_dir) {
        ::closedir(dirCast(m_dir));
        m_dir = nullptr;
    }
}

} /* namespace swordxx */
