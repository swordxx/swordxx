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

#include <string>


namespace swordxx {

class DirectoryEnumerator {

public: /* Methods: */

    /**
      \brief Constructs the object and attempts to open the directory for
             enumeration.
      \param[in] path The path to the directory to enumerate, must not be
                      nullptr.
    */
    DirectoryEnumerator(char const * const path);

    /**
      \brief Constructs the object and attempts to open the directory for
             enumeration.
      \param[in] path The path to the directory to enumerate.
    */
    DirectoryEnumerator(std::string const & path);

    DirectoryEnumerator(DirectoryEnumerator && move) noexcept;
    DirectoryEnumerator(DirectoryEnumerator const &) = delete;

    ~DirectoryEnumerator() noexcept;

    DirectoryEnumerator & operator=(DirectoryEnumerator && move) noexcept;
    DirectoryEnumerator & operator=(DirectoryEnumerator const &) = delete;

    /** \returns whether opening the directory for enumeration succeeded. */
    bool isOpen() const noexcept { return m_dir; }

    /** \returns isOpen() */
    explicit operator bool() const noexcept { return m_dir; }

    /**
       \warning The returned value might become invalid on the next call to
                readEntry(), close() or the destructor of DirectoryEnumerator.
       \returns a pointer to the name of the current entry, or nullptr on error
                or when there are no more entries.
       \pre isOpen() must hold, otherwise undefined behavior.
       \note on error, errno is set, otherwise errno is set to 0.
    */
    char const * readEntry() noexcept;

    /** \brief Closes the directory for enumeration.'*/
    void close() noexcept;

private: /* Fields: */

    void * m_dir;

};

} /* namespace swordxx */

#endif /* SWORDXX_SWMGR_H */
