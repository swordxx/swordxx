/*****************************************************************************
 *
 *  curlftpt.cpp -    CURLFTPTransport
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

#include "curlftpt.h"

#include <cstring>
#include <curl/curl.h>
#include <curl/easy.h>
#include <fcntl.h>
#include "swlog.h"


namespace swordxx {

namespace {

    struct FtpFile {
        const char *filename;
        FILE *stream;
        std::string *destBuf;
    };


    // initialize/cleanup SYSTEMWIDE library with life of this static.
    static class CURLFTPTransport_init {
    public:
        CURLFTPTransport_init() {
            curl_global_init(CURL_GLOBAL_DEFAULT);  // curl_easy_init automatically calls it if needed
        }

        ~CURLFTPTransport_init() {
            curl_global_cleanup();
        }
    } _curlFTPTransport_init;


    static int my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream) {
        struct FtpFile *out=(struct FtpFile *)stream;
        if (out && !out->stream && !out->destBuf) {
            /* open file for writing */
            out->stream=fopen(out->filename, "wb");
            if (!out->stream)
                return -1; /* failure, can't open file to write */
        }
        if (out->destBuf) {
            int s = out->destBuf->size();
            out->destBuf->resize(s+(size*nmemb), '\0');
            std::memcpy(&out->destBuf[s], buffer, size*nmemb);
            return nmemb;
        }
        return fwrite(buffer, size, nmemb, out->stream);
    }


    struct MyProgressData {
        StatusReporter *sr;
        std::atomic<bool> & term;
    };


    static int my_fprogress(void *clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
        if (clientp) {
            MyProgressData *pd = (MyProgressData *)clientp;
            SWLog::getSystemLog()->logDebug("CURLFTPTransport report progress: totalSize: %ld; xfered: %ld\n", (long)dltotal, (long)dlnow);
            if (pd->sr) {
                if (dltotal < 0) dltotal = 0;
                if (dlnow < 0) dlnow = 0;
                if (dlnow > dltotal) dlnow = dltotal;
                pd->sr->update(dltotal, dlnow);
            }
            if (pd->term.load(std::memory_order_acquire))
                return 1;
        }
        return 0;
    }

}


CURLFTPTransport::CURLFTPTransport(const char *host, StatusReporter *sr) : RemoteTransport(host, sr) {
    session = (CURL *)curl_easy_init();
}


CURLFTPTransport::~CURLFTPTransport() {
    curl_easy_cleanup(session);
}


char CURLFTPTransport::getURL(const char *destPath, const char *sourceURL, std::string *destBuf) {
    signed char retVal = 0;
    struct FtpFile ftpfile = {destPath, 0, destBuf};

    CURLcode res;

    if (session) {

        struct MyProgressData pd{statusReporter, term};

        curl_easy_setopt(session, CURLOPT_URL, sourceURL);

        std::string credentials = u + ":" + p;
        curl_easy_setopt(session, CURLOPT_USERPWD, credentials.c_str());
        curl_easy_setopt(session, CURLOPT_WRITEFUNCTION, my_fwrite);
        if (!passive)
            curl_easy_setopt(session, CURLOPT_FTPPORT, "-");
        curl_easy_setopt(session, CURLOPT_NOPROGRESS, 0);
        curl_easy_setopt(session, CURLOPT_PROGRESSDATA, &pd);
        curl_easy_setopt(session, CURLOPT_PROGRESSFUNCTION, my_fprogress);
        /* Set a pointer to our struct to pass to the callback */
        curl_easy_setopt(session, CURLOPT_FILE, &ftpfile);

        /* Switch on full protocol/debug output */
        curl_easy_setopt(session, CURLOPT_VERBOSE, true);
        curl_easy_setopt(session, CURLOPT_CONNECTTIMEOUT, 45);

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

        // it seems CURL tries to use this option data later for some reason, so we unset here
        curl_easy_setopt(session, CURLOPT_PROGRESSDATA, (void*)NULL);

        if(CURLE_OK != res) {
            retVal = -1;
        }
    }

    if (ftpfile.stream)
        fclose(ftpfile.stream); /* close the local file */

    return retVal;
}


} /* namespace swordxx */

