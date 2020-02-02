/******************************************************************************
 *
 * Copyright 2009-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWORDXX_CURLHTTPT_H
#define SWORDXX_CURLHTTPT_H

#include "curltransport.h"

#include "defs.h"


namespace swordxx {

class SWDLLEXPORT CURLHTTPTransport: public CurlTransport {

public: /* Methods: */

    using CurlTransport::CurlTransport;

    std::vector<DirEntry> getDirList(const char * dirURL) override;
    bool getUrl(const char * destPath,
                const char * sourceURL,
                std::string * destBuf = nullptr) override;

};

} /* namespace swordxx */

#endif /* SWORDXX_CURLHTTPT_H */
