/*****************************************************************************
 *
 *  rawstr4.h -    code for class 'RawStr4'- a module that reads raw text
 *        files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *        and provides lookup and parsing functions based on
 *        class StrKey
 *
 * $Id$
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef RAWSTR4_H
#define RAWSTR4_H

#include <cstdint>
#include <defs.h>

namespace swordxx {

class FileDesc;
class SWBuf;

class SWDLLEXPORT RawStr4 {
    static int instance;        // number of instantiated RawStr4 objects or derivitives
    char *path;
    bool caseSensitive;
    mutable long lastoff;        // for caching and optimizations

protected:
    static const int IDXENTRYSIZE;

    FileDesc *idxfd;
    FileDesc *datfd;
    void doSetText(const char *key, const char *buf, long len = -1);
    void doLinkEntry(const char *destkey, const char *srckey);
public:
    static const char nl;
    RawStr4(const char *ipath, int fileMode = -1, bool caseSensitive = false);
    virtual ~RawStr4();
    void getIDXBuf(long ioffset, char **buf) const;
    void getIDXBufDat(long ioffset, char **buf) const;
    signed char findOffset(const char *key, uint32_t *start, uint32_t *size, long away = 0, uint32_t *idxoff = 0) const;
    void readText(uint32_t start, uint32_t *size, char **idxbuf, SWBuf &buf) const;
    static signed char createModule(const char *path);
};

} /* namespace swordxx */
#endif
