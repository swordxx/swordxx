/******************************************************************************
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

#ifndef SWORDXX_REMOTETRANS_H
#define SWORDXX_REMOTETRANS_H

#include <atomic>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include "defs.h"


namespace swordxx {

/** Class for reporting status
*/
class SWDLLEXPORT StatusReporter {

public: /* Methods: */

    virtual ~StatusReporter() noexcept;

    /** Messages before stages of a batch download */
    virtual void preStatus(std::size_t totalBytes,
                           std::size_t completedBytes,
                           char const * message) noexcept;

    /** Frequently called throughout a download, to report status */
    virtual void update(std::size_t totalBytes, std::size_t completedBytes)
            noexcept;

};


/**
* A base class to be used for reimplementation of network services.
*/
class SWDLLEXPORT RemoteTransport {

protected:
    std::shared_ptr<StatusReporter> m_statusReporter;
    std::atomic<bool> m_term;
    std::string m_host;
    std::string m_u;
    std::string m_p;

public:
    RemoteTransport(char const * host,
                    std::shared_ptr<StatusReporter> statusReporter
                            = std::shared_ptr<StatusReporter>());
    virtual ~RemoteTransport();

    /***********
     * override this method in your real impl
     *
     * if destBuf then write to buffer instead of file
     */
    virtual bool getUrl(const char * destPath,
                        const char * sourceURL,
                        std::string * destBuf = nullptr) = 0;

    virtual std::vector<struct DirEntry> getDirList(const char * dirURL) = 0;

    int copyDirectory(const char *urlPrefix, const char *dir, const char *dest, const char *suffix);

    void setUser(const char *user) { m_u = user; }
    void setPasswd(const char *passwd) { m_p = passwd; }
    void terminate() { m_term.store(true, std::memory_order_release); }
};


} /* namespace swordxx */

#endif /* SWORDXX_REMOTETRANS_H */
