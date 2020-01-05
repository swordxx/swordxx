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

    using namespace std::string_view_literals;
    decltype(m_sections) data;
    {
        std::string section;
        std::string line;
        std::ifstream inFile(m_filename);
        while (std::getline(inFile, line)) {
            trimString(line);
            if (line.empty() || line.front() == '#')
                continue;
            if (line.front() == '[') {
                if (line.back() != ']')
                    return false;
                section.assign(line, 1u, line.size() - 2u);
                trimString(section);
                if (section.empty())
                    return false;
            } else {
                auto it(std::find(line.begin(), line.end(), '='));
                if (it == line.begin() || it == line.end())
                    return false;
                std::string key(line.begin(), it);
                rightTrimString(key);
                std::string value(++it, line.end());
                leftTrimString(value);

                // Handle "continuation lines":
                while (endsWith(value, "\\"sv)) {
                    /* Similarly to Sword we only trim the backslash but not the
                       end of the line before the backslash: */
                    value.pop_back();

                    if (!std::getline(inFile, line))
                        break;

                    /* Since we are still on the same "continuation" line, we
                       only trim the whitespace on the right so that any inner
                       whitespace is left unchanged, even if it is in the
                       beginning of the regular line: */
                    rightTrimString(line);

                    value.append(line);
                }

                data[section].emplace(std::move(key), std::move(value));
            }
        }
        if (!inFile.eof())
            return false;
    }

    m_sections = std::move(data);
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
        assert(!sp.first.empty());
        if (!start) {
            outFile << "\n[" << sp.first << "]\n";
            start = true;
        } else {
            outFile << '[' << sp.first << "]\n";
        }
        for (auto const & ep : sp.second) {
            assert(!ep.first.empty());
            outFile << ep.first << '=' << ep.second << '\n';
        }
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
