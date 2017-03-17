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

#include "normalizedpath.h"

#include <cassert>
#include "utilstr.h"


namespace swordxx {
namespace {

std::string normalizePath(std::string path) {
    removeTrailingDirectorySlashes(path);
    return path;
}

std::string normalizePathFromCString(char const * const path) {
    assert(path);
    return normalizePath(std::string(path));
}

} // anonymous namespace

NormalizedPath::NormalizedPath(char const * path)
    : m_path(normalizePathFromCString(path))
{}

NormalizedPath::NormalizedPath(std::string path)
    : m_path(normalizePath(std::move(path)))
{}

NormalizedPath NormalizedPath::getParentDirectory() const {
    if (m_path.empty())
        return m_path;
    auto const r(m_path.find_last_of("/\\"));
    if (r == std::string::npos)
        return m_path;
    return m_path.substr(0, r);
}

} /* namespace swordxx */
