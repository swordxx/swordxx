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
#include <curl/curl.h>
#include <curl/easy.h>
#include <fcntl.h>
#include <vector>
#include "filemgr.h"
#include "utilstr.h"


using std::vector;

namespace swordxx {

CURLHTTPTransport::CURLHTTPTransport(const char * const host,
                                     StatusReporter * const sr)
    : RemoteTransport(host, sr)
    , m_session(static_cast<CURL *>(curl_easy_init()))
{ /** \todo throw exception on !m_session */ }


CURLHTTPTransport::~CURLHTTPTransport() noexcept
{ curl_easy_cleanup(m_session); }


char CURLHTTPTransport::getURL(const char * destPath,
                               const char * sourceURL,
                               std::string * destBuf)
{
    if (!m_session)
        return -1;

    struct HttpFile {
        inline ~HttpFile() noexcept {
            if (stream)
                std::fclose(stream);
        }

        const char * const filename;
        std::string * const destBuf;
        FILE * stream;
    } httpFile{destPath, destBuf, nullptr};

    curl_easy_setopt(m_session, CURLOPT_URL, sourceURL);
    curl_easy_setopt(m_session, CURLOPT_USERNAME, u.c_str());
    curl_easy_setopt(m_session, CURLOPT_PASSWORD, p.c_str());
    static auto const my_httpfwrite =
            [](void * const buffer,
               std::size_t const size,
               std::size_t const nmemb,
               void * const stream) -> int
            {
                assert(stream);
                HttpFile * const out = static_cast<HttpFile *>(stream);
                if (!out->stream && !out->destBuf) {
                    out->stream = std::fopen(out->filename, "wb");
                    if (!out->stream) // Failure, can't open file to write
                        return -1;
                }
                if (out->destBuf) {
                    auto const s = out->destBuf->size();
                    out->destBuf->resize(s + (size * nmemb), '\0');
                    std::memcpy(&out->destBuf[s], buffer, size * nmemb);
                    return nmemb;
                }
                return std::fwrite(buffer, size, nmemb, out->stream);
            };
    curl_easy_setopt(m_session, CURLOPT_WRITEFUNCTION, &my_httpfwrite);
    curl_easy_setopt(m_session, CURLOPT_NOPROGRESS, 0);
    curl_easy_setopt(m_session, CURLOPT_FAILONERROR, 1);
    curl_easy_setopt(m_session, CURLOPT_PROGRESSDATA, statusReporter);

    static auto const my_httpfprogress =
            [](void * clientp,
               double dltotal,
               double dlnow,
               double ultotal,
               double ulnow) -> int
    {
        (void) ultotal;
        (void) ulnow;
        if (clientp) {
            if (dltotal < 0)
                dltotal = 0;
            if (dlnow < 0)
                dlnow = 0;
            if (dlnow > dltotal)
                dlnow = dltotal;
            static_cast<StatusReporter *>(clientp)->update(dltotal, dlnow);
        }
        return 0;
    };
    curl_easy_setopt(m_session, CURLOPT_PROGRESSFUNCTION, &my_httpfprogress);
    /* Set a pointer to our struct to pass to the callback */
    curl_easy_setopt(m_session, CURLOPT_FILE, &httpFile);

    /* Switch on full protocol/debug output */
    curl_easy_setopt(m_session, CURLOPT_VERBOSE, true);
    curl_easy_setopt(m_session, CURLOPT_CONNECTTIMEOUT, 45);

    return (curl_easy_perform(session.m_session) == CURLE_OK) ? 0 : -1;
}


vector<DirEntry> CURLHTTPTransport::getDirList(const char *dirURL) {
    /* We need to find the 2nd "<td" & then find the ">" after that. The size
       starts with the next non-space char */
    static auto const findSizeStart =
            [](const char * buffer) -> const char * {
                const char *listing = buffer;
                const char *pEnd;

                pEnd = strstr(listing, "<td");
                if(pEnd == NULL) {
                    return NULL;
                }
                listing = pEnd+2;
                pEnd = strstr(listing, "<td");
                if(pEnd == NULL)
                    return NULL;
                listing = pEnd+2;
                pEnd = strchr(listing, '>');
                if(pEnd == NULL)
                    return NULL;

                return pEnd+1;
            };
    vector<DirEntry> dirList;

    std::string dirBuf;
    const char *pBuf;
    char *pBufRes;
    std::string possibleName;
    double fSize;
    int possibleNameLength = 0;

    if (!getURL("", dirURL, &dirBuf)) {
        // Find the next link to a possible file name;
        pBuf = std::strstr(dirBuf.c_str(), "<a href=\"");
        while (pBuf != NULL) {
            pBuf += 9;//move to the start of the actual name.
            // Find the end of the possible file name:
            pBufRes = (char *)strchr(pBuf, '\"');
            if (!pBufRes)
                break;
            possibleNameLength = pBufRes - pBuf;
            possibleName = formatted("%.*s", possibleNameLength, pBuf);
            if (isalnum(possibleName[0])) {
                #if 0
                SWLog::getSystemLog()->logDebug(
                            "getDirListHTTP: Found a file: %s",
                            possibleName.c_str());
                #endif
                pBuf = pBufRes;
                pBufRes = (char *)findSizeStart(pBuf);
                fSize = 0;
                if(pBufRes != NULL) {
                    pBuf = pBufRes;
                    fSize = strtod(pBuf, &pBufRes);
                    if (pBufRes[0] == 'K')
                        fSize *= 1024;
                    else if (pBufRes[0] == 'M')
                        fSize *= 1048576;
                    pBuf = pBufRes;
                }
                DirEntry i;
                i.name = possibleName;
                i.size = (long unsigned int)fSize;
                i.isDirectory = (!possibleName.empty()
                                 && *possibleName.rbegin() == '/');
                dirList.push_back(i);
            } else {
                pBuf += possibleNameLength;
            }
            pBuf++;
            // Find the next link to a possible file name:
            pBuf = strstr(pBuf, "<a href=\"");
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

