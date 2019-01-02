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

#ifndef SWORDXX_DIRECTORYENUMERATOR_H
#define SWORDXX_DIRECTORYENUMERATOR_H

namespace swordxx {

class DirectoryEnumerator {

public: /* Methods: */

    DirectoryEnumerator(char const * const path);
    DirectoryEnumerator(DirectoryEnumerator && move) noexcept;
    DirectoryEnumerator(DirectoryEnumerator const &) = delete;

    ~DirectoryEnumerator() noexcept;

    DirectoryEnumerator & operator=(DirectoryEnumerator && move) noexcept;
    DirectoryEnumerator & operator=(DirectoryEnumerator const &) = delete;

    explicit operator bool() const noexcept { return m_dir; }

    char const * readEntry() noexcept;

    void close() noexcept;

private: /* Fields: */

    void * m_dir;

};

} /* namespace swordxx */

#endif /* SWORDXX_SWMGR_H */
