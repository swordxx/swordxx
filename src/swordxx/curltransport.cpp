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

#include "curltransport.h"

#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <curl/curl.h>
#include <curl/easy.h>
#include <string>
#include <type_traits>
#include "max_v.h"


namespace swordxx {

namespace {

// Used to initialize cURL as static global:
static struct CurlGlobalInit {
    CurlGlobalInit() { ::curl_global_init(CURL_GLOBAL_DEFAULT); }
    ~CurlGlobalInit() { ::curl_global_cleanup(); }
} curlGlobalInit;

template <typename PD>
int curlProgress(void * clientp,
                 ::curl_off_t dltotal,
                 ::curl_off_t dlnow,
                 ::curl_off_t ultotal,
                 ::curl_off_t ulnow) noexcept
{
    (void) ultotal;
    (void) ulnow;
    assert(clientp);
    PD * const pd = static_cast<PD *>(clientp);
    if (pd->m_statusReporter) {
        static_assert(std::is_signed<::curl_off_t>::value,
                      "cURL has unexpectedly fixed their interfaces!");
        if (dltotal < 0)
            dltotal = 0;
        if (dlnow < 0)
            dlnow = 0;
        if (dlnow > dltotal)
            dlnow = dltotal;

        static_assert(max_v<::curl_off_t> <= max_v<std::size_t>, "");
        pd->m_statusReporter->update(static_cast<std::size_t>(dltotal),
                                     static_cast<std::size_t>(dlnow));
    }
    return pd->m_term.load(std::memory_order_acquire) ? 1 : 0;
};

template <typename OF>
std::size_t curlOutWrite(void * const buffer,
                         std::size_t const size,
                         std::size_t const nmemb,
                         void * const stream)
{
    assert(stream);
    OF * const out = static_cast<OF *>(stream);
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
            if (max_v<std::size_t> - s < totalSize)
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

} // anonymous namespace

CurlTransport::~CurlTransport() noexcept = default;

CurlTransport::Download::Download(const char * const destPath,
                                  const char * const sourceURL,
                                  std::string * const destBuf,
                                  CurlTransport & transport) noexcept
    : m_session(static_cast<CURL *>(curl_easy_init()))
    , m_statusReporter(transport.m_statusReporter)
    , m_term(transport.m_term)
    , m_outFile{destPath, destBuf, nullptr}
{
    if (!m_session)
        return;
    #define SETOPTION(a, ...) \
        if (curl_easy_setopt(m_session, a, __VA_ARGS__) != CURLE_OK) \
            goto cleanup;
    SETOPTION(CURLOPT_URL, sourceURL);
    SETOPTION(CURLOPT_XFERINFOFUNCTION, &curlProgress<Download>);
    SETOPTION(CURLOPT_XFERINFODATA, this);
    SETOPTION(CURLOPT_WRITEFUNCTION, &curlOutWrite<OutFile>);
    SETOPTION(CURLOPT_WRITEDATA, &m_outFile);
    SETOPTION(CURLOPT_USERNAME, transport.m_u.c_str());
    SETOPTION(CURLOPT_PASSWORD, transport.m_p.c_str());
    SETOPTION(CURLOPT_FAILONERROR, 1);
    SETOPTION(CURLOPT_CONNECTTIMEOUT, 45);
    SETOPTION(CURLOPT_NOPROGRESS, 0);
    return;

cleanup:

    curl_easy_cleanup(m_session);
    m_session = nullptr;
}

CurlTransport::Download::~Download() noexcept {
    if (m_session)
        curl_easy_cleanup(m_session);
}

bool CurlTransport::Download::perform() noexcept
{ return ::curl_easy_perform(m_session) == CURLE_OK; }

} /* namespace swordxx */
