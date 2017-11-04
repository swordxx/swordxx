/******************************************************************************
 *
 * Copyright 2016 Jaak Ristioja
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

#include "swconfig.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <tuple>
#include "utilstr.h"


namespace swordxx {

SWConfig::SWConfig() {}

SWConfig::SWConfig(std::string filename)
    : m_filename(std::move(filename))
{
    if (!m_filename.empty())
        reload();
}

SWConfig::~SWConfig() {}

bool SWConfig::reload() {
    assert(!m_filename.empty());

    decltype(m_sections) data;
    {
        std::string section;
        std::string line;
        std::ifstream inFile(m_filename);
        while (std::getline(inFile, line)) {
            if (line.empty() || (*line.begin()) == '#')
                continue;
            if (*line.begin() == '[') {
                if (line.size() < 3
                        || line.find(']', 1u) != line.size() - 1u)
                    return false;
                section.assign(line, 1u, line.size() - 2u);
                trimString(section);
            } else {
                trimString(line);
                auto it(std::find(line.begin(), line.end(), '='));
                if (it == line.begin() || it == line.end())
                    return false;
                std::string key(line.begin(), it);
                rightTrimString(key);
                std::string value(++it, line.end());
                leftTrimString(value);
                data[section].emplace(std::move(key), std::move(value));
            }
        }
        if (!inFile.eof())
            return false;
    }

    m_sections = data;
    return true;
}

bool SWConfig::save() {
    assert(!m_filename.empty());

    /// \todo Add proper error handling, perhaps even rollback.
    std::ofstream outFile(m_filename,
                          std::ios_base::out | std::ios_base::trunc);
    bool start = true;
    for (auto const & sp : m_sections) {
        if (sp.second.empty())
            continue;
        if (!start) {
            outFile << "\n[" << sp.first << "]\n";
            start = true;
        } else {
            outFile << '[' << sp.first << "]\n";
        }
        for (auto const & ep : sp.second)
            outFile << ep.first << '=' << ep.second << '\n';
    }
    return outFile.good();
}

void SWConfig::augment(SWConfig const & addFrom) {
    /* Currently, we write all changes to m_sections directly. Hence an
       exclusive write lock must be used for the whole duration of this if this
       function needs to be atomic. One could optimize by using copy-on-write
       semantics and/or a journal if performance is a must. */
    bool needRestoreFromBackup = false;
    decltype(m_sections) backup;
    auto const safeWriteOperation =
            [this, &backup, &needRestoreFromBackup](auto op) {
                if (!needRestoreFromBackup)
                    backup = m_sections;
                op();
                needRestoreFromBackup = true;
            };
    try {
        for (auto const & sp : addFrom.m_sections) {
            if (sp.second.empty()) // Skip empty sections
                continue;
            auto sectionIterator(m_sections.find(sp.first));
            if (sectionIterator == m_sections.end()) {
                safeWriteOperation([this, &sp]() { m_sections.emplace(sp); });
            } else {
                auto & section = sectionIterator->second;
                for (auto const & ep : sp.second) {
                    decltype(section.equal_range(ep.first).first) start;
                    decltype(section.equal_range(ep.first).second) end;
                    std::tie(start, end) = section.equal_range(ep.first);
                    if (std::find(start, end, ep) == end) // Skip duplicates
                        safeWriteOperation(
                                    [&section, &ep]() { section.emplace(ep); });
                }
            }
        }
    } catch (...) {
        if (needRestoreFromBackup)
            m_sections = std::move(backup);
        throw;
    }
}

} /* namespace swordxx */
