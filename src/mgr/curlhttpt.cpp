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

#include <cctype>
#include <curl/curl.h>
#include <curl/easy.h>
#include <fcntl.h>
#include <vector>
#include "../frontend/swlog.h"
#include "../utilfuns/utilstr.h"
#include "filemgr.h"


using std::vector;

namespace swordxx {

namespace {

    struct FtpFile {
        const char *filename;
        FILE *stream;
        std::string *destBuf;
    };


    static int my_httpfwrite(void *buffer, size_t size, size_t nmemb, void *stream) {
        struct FtpFile *out=(struct FtpFile *)stream;
        if (out && !out->stream && !out->destBuf) {
            /* open file for writing */
            out->stream=fopen(out->filename, "wb");
            if (!out->stream)
                return -1; /* failure, can't open file to write */
        }
        if (out->destBuf) {
            int s = out->destBuf->size();
            out->destBuf->resize(s + (size * nmemb), '\0');
            std::memcpy(&out->destBuf[s], buffer, size*nmemb);
            return nmemb;
        }
        return fwrite(buffer, size, nmemb, out->stream);
    }


    static int my_httpfprogress(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
        if (clientp) {
            if (dltotal < 0) dltotal = 0;
            if (dlnow < 0) dlnow = 0;
            if (dlnow > dltotal) dlnow = dltotal;
            ((StatusReporter *)clientp)->update(dltotal, dlnow);
        }
        return 0;
    }

}


CURLHTTPTransport::CURLHTTPTransport(const char *host, StatusReporter *sr) : RemoteTransport(host, sr) {
    session = (CURL *)curl_easy_init();
}


CURLHTTPTransport::~CURLHTTPTransport() {
    curl_easy_cleanup(session);
}


char CURLHTTPTransport::getURL(const char *destPath, const char *sourceURL, std::string *destBuf) {
    signed char retVal = 0;
    struct FtpFile ftpfile = {destPath, 0, destBuf};

    CURLcode res;

    if (session) {
        curl_easy_setopt(session, CURLOPT_URL, sourceURL);

        std::string credentials = u + ":" + p;
        curl_easy_setopt(session, CURLOPT_USERPWD, credentials.c_str());
        curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, my_httpfwrite);
        if (!passive)
            curl_easy_setopt(session, CURLOPT_FTPPORT, "-");
        curl_easy_setopt(session, CURLOPT_NOPROGRESS, 0);
        curl_easy_setopt(session, CURLOPT_FAILONERROR, 1);
        curl_easy_setopt(session, CURLOPT_PROGRESSDATA, statusReporter);
        curl_easy_setopt(session, CURLOPT_PROGRESSFUNCTION, my_httpfprogress);
        /* Set a pointer to our struct to pass to the callback */
        curl_easy_setopt(session, CURLOPT_FILE, &ftpfile);

        /* Switch on full protocol/debug output */
        curl_easy_setopt(session, CURLOPT_VERBOSE, true);
        curl_easy_setopt(session, CURLOPT_CONNECTTIMEOUT, 45);

        /* Disable checking host certificate */
        curl_easy_setopt(session, CURLOPT_SSL_VERIFYPEER, false);

        /* FTP connection settings */

#if (LIBCURL_VERSION_MAJOR > 7) || \
   ((LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR > 10)) || \
   ((LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR == 10) && (LIBCURL_VERSION_PATCH >= 5))
#      define EPRT_AVAILABLE 1
#endif

#ifdef EPRT_AVAILABLE
        curl_easy_setopt(session, CURLOPT_FTP_USE_EPRT, 0);
        SWLog::getSystemLog()->logDebug("***** using CURLOPT_FTP_USE_EPRT\n");
#endif


        SWLog::getSystemLog()->logDebug("***** About to perform curl easy action. \n");
        SWLog::getSystemLog()->logDebug("***** destPath: %s \n", destPath);
        SWLog::getSystemLog()->logDebug("***** sourceURL: %s \n", sourceURL);
        res = curl_easy_perform(session);
        SWLog::getSystemLog()->logDebug("***** Finished performing curl easy action. \n");

        if(CURLE_OK != res) {
            retVal = -1;
        }
    }

    if (ftpfile.stream)
        fclose(ftpfile.stream); /* close the local file */

    return retVal;
}


// we need to find the 2nd "<td" & then find the ">" after that.  The size starts with the next non-space char
const char *findSizeStart(const char *buffer) {
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
}


vector<struct DirEntry> CURLHTTPTransport::getDirList(const char *dirURL) {

    vector<struct DirEntry> dirList;

    std::string dirBuf;
    const char *pBuf;
    char *pBufRes;
    std::string possibleName;
    double fSize;
    int possibleNameLength = 0;

    if (!getURL("", dirURL, &dirBuf)) {
        pBuf = std::strstr(dirBuf.c_str(), "<a href=\"");//Find the next link to a possible file name.
        while (pBuf != NULL) {
            pBuf += 9;//move to the start of the actual name.
            pBufRes = (char *)strchr(pBuf, '\"');//Find the end of the possible file name
            if (!pBufRes)
                break;
            possibleNameLength = pBufRes - pBuf;
            possibleName = formatted("%.*s", possibleNameLength, pBuf);
            if (isalnum(possibleName[0])) {
                SWLog::getSystemLog()->logDebug("getDirListHTTP: Found a file: %s", possibleName.c_str());
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
                struct DirEntry i;
                i.name = possibleName;
                i.size = (long unsigned int)fSize;
                i.isDirectory = (!possibleName.empty() && *possibleName.rbegin() == '/');
                dirList.push_back(i);
            } else {
                pBuf += possibleNameLength;
            }
            pBuf++;
            pBuf = strstr(pBuf, "<a href=\"");//Find the next link to a possible file name.
        }
    }
    else
    {
        SWLog::getSystemLog()->logWarning("FTPURLGetDir: failed to get dir %s\n", dirURL);
    }
    return dirList;
}

} /* namespace swordxx */

