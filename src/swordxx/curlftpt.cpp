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

#include <curl/curl.h>
#include <curl/easy.h>


namespace swordxx {

char CURLFTPTransport::getURL(const char * destPath,
                              const char * sourceURL,
                              std::string * destBuf)
{
    Download download(destPath, sourceURL, destBuf, *this);
    if (!download.valid())
        return -1;
    #define SETOPTION(a,...) \
        if (curl_easy_setopt(download.m_session, a, __VA_ARGS__) != CURLE_OK) \
            return -1

    /* FTP connection settings: */
    if (!passive)
        SETOPTION(CURLOPT_FTPPORT, "-");

    #if (LIBCURL_VERSION_MAJOR > 7) || \
        ((LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR > 10)) || \
        ((LIBCURL_VERSION_MAJOR == 7) && (LIBCURL_VERSION_MINOR == 10) \
         && (LIBCURL_VERSION_PATCH >= 5))
    SETOPTION(CURLOPT_FTP_USE_EPRT, 0);
    #endif

    return download.perform() ? 0 : -1;
}

} /* namespace swordxx */
