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

#ifndef SWORDXX_CURLTRANSPORT_H
#define SWORDXX_CURLTRANSPORT_H

#include "remotetrans.h"

#include <atomic>
#include <memory>
#include "defs.h"


namespace swordxx {

class SWDLLEXPORT CurlTransport: public RemoteTransport {

private: /* Types: */

    struct OutFile {

    /* Methods: */

        inline ~OutFile() noexcept {
            if (stream)
                std::fclose(stream);
        }

    /* Fields: */

        const char * const filename;
        std::string * const destBuf;
        FILE * stream;

    };

protected: /* Types: */

    struct Download {

    /* Methods: */

        Download(const char * const destPath,
                 const char * const sourceURL,
                 std::string * const destBuf,
                 CurlTransport & transport) noexcept;
        ~Download() noexcept;

        inline bool valid() noexcept { return m_session; }

        bool perform() noexcept;

    /* Fields: */

        void * m_session;
        std::shared_ptr<StatusReporter> m_statusReporter;
        std::atomic<bool> & m_term;
        OutFile m_outFile;

    };
    friend struct Download;

    #define SWORDXX_CURLDOWNLOAD_SETOPTION(a,...) \
        if (curl_easy_setopt(session.m_session, a, __VA_ARGS__) != CURLE_OK) \
            return -1

public: /* Methods: */

    using RemoteTransport::RemoteTransport;

    ~CurlTransport() noexcept override;

};

} /* namespace swordxx */

#endif /* SWORDXX_CURLTRANSPORT_H */
