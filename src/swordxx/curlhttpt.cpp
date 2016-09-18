/*****************************************************************************
 *
 *  curlhttpt.cpp -    CURLHTTPTransport
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

#include "curlhttpt.h"

#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <curl/curl.h>
#include <curl/easy.h>
#include <fcntl.h>
#include <limits>
#include <type_traits>
#include <vector>
#include "filemgr.h"
#include "utilstr.h"


namespace swordxx {

CURLHTTPTransport::CURLHTTPTransport(const char * const host,
                                     StatusReporter * const sr)
    : RemoteTransport(host, sr)
{}

CURLHTTPTransport::~CURLHTTPTransport() noexcept {}

char CURLHTTPTransport::getURL(const char * destPath,
                               const char * sourceURL,
                               std::string * destBuf)
{
    CURL * const s = static_cast<CURL *>(curl_easy_init());
    if (!s)
        return -1;

    struct MyCurl {
        inline ~MyCurl() noexcept { curl_easy_cleanup(m_session); }

        CURL * const m_session;
    } session{s};

    #define SETOPTION(a,...) \
        if (curl_easy_setopt(session.m_session, a, __VA_ARGS__) != CURLE_OK) \
            return -1
    SETOPTION(CURLOPT_URL, sourceURL);
    SETOPTION(CURLOPT_USERNAME, u.c_str());
    SETOPTION(CURLOPT_PASSWORD, p.c_str());
    SETOPTION(CURLOPT_FAILONERROR, 1);
    SETOPTION(CURLOPT_CONNECTTIMEOUT, 45);

    static auto const my_httpfprogress =
            [](void * clientp,
               ::curl_off_t dltotal,
               ::curl_off_t dlnow,
               ::curl_off_t ultotal,
               ::curl_off_t ulnow) -> int
    {
        (void) ultotal;
        (void) ulnow;
        if (clientp) {
            static_assert(std::is_signed<::curl_off_t>::value,
                          "cURL has unexpectedly fixed their interfaces!");
            if (dltotal < 0)
                dltotal = 0;
            if (dlnow < 0)
                dlnow = 0;
            if (dlnow > dltotal)
                dlnow = dltotal;

            static_assert(std::numeric_limits<::curl_off_t>::max()
                          <= std::numeric_limits<std::size_t>::max(), "");
            static_cast<StatusReporter *>(clientp)->update(
                        static_cast<std::size_t>(dltotal),
                        static_cast<std::size_t>(dlnow));
        }
        return 0;
    };
    SETOPTION(CURLOPT_XFERINFOFUNCTION, &my_httpfprogress);
    SETOPTION(CURLOPT_XFERINFODATA, statusReporter);
    SETOPTION(CURLOPT_NOPROGRESS, 0);

    struct HttpFile {
        inline ~HttpFile() noexcept {
            if (stream)
                std::fclose(stream);
        }

        const char * const filename;
        std::string * const destBuf;
        FILE * stream;
    };

    static auto const my_httpfwrite =
            [](void * const buffer,
               std::size_t const size,
               std::size_t const nmemb,
               void * const stream) -> std::size_t
            {
                assert(stream);
                HttpFile * const out = static_cast<HttpFile *>(stream);
                /* We assume that this does not overflow, because CURL expects
                   us to return this: */
                std::size_t const totalSize = size * nmemb;
                if (out->destBuf) {
                    auto & destBuffer = *out->destBuf;
                    if (totalSize <= 0u)
                        return 0u;
                    auto const s = destBuffer.size();
                    try {
                        // Check for overflow:
                        if (std::numeric_limits<std::size_t>::max() - s
                            < totalSize)
                            return 0u;
                        destBuffer.resize(totalSize + s, '\0');
                    } catch (...) {
                        return 0u;
                    }
                    std::memcpy(&(destBuffer[s]), buffer, totalSize);
                    return nmemb;
                }
                if (!out->stream) {
                    /* We attempt to create the file even if totalSize == 0u in
                       case the received file is empty. */
                    out->stream = std::fopen(out->filename, "wb");
                    if (!out->stream) { // Failure, can't open file to write
                        /* The failure return value for CURL must be different
                           from totalSize (which might itself equal 0u): */
                        return totalSize ? 0u : ~totalSize;
                    }
                }
                return std::fwrite(buffer, size, nmemb, out->stream);
            };
    SETOPTION(CURLOPT_WRITEFUNCTION, &my_httpfwrite);
    /* Set a pointer to our struct to pass to the callback */
    HttpFile httpFile{destPath, destBuf, nullptr};
    SETOPTION(CURLOPT_WRITEDATA, &httpFile);

    return (curl_easy_perform(session.m_session) == CURLE_OK) ? 0 : -1;
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

    if (!getURL("", dirURL, &dirBuf)) {
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
            if (std::isalnum(possibleName[0])) {
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
                i.size = (long unsigned int) fSize;
                i.isDirectory = (!possibleName.empty()
                                 && *possibleName.rbegin() == '/');
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
