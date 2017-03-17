/******************************************************************************
 *
 * Copyright 2017 Jaak Ristioja
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

#ifndef SWORDXX_NORMALIZEDPATH_H
#define SWORDXX_NORMALIZEDPATH_H

#include <string>


namespace swordxx {

class NormalizedPath {

public: /* Methods: */

    NormalizedPath(char const * path);
    NormalizedPath(std::string path);

    NormalizedPath getParentDirectory() const;

    bool empty() const noexcept { return m_path.empty(); }
    std::string::size_type size() const noexcept { return m_path.size(); }
    std::string const & str() const noexcept { return m_path; }
    char const * c_str() const noexcept { return m_path.c_str(); }
    operator std::string const & () const noexcept { return m_path; }

private: /* Fields: */

    std::string const m_path;

};

} /* namespace swordxx */

#endif /* SWORDXX_NORMALIZEDPATH_H */
