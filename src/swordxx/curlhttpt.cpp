/*****************************************************************************
 *
 *  curlhttpt.cpp -    CURLHTTPTransport
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

#include "curlhttpt.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "filemgr.h"
#include "utilstr.h"


namespace swordxx {

bool CURLHTTPTransport::getUrl(const char * destPath,
                               const char * sourceURL,
                               std::string * destBuf)
{
    Download download(destPath, sourceURL, destBuf, *this);
    return download.valid() && download.perform();
}

std::vector<DirEntry> CURLHTTPTransport::getDirList(const char *dirURL) {
    /* We need to find the 2nd "<td" & then find the ">" after that. The size
       starts with the next non-space char */
    static auto const findSizeStart =
            [](char const * inPtr) -> char const * {
                char const * pEnd = std::strstr(inPtr, "<td");
                if (!pEnd)
                    return nullptr;
                inPtr = pEnd + 2;
                pEnd = std::strstr(inPtr, "<td");
                if (!pEnd)
                    return nullptr;
                inPtr = pEnd + 2;
                pEnd = std::strchr(inPtr, '>');
                return pEnd ? (pEnd + 1) : nullptr;
            };
    std::vector<DirEntry> dirList;

    std::string dirBuf;

    if (getUrl("", dirURL, &dirBuf)) {
        std::string possibleName;

        // Find the next link to a possible file name;
        const char * pBuf = std::strstr(dirBuf.c_str(), "<a href=\"");
        while (pBuf) {
            pBuf += 9; // Move to the start of the actual name.
            // Find the end of the possible file name:
            char const * pBufRes = std::strchr(pBuf, '"');
            if (!pBufRes)
                break;
            int const possibleNameLength = pBufRes - pBuf;
            possibleName = formatted("%.*s", possibleNameLength, pBuf);
            if (charIsAlnum(possibleName[0])) {
                #if 0
                SWLog::getSystemLog()->logDebug(
                            "getDirListHTTP: Found a file: %s",
                            possibleName.c_str());
                #endif
                pBuf = pBufRes;
                pBufRes = findSizeStart(pBufRes);
                double fSize = 0.0;
                if (pBufRes) {
                    pBuf = pBufRes;
                    char * r;
                    fSize = std::strtod(pBuf, &r);
                    if (r[0] == 'K')
                        fSize *= 1024;
                    else if (r[0] == 'M')
                        fSize *= 1048576;
                    pBuf = r;
                }
                DirEntry i;
                i.name = possibleName;
                i.approximateSizeInBytes = static_cast<std::size_t>(fSize);
                i.isDirectory = (!possibleName.empty()
                                 && possibleName.back() == '/');
                dirList.push_back(i);
            } else {
                pBuf += possibleNameLength;
            }
            pBuf++;
            // Find the next link to a possible file name:
            pBuf = std::strstr(pBuf, "<a href=\"");
        }
    }
    #if 0
    else {
        SWLog::getSystemLog()->logWarning(
                    "CURLHTTPTransport: getDirList: failed to get dir %s\n",
                    dirURL);
    }
    #endif
    return dirList;
}

} /* namespace swordxx */
