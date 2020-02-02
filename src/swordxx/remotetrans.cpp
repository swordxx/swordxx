/*****************************************************************************
 *
 * Copyright 2004-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "remotetrans.h"

#include <cassert>
#include <cstring>
#include <fcntl.h>
#include <string>
#include "filemgr.h"
#include "swlog.h"
#include "utilstr.h"


namespace swordxx {

StatusReporter::~StatusReporter() noexcept = default;

void StatusReporter::preStatus(std::size_t, std::size_t, char const *) noexcept
{}

void StatusReporter::update(std::size_t, std::size_t) noexcept {}

RemoteTransport::RemoteTransport(const char * host,
                                 std::shared_ptr<StatusReporter> statusReporter)
    : m_statusReporter(std::move(statusReporter))
    , m_term(false)
{
    this->m_host = host;
    m_u = "ftp";
    m_p = "installmgr@user.com";
}


RemoteTransport::~RemoteTransport() {
}

int RemoteTransport::copyDirectory(const char * urlPrefix_,
                                   const char * dir_,
                                   const char * dest_,
                                   const char * suffix_)
{
    SWLog::getSystemLog()->logDebug("RemoteTransport::copyDirectory");
    std::string const suffix(suffix_ ? suffix_ : "");
    std::string const url(
                [&urlPrefix_, &dir_]() {
                    std::string r(std::string(urlPrefix_ ? urlPrefix_ : "")
                                  + std::string(dir_ ? dir_ : ""));
                    removeTrailingDirectorySlashes(r);
                    r += '/';
                    return r;
                }());
    std::string const dest(
                [&dest_](){
                    std::string r(dest_ ? dest_ : "");
                    removeTrailingDirectorySlashes(r);
                    r += "/";
                    return r;
                }());

    SWLog::getSystemLog()->logDebug("NetTransport: getting dir %s\n",
                                    url.c_str());
    std::vector<DirEntry> dirList = getDirList(url.c_str());

    if (dirList.empty()) {
        SWLog::getSystemLog()->logWarning(
                    "NetTransport: failed to read dir %s\n",
                    url.c_str());
        return -1;
    }

    // append files in sub directories and calculate total download size
    std::size_t totalBytes = 0u;
    for (std::size_t i = 0u; i != dirList.size(); ) {
        DirEntry const & e = dirList[i];
        if (e.isDirectory) {
            std::string const name(e.name + '/');
            dirList.erase(dirList.begin() +
                          static_cast<decltype(dirList)::difference_type>(i));
            for (auto & dirEntry : getDirList((url + name).c_str())) {
                dirEntry.name = name + dirEntry.name;
                dirList.push_back(dirEntry);
            }
        } else {
            totalBytes += e.approximateSizeInBytes;
            i++;
        }
    }

    std::size_t completedBytes = 0u;
    for (std::size_t i = 0u; i < dirList.size(); ++i) {
        DirEntry const & dirEntry = dirList[i];
        std::string entryName = dirEntry.name;
        assert(!entryName.empty());
        while (entryName.back() == '\0') { // Remove trailing null chars
            entryName.pop_back();
            if (entryName.empty())
                break;
        }
        std::string const buffer(dest + entryName);
        if (buffer.size() < suffix.size())
            continue;
        if (buffer.compare(buffer.size() - suffix.size(),
                           suffix.size(),
                           suffix) == 0)
            continue;
        if (m_statusReporter) {
            std::string buffer2 = "Downloading (";
            buffer2 += formatted("%d", i+1);
            buffer2 += " of ";
            buffer2 += formatted("%d", dirList.size());
            buffer2 += "): ";
            buffer2 += entryName;
            m_statusReporter->preStatus(totalBytes,
                                      completedBytes,
                                      buffer2.c_str());
        }
        // make sure parent directory exists:
        FileMgr::createParent(buffer.c_str());
        std::string const url2(url + entryName);
        if (!getUrl(buffer.c_str(), url2.c_str())) {
            SWLog::getSystemLog()->logWarning(
                        "copyDirectory: failed to get file %s\n",
                        url2.c_str());
            return -2;
        }
        completedBytes += dirEntry.approximateSizeInBytes;
        if (m_term.load(std::memory_order_acquire))
            return -3;
    }
    return 0;
}

} /* namespace swordxx */
