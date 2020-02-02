/*****************************************************************************
 *
 *  curlftpt.cpp -    CURLFTPTransport
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

#include "curlftpt.h"

#include <curl/curl.h>
#include <curl/easy.h>
#include "filemgr.h"
extern "C" {
#include "ftpparse.h"
}
#include "swlog.h"


namespace swordxx {

bool CURLFTPTransport::getUrl(const char * destPath,
                              const char * sourceURL,
                              std::string * destBuf)
{
    Download download(destPath, sourceURL, destBuf, *this);
    if (!download.valid())
        return false;
    #define SETOPTION(a,...) \
        if (curl_easy_setopt(download.m_session, a, __VA_ARGS__) != CURLE_OK) \
            return false

    /* FTP connection settings: */
    if (!m_passive)
        SETOPTION(CURLOPT_FTPPORT, "-");

    #if (LIBCURL_VERSION_MAJOR > 7) || \
        ((LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR > 10)) || \
        ((LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR == 10) \
         && (LIBCURL_VERSION_PATCH >= 5))
    SETOPTION(CURLOPT_FTP_USE_EPRT, 0);
    #endif

    return download.perform();
}

std::vector<struct DirEntry> CURLFTPTransport::getDirList(const char * dirURL) {

SWLog::getSystemLog()->logDebug("RemoteTransport::getDirList(%s)", dirURL);
    std::vector<struct DirEntry> dirList;

    std::string dirBuf;
    if (getUrl("", dirURL, &dirBuf)) {
        char *start = &dirBuf[0u];
        char *end = start;
        while (start < ((&dirBuf[0u]) + dirBuf.size())) {
            FtpParseData item;
            bool looking = true;
            for (end = start; *end; end++) {
                if (looking) {
                    if ((*end == 10) || (*end == 13)) {
                        *end = 0;
                        looking = false;
                    }
                }
                else if ((*end != 10) && (*end != 13))
                    break;
            }
            SWLog::getSystemLog()->logDebug("getDirList: parsing item %s(%d)\n", start, end-start);
            int status = ftpparse(&item, start, end - start);
            // in ftpparse.h, there is a warning that name is not necessarily null terminated
            std::string name;
            name.append(item.name, item.namelen);
            SWLog::getSystemLog()->logDebug("getDirList: got item %s\n", name.c_str());
            if (status && name != "." && name != "..") {
                struct DirEntry i;
                i.name = name;
                i.approximateSizeInBytes = item.size;
                i.isDirectory = (item.flagtrycwd == 1);
                dirList.push_back(i);
            }
            start = end;
        }
    }
    else {
        SWLog::getSystemLog()->logWarning("getDirList: failed to get dir %s\n", dirURL);
    }
    return dirList;
}

} /* namespace swordxx */
