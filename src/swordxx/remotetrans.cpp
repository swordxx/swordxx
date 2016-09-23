/*****************************************************************************
 *
 *  remotetrans.cpp -
 *
 * $Id$
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

#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <string>
#include "filemgr.h"
#include "swlog.h"
#include "utilstr.h"


using std::vector;

namespace swordxx {

RemoteTransport::RemoteTransport(const char *host, StatusReporter *statusReporter)
    : term(false)
{
    this->statusReporter = statusReporter;
    this->host = host;
    u = "ftp";
    p = "installmgr@user.com";
}


RemoteTransport::~RemoteTransport() {
}

int RemoteTransport::copyDirectory(const char *urlPrefix, const char *dir, const char *dest, const char *suffix) {
    SWLog::getSystemLog()->logDebug("RemoteTransport::copyDirectory");
    int retVal = 0;

    std::string url = std::string(urlPrefix ? urlPrefix : "") + std::string(dir ? dir : "");
    removeTrailingDirectorySlashes(url);
    url += '/';

    SWLog::getSystemLog()->logDebug("NetTransport: getting dir %s\n", url.c_str());
    vector<struct DirEntry> dirList = getDirList(url.c_str());

    if (!dirList.size()) {
        SWLog::getSystemLog()->logWarning("NetTransport: failed to read dir %s\n", url.c_str());
        return -1;
    }

    // append files in sub directories and calculate total download size
    unsigned int i = 0;
    long totalBytes = 0;
    for (;;) {
        if (i == dirList.size())
            break;

        struct DirEntry &e = dirList.at(i);

        if (e.isDirectory) {
            std::string name(e.name); // &e will be invalidated after first insertion
            vector<struct DirEntry> sd = getDirList((url + name + '/').c_str());
            for (unsigned int ii = 0; ii < sd.size(); ii++) {
                sd[ii].name = name + '/' + sd[ii].name;
                dirList.push_back(sd[ii]);
            }
            dirList.erase(dirList.begin() + i);
        }
        else {
            totalBytes += e.size;
            i++;
        }
    }

    long completedBytes = 0;
    for (i = 0; i < dirList.size(); i++) {
        struct DirEntry &dirEntry = dirList[i];
        std::string buffer(dest ? dest : "");
        removeTrailingDirectorySlashes(buffer);
        buffer += "/";
        buffer += dirEntry.name;
        if (!strcmp(&buffer.c_str()[buffer.length()-strlen(suffix)], suffix)) {
            std::string buffer2 = "Downloading (";
            buffer2 += formatted("%d", i+1);
            buffer2 += " of ";
            buffer2 += formatted("%d", dirList.size());
            buffer2 += "): ";
            buffer2 += dirEntry.name;
            if (statusReporter)
                statusReporter->preStatus(totalBytes, completedBytes, buffer2.c_str());
            FileMgr::createParent(buffer.c_str());	// make sure parent directory exists
            try {
                std::string url = (std::string)urlPrefix + (std::string)dir;
                removeTrailingDirectorySlashes(url);
                url += "/";
                url += dirEntry.name;
                if (!getUrl(buffer.c_str(), url.c_str())) {
                    SWLog::getSystemLog()->logWarning("copyDirectory: failed to get file %s\n", url.c_str());
                    return -2;
                }
                completedBytes += dirEntry.size;
            }
            catch (...) {}
            if (term.load(std::memory_order_acquire)) {
                retVal = -3;
                break;
            }
        }
    }
    return retVal;
}

} /* namespace swordxx */
